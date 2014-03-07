#include "qmm.hpp"

#include "common.hpp"


size_t QMM::MyMmapHash::operator()( const MyMmap::const_iterator &a ) const
{
	return std::hash< const MyMmap::value_type * >()( &( *a ) );
}


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
	// insert & combine & merge values
	MyMmapRmList rm_list;
	for( const auto &i : my_input )
	{
		insertMinterm( i , UINT_MAX , rm_list );
	}

	// filter out combined minterms
	for( const auto &i : rm_list )
	{
		my_multimap.erase( i );
	}
	rm_list.clear();

	petrickMethod();

	return;
}


void QMM::insertMinterm( const uint32_t my_val , const uint32_t my_mask , MyMmapRmList &rm_list )
{
	const auto t = my_multimap.equal_range( my_mask );
	for( auto j = t.first ; j != t.second ; ++j )
	{
		if( j->second == my_val )  // already exist
		{
			return;
		}
	}

	const auto my_val_itr = my_multimap.emplace( my_mask , my_val );

	for( auto j = t.first ; j != t.second ; ++j )
	{
		// try to merge with other values
		const auto other_val = j->second;
		const bool if_merge = diffOneBit( my_val , other_val );
		if( if_merge )
		{
			const uint32_t new_mask = my_mask & ( ~( my_val xor other_val ) );
			const uint32_t new_val = std::min( my_val , other_val );

			// record used values
			rm_list.emplace( j );
			rm_list.emplace( my_val_itr );

			insertMinterm( new_val , new_mask , rm_list );
		}
	}

	return;
}


void QMM::petrickMethod()
{
	// swap
	MyMmap tmp_map( std::move( my_multimap ) );
	my_multimap.clear();

	// filter out obvious prime implicants
	const auto t = tmp_map.equal_range( UINT_MAX );
	for( auto i = t.first ; i != t.second ; )
	{
		my_multimap.emplace( std::move( *i ) );
		tmp_map.erase( i++ );
	}

	// filter out unobvious prime implicants
	MyMmapRmList p_i_list;
	for( const auto &i : my_input )
	{
		size_t count = 0;
		auto itr = tmp_map.cend();
		for( auto j = tmp_map.cbegin() ; j != tmp_map.cend() ; ++j )
		{
			if( checkMasked( j->first , j->second , i ) )
			{
				++count;
				itr = j;
			}
		}

		if( count == 1 )
		{
			p_i_list.emplace( std::move( itr ) );
		}
	}

	// handle prime implicants
	decltype( my_input ) tmp_input( my_input );
	for( const auto &i : p_i_list )
	{
		for( auto j = tmp_input.cbegin() ; j != tmp_input.cend() ; )
		{
			if( checkMasked( i->first , i->second , *j ) )
			{
				tmp_input.erase( j++ );
			}
			else
				++j;
		}
		my_multimap.emplace( std::move( *i ) );
		tmp_map.erase( i );
	}

	// product-of-sums to sum-of-products
	// stuff in one-by-one
	std::list< MyUnorderedSet > tmp_list;
	for( const auto &i : tmp_input )
	{
		size_t map_index = 0;
		std::unordered_set< size_t > tmp_set;
		for( const auto &j : tmp_map )
		{
			if( checkMasked( j.first , j.second , i ) )
			{
				tmp_set.insert( map_index );
			}
			++map_index;
		}
		insertAndMutiply( tmp_list , tmp_set );

		simplify( tmp_list );
	}

	// select solution
	size_t opt_sol_size = UINT_MAX;
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
	size_t counter = 0;
	for( const auto &i : tmp_map )
	{
		if( opt_sol.find( counter ) != opt_sol.end() )
		{
			my_multimap.emplace( i );
		}
		++counter;
	}

	return;
}


void QMM::insertAndMutiply( std::list< MyUnorderedSet > &a , const MyUnorderedSet &b )
{
	if( a.empty() )
	{
		for( const auto &i : b )
		{
			a.emplace_back( i );
		}
		return;
	}

	std::list< MyUnorderedSet > tmp_list( std::move( a ) );
	a.clear();
	for( const auto &i : tmp_list )
	{
		const MyUnorderedSet base( i );
		for( const auto &j : b )
		{
			auto tmp = base;
			tmp.emplace( j );
			a.emplace_back( std::move( tmp ) );
		}
	}

	return;
}


void QMM::simplify( std::list< MyUnorderedSet > &a )
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
			}

			++j;
		}
	}

	return;
}


const MyMmap *QMM::getOutput() const
{
	return &my_multimap;
}


void QMM::reset()
{
	my_multimap.clear();
	my_input.clear();
	return;
}


void QMM::printList( const std::list< MyUnorderedSet > &a , const std::string &b ) const
{
	if( !b.empty() )
		printf( "%s:\n" , b.c_str() );
	for( const auto &i : a )
	{
		printUnorderedSet( i , "" );
	}
	printf( "\n" );
	return;
}


void QMM::printMap( const MyMmap &a , const std::string &b ) const
{
	if( !b.empty() )
		printf( "%s:\n" , b.c_str() );
	for( const auto &i : a )
	{
		printf( "%u / %x\n" , i.second , i.first );
	}
	printf( "\n" );
	return;
}


void QMM::printUnorderedSet( const MyUnorderedSet &a , const std::string &b ) const
{
	if( !b.empty() )
		printf( "%s:\n" , b.c_str() );
	for( const auto &i : a )
	{
		printf( " %zu" , i );
	}
	printf( "\n" );
	return;
}
