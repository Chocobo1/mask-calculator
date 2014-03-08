#ifndef greedymerge_HPP
#define greedymerge_HPP

#include <cstdint>
#include <map>
#include <unordered_set>


class GreedyMerge
{
		typedef std::unordered_set< uint32_t > MyUnorderedSet;
		typedef std::map< uint32_t , GreedyMerge::MyUnorderedSet > MyMap;  // limit to 32 bits, <Key, set<> > = <mask, set<value> >


	public:
		void addNum( const uint32_t a );
		void addNum( const uint32_t a , const uint32_t b );

		void doCalc();
		const GreedyMerge::MyMap *getOutput() const;

		void reset();


	private:
		void calcMask( const uint32_t my_val , const uint32_t my_mask );
		void stage2( const uint32_t my_val , const uint32_t my_mask );

		GreedyMerge::MyMap my_map;
};


#endif
