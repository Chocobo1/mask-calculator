#include "qmm.hpp"

#include <forward_list>


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

	for( auto i = start ; i <= end ; ++i )
	{
		my_input.emplace( i );
	}
	return;
}


void QMM::doCalc()
{
	return;
}


void QMM::calcMask( const uint32_t my_val , const uint32_t my_mask )
{
	return;
}


void QMM::stage2( const uint32_t my_val , const uint32_t my_mask )
{
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
	for( auto &i : my_input )
	{
		size_t map_index = 0;
		std::unordered_set< size_t > tmp_set;
		for( auto &j : tmp_map )
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


const std::multimap< uint32_t , uint32_t > *QMM::getOutput() const
{
	return &my_multimap;
}


void QMM::reset()
{
	my_multimap.clear();
	return;
}


bool QMM::checkMasked( const uint32_t a , const uint32_t a_mask , const uint32_t b ) const
{
	if( ( a & a_mask ) == ( b & a_mask ) )
	{
		return true;
	}
	return false;
}


bool QMM::diffOneBit( const uint32_t a , const uint32_t b ) const
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
