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
