/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**
** Copyright (C) Steve Quenette 2011
**
** <license>
**
** This test is taken from StGermain ... it works ... why re-invent the wheel.
**
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "types.h"
#include "memory.h"
#include "journal.h"
#include "Object.h"
#include "Unit.h"

#include <string.h>


typedef struct {} coAsprintfSuiteData;

void coAsprintfSuite_Setup( coAsprintfSuiteData* data ) {}

void coAsprintfSuite_Teardown( coAsprintfSuiteData* data ) {}


void coAsprintfSuite_TestPrint( coAsprintfSuiteData* data ) {
	const char*  fiftyBytes = "01234567890123456789012345678901234567890123456789";
	char*        testString;
	char*        testStringPtr;
	unsigned int offset=0;

	/* Stress testing coAsprintf beyond the default alloc number of bytes */
	coAsprintf( &testString, "%s%s%s%s", fiftyBytes, fiftyBytes, fiftyBytes, fiftyBytes );
	coUnit_check_true( 200 == strlen( testString ) );
	for( offset=0; offset < 200; offset+=50 ) {
		testStringPtr = testString + offset;
		coUnit_check_true( 0 == strncmp( fiftyBytes, testStringPtr, 50 ) );
	}
	coFree( testString );
}

void coAsprintfSuite( coUnit* unit ) {
	coUnit_setData( unit, coAsprintfSuiteData );
	coUnit_setFixtures( unit, coAsprintfSuite_Setup, coAsprintfSuite_Teardown );
	coUnit_addTest( unit, coAsprintfSuite_TestPrint );
}

int main( int argc, char* argv[] ) {
	coUnit* unit = coUnit_new();

	coAsprintfSuite( unit );
	coUnit_run( unit );

	coObject_delete( unit );
	return 0;
}

