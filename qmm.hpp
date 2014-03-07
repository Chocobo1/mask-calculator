#ifndef qmm_HPP
#define qmm_HPP

#include <cstdint>
#include <map>
#include <list>
#include <unordered_set>


typedef std::multimap< uint32_t , uint32_t > MyMmap;
typedef std::unordered_set< size_t > MyUnorderedSet;


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
		typedef std::unordered_set<MyMmap::const_iterator , MyMmapHash> MyMmapRmList;

		void insertMinterm( const uint32_t my_val , const uint32_t my_mask , MyMmapRmList &rm_list );
		void petrickMethod();
		void simplify( std::list< MyUnorderedSet > &a );
		void insertAndMutiply( std::list< MyUnorderedSet > &a , const MyUnorderedSet &b );

		void printMap( const MyMmap &a , const std::string &b ) const;
		void printList( const std::list< MyUnorderedSet > &a , const std::string &b ) const;
		void printUnorderedSet( const MyUnorderedSet &a , const std::string &b ) const;

		MyMmap my_multimap;  // limit to 32 bits, <Key, Value> = <mask, value>
		std::unordered_set< uint32_t > my_input;  // for storing every inputs
};


#endif
