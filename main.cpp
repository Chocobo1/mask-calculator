#include <map>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <unordered_set>

using namespace std;


void printArg( const int argc , char *argv[] );
bool diffOneBit( const uint32_t a, const uint32_t b );


int main( const int argc , char *argv[] )
{
	// input handling
//	printArg( argc , argv );

	// get/sort numbers
	unordered_set< uint32_t > my_set;  // limit to 32 bytes
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
				fprintf( stderr, "input invalid: %s\n" , argv[i] );
				exit( EXIT_FAILURE );
			}

			const uint32_t a = strtoul( argv[i] , NULL , 0 );
			const uint32_t b = strtoul( if_range + 1 , NULL , 0 );
//			printf("a: %c %u\n" , argv[i][0] , a );
//			printf("b: %c %u\n" , *(if_range + 1) , b );
			for( auto i = min( a , b ) , end = max( a , b ); i <= end ; ++i )
			{
				my_set.emplace( i );
			}
		}
		else
		{
			// single number
			const uint32_t a = strtoul( argv[i] , NULL , 0 );
			my_set.emplace( a );
		}
	}

	// start algorithm
	// first time insert
	multimap< uint32_t , uint32_t > my_multimap;  // <Key, Value> = <mask, value>
	for( auto i = my_set.begin() ; i != my_set.end() ; ++i )
	{
		my_multimap.emplace( UINT_MAX , *i );
	}
	my_set.clear();

	// for each mask
	bool mod_flag = true;
	while( mod_flag )
	{
		mod_flag = false;

		for( auto i = my_multimap.begin() ; i != my_multimap.end() ; i = my_multimap.upper_bound( i->first ) )
		{
			// for each value sharing the same mask
			const uint32_t now_mask = i->first;
//			printf( "now_mask: 0x%x\n" , now_mask );

			const auto t = my_multimap.equal_range( now_mask );
			for( auto j = t.first ; j != t.second ; ++j )
			{
				// get first merge value
				const auto v1 = j->second;
//				printf( "v1: %u/0x%x\n" , v1 , j->first );

				// compare to other values
				auto k = j; ++k;
				for( ; k != t.second ; ++k )
				{
					// get second merge value
					const auto v2 = k->second;
//					printf( "v2: %u/0x%x\n" , v2 , k->first );

					const bool if_merge = diffOneBit( v1 , v2 );
					// printf( "v1: %u, v2: %u, result: %d\n" , v1 , v2 , if_merge );
					if( if_merge )
					{
						// can merge
						const uint32_t new_mask = now_mask & ( ~( v1 xor v2 ) );
						const uint32_t new_value = min( v1 , v2 );

						my_multimap.emplace( new_mask , new_value );
//						printf( "emplace, new_mask: 0x%x, new_value: %u\n" , new_mask , new_value );

						// remove merged values
						my_multimap.erase( j );
						my_multimap.erase( k );

						// restart all over again
						mod_flag = true;
						goto restart;
					}
				}
			}
		}

	restart:;
//		printf( "restart\n\n" );
	}

	// pretty output
	size_t counter = 0;
	printf( "\nvalue / mask\n" );
	for( auto i = my_multimap.begin() ; i != my_multimap.end() ; ++i )
	{
		printf( "%zu: %u / %x\n" , counter++ , i->second , i->first );
	}
	return 0;
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


bool diffOneBit( const uint32_t a, const uint32_t b )
{
	const uint32_t c = a xor b;
	switch( c )
	{
		case 1<<0:
		case 1<<1:
		case 1<<2:
		case 1<<3:
		case 1<<4:
		case 1<<5:
		case 1<<6:
		case 1<<7:
		case 1<<8:
		case 1<<9:
		case 1<<10:
		case 1<<11:
		case 1<<12:
		case 1<<13:
		case 1<<14:
		case 1<<15:
		case 1<<16:
		case 1<<17:
		case 1<<18:
		case 1<<19:
		case 1<<20:
		case 1<<21:
		case 1<<22:
		case 1<<23:
		case 1<<24:
		case 1<<25:
		case 1<<26:
		case 1<<27:
		case 1<<28:
		case 1<<29:
		case 1<<30:
		case 1<<31:
		{
			return true;
		}
	}
	return false;
}
