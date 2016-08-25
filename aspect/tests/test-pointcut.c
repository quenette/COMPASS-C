/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**
** <copyright>
**
** <license>
**
** $Id$
**
** These tests are very purposely the same tests as EntrypointSuite (for StGermain1 EntryPoint points).
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
#include "JPs.h"
#include "joinpoint.h"
#include "mode.h"
#include "Aspects.h"
#include "pointcut.h"
#include "Unit.h"
#include "init.h"
#include "debug.h"
#include "finalise.h"

#include <string.h>

coStream* stream;

#define NUM_TEST_FUNCS 10

typedef struct {
   coBool testFuncsRan[NUM_TEST_FUNCS];
   int  rank;
} pointcutSuiteData;

typedef void (TestProto)( pointcutSuiteData* data );

static void TestFunc0( pointcutSuiteData* data ) {
   data->testFuncsRan[0] = coTrue;
}

static void TestFunc1( pointcutSuiteData* data ) {
   data->testFuncsRan[1] = coTrue;
}

static void TestFunc2( pointcutSuiteData* data ) {
   data->testFuncsRan[2] = coTrue;
}

static void TestFunc3( pointcutSuiteData* data ) {
   data->testFuncsRan[3] = coTrue;
}

static void TestFunc4( pointcutSuiteData* data ) {
   data->testFuncsRan[4] = coTrue;
}

static void TestFunc5( pointcutSuiteData* data ) {
   data->testFuncsRan[5] = coTrue;
}

static void TestFunc6( pointcutSuiteData* data ) {
   data->testFuncsRan[6] = coTrue;
}

static void TestFunc7( pointcutSuiteData* data ) {
   data->testFuncsRan[7] = coTrue;
}

static void TestFunc8( pointcutSuiteData* data ) {
   data->testFuncsRan[8] = coTrue;
}

static void TestFunc9( pointcutSuiteData* data ) {
   data->testFuncsRan[9] = coTrue;
}


void pointcutSuite_Setup( pointcutSuiteData* data ) {
   unsigned int ii;

   for (ii=0; ii < NUM_TEST_FUNCS; ii++ ) {
      data->testFuncsRan[ii] = coFalse;
   }
   data->rank = 0;
}


void pointcutSuite_Teardown( pointcutSuiteData* data ) {
   /* Reset the register - mirror the EP tests need to work from scratch */
   coAspectReset();
}


void pointcutSuite_TestRunEmpty( pointcutSuiteData* data ) {
	coWeave();
	pointcut( TestProto, 0 );
	coUnit_check_true( coAspects_s->jps->count == 1 ); /* unique to pointcut test */
	coUnit_check_true( coAspects_s->jps->jp[0]->_funcMetas->count == 0 );
}


static void someFunc() {}


void pointcutSuite_TestAppendPrepend( pointcutSuiteData* data ) {
   unsigned int ii = 0;

   advice( someFunc, TestProto, TestFunc0, coAdviceAppend );
   /* TestFunc0 */
   advice( someFunc, TestProto, TestFunc1, coAdvicePrepend );
   /* TestFunc1, TestFunc0 */

   coUnit_check_true( coAspects_s->jps->count == 1 ); /* unique to pointcut test */
   coUnit_check_true( coAspects_s->jps->jp[0]->_funcMetas->count == 2 );
   coUnit_check_streq( coAspects_s->jps->jp[0]->_funcMetas->data[0]->name, "TestFunc1" );
   coUnit_check_true( ((coAspectFunc*)coAspects_s->jps->jp[0]->_funcMetas->data[0])->funcPtr == TestFunc1 );
   coUnit_check_streq( coAspects_s->jps->jp[0]->_funcMetas->data[1]->name, "TestFunc0" );
   coUnit_check_true( ((coAspectFunc*)coAspects_s->jps->jp[0]->_funcMetas->data[1])->funcPtr == TestFunc0 );
   for( ii = 0; ii < coAspects_s->jps->jp[0]->_funcMetas->count; ii++ ) {
      coUnit_check_streq( ((coAspectFunc*)coAspects_s->jps->jp[0]->_funcMetas->data[ii])->fromFunc, __func__ );
   }
}

void pointcutSuite_TestInsertBeforeAfterReplace( pointcutSuiteData* data ) {
   advice( someFunc, TestProto, TestFunc2, coAdviceAppend );
   advice( someFunc, TestProto, TestFunc3, coAdviceAppend );
   /* TestFunc2, TestFunc3 */
   advice( someFunc, TestProto, TestFunc4, coAdvicePrepend );
   /* TestFunc4, TestFunc2, TestFunc3 */
   radvice( someFunc, TestProto, TestFunc5, coAdviceInsertBefore, "TestFunc3" );
   /* TestFunc4, TestFunc2, TestFunc5, TestFunc3 */
   radvice( someFunc, TestProto, TestFunc6, coAdviceInsertAfter, "TestFunc4" );
   /* TestFunc4, TestFunc6, TestFunc2, TestFunc5, TestFunc3 */
   radvice( someFunc, TestProto, TestFunc7, coAdviceReplace, "TestFunc5" );
   /* TestFunc4, TestFunc6, TestFunc2, TestFunc7, TestFunc3 */

   coUnit_check_true( coAspects_s->jps->count == 1 ); /* unique to pointcut test */
   coUnit_check_true( coAspects_s->jps->jp[0]->_funcMetas->count == 5 );
   coUnit_check_streq( coAspects_s->jps->jp[0]->_funcMetas->data[0]->name, "TestFunc4" );
   coUnit_check_true( ((coAspectFunc*)coAspects_s->jps->jp[0]->_funcMetas->data[0])->funcPtr == TestFunc4 );
   coUnit_check_streq( coAspects_s->jps->jp[0]->_funcMetas->data[1]->name, "TestFunc6" );
   coUnit_check_true( ((coAspectFunc*)coAspects_s->jps->jp[0]->_funcMetas->data[1])->funcPtr == TestFunc6 );
   coUnit_check_streq( coAspects_s->jps->jp[0]->_funcMetas->data[2]->name, "TestFunc2" );
   coUnit_check_true( ((coAspectFunc*)coAspects_s->jps->jp[0]->_funcMetas->data[2])->funcPtr == TestFunc2 );
   coUnit_check_streq( coAspects_s->jps->jp[0]->_funcMetas->data[3]->name, "TestFunc7" );
   coUnit_check_true( ((coAspectFunc*)coAspects_s->jps->jp[0]->_funcMetas->data[3])->funcPtr == TestFunc7 );
   coUnit_check_streq( coAspects_s->jps->jp[0]->_funcMetas->data[4]->name, "TestFunc3" );
   coUnit_check_true( ((coAspectFunc*)coAspects_s->jps->jp[0]->_funcMetas->data[4])->funcPtr == TestFunc3 );
}


void pointcutSuite_TestAlwaysFirstLast( pointcutSuiteData* data ) {
   advice( someFunc, TestProto, TestFunc8, coAdviceAlwaysLast );
   /* - TestFunc8 */
   advice( someFunc, TestProto, TestFunc9, coAdviceAppend );
   /* TestFunc9 - TestFunc8 */
   advice( someFunc, TestProto, TestFunc0, coAdviceAlwaysFirst );
   /* TestFunc0 - TestFunc9 - TestFunc8 */
   advice( someFunc, TestProto, TestFunc1, coAdvicePrepend );
   /* TestFunc0 - TestFunc1, TestFunc9 - TestFunc8 */

   coUnit_check_true( coAspects_s->jps->count == 1 ); /* unique to pointcut test */
   coUnit_check_true( coAspects_s->jps->jp[0]->_funcMetas->count == 4 );
   coUnit_check_streq( coAspects_s->jps->jp[0]->_funcMetas->data[0]->name, "TestFunc0" );
   coUnit_check_true( ((coAspectFunc*)coAspects_s->jps->jp[0]->_funcMetas->data[0])->funcPtr == TestFunc0 );
   coUnit_check_streq( coAspects_s->jps->jp[0]->_funcMetas->data[1]->name, "TestFunc1" );
   coUnit_check_true( ((coAspectFunc*)coAspects_s->jps->jp[0]->_funcMetas->data[1])->funcPtr == TestFunc1 );
   coUnit_check_streq( coAspects_s->jps->jp[0]->_funcMetas->data[2]->name, "TestFunc9" );
   coUnit_check_true( ((coAspectFunc*)coAspects_s->jps->jp[0]->_funcMetas->data[2])->funcPtr == TestFunc9 );
   coUnit_check_streq( coAspects_s->jps->jp[0]->_funcMetas->data[3]->name, "TestFunc8" );
   coUnit_check_true( ((coAspectFunc*)coAspects_s->jps->jp[0]->_funcMetas->data[3])->funcPtr == TestFunc8 );
   coUnit_check_streq( coAspects_s->jps->jp[0]->_alwaysFirstFunc->name, "TestFunc0" );
   coUnit_check_true( ((coAspectFunc*)coAspects_s->jps->jp[0]->_alwaysFirstFunc)->funcPtr == TestFunc0 );
   coUnit_check_streq( coAspects_s->jps->jp[0]->_alwaysLastFunc->name, "TestFunc8" );
   coUnit_check_true( ((coAspectFunc*)coAspects_s->jps->jp[0]->_alwaysLastFunc)->funcPtr == TestFunc8 );
}


void pointcutSuite_TestReplaceAll( pointcutSuiteData* data ) {
   advice( someFunc, TestProto, TestFunc0, coAdviceAppend );
   advice( someFunc, TestProto, TestFunc1, coAdviceAppend );
   /* TestFunc0, TestFunc1 */
   advice( someFunc, TestProto, TestFunc2, coAdviceAround );
   /* TestFunc2 */
   coUnit_check_true( coAspects_s->jps->count == 1 ); /* unique to pointcut test */
   coUnit_check_true( coAspects_s->jps->jp[0]->_funcMetas->count == 1 );
   coUnit_check_streq( coAspects_s->jps->jp[0]->_funcMetas->data[0]->name, "TestFunc2" );
   coUnit_check_true( ((coAspectFunc*)coAspects_s->jps->jp[0]->_funcMetas->data[0])->funcPtr == TestFunc2 );
}


void pointcutSuite_TestPurge( pointcutSuiteData* data ) {
   advice( someFunc, TestProto, TestFunc2, coAdviceAppend );
   advice( someFunc, TestProto, TestFunc3, coAdviceAppend );
   /* TestFunc2, TestFunc3 */
   coFuncJP_purge( coAspects_s->jps->jp[0] );
   /* */
   coUnit_check_true( coAspects_s->jps->count == 1 ); /* unique to pointcut test */
   coUnit_check_true( coAspects_s->jps->jp[0]->_funcMetas->count == 0 );
}


void pointcutSuite_TestRun( pointcutSuiteData* data ) {
   unsigned int funcIndex;

   advice( pointcutSuite_TestRun, TestProto, TestFunc0, coAdviceAppend );
   advice( pointcutSuite_TestRun, TestProto, TestFunc1, coAdviceAppend );
   advice( pointcutSuite_TestRun, TestProto, TestFunc2, coAdviceAppend );
   advice( pointcutSuite_TestRun, TestProto, TestFunc3, coAdviceAppend );
   advice( pointcutSuite_TestRun, TestProto, TestFunc4, coAdviceAppend );
   
   coUnit_check_true( coAspects_s->jps->count == 1 ); /* unique to pointcut test */
   coUnit_check_true( coAspects_s->jps->jp[0]->_funcMetas->count == 5 );

   coWeave();

   pointcut( TestProto, data );

   for( funcIndex = 0; funcIndex < coAspects_s->jps->jp[0]->_funcMetas->count; funcIndex++ ) {
      coUnit_check_true( data->testFuncsRan[funcIndex] == coTrue );
   }
   for( funcIndex = coAspects_s->jps->jp[0]->_funcMetas->count; funcIndex < NUM_TEST_FUNCS; funcIndex++ ) {
      coUnit_check_true( data->testFuncsRan[funcIndex] == coFalse );
   }
}


#if 0
... Need to implement re-directing a stream to a file for this to work...
static void testEntryPoint( pointcutSuiteData* data ) {
   /* Note: this function would be called pointcutSuite_TestPrintConcise and non-static as seems
      to be the convention, but, such that it can rely on the same test file 
      ("testEP-PrintConcise.txt") as for the EntryPoint and coEP tests, I've named this function
      such that "pointcut" yeilds the same result. "testCode" exists for the same purpose. */
   coStream*        stream = NULL;
   const char*    testFilename = "testPointCut-PrintConcise.txt";

   testCode();
   
   coUnit_check_true( coAspects_s->jps->count == 1 ); /* unique to pointcut test */
   coUnit_check_true( coAspects_s->jps->jp[0]->_funcMetas->count == 5 );

   stream = Journal_Register( InfoStream_Type, (Name)coEP_Type  );
   Stream_RedirectFile( stream, testFilename );
   coEP_PrintConcise( coAspects_s->jps->jp[0], stream );

   if( data->rank == 0 ) {
      char expectedFilename[PCU_PATH_MAX];

      pcu_filename_expected( testFilename, expectedFilename );
      pcu_check_fileEq( testFilename, expectedFilename );
      remove( testFilename );
   }
}
#endif

/******** For the MinMax test *****************/
typedef double (ReturnProto)( coStream* stream );
typedef double (ReturnProto2)( coStream* stream ); /* trick to enable more than one pc in a function */

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

void pointcutSuite_TestMinMax( pointcutSuiteData* data ) {
   double      result;
   coStream*     stream;

   stream = coOutJrn( "myStream"  );
   /*Stream_Enable( stream, coFalse );*/

   advice( pointcutSuite_TestMinMax, ReturnProto, Return1, coAdviceAppend );
   advice( pointcutSuite_TestMinMax, ReturnProto, Return89, coAdviceAppend );
   advice( pointcutSuite_TestMinMax, ReturnProto, ReturnNeg43, coAdviceAppend );
   advice( pointcutSuite_TestMinMax, ReturnProto, ReturnZero, coAdviceAppend );
   coWeave();
   pointcutCollab( ReturnProto, double, result, coPointcutMax, stream );
   coUnit_check_true( result == 89.0 );

   coAspectReset();

   /* Get Minimum of Values - note use of 2nd proto, to differential the point cuts in this function */
   advice( pointcutSuite_TestMinMax, ReturnProto2, Return1, coAdviceAppend );
   advice( pointcutSuite_TestMinMax, ReturnProto2, Return89, coAdviceAppend );
   advice( pointcutSuite_TestMinMax, ReturnProto2, ReturnNeg43, coAdviceAppend );
   advice( pointcutSuite_TestMinMax, ReturnProto2, ReturnZero, coAdviceAppend );
   coWeave();
   pointcutCollab( ReturnProto2, double, result, coPointcutMin, stream );
   coUnit_check_true( result == -43 );
}

void pointcutSuite( coUnit* suite ) {
   coUnit_setData( suite, pointcutSuiteData );
   coUnit_setFixtures( suite, pointcutSuite_Setup, pointcutSuite_Teardown );
   coUnit_addTest( suite, pointcutSuite_TestRunEmpty );
   coUnit_addTest( suite, pointcutSuite_TestAppendPrepend );
   coUnit_addTest( suite, pointcutSuite_TestInsertBeforeAfterReplace );
   coUnit_addTest( suite, pointcutSuite_TestAlwaysFirstLast );
   coUnit_addTest( suite, pointcutSuite_TestReplaceAll );
   coUnit_addTest( suite, pointcutSuite_TestPurge );
   coUnit_addTest( suite, pointcutSuite_TestRun );
#if 0
   coUnit_addTest( suite, testEntryPoint/*pointcutSuite_TestPrintConcise*/ );
#endif
   coUnit_addTest( suite, pointcutSuite_TestMinMax );
}

int main( int argc, char* argv[] ) {
	coUnit* unit;

	coAspectInit( &argc, &argv, coStaticWeave | coStaticAdvice | coStaticPointcut );
	unit = coUnit_new();
	pointcutSuite( unit );

	coUnit_run( unit );
	someFunc(); /* called just so the warning goes away - does nothing */

	coObject_delete( unit );
	coAspectFinalise();
	return 0;
}

