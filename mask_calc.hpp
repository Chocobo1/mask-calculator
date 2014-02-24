#ifndef mask_calc_HPP
#define mask_calc_HPP

#include <map>
#include <set>
#include <cstdint>


bool diffOneBit( const uint32_t a, const uint32_t b );

class MaskCalc
{
	public:
		void addNum( const uint32_t a );
		void addNum( const uint32_t a , const uint32_t b );

		void calcMask();
		const std::multimap< uint32_t , uint32_t > *getOutput();

		void reset();

	private:
		bool diffOneBit( const uint32_t a, const uint32_t b ) const;

		std::set< uint32_t > my_set;  // limit to 32 bytes
		std::multimap< uint32_t , uint32_t > my_multimap;  // <Key, Value> = <mask, value>
};


#endif
