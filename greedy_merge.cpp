/*
* (C) 2014 Mike Tzou
*
* This file is part of "Mask calcuator".
*
* "Mask calcuator" is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* "Mask calcuator" is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/

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


void GreedyMerge::addDontCare( const uint32_t a )
{
	addNum( a );
	return;
}


void GreedyMerge::addDontCare( const uint32_t a , const uint32_t b )
{
	addNum( a , b );
	return;
}


void GreedyMerge::doCalc()
{
	// nothing to do
	return;
}


void GreedyMerge::calcMask( const uint32_t my_val , const uint32_t my_mask )
{
	// stage 1, check if can be covered by existing value
	for( const auto &i : my_map )
	{
		const uint32_t &now_mask = i.first;
		if( now_mask == my_mask )  // skip same mask
			continue;

		const GreedyMerge::MyUnorderedSet &tmp_set = i.second;
		for( const auto &j : tmp_set )  // for each same mask values
		{
			if( checkMasked( now_mask , my_val , j ) )
			{
				// can be merged with existing mask
				return;
			}
		}
	}

	stage2( my_val , my_mask );

	return;
}


void GreedyMerge::stage2( const uint32_t my_val , const uint32_t my_mask )
{
	// stage2, try to merge with other value which have the same mask
	GreedyMerge::MyUnorderedSet &tmp_set = my_map[ my_mask ];
	for( const auto &other_val : tmp_set )
	{
		const bool if_merge = diffOneBit( my_val , other_val );
		if( if_merge )
		{
			const uint32_t new_mask = my_mask & ( ~( my_val xor other_val ) );
			const uint32_t new_val = std::min( my_val , other_val );

			// remove merged values
			tmp_set.erase( other_val );
			/*
			if( tmp_set.empty() )  // took much more CPU cycles, but does not save significant memory
			{
				my_map.erase( my_mask );
			}
			*/

			stage2( new_val , new_mask );
			return;
		}
	}

	// cannot merge with any other values
	tmp_set.emplace( my_val );

	return;
}


const GreedyMerge::MyMap *GreedyMerge::getOutput() const
{
	return &my_map;
}


void GreedyMerge::reset()
{
	my_map.clear();
	return;
}
