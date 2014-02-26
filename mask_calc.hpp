#ifndef mask_calc_HPP
#define mask_calc_HPP

#include <map>
#include <vector>
#include <cstdint>


class MaskCalc
{
	public:
		void addNum( const uint32_t a );
		void addNum( const uint32_t a , const uint32_t b );

		void doCalc();
		const std::multimap< uint32_t , uint32_t > *getOutput() const;

		void reset();

	private:
		void calcMask( const uint32_t my_val , const uint32_t my_mask );
		void stage2( const uint32_t my_val , const uint32_t my_mask );

		bool diffOneBit( const uint32_t a, const uint32_t b ) const;
		bool checkMasked( const uint32_t a , const uint32_t a_mask , const uint32_t b ) const;

		std::multimap< uint32_t , uint32_t > my_multimap;  // limit to 32 bits, <Key, Value> = <mask, value>
};


#endif
