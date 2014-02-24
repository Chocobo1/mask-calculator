#ifndef mask_calc_HPP
#define mask_calc_HPP

#include <map>
#include <vector>
#include <cstdint>


class MaskCalc
{
	public:
		MaskCalc();
		void addNum( const uint32_t a );
		void addNum( const uint32_t a , const uint32_t b );

		void calcMask();
		const std::multimap< uint32_t , uint32_t > *getOutput() const;

		void reset();

	private:
		bool diffOneBit( const uint32_t a, const uint32_t b ) const;

		std::vector< bool > my_set;
		std::multimap< uint32_t , uint32_t > my_multimap;  // limit to 32 bits, <Key, Value> = <mask, value>
};


#endif
