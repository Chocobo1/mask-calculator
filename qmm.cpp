#include "qmm.hpp"

#include "common.hpp"
#include <forward_list>


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
	std::unordered_set< MyMmap::const_iterator , MyMmapHash > rm_list;
	for( const auto &i : my_input )
	{
		stage2( i , UINT_MAX , rm_list );
//		printf("\n");
	}
	printf( "my_multimap.size(): %zu\n" , my_multimap.size() );
	printf( "rm_list.size(): %zu\n" , rm_list.size() );

	fprintf( stderr, "kero1\n" );

	// filter out used values
//	size_t c = 1;
	for( const auto &i : rm_list )
	{
//		printf( "%zu: %u/0x%x\n" , c++ , i->second , i->first );
//		my_multimap.erase( i );
	}
	//rm_list.clear();

	fprintf( stderr, "kero2\n" );

	// remove redundancy
	removeRedundancy();

	return;
}


void QMM::stage2( const uint32_t my_val , const uint32_t my_mask , std::unordered_set<MyMmap::const_iterator , MyMmapHash > &rm_list )
{
	const auto my_val_itr = my_multimap.emplace( my_mask , my_val );

//	printf( "\n [s2] start: %u/0x%x\n" , my_val , my_mask );
	const auto t = my_multimap.equal_range( my_mask );
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
			stage2( new_val , new_mask , rm_list );
		}
	}

//	printf( " [s2] end\n" );
	return;
}


void QMM::applyPetrickMethod()
{
	// swap
	std::multimap< uint32_t , uint32_t > tmp_map( std::move( my_multimap ) );

	// filter out prime implicant
	// stage1
	const auto t = tmp_map.equal_range( UINT_MAX );
	for( auto i = t.first ; i != t.second ; )
	{
		my_multimap.emplace( i->first , i->second );
		tmp_map.erase( i++ );
	}

	// stage2
	for( const auto &i : my_input )
	{
		size_t count = 0;
		auto itr = tmp_map.end();
		for( auto j = tmp_map.begin() ; j != tmp_map.end() ; ++j )
		{
			if( checkMasked( j->second , j->first , i ) )
			{
				++count;
				itr = j;
			}
		}

		if( count == 1 )
		{
			my_multimap.emplace( itr->first , itr->second );
			tmp_map.erase( itr );
		}
	}


	// multiply
	// stuff in first
	std::forward_list< std::unordered_set< size_t > > aaa;
	for( const auto &i : my_input )
	{
		size_t map_index = 0;
		std::unordered_set< size_t > tmp_set;
		for( const auto &j : tmp_map )
		{
			if( checkMasked( j.second , j.first , i ) )
			{
				tmp_set.insert( map_index );
			}
			++map_index;
			printf( "j.second(): %u\n", j.second );
			printf( "j.first(): %u\n", j.first );
		}
//		printf( "tmp_set.size1(): %u\n", tmp_set.size() );
		aaa.emplace_front( std::move( tmp_set ) );
//		printf( "tmp_set.size2(): %u\n", tmp_set.size() );
	}





	// reduce

	// choose solution

	return;
}


void QMM::removeRedundancy()
{
	for( auto i = my_multimap.begin() ; i != my_multimap.end() ; )
	{
		bool flag = false;
		auto j = i; ++j;
		for( ; j != my_multimap.end() ; ++j )
		{
			if( ( i->first == j->first ) && ( i->second == j->second ) )
			{
				flag = true;
				break;
			}
		}

		if( flag )
			my_multimap.erase( i++ );
		else
			++i;
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
