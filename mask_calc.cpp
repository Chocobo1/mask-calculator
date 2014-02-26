#include "mask_calc.hpp"


void MaskCalc::addNum( const uint32_t a )
{
	calcMask( a , UINT_MAX );
	return;
}


void MaskCalc::addNum( const uint32_t a , const uint32_t b )
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


void MaskCalc::doCalc()
{
	// nothing to do
	return;
}


void MaskCalc::calcMask( const uint32_t my_val , const uint32_t my_mask )
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

			if( checkMasked( other_val , now_mask , my_val ) )
			{
				// can be merged into existing mask
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


void MaskCalc::stage2( const uint32_t my_val , const uint32_t my_mask )
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


const std::multimap< uint32_t , uint32_t > *MaskCalc::getOutput() const
{
	return &my_multimap;
}


void MaskCalc::reset()
{
	my_multimap.clear();
	return;
}


bool MaskCalc::checkMasked( const uint32_t a , const uint32_t a_mask , const uint32_t b ) const
{
	if( ( a & a_mask ) == ( b & a_mask ) )
	{
		return true;
	}
	return false;
}


bool MaskCalc::diffOneBit( const uint32_t a , const uint32_t b ) const
{
	const uint32_t c = a xor b;
	switch( c )
	{
		case 1<<0:
		case 1<<1:
		case 1<<2:
		case 1<<3:
		case 1<<4:
		case 1<<5:
		case 1<<6:
		case 1<<7:
		case 1<<8:
		case 1<<9:
		case 1<<10:
		case 1<<11:
		case 1<<12:
		case 1<<13:
		case 1<<14:
		case 1<<15:
		case 1<<16:
		case 1<<17:
		case 1<<18:
		case 1<<19:
		case 1<<20:
		case 1<<21:
		case 1<<22:
		case 1<<23:
		case 1<<24:
		case 1<<25:
		case 1<<26:
		case 1<<27:
		case 1<<28:
		case 1<<29:
		case 1<<30:
		case 1<<31:
		{
			return true;
		}
	}
	return false;
}
