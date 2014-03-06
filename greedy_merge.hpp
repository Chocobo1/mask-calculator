#ifndef greedymerge_HPP
#define greedymerge_HPP

#include <map>
#include <cstdint>


class GreedyMerge
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

		std::multimap< uint32_t , uint32_t > my_multimap;  // limit to 32 bits, <Key, Value> = <mask, value>
};


#endif
