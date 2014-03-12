#include "qmm.hpp"

#include "common.hpp"


void QMM::addNum( const uint32_t a )
{
	my_input.emplace( a );
	return;
}


void QMM::addNum( const uint32_t a , const uint32_t b )
{
	// add a range of numbers [a, b]
	const uint32_t start = std::min( a , b );
	const uint32_t end = std::max( a , b );

	auto h = my_input.cend();
	for( auto i = start ; i <= end ; ++i )
	{
		h = my_input.emplace_hint( h , i );
	}
	return;
}


void QMM::doCalc()
{
	// insert & combine minterms
	MyMapRmList rm_list;

	my_multimap[ UINT_MAX ].reserve( my_input.size() );  // save CPU cycles when input is many
	rm_list[ UINT_MAX ].reserve( my_input.size() );

	for( const auto &i : my_input )
	{
		insertMinterm( i , UINT_MAX , rm_list );
	}

	// remove combined minterms
	for( const auto &i : rm_list )
	{
		const auto &rm_set = i.second;

		const auto &tmp_set_itr = my_multimap.find( i.first );
		auto &tmp_set = tmp_set_itr->second;
		if( tmp_set.size() == rm_set.size() )
		{
			my_multimap.erase( tmp_set_itr );
		}
		else
		{
			for( const auto &j : rm_set )
			{
				tmp_set.erase( j );
			}
		}
	}
	rm_list.clear();

	petrickMethod();

	return;
}


void QMM::insertMinterm( const uint32_t my_val , const uint32_t my_mask , MyMapRmList &rm_list )
{
	auto &tmp_set = my_multimap[ my_mask ];
	if( tmp_set.find( my_val ) != tmp_set.end() )  // has already been genereated
		return;
	tmp_set.emplace( my_val );

	for( const auto &other_val : tmp_set )
	{
		// try to merge with other values
		const bool if_merge = diffOneBit( my_val , other_val );
		if( if_merge )
		{
			const uint32_t new_mask = my_mask & ( ~( my_val xor other_val ) );
			const uint32_t new_val = std::min( my_val , other_val );

			// record used values
			auto &rm_set = rm_list[ my_mask ];
			rm_set.emplace( my_val );
			rm_set.emplace( other_val );

			insertMinterm( new_val , new_mask , rm_list );
		}
	}

	return;
}


void QMM::petrickMethod()
{
	// swap
	QMM::MyMap tmp_map;
	tmp_map.swap( my_multimap );

	// filter out obvious prime implicants
	my_multimap[ UINT_MAX ] = std::move( tmp_map[ UINT_MAX ] );
	tmp_map.erase( UINT_MAX );

	// filter out unobvious prime implicants
	MyMapRmList p_i_list;
	for( const auto &i : my_input )
	{
		size_t count = 0;
		auto mask = 0;
		auto val = 0;
		for( const auto &j : tmp_map )
		{
			for( const auto &k : j.second )
			{
				if( checkMasked( j.first , i , k ) )
				{
					++count;
					mask = j.first;
					val = k;
				}
			}
		}

		if( count == 1 )
		{
			p_i_list[ mask ].emplace( val );
		}
	}

	// handle prime implicants
	decltype( my_input ) tmp_input( my_input );
	for( const auto &i : p_i_list )
	{
		auto &tmp_set = tmp_map[ i.first ];
		for( const auto &j : i.second )
		{
			for( auto k = tmp_input.cbegin() ; k != tmp_input.cend() ; )
			{
				if( checkMasked( i.first , j , *k ) )
					tmp_input.erase( k++ );
				else
					++k;
			}
			tmp_set.erase( j );
		}
		my_multimap.emplace( std::move( i ) );
	}
	p_i_list.clear();

	// product-of-sums to sum-of-products
	// stuff in one-by-one
	std::list< QMM::MySolSet > tmp_list;
	for( const auto &i : tmp_input )
	{
		QMM::MySolSet::key_type map_index = 0;
		QMM::MySolSet tmp_set;
		for( const auto &j : tmp_map )
		{
			for( const auto &k : j.second )
			{
				if( checkMasked( j.first , i , k ) )
				{
					tmp_set.emplace( map_index );
				}
				++map_index;
			}
		}
		insertAndMutiply( tmp_list , tmp_set );

		simplify( tmp_list );
	}

	// select solution
	auto opt_sol_size = UINT_MAX;
	decltype( tmp_list )::value_type opt_sol;
	for( auto &i : tmp_list )
	{
		if( i.size() < opt_sol_size )
		{
			opt_sol.swap( i );
			opt_sol_size = opt_sol.size();
		}
	}

	// place solution back
	QMM::MySolSet::key_type counter = 0;
	for( const auto &i : tmp_map )
	{
		auto &target_set = my_multimap[ i.first ];
		for( const auto &j : i.second )
		{
			if( opt_sol.find( counter ) != opt_sol.end() )
			{
				target_set.emplace( j );
			}
			++counter;
		}
	}
	tmp_map.clear();

	return;
}


void QMM::insertAndMutiply( std::list< QMM::MySolSet > &a , const QMM::MySolSet &b )
{
	if( a.empty() )
	{
		for( const auto &i : b )
		{
			a.push_back( { i } );
		}
		return;
	}

	std::list< QMM::MySolSet > tmp_list;
	tmp_list.swap( a );
	for( const auto &i : tmp_list )
	{
		for( const auto &j : b )
		{
			QMM::MySolSet base( i );
			base.emplace( j );
			a.emplace_back( std::move( base ) );
		}
	}

	return;
}


void QMM::simplify( std::list< QMM::MySolSet > &a )
{
	for( auto i = a.cbegin() ; i != a.cend() ; ++i )
	{
		for( auto j = a.begin() ; j != a.end() ; )
		{
			if( i == j )
			{
				++j;
				continue;
			}

			if( i->size() < j->size() )
			{
				bool merge_flag = true;
				for( const auto &k : *i )
				{
					if( j->find( k ) == j->end() )
					{
						merge_flag = false;
						break;
					}
				}
				if( merge_flag )
				{
					a.erase( j++ );
					continue;
				}
			}
			else if( i->size() == j->size() )
			{
				if( ( *i ) == ( *j ) )
				{
					a.erase( j++ );
					continue;
				}
			}
			else if( i->size() > j->size() )
			{
				// do nothing
			}

			++j;
		}
	}

	return;
}


const QMM::MyMap *QMM::getOutput() const
{
	return &my_multimap;
}


void QMM::reset()
{
	my_multimap.clear();
	my_input.clear();
	return;
}


void QMM::printMap( const QMM::MyMap &a , const std::string &b ) const
{
	if( !b.empty() )
		printf( "%s:\n" , b.c_str() );
	for( const auto &i : a )
	{
		printf( " %x:\n" , i.first );
		printUnorderedSet( i.second , "" );
	}
	printf( "\n" );
	return;
}


void QMM::printList( const std::list< QMM::MySolSet > &a , const std::string &b ) const
{
	if( !b.empty() )
		printf( "%s:\n" , b.c_str() );
	for( const auto &i : a )
	{
		for( const auto &j : i )
		{
			printf( " %zu" , j );
		}
		printf( "\n" );
	}
	printf( "\n" );
	return;
}


void QMM::printUnorderedSet( const QMM::MyUnorderedSet &a , const std::string &b ) const
{
	if( !b.empty() )
		printf( "%s:\n" , b.c_str() );
	for( const auto &i : a )
	{
		printf( " %u" , i );
	}
	printf( "\n" );
	return;
}

/*
size_t QMM::MyMapHash::operator()( const QMM::MyUnorderedSet::const_iterator &a ) const
{
	return std::hash< const QMM::MyUnorderedSet::value_type * >()( &( *a ) );
}
*/
