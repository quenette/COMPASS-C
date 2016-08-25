/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**
** Copyright (C) Steve Quenette 2011
**
** <license>
**
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include <stdlib.h>
#include "types.h"
#include "memory.h"

#include <stdio.h>
#include <string.h>

int main( int argc, char* argv[] ) {
	char* test;
	void* ptr1;

	test = coStrdup( "hello" );
	printf( "test -\"%s\"\n", test );

	ptr1 = coMalloc( 1024*1024, __func__ );
	memset( ptr1, 0, 1024*1024 );
	coRealloc( ptr1, 1024*1024*2 );
	memset( ptr1, 0, 1024*1024*2 );

	coFree( ptr1 );
	coFree( test );

	return 0;
}
