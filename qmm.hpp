/*
* (C) 2014 Mike Tzou
*
* This file is part of "Mask calcuator".
*
* "Mask calcuator" is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* "Mask calcuator" is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/

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

		void addDontCare( const uint32_t a );
		void addDontCare( const uint32_t a , const uint32_t b );

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
		MyUnorderedSet my_dontcares;

//		struct MyMapHash
//		{
//			size_t operator()( const QMM::MyUnorderedSet::const_iterator &a ) const;
//		};
//		typedef std::map< uint32_t , std::unordered_set< QMM::MyUnorderedSet::const_iterator , MyMapHash > > MyMapRmList;
};


#endif
