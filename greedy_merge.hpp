/*
* (C) 2014 Mike Tzou
*
* This file is part of "Mask calculator".
*
* "Mask calculator" is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* "Mask calculator" is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef greedymerge_HPP
#define greedymerge_HPP

#include <cstdint>
#include <map>
#include <unordered_set>


class GreedyMerge
{
		typedef std::unordered_set< uint32_t > MyUnorderedSet;
		typedef std::map< uint32_t , GreedyMerge::MyUnorderedSet > MyMap;  // limit to 32 bits, <Key, set<> > = <mask, set<value> >


	public:
		void addNum( const uint32_t a );
		void addNum( const uint32_t a , const uint32_t b );

		void addDontCare( const uint32_t a );
		void addDontCare( const uint32_t a , const uint32_t b );

		void doCalc();
		const GreedyMerge::MyMap *getOutput() const;

		void reset();


	private:
		void calcMask( const uint32_t my_val , const uint32_t my_mask );
		void stage2( const uint32_t my_val , const uint32_t my_mask );

		GreedyMerge::MyMap my_map;
};


#endif
