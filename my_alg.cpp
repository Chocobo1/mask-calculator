#include "my_alg.hpp"

#include "common.hpp"


void MyAlg::addNum( const uint32_t a )
{
	calcMask( a , UINT_MAX );
	return;
}


void MyAlg::addNum( const uint32_t a , const uint32_t b )
{
	// add a range of numbers [a, b]
	const uint32_t start = std::min( a , b );
	const uint32_t end = std::max( a , b );

	for( auto i = start ; i <= end ; ++i )
	{
		calcMask( i , UINT_MAX );
	}
	return;
}


void MyAlg::doCalc()
{
	// nothing to do
	return;
}


void MyAlg::calcMask( const uint32_t my_val , const uint32_t my_mask )
{
	// stage 1, check if covered by existing mask

//	printf( "\n[s1] start, %u/0x%x\n" , my_val , my_mask );
	for( auto i = my_multimap.begin() ; i != my_multimap.end() ; i = my_multimap.upper_bound( i->first ) )
	{
		const uint32_t now_mask = i->first;
//		printf( "[s1] now_mask: 0x%x\n" , now_mask );

		if( now_mask == my_mask )  // skip same mask
			continue;

		const auto t = my_multimap.equal_range( now_mask );
		for( auto j = t.first ; j != t.second ; ++j )  // for each same mask values
		{
			const auto other_val = j->second;
//			printf( "[s1] try merge: %u/0x%x\n" , other_val , now_mask );

			if( checkMasked( now_mask , other_val , my_val ) )
			{
				// can be merged with existing mask
//				printf( "[s1] merged into: %u/0x%x\n" , other_val , now_mask );
//				printf( "[s1] end\n" );
				return;
			}
		}
	}
//	printf( "[s1] end\n" );

	stage2( my_val , my_mask );

	return;
}


void MyAlg::stage2( const uint32_t my_val , const uint32_t my_mask )
{
	// stage2, try to merge with other values which have the same mask

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

			// remove merged values
			my_multimap.erase( j );

//			printf( " [s2] recursive\n" );
			stage2( new_val , new_mask );

			return;
		}
	}

	// cannot merge with any other values
	my_multimap.emplace( my_mask , my_val );
//	printf( " [s2] emplace, %u/0x%x\n" , my_val , my_mask );
//	printf( " [s2] end\n" );

	return;
}


const std::multimap< uint32_t , uint32_t > *MyAlg::getOutput() const
{
	return &my_multimap;
}


void MyAlg::reset()
{
	my_multimap.clear();
	return;
}
