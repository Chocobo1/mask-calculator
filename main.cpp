#include <cstdio>
#include <cstring>

#include "mask_calc.hpp"


void printUsage( const char *a );
void printArg( const int argc , char *argv[] );


int main( const int argc , char *argv[] )
{
	// input handling
//	printArg( argc , argv );
	if( argc < 2 )
	{
		printUsage( argv[0] );
		exit( EXIT_FAILURE );
	}

	MaskCalc my_calc;

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
//			printf("a: %c %u\n" , argv[i][0] , a );
//			printf("b: %c %u\n" , *(if_range + 1) , b );
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
		printf( "%zu: %u / %x\n" , counter++ , i.second , i.first );
	}
	return 0;
}


void printUsage( const char *a )
{
	fprintf( stderr , "\n" );
	fprintf( stderr , "Mask calculator\n\n" );
	fprintf( stderr , "Usage: %s port1 port2 [...]\n" , a );
	fprintf( stderr , "Example1: %s 50 100\n" , a );
	fprintf( stderr , "Example2: %s 50-100\n" , a );
	fprintf( stderr , "Example3: %s 50 100 1000-1020\n\n" , a );
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
