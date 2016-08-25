/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**
** Copyright (C) Steve Quenette 2011
**
** <license>
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
#include "AspectFunc.h"

#include <string.h>


typedef struct {} coAspectFuncSuiteData;

void coAspectFuncSuite_Setup( coAspectFuncSuiteData* data ) {}

void coAspectFuncSuite_Teardown( coAspectFuncSuiteData* data ) {}


void coAspectFuncSuite_TestSimple( coAspectFuncSuiteData* data ) {
	coAspectFunc* af1;
	coAspectFunc* af2;

	af1 = coAspectFunc_new( "main", coAspectFuncSuite_Setup, __func__ );
	af2 = coAspectFunc_newWithAspect( "main", coAspectFuncSuite_Setup, __func__, data );
	
	coUnit_check_true( strcmp( "main", af1->name ) == 0 );
	coUnit_check_true( strcmp( "main", af2->name ) == 0 );
	coUnit_check_true( strcmp( __func__, af1->fromFunc ) == 0 );
	coUnit_check_true( strcmp( __func__, af2->fromFunc ) == 0 );
	coUnit_check_true( af1->funcPtr == coAspectFuncSuite_Setup );
	coUnit_check_true( af2->funcPtr == coAspectFuncSuite_Setup );
	coUnit_check_true( af1->hasAspect == coFalse );
	coUnit_check_true( af2->hasAspect == coTrue );
	coUnit_check_true( af2->fromAspect == data );

	coObject_delete( af2 );
	coObject_delete( af1 );
}

void coAspectFuncSuite( coUnit* unit ) {
	coUnit_setData( unit, coAspectFuncSuiteData );
	coUnit_setFixtures( unit, coAspectFuncSuite_Setup, coAspectFuncSuite_Teardown );
	coUnit_addTest( unit, coAspectFuncSuite_TestSimple );
}

int main( int argc, char* argv[] ) {
	coUnit* unit = coUnit_new();

	coAspectFuncSuite( unit );
	coUnit_run( unit );

	coObject_delete( unit );
	return 0;
}

