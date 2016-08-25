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
#include "types.h"
#include "memory.h"
#include "journal.h"
#include "Object.h"
#include "ObjectList.h"
#include "AspectFunc.h"
#include "FuncJP.h"
#include "Unit.h"

#include <string.h>

#define NUM_TEST_FUNCS 10

typedef struct {
	coFuncJP* jp;
	coBool testFuncsRan[NUM_TEST_FUNCS];
	int rank;
} FuncJPSuiteData;

typedef void (TestFunc_P)( FuncJPSuiteData* data );

static void TestFunc0( FuncJPSuiteData* data ) {
	data->testFuncsRan[0] = coTrue;
}

static void TestFunc1( FuncJPSuiteData* data ) {
	data->testFuncsRan[1] = coTrue;
}

static void TestFunc2( FuncJPSuiteData* data ) {
	data->testFuncsRan[2] = coTrue;
}

static void TestFunc3( FuncJPSuiteData* data ) {
	data->testFuncsRan[3] = coTrue;
}

static void TestFunc4( FuncJPSuiteData* data ) {
	data->testFuncsRan[4] = coTrue;
}

static void TestFunc5( FuncJPSuiteData* data ) {
	data->testFuncsRan[5] = coTrue;
}

static void TestFunc6( FuncJPSuiteData* data ) {
	data->testFuncsRan[6] = coTrue;
}

static void TestFunc7( FuncJPSuiteData* data ) {
	data->testFuncsRan[7] = coTrue;
}

static void TestFunc8( FuncJPSuiteData* data ) {
	data->testFuncsRan[8] = coTrue;
}

static void TestFunc9( FuncJPSuiteData* data ) {
	data->testFuncsRan[9] = coTrue;
}


void FuncJPSuite_Setup( FuncJPSuiteData* data ) {
	unsigned int ii;

	data->jp = NULL;
	for (ii=0; ii < NUM_TEST_FUNCS; ii++ ) {
		data->testFuncsRan[ii] = coFalse;
	}

	data->rank = 0;
}


void FuncJPSuite_Teardown( FuncJPSuiteData* data ) {
	coObject_delete( data->jp );
}


void FuncJPSuite_TestRunEmpty( FuncJPSuiteData* data ) {
	data->jp = coFuncJP_new( "test", EntryPoint_VoidPtr_Cast );
	coUnit_check_true( data->jp->_funcMetas->count == 0 );
	coFuncJP_run( data->jp, TestFunc_P, NULL );
}


void FuncJPSuite_TestAppendPrepend( FuncJPSuiteData* data ) {
	unsigned int    ii=0;

	data->jp = coFuncJP_new( "testEntryPoint", EntryPoint_0_Cast );
	coFuncJP_append( data->jp, (void*)TestFunc0, "TestFunc0", "testCode" );
	/* TestFunc0 */
	coFuncJP_prepend( data->jp, (void*)TestFunc1, "TestFunc1", "testCode" );
	/* TestFunc1, TestFunc0 */

	coUnit_check_true( data->jp->_funcMetas->count == 2 );
	coUnit_check_streq( data->jp->_funcMetas->data[0]->name, "TestFunc1" );
	coUnit_check_true( ((coAspectFunc*)data->jp->_funcMetas->data[0])->funcPtr == TestFunc1 );
	coUnit_check_streq( data->jp->_funcMetas->data[1]->name, "TestFunc0" );
	coUnit_check_true( ((coAspectFunc*)data->jp->_funcMetas->data[1])->funcPtr == TestFunc0 );
	for (ii=0; ii < data->jp->_funcMetas->count; ii++ ) {
		coUnit_check_streq( ((coAspectFunc*)data->jp->_funcMetas->data[ii])->fromFunc, "testCode" );
	}
}


void FuncJPSuite_TestInsertBeforeAfterReplace( FuncJPSuiteData* data ) {
	data->jp = coFuncJP_new( "testEntryPoint", EntryPoint_0_Cast );
	coFuncJP_append( data->jp, (void*)TestFunc2, "TestFunc2", "testCode" );
	coFuncJP_append( data->jp, (void*)TestFunc3, "TestFunc3", "testCode" );
	/* TestFunc2, TestFunc3 */
	coFuncJP_prepend( data->jp, (void*)TestFunc4, "TestFunc4", "testCode" );
	/* TestFunc4, TestFunc2, TestFunc3 */
	coFuncJP_insertBefore( data->jp, (void*)TestFunc5, "TestFunc5", "testCode", "TestFunc3" );
	/* TestFunc4, TestFunc2, TestFunc5, TestFunc3 */
	coFuncJP_insertAfter( data->jp, (void*)TestFunc6, "TestFunc6", "testCode", "TestFunc4" );
	/* TestFunc4, TestFunc6, TestFunc2, TestFunc5, TestFunc3 */
	coFuncJP_replace( data->jp, (void*)TestFunc7, "TestFunc7", "testCode", "TestFunc5" );
	/* TestFunc4, TestFunc6, TestFunc2, TestFunc7, TestFunc3 */

	coUnit_check_true( data->jp->_funcMetas->count == 5 );
	coUnit_check_streq( data->jp->_funcMetas->data[0]->name, "TestFunc4" );
	coUnit_check_true( ((coAspectFunc*)data->jp->_funcMetas->data[0])->funcPtr == TestFunc4 );
	coUnit_check_streq( data->jp->_funcMetas->data[1]->name, "TestFunc6" );
	coUnit_check_true( ((coAspectFunc*)data->jp->_funcMetas->data[1])->funcPtr == TestFunc6 );
	coUnit_check_streq( data->jp->_funcMetas->data[2]->name, "TestFunc2" );
	coUnit_check_true( ((coAspectFunc*)data->jp->_funcMetas->data[2])->funcPtr == TestFunc2 );
	coUnit_check_streq( data->jp->_funcMetas->data[3]->name, "TestFunc7" );
	coUnit_check_true( ((coAspectFunc*)data->jp->_funcMetas->data[3])->funcPtr == TestFunc7 );
	coUnit_check_streq( data->jp->_funcMetas->data[4]->name, "TestFunc3" );
	coUnit_check_true( ((coAspectFunc*)data->jp->_funcMetas->data[4])->funcPtr == TestFunc3 );
}


void FuncJPSuite_TestAlwaysFirstLast( FuncJPSuiteData* data ) {
	data->jp = coFuncJP_new( "testEntryPoint", EntryPoint_0_Cast );
	coFuncJP_append_AlwaysLast( data->jp, (void*)TestFunc8, "TestFunc8", "testCode" );
	/* - TestFunc8 */
	coFuncJP_append( data->jp, (void*)TestFunc9, "TestFunc9", "testCode" );
	/* TestFunc9 - TestFunc8 */
	coFuncJP_prepend_AlwaysFirst( data->jp, (void*)TestFunc0, "TestFunc0", "testCode" );
	/* TestFunc0 - TestFunc9 - TestFunc8 */
	coFuncJP_prepend( data->jp, (void*)TestFunc1, "TestFunc1", "testCode" );
	/* TestFunc0 - TestFunc1, TestFunc9 - TestFunc8 */

	coUnit_check_true( data->jp->_funcMetas->count == 4 );
	coUnit_check_streq( data->jp->_funcMetas->data[0]->name, "TestFunc0" );
	coUnit_check_true( ((coAspectFunc*)data->jp->_funcMetas->data[0])->funcPtr == TestFunc0 );
	coUnit_check_streq( data->jp->_funcMetas->data[1]->name, "TestFunc1" );
	coUnit_check_true( ((coAspectFunc*)data->jp->_funcMetas->data[1])->funcPtr == TestFunc1 );
	coUnit_check_streq( data->jp->_funcMetas->data[2]->name, "TestFunc9" );
	coUnit_check_true( ((coAspectFunc*)data->jp->_funcMetas->data[2])->funcPtr == TestFunc9 );
	coUnit_check_streq( data->jp->_funcMetas->data[3]->name, "TestFunc8" );
	coUnit_check_true( ((coAspectFunc*)data->jp->_funcMetas->data[3])->funcPtr == TestFunc8 );
	coUnit_check_streq( data->jp->_alwaysFirstFunc->name, "TestFunc0" );
	coUnit_check_true( ((coAspectFunc*)data->jp->_alwaysFirstFunc)->funcPtr == TestFunc0 );
	coUnit_check_streq( data->jp->_alwaysLastFunc->name, "TestFunc8" );
	coUnit_check_true( ((coAspectFunc*)data->jp->_alwaysLastFunc)->funcPtr == TestFunc8 );
}


void FuncJPSuite_TestReplaceAll( FuncJPSuiteData* data ) {
	data->jp = coFuncJP_new( "testEntryPoint", EntryPoint_0_Cast );
	coFuncJP_append( data->jp, (void*)TestFunc0, "TestFunc0", "testCode" );
	coFuncJP_append( data->jp, (void*)TestFunc0, "TestFunc1", "testCode" );
	/* TestFunc0, TestFunc1 */
	coFuncJP_replaceAll( data->jp, (void*)TestFunc2, "TestFunc2", "testCode" );
	/* TestFunc2 */
	coUnit_check_true( data->jp->_funcMetas->count == 1 );
	coUnit_check_streq( data->jp->_funcMetas->data[0]->name, "TestFunc2" );
	coUnit_check_true( ((coAspectFunc*)data->jp->_funcMetas->data[0])->funcPtr == TestFunc2 );
}


void FuncJPSuite_TestPurge( FuncJPSuiteData* data ) {
	data->jp = coFuncJP_new( "testEntryPoint", EntryPoint_0_Cast );
	coFuncJP_append( data->jp, (void*)TestFunc2, "TestFunc2", "testCode" );
	coFuncJP_append( data->jp, (void*)TestFunc3, "TestFunc3", "testCode" );
	/* TestFunc2, TestFunc3 */
	coFuncJP_purge( data->jp );
	/* */
	coUnit_check_true( data->jp->_funcMetas->count == 0 );
}


void FuncJPSuite_TestRun( FuncJPSuiteData* data ) {
	unsigned int hookIndex;

	data->jp = coFuncJP_new( "testEntryPoint", TestFunc_P );
	coFuncJP_append( data->jp, (void*)TestFunc0, "TestFunc0", "testCode" );
	coFuncJP_append( data->jp, (void*)TestFunc1, "TestFunc1", "testCode" );
	coFuncJP_append( data->jp, (void*)TestFunc2, "TestFunc2", "testCode" );
	coFuncJP_append( data->jp, (void*)TestFunc3, "TestFunc3", "testCode" );
	coFuncJP_append( data->jp, (void*)TestFunc4, "TestFunc4", "testCode" );

	coUnit_check_true( data->jp->_funcMetas->count == 5 );

	coFuncJP_run( data->jp, TestFunc_P, data );

	for (hookIndex = 0; hookIndex < data->jp->_funcMetas->count; hookIndex++ ) {
		coUnit_check_true( data->testFuncsRan[hookIndex] == coTrue );
	}
	for (hookIndex = data->jp->_funcMetas->count; hookIndex < NUM_TEST_FUNCS; hookIndex++ ) {
		coUnit_check_true( data->testFuncsRan[hookIndex] == coFalse );
	}
}


#if 0
... Need to implement re-directing a stream to a file for this to work...
void FuncJPSuite_TestPrintConcise( FuncJPSuiteData* data ) {
	coStream*        stream = NULL;
	const char*    testFilename = "testFuncJP-PrintConcise.txt";

	data->jp = coFuncJP_new( "testEntryPoint", TestFunc_P );
	coFuncJP_append( data->jp, (void*)TestFunc0, "TestFunc0", "testCode" );
	coFuncJP_append( data->jp, (void*)TestFunc1, "TestFunc1", "testCode" );
	coFuncJP_append( data->jp, (void*)TestFunc2, "TestFunc2", "testCode" );
	coFuncJP_append( data->jp, (void*)TestFunc3, "TestFunc3", "testCode" );
	coFuncJP_append( data->jp, (void*)TestFunc4, "TestFunc4", "testCode" );

	coUnit_check_true( data->jp->_funcMetas->count == 5 );

	stream = Journal_Register( InfoStream_Type, coFuncJP_Type  );
	Stream_RedirectFile( stream, testFilename );
	coFuncJP_PrintConcise( data->jp, stream );

	if (data->rank==0) {
		char expectedFilename[PCU_PATH_MAX];

		pcu_filename_expected( testFilename, expectedFilename );
		pcu_check_fileEq( testFilename, expectedFilename );
		remove( testFilename );
	}
}
#endif

/******** For the MinMax test *****************/
typedef double (ReturnProto)( coStream* stream );

static double Return1( coStream* stream ) {
	coPrintf( stream, "In func %s\n", __func__ );
	return 1.0;
}

static double Return89( coStream* stream ) {
	coPrintf( stream, "In func %s\n", __func__ );
	return 89.0;
}

static double ReturnNeg43( coStream* stream ) {
	coPrintf( stream, "In func %s\n", __func__ );
	return -43;
}

static double ReturnZero( coStream* stream ) {
	coPrintf( stream, "In func %s\n", __func__ );
	return 0.0;
}


void FuncJPSuite_TestMinMax( FuncJPSuiteData* data ) {
	const char* testEpName = "testEntryPoint";
	double result;
	coStream* stream;

	stream = coOutJrn( "myStream"  );
	/*coStream_enable( stream, coFalse );*/

	data->jp = coFuncJP_new( testEpName, ReturnProto );
	coFuncJP_append( data->jp, Return1, "TestFunc0", "testMinMaxFunc" );
	coFuncJP_append( data->jp, Return89, "TestFunc1", "testMinMaxFunc" );
	coFuncJP_append( data->jp, ReturnNeg43, "TestFunc2", "testMinMaxFunc" );
	coFuncJP_append( data->jp, ReturnZero, "TestFunc3", "testMinMaxFunc" );
	coFuncJP_runFunc( data->jp, ReturnProto, double, result, coRunFuncMax, stream );
	coUnit_check_true( result == 89.0 );
	coObject_delete( data->jp );

	/* Get Minimum of Values */
	data->jp = coFuncJP_new( testEpName, ReturnProto );
	coFuncJP_append( data->jp, Return1, "TestFunc0", "testMinMaxFunc" );
	coFuncJP_append( data->jp, Return89, "TestFunc1", "testMinMaxFunc" );
	coFuncJP_append( data->jp, ReturnNeg43, "TestFunc2", "testMinMaxFunc" );
	coFuncJP_append( data->jp, ReturnZero, "TestFunc3", "testMinMaxFunc" );
	coFuncJP_runFunc( data->jp, ReturnProto, double, result, coRunFuncMin, stream );
	coUnit_check_true( result == -43 );
}

void FuncJPSuite( coUnit* suite ) {
	coUnit_setData( suite, FuncJPSuiteData );
	coUnit_setFixtures( suite, FuncJPSuite_Setup, FuncJPSuite_Teardown );
	coUnit_addTest( suite, FuncJPSuite_TestRunEmpty );
	coUnit_addTest( suite, FuncJPSuite_TestAppendPrepend );
	coUnit_addTest( suite, FuncJPSuite_TestInsertBeforeAfterReplace );
	coUnit_addTest( suite, FuncJPSuite_TestAlwaysFirstLast );
	coUnit_addTest( suite, FuncJPSuite_TestReplaceAll );
	coUnit_addTest( suite, FuncJPSuite_TestPurge );
	coUnit_addTest( suite, FuncJPSuite_TestRun );
#if 0
	coUnit_addTest( suite, FuncJPSuite_TestPrintConcise );
#endif
	coUnit_addTest( suite, FuncJPSuite_TestMinMax );
}



int main( int argc, char* argv[] ) {
	coUnit* unit = coUnit_new();

	FuncJPSuite( unit );
	coUnit_run( unit );

	coObject_delete( unit );
	return 0;
}
