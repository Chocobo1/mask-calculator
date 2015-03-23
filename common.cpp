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

#include "common.hpp"


bool checkMasked( const uint32_t mask , const uint32_t a , const uint32_t b )
{
	if( ( a & mask ) == ( b & mask ) )
	{
		return true;
	}
	return false;
}


bool diffOneBit( const uint32_t a , const uint32_t b )
{
	// Calculating Hamming weight
	// see http://stackoverflow.com/questions/109023/how-to-count-the-number-of-set-bits-in-a-32-bit-integer
	// and http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel

	uint32_t i = a xor b;

	i = i - ( ( i >> 1 ) & 0x55555555 );
	i = ( i & 0x33333333 ) + ( ( i >> 2 ) & 0x33333333 );
	i = ( ( ( ( i + ( i >> 4 ) ) & 0x0F0F0F0F ) * 0x01010101 ) >> 24 );
	return ( i == 1 ) ? true : false;
}
