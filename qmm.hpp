#ifndef qmm_HPP
#define qmm_HPP

#include <cstdint>
#include <map>
#include <unordered_set>


typedef std::multimap< uint32_t , uint32_t > MyMmap;


class QMM
{
	public:
		void addNum( const uint32_t a );
		void addNum( const uint32_t a , const uint32_t b );

		void doCalc();
		const MyMmap *getOutput() const;

		void reset();

	private:
		struct MyMmapHash
		{
			size_t operator()( const MyMmap::const_iterator &a ) const;
		};

		void removeRedundancy();
		void stage2( const uint32_t my_val , const uint32_t my_mask , std::unordered_set<MyMmap::const_iterator , MyMmapHash> &rm_list );
		void applyPetrickMethod();

		MyMmap my_multimap;  // limit to 32 bits, <Key, Value> = <mask, value>
		std::unordered_set< uint32_t > my_input;  // for storing every inputs
};


#endif
