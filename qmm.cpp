#include "qmm.hpp"

#include "common.hpp"
#include <list>

void printMap( const MyMmap &a , const std::string &b );
void printList( const std::list< std::unordered_set< size_t > > &a , const std::string &b );
void printUnorderedSet( const std::unordered_set< size_t > &a , const std::string &b );

void simplify( std::list< std::unordered_set< size_t > > &a );
void insertAndMutiply( std::list< std::unordered_set< size_t > > &a , const std::unordered_set< size_t > &b );


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
//		printf("\n");
	}
//	printf( "my_multimap.size(): %zu\n" , my_multimap.size() );
//	printf( "rm_list.size(): %zu\n" , rm_list.size() );

	// filter out used values
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

//	printf( "\n [s2] start: %u/0x%x\n" , my_val , my_mask );
	for( auto j = t.first ; j != t.second ; ++j )
	{
		// try to merge with other values
		const auto other_val = j->second;
		const bool if_merge = diffOneBit( my_val , other_val );
//		printf( " [s2] other_val: %u/0x%x, result: %d\n" , other_val , my_mask , if_merge );
		if( if_merge )
		{
			const uint32_t new_mask = my_mask & ( ~( my_val xor other_val ) );
			const uint32_t new_val = std::min( my_val , other_val );

			// record used values
			rm_list.emplace( j );
			rm_list.emplace( my_val_itr );
//			printf( " [s2] rm_list.emplace, %u/0x%x\n" , j->second , my_mask );
//			printf( " [s2] rm_list.emplace, %u/0x%x\n" , my_val_itr->second , my_mask );

//			printf( " [s2] recursive\n" );
			insertMinterm( new_val , new_mask , rm_list );
		}
	}

//	printf( " [s2] end\n" );
	return;
}


void QMM::petrickMethod()
{
	// swap
	std::multimap< uint32_t , uint32_t > tmp_map( std::move( my_multimap ) );

	// filter out obvious prime implicants
	const auto t = tmp_map.equal_range( UINT_MAX );
	for( auto i = t.first ; i != t.second ; )
	{
		my_multimap.emplace( *i );
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
			p_i_list.emplace( itr );
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
	}

	// product-of-sums to sum-of-products
	// stuff in one-by-one
	std::list< std::unordered_set< size_t > > tmp_list;
	for( const auto &i : tmp_input )
	{
//		printf(" i: %u\n",i);
//		printList( tmp_list , "a" );

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
//		printUnorderedSet( tmp_set , "set" );
		insertAndMutiply( tmp_list , tmp_set );

//		printList( tmp_list , "b" );
//		printf(" end\n\n");
		simplify( tmp_list );
	}

	// select optimal solution
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


void insertAndMutiply( std::list< std::unordered_set< size_t > > &a , const std::unordered_set< size_t > &b )
{
	if( a.empty() )
	{
		for( const auto &i : b )
		{
			std::unordered_set< size_t > tmp( { i } );
			a.emplace_back( std::move( tmp ) );
		}
		return;
	}

	std::list< std::unordered_set< size_t > > tmp_list( std::move( a ) );
	for( const auto &i : tmp_list )
	{
		const std::unordered_set< size_t > base( i.cbegin() , i.cend() );
		for( const auto &j : b )
		{
			auto tmp = base;
			tmp.emplace( j );
			a.emplace_back( std::move( tmp ) );
		}
	}

	return;
}


void simplify( std::list< std::unordered_set< size_t > > &a )
{
	for( auto i = a.cbegin() ; i != a.cend() ; ++i )
	{
		for( auto j = a.begin() ; j != a.end() ; )
		{
//			printUnorderedSet( *i , "i" );
//			printUnorderedSet( *j , "j" );
//			printf("\n\n");

			if( i == j )
			{
				++j;
//				printf("i, j same iterator\n");
				continue;
			}

			if( i->size() < j->size() )
			{
//				printf("i->size() < j->size()\n");
				bool merge_flag = true;
				for( const auto &k : *i )
				{
					if( j->find( k ) == j->end() )
					{
						merge_flag = false;
//						printf("merge_flag == false\n");
						break;
					}
				}
				if( merge_flag )
				{
					a.erase( j++ );
//					printf("merge_flag == true\n");
					continue;
				}
			}
			else if( i->size() == j->size() )
			{
//				printf("i->size() == j->size()\n");
				if( ( *i ) == ( *j ) )
				{
//					printf("*i == *j\n");
					a.erase( j++ );
					continue;
				}
			}
			else if( i->size() > j->size() )
			{
//				printf("i->size() > j->size()\n");
			}

			++j;
		}
	}

	return;
}


void printList( const std::list< std::unordered_set< size_t > > &a , const std::string &b )
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


void printMap( const MyMmap &a , const std::string &b )
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


void printUnorderedSet( const std::unordered_set< size_t > &a , const std::string &b )
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
