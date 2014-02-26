#ifndef qmm_HPP
#define qmm_HPP

#include <map>
#include <unordered_set>
#include <cstdint>


class QMM
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
		void applyPetrickMethod();

		std::multimap< uint32_t , uint32_t > my_multimap;  // limit to 32 bits, <Key, Value> = <mask, value>
		std::unordered_set< uint32_t > my_input;  // for storing every inputs
};


#endif
