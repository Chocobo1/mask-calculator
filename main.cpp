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

#include <cstdio>
#include <cstring>

#include "greedy_merge.hpp"
#include "qmm.hpp"


void printUsage( const std::string &a );
void printArg( const int argc , char *argv[] );


int main( const int argc , char *argv[] )
{
	// input handling
	if( argc < 2 )
	{
		printUsage( argv[0] );
		exit( EXIT_FAILURE );
	}

	GreedyMerge my_calc;
//	QMM my_calc;

	// get/sort numbers
	for( auto i = 1 ; i < argc ; ++i )
	{
		// check if is range of numbers
		const char *if_range = strchr( argv[i] , '-' );
		if( if_range != NULL )
		{
			// check for invalid input
			if( ( if_range == argv[i] ) ||            // first char is delimiter
					( *( if_range + 1 ) == '\0' ) ||  // last char is delimiter
					( *( if_range + 1 ) == '-' ) )    // 2 delimiters
			{
				printUsage( argv[0] );
				fprintf( stderr, "input invalid: %s\n" , argv[i] );
				exit( EXIT_FAILURE );
			}

			const uint32_t a = strtoul( argv[i] , NULL , 0 );
			const uint32_t b = strtoul( if_range + 1 , NULL , 0 );
			my_calc.addNum( a, b );
		}
		else
		{
			// single number
			const uint32_t a = strtoul( argv[i] , NULL , 0 );
			my_calc.addNum( a );
		}
	}

	my_calc.doCalc();

	// pretty output
	size_t counter = 1;
	auto *results = my_calc.getOutput();
	printf( "\nvalue(dec) / mask(hex)\n" );
	for( const auto &i : ( *results ) )
	{
		for( const auto &j : i.second )
			printf( "%zu: %u / %x\n" , counter++ , j , i.first );
	}

//	system( "pause" );
	return 0;
}


void printUsage( const std::string &a )
{
	fprintf( stderr , "\n" );
	fprintf( stderr , "Mask calculator\n\n" );
	fprintf( stderr , "Usage: %s value1 [value2 ...]\n" , a.c_str() );
	fprintf( stderr , "Example1: %s 50 100\n" , a.c_str() );
	fprintf( stderr , "Example2: %s 50-100\n" , a.c_str() );
	fprintf( stderr , "Example3: %s 50 100 1000-1020\n\n" , a.c_str() );
	return;
}


void printArg( const int argc , char *argv[] )
{
	printf( "argc: %d\n" , argc );

	for( auto i = 1 ; i < argc ; ++i )
	{
		printf( "argv[%d]: %s\n" , i , argv[i] );
	}
	return;
}
