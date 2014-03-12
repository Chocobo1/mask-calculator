#ifndef qmm_HPP
#define qmm_HPP

#include <cstdint>
#include <map>
#include <list>
#include <unordered_set>


class QMM
{
		typedef std::unordered_set< size_t > MySolSet;
		typedef std::unordered_set< uint32_t > MyUnorderedSet;
		typedef std::map< uint32_t , QMM::MyUnorderedSet > MyMap;  // limit to 32 bits, <Key, set<> > = <mask, set<value> >
		typedef QMM::MyMap MyMapRmList;


	public:
		void addNum( const uint32_t a );
		void addNum( const uint32_t a , const uint32_t b );

		void doCalc();
		const QMM::MyMap *getOutput() const;

		void reset();


	private:
		void insertMinterm( const uint32_t my_val , const uint32_t my_mask , MyMapRmList &rm_list );
		void petrickMethod();
		void simplify( std::list< QMM::MySolSet > &a );
		void insertAndMutiply( std::list< QMM::MySolSet > &a , const QMM::MySolSet &b );

		void printMap( const QMM::MyMap &a , const std::string &b ) const;
		void printList( const std::list< QMM::MySolSet > &a , const std::string &b ) const;
		void printUnorderedSet( const QMM::MyUnorderedSet &a , const std::string &b ) const;

		QMM::MyMap my_multimap;  // limit to 32 bits, <Key, Value> = <mask, value>
		MyUnorderedSet my_input;  // for storing every inputs

//		struct MyMapHash
//		{
//			size_t operator()( const QMM::MyUnorderedSet::const_iterator &a ) const;
//		};
//		typedef std::map< uint32_t , std::unordered_set< QMM::MyUnorderedSet::const_iterator , MyMapHash > > MyMapRmList;
};


#endif
