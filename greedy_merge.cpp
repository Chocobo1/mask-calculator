#include "greedy_merge.hpp"

#include "common.hpp"


void GreedyMerge::addNum( const uint32_t a )
{
	calcMask( a , UINT_MAX );
	return;
}


void GreedyMerge::addNum( const uint32_t a , const uint32_t b )
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


void GreedyMerge::doCalc()
{
	// nothing to do
	return;
}


void GreedyMerge::calcMask( const uint32_t my_val , const uint32_t my_mask )
{
	// stage 1, check if covered by existing mask

	for( auto i = my_multimap.cbegin() ; i != my_multimap.cend() ; i = my_multimap.upper_bound( i->first ) )
	{
		const uint32_t now_mask = i->first;

		if( now_mask == my_mask )  // skip same mask
			continue;

		const auto t = my_multimap.equal_range( now_mask );
		for( auto j = t.first ; j != t.second ; ++j )  // for each same mask values
		{
			const auto other_val = j->second;

			if( checkMasked( now_mask , other_val , my_val ) )
			{
				// can be merged with existing mask
				return;
			}
		}
	}
//	printf( "[s1] end\n" );

	stage2( my_val , my_mask );

	return;
}


void GreedyMerge::stage2( const uint32_t my_val , const uint32_t my_mask )
{
	// stage2, try to merge with other values which have the same mask

	const auto t = my_multimap.equal_range( my_mask );
	for( auto j = t.first ; j != t.second ; ++j )
	{
		// try to merge with other values
		const auto other_val = j->second;
		const bool if_merge = diffOneBit( my_val , other_val );
		if( if_merge )
		{
			const uint32_t new_mask = my_mask & ( ~( my_val xor other_val ) );
			const uint32_t new_val = std::min( my_val , other_val );

			// remove merged values
			my_multimap.erase( j );

			stage2( new_val , new_mask );

			return;
		}
	}

	// cannot merge with any other values
	my_multimap.emplace( my_mask , my_val );

	return;
}


const std::multimap< uint32_t , uint32_t > *GreedyMerge::getOutput() const
{
	return &my_multimap;
}


void GreedyMerge::reset()
{
	my_multimap.clear();
	return;
}
