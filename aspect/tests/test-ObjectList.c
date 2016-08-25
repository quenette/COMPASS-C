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
#include "Unit.h"
#include "ObjectList.h"

#include <string.h>

const const char* DummyClass_type = "DummyClass_type";


typedef struct {
	coObjectList* ol0;
	coObjectList* ol1;
	coObjectList* ol2;
	coObjectList* ol3;
	coObject* addPtr0;
	coObject* addPtr1;
#if 0 /*Yet to implement */
	int*            addPtr2;
	int*            addPtr3;
#endif
} ObjectListSuiteData;

coObject* DummyObject_new( const char* name ) {
	return coObject_new( sizeof(coObject), DummyClass_type, _coObject_delete, _coObject_print, name );
}

int* DummyPointer_new( void ) {
	return (int*)coMalloc( sizeof(int) * 1, DummyClass_type );
}

void DummyPointer_print( void* ptr, coStream* stream, coBool concise ) {
	if( concise ) {
		_coObject_print( ptr, stream, coTrue );
		coPrintf( stream, "value: %i", *(int*)ptr );
	}
	else {
		_coObject_print( ptr, stream, coFalse );
		coPrintf( stream, "value: %i\n", *(int*)ptr );
	}
}


void DummyFunc1( void ) {
}

void DummyFunc2( void ) {
}

void DummyFunc3( void ) {
}

void DummyFunc4( void ) {
}

void DummyFunc5( void ) {
}

void DummyFunc6( void ) {
}


void ObjectListSuite_Setup( ObjectListSuiteData* data ) {
	data->ol0 = coOL_new();
	data->ol1 = coOL_new();
	data->ol2 = coOL_new();
	data->ol3 = coOL_new();
#if 0 /*Yet to implement */
	data->addPtr0 = NULL;
	data->addPtr1 = NULL;
	data->addPtr2 = NULL;
	data->addPtr3 = NULL;
#endif
}


void ObjectListSuite_Teardown( ObjectListSuiteData* data ) {
      coObject_delete( data->ol3 );
      coObject_delete( data->ol2 );
      coObject_delete( data->ol1 );
      coObject_delete( data->ol0 );
}


/* Test 2: Can we append the first entry? */
void ObjectListSuite_TestAppend( ObjectListSuiteData* data ) {
	coOL_append( data->ol0, (data->addPtr0 = DummyObject_new( "a" )) );
	coOL_append( data->ol1, (data->addPtr1 = DummyObject_new( "a" )) );
#if 0 /*Yet to implement */
	coOL_PointerAppend( data->ol2, (data->addPtr2 = DummyPointer_new()), (Name)"a", 0, DummyPointer_Print, DummyPointer_Copy  );
	coOL_GlobalPointerAppend( data->ol3, (data->addPtr3 = (void*)DummyFunc1), (Name)"a"  );
#endif
	coUnit_check_true(
		data->addPtr0 == coOL_get( data->ol0, "a" ) &&
		data->addPtr0 == coOL_at( data->ol0, 0 ) &&
		coOL_count( data->ol0 ) == 1 &&
		data->addPtr1 == coOL_get( data->ol1, "a" ) &&
		data->addPtr1 == coOL_at( data->ol1, 0 ) &&
		coOL_count( data->ol1 ) == 1
#if 0 /*Yet to implement */
		&&
		data->addPtr2 == coOL_Get( data->ol2, (Name)"a"  ) &&
		data->addPtr2 == coOL_ObjectAt( data->ol2, 0 ) &&
		coOL_Count( data->ol2 ) == 1 &&
		data->addPtr3 == coOL_Get( data->ol3, (Name)"a"  ) &&
		data->addPtr3 == coOL_ObjectAt( data->ol3, 0 ) &&
		coOL_Count( data->ol3 ) == 1
#endif
	);
}


/* Test 3: Can we prepend the second entry? */
void ObjectListSuite_TestPrepend( ObjectListSuiteData* data ) {
   coOL_append( data->ol0, (data->addPtr0 = DummyObject_new( "a" )) );
   coOL_append( data->ol1, (data->addPtr1 = DummyObject_new( "a" )) );
#if 0 /*Yet to implement */
   coOL_PointerAppend( data->ol2, (data->addPtr2 = DummyPointer_new()), (Name)"a", 0, DummyPointer_Print, DummyPointer_Copy  );
   coOL_GlobalPointerAppend( data->ol3, (data->addPtr3 = (void*)DummyFunc1), (Name)"a"  );
#endif

   coOL_prepend( data->ol0, (data->addPtr0 = DummyObject_new( "b" )) );
   coOL_prepend( data->ol1, (data->addPtr1 = DummyObject_new( "b" )) );
#if 0 /*Yet to implement */
   coOL_PointerPrepend(  data->ol2, (data->addPtr2 = DummyPointer_new()), "b", 0, DummyPointer_Print, DummyPointer_Copy );
   coOL_GlobalPointerPrepend(  data->ol3, (data->addPtr3 = (void*)DummyFunc2), "b" );
#endif

   coUnit_check_true(
      data->addPtr0 == coOL_get( data->ol0, "b" ) &&
      data->addPtr0 == coOL_at( data->ol0, 0 ) &&
      coOL_count( data->ol0 ) == 2 &&
      strcmp( coObject_getName( (coObject*)coOL_at( data->ol0, 1 ) ), "a" ) == 0 );
   coUnit_check_true(
      data->addPtr1 == coOL_get( data->ol1, "b" ) &&
      data->addPtr1 == coOL_at( data->ol1, 0 ) &&
      coOL_count( data->ol1 ) == 2 &&
      strcmp( coObject_getName( (coObject*)coOL_at( data->ol1, 1 ) ), "a" ) == 0 );
#if 0 /*Yet to implement */
   pcu_check_true(
      data->addPtr2 == coOL_Get( data->ol2, (Name)"b"  ) &&
      data->addPtr2 == coOL_ObjectAt( data->ol2, 0 ) &&
      coOL_Count( data->ol2 ) == 2 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol2, 1 ) ), "a" ) == 0 );
   pcu_check_true(
      data->addPtr3 == coOL_Get( data->ol3, (Name)"b"  ) &&
      data->addPtr3 == coOL_ObjectAt( data->ol3, 0 ) &&
      coOL_Count( data->ol3 ) == 2 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol3, 1 ) ), "a" ) == 0 );
#endif
}


/* Test 4: Can we insert before "a" the third entry? */
void ObjectListSuite_TestInsertBefore( ObjectListSuiteData* data ) {
#if 0 /*Yet to implement */
   coOL_ClassAppend( data->ol0, (data->addPtr0 = DummyClass_new()), (Name)"b"  );
   coOL_ClassAppend( &data->ol1, (data->addPtr1 = DummyClass_new()), (Name)"b"  );
   coOL_PointerAppend( data->ol2, (data->addPtr2 = DummyPointer_new()), (Name)"b", 0, DummyPointer_Print, DummyPointer_Copy  );
   coOL_GlobalPointerAppend( data->ol3, (data->addPtr3 = (void*)DummyFunc1), (Name)"b"  );
   coOL_ClassAppend( data->ol0, (data->addPtr0 = DummyClass_new()), (Name)"a"  );
   coOL_ClassAppend( &data->ol1, (data->addPtr1 = DummyClass_new()), (Name)"a"  );
   coOL_PointerAppend( data->ol2, (data->addPtr2 = DummyPointer_new()), (Name)"a", 0, DummyPointer_Print, DummyPointer_Copy  );
   coOL_GlobalPointerAppend( data->ol3, (data->addPtr3 = (void*)DummyFunc1), (Name)"a"  );

   coOL_ClassInsertBefore(  data->ol0, "a", (data->addPtr0 = DummyClass_new()), "c" );
   coOL_ClassInsertBefore( &data->ol1, "a", (data->addPtr1 = DummyClass_new()), "c" );
   coOL_PointerInsertBefore(
      data->ol2,
      "a",
      (data->addPtr2 = DummyPointer_new()),
      "c",
      0,
      DummyPointer_Print,
      DummyPointer_Copy );
   coOL_GlobalPointerInsertBefore(  data->ol3, "a", (data->addPtr3 = (void*)DummyFunc3), "c" );

   pcu_check_true(
      data->addPtr0 == coOL_Get( data->ol0, (Name)"c"  ) &&
      data->addPtr0 == coOL_ObjectAt( data->ol0, 1 ) &&
      coOL_Count( data->ol0 ) == 3 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol0, 0 ) ), "b" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol0, 2 ) ), "a" ) == 0 );
   pcu_check_true(
      data->addPtr1 == coOL_Get( &data->ol1, (Name)"c"  ) &&
      data->addPtr1 == coOL_ObjectAt( &data->ol1, 1 ) &&
      coOL_Count( &data->ol1 ) == 3 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( &data->ol1, 0 ) ), "b" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( &data->ol1, 2 ) ), "a" ) == 0 );
   pcu_check_true(
      data->addPtr2 == coOL_Get( data->ol2, (Name)"c"  ) &&
      data->addPtr2 == coOL_ObjectAt( data->ol2, 1 ) &&
      coOL_Count( data->ol2 ) == 3 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol2, 0 ) ), "b" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol2, 2 ) ), "a" ) == 0 );
   pcu_check_true(
      data->addPtr3 == coOL_Get( data->ol3, (Name)"c"  ) &&
      data->addPtr3 == coOL_ObjectAt( data->ol3, 1 ) &&
      coOL_Count( data->ol3 ) == 3 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol3, 0 ) ), "b" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol3, 2 ) ), "a" ) == 0 );
#endif
}


/* Test 5: Can we insert after "c" the fourth entry? */
void ObjectListSuite_TestInsertAfter( ObjectListSuiteData* data ) {
#if 0 /*Yet to implement */
   coOL_ClassAppend( data->ol0, (data->addPtr0 = DummyClass_new()), (Name)"b"  );
   coOL_ClassAppend( &data->ol1, (data->addPtr1 = DummyClass_new()), (Name)"b"  );
   coOL_PointerAppend( data->ol2, (data->addPtr2 = DummyPointer_new()), (Name)"b", 0, DummyPointer_Print, DummyPointer_Copy  );
   coOL_GlobalPointerAppend( data->ol3, (data->addPtr3 = (void*)DummyFunc1), (Name)"b"  );
   coOL_ClassAppend( data->ol0, (data->addPtr0 = DummyClass_new()), (Name)"c"  );
   coOL_ClassAppend( &data->ol1, (data->addPtr1 = DummyClass_new()), (Name)"c"  );
   coOL_PointerAppend( data->ol2, (data->addPtr2 = DummyPointer_new()), (Name)"c", 0, DummyPointer_Print, DummyPointer_Copy  );
   coOL_GlobalPointerAppend( data->ol3, (data->addPtr3 = (void*)DummyFunc1), (Name)"c"  );
   coOL_ClassAppend( data->ol0, (data->addPtr0 = DummyClass_new()), (Name)"a"  );
   coOL_ClassAppend( &data->ol1, (data->addPtr1 = DummyClass_new()), (Name)"a"  );
   coOL_PointerAppend( data->ol2, (data->addPtr2 = DummyPointer_new()), (Name)"a", 0, DummyPointer_Print, DummyPointer_Copy  );
   coOL_GlobalPointerAppend( data->ol3, (data->addPtr3 = (void*)DummyFunc1), (Name)"a"  );

   coOL_ClassInsertAfter(  data->ol0, "c", (data->addPtr0 = DummyClass_new()), "d" );
   coOL_ClassInsertAfter( &data->ol1, "c", (data->addPtr1 = DummyClass_new()), "d" );

   coOL_PointerInsertAfter(
      data->ol2,
      "c",
      (data->addPtr2 = DummyPointer_new()),
      "d",
      0,
      DummyPointer_Print,
      DummyPointer_Copy );
   coOL_GlobalPointerInsertAfter(  data->ol3, "c", (data->addPtr3 = (void*)DummyFunc4), "d" );

   pcu_check_true(
      data->addPtr0 == coOL_Get( data->ol0, (Name)"d"  ) &&
      data->addPtr0 == coOL_ObjectAt( data->ol0, 2 ) &&
      coOL_Count( data->ol0 ) == 4 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol0, 0 ) ), "b" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol0, 1 ) ), "c" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol0, 3 ) ), "a" ) == 0 );
   pcu_check_true(
      data->addPtr1 == coOL_Get( &data->ol1, (Name)"d"  ) &&
      data->addPtr1 == coOL_ObjectAt( &data->ol1, 2 ) &&
      coOL_Count( &data->ol1 ) == 4 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( &data->ol1, 0 ) ), "b" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( &data->ol1, 1 ) ), "c" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( &data->ol1, 3 ) ), "a" ) == 0 );
   pcu_check_true(
      data->addPtr2 == coOL_Get( data->ol2, (Name)"d"  ) &&
      data->addPtr2 == coOL_ObjectAt( data->ol2, 2 ) &&
      coOL_Count( data->ol2 ) == 4 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol2, 0 ) ), "b" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol2, 1 ) ), "c" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol2, 3 ) ), "a" ) == 0 );
   pcu_check_true(
      data->addPtr3 == coOL_Get( data->ol3, (Name)"d"  ) &&
      data->addPtr3 == coOL_ObjectAt( data->ol3, 2 ) &&
      coOL_Count( data->ol3 ) == 4 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol3, 0 ) ), "b" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol3, 1 ) ), "c" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol3, 3 ) ), "a" ) == 0 );
#endif
}


/* Test 6: Can we replace "d" with the fifth entry? */
void ObjectListSuite_TestReplace( ObjectListSuiteData* data ) {
#if 0 /*Yet to implement */
   coOL_ClassAppend( data->ol0, (data->addPtr0 = DummyClass_new()), (Name)"b"  );
   coOL_ClassAppend( &data->ol1, (data->addPtr1 = DummyClass_new()), (Name)"b"  );
   coOL_PointerAppend( data->ol2, (data->addPtr2 = DummyPointer_new()), (Name)"b", 0, DummyPointer_Print, DummyPointer_Copy  );
   coOL_GlobalPointerAppend( data->ol3, (data->addPtr3 = (void*)DummyFunc1), (Name)"b"  );
   coOL_ClassAppend( data->ol0, (data->addPtr0 = DummyClass_new()), (Name)"c"  );
   coOL_ClassAppend( &data->ol1, (data->addPtr1 = DummyClass_new()), (Name)"c"  );
   coOL_PointerAppend( data->ol2, (data->addPtr2 = DummyPointer_new()), (Name)"c", 0, DummyPointer_Print, DummyPointer_Copy  );
   coOL_GlobalPointerAppend( data->ol3, (data->addPtr3 = (void*)DummyFunc1), (Name)"c"  );
   coOL_ClassAppend( data->ol0, (data->addPtr0 = DummyClass_new()), (Name)"d"  );
   coOL_ClassAppend( &data->ol1, (data->addPtr1 = DummyClass_new()), (Name)"d"  );
   coOL_PointerAppend( data->ol2, (data->addPtr2 = DummyPointer_new()), (Name)"d", 0, DummyPointer_Print, DummyPointer_Copy  );
   coOL_GlobalPointerAppend( data->ol3, (data->addPtr3 = (void*)DummyFunc1), (Name)"d"  );
   coOL_ClassAppend( data->ol0, (data->addPtr0 = DummyClass_new()), (Name)"a"  );
   coOL_ClassAppend( &data->ol1, (data->addPtr1 = DummyClass_new()), (Name)"a"  );
   coOL_PointerAppend( data->ol2, (data->addPtr2 = DummyPointer_new()), (Name)"a", 0, DummyPointer_Print, DummyPointer_Copy  );
   coOL_GlobalPointerAppend( data->ol3, (data->addPtr3 = (void*)DummyFunc1), (Name)"a"  );

   coOL_ClassReplace(  data->ol0, "d", DELETE, (data->addPtr0 = DummyClass_new()), "e" );
   coOL_ClassReplace( &data->ol1, "d", DELETE, (data->addPtr1 = DummyClass_new()), "e" );
   coOL_PointerReplace( data->ol2, "d", DELETE, (data->addPtr2 = DummyPointer_new()), (Name)"e", 0, DummyPointer_Print, DummyPointer_Copy  );
   coOL_GlobalPointerReplace( data->ol3, (Name)"d", DELETE, (data->addPtr3 = (void*)DummyFunc5), (Name)"e"  );
   pcu_check_true(
      data->addPtr0 == coOL_Get( data->ol0, (Name)"e"  ) &&
      data->addPtr0 == coOL_ObjectAt( data->ol0, 2 ) &&
      coOL_Count( data->ol0 ) == 4 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol0, 0 ) ), "b" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol0, 1 ) ), "c" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol0, 3 ) ), "a" ) == 0 );
   pcu_check_true(
      data->addPtr1 == coOL_Get( &data->ol1, (Name)"e"  ) &&
      data->addPtr1 == coOL_ObjectAt( &data->ol1, 2 ) &&
      coOL_Count( &data->ol1 ) == 4 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( &data->ol1, 0 ) ), "b" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( &data->ol1, 1 ) ), "c" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( &data->ol1, 3 ) ), "a" ) == 0 );
   pcu_check_true(
      data->addPtr2 == coOL_Get( data->ol2, (Name)"e"  ) &&
      data->addPtr2 == coOL_ObjectAt( data->ol2, 2 ) &&
      coOL_Count( data->ol2 ) == 4 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol2, 0 ) ), "b" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol2, 1 ) ), "c" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol2, 3 ) ), "a" ) == 0 );
   pcu_check_true(
      data->addPtr3 == coOL_Get( data->ol3, (Name)"e"  ) &&
      data->addPtr3 == coOL_ObjectAt( data->ol3, 2 ) &&
      coOL_Count( data->ol3 ) == 4 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol3, 0 ) ), "b" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol3, 1 ) ), "c" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol3, 3 ) ), "a" ) == 0 );
#endif
}


/* Test 7: Can we remove the "c" entry? */
void ObjectListSuite_TestRemove( ObjectListSuiteData* data ) {
#if 0 /*Yet to implement */
   coOL_ClassAppend( data->ol0, (data->addPtr0 = DummyClass_new()), (Name)"b"  );
   coOL_ClassAppend( &data->ol1, (data->addPtr1 = DummyClass_new()), (Name)"b"  );
   coOL_PointerAppend( data->ol2, (data->addPtr2 = DummyPointer_new()), (Name)"b", 0, DummyPointer_Print, DummyPointer_Copy  );
   coOL_GlobalPointerAppend( data->ol3, (data->addPtr3 = (void*)DummyFunc1), (Name)"b"  );
   coOL_ClassAppend( data->ol0, (data->addPtr0 = DummyClass_new()), (Name)"c"  );
   coOL_ClassAppend( &data->ol1, (data->addPtr1 = DummyClass_new()), (Name)"c"  );
   coOL_PointerAppend( data->ol2, (data->addPtr2 = DummyPointer_new()), (Name)"c", 0, DummyPointer_Print, DummyPointer_Copy  );
   coOL_GlobalPointerAppend( data->ol3, (data->addPtr3 = (void*)DummyFunc1), (Name)"c"  );
   coOL_ClassAppend( data->ol0, (data->addPtr0 = DummyClass_new()), (Name)"e"  );
   coOL_ClassAppend( &data->ol1, (data->addPtr1 = DummyClass_new()), (Name)"e"  );
   coOL_PointerAppend( data->ol2, (data->addPtr2 = DummyPointer_new()), (Name)"e", 0, DummyPointer_Print, DummyPointer_Copy  );
   coOL_GlobalPointerAppend( data->ol3, (data->addPtr3 = (void*)DummyFunc1), (Name)"e"  );
   coOL_ClassAppend( data->ol0, (data->addPtr0 = DummyClass_new()), (Name)"a"  );
   coOL_ClassAppend( &data->ol1, (data->addPtr1 = DummyClass_new()), (Name)"a"  );
   coOL_PointerAppend( data->ol2, (data->addPtr2 = DummyPointer_new()), (Name)"a", 0, DummyPointer_Print, DummyPointer_Copy  );
   coOL_GlobalPointerAppend( data->ol3, (data->addPtr3 = (void*)DummyFunc1), (Name)"a"  );

   coOL_Remove( data->ol0, (Name)"c", DELETE  );
   coOL_Remove( &data->ol1, (Name)"c", DELETE  );
   coOL_Remove( data->ol2, (Name)"c", DELETE  );
   coOL_Remove( data->ol3, (Name)"c", DELETE );

   pcu_check_true(
      coOL_Count( data->ol0 ) == 3 &&
      strcmp( Stg_Object_GetName( (Stg_Object* )coOL_At( data->ol0, 0 ) ), "b" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol0, 1 ) ), "e" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol0, 2 ) ), "a" ) == 0 );
   pcu_check_true(
      coOL_Count( &data->ol1 ) == 3 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( &data->ol1, 0 ) ), "b" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( &data->ol1, 1 ) ), "e" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( &data->ol1, 2 ) ), "a" ) == 0 );
   pcu_check_true(
      coOL_Count( data->ol2 ) == 3 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol2, 0 ) ), "b" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol2, 1 ) ), "e" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol2, 2 ) ), "a" ) == 0 );
   pcu_check_true(
      coOL_Count( data->ol3 ) == 3 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol3, 0 ) ), "b" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol3, 1 ) ), "e" ) == 0 &&
      strcmp( Stg_Object_GetName( (Stg_Object*)coOL_At( data->ol3, 2 ) ), "a" ) == 0 );
#endif
}


   /* Test 8: Can we replace all with the sixth entry? */
void ObjectListSuite_TestReplaceAll( ObjectListSuiteData* data ) {
#if 0 /*Yet to implement */
   coOL_ClassAppend( data->ol0, (data->addPtr0 = DummyClass_new()), (Name)"b"  );
   coOL_ClassAppend( &data->ol1, (data->addPtr1 = DummyClass_new()), (Name)"b"  );
   coOL_PointerAppend( data->ol2, (data->addPtr2 = DummyPointer_new()), (Name)"b", 0, DummyPointer_Print, DummyPointer_Copy  );
   coOL_GlobalPointerAppend( data->ol3, (data->addPtr3 = (void*)DummyFunc1), (Name)"b"  );
   coOL_ClassAppend( data->ol0, (data->addPtr0 = DummyClass_new()), (Name)"e"  );
   coOL_ClassAppend( &data->ol1, (data->addPtr1 = DummyClass_new()), (Name)"e"  );
   coOL_PointerAppend( data->ol2, (data->addPtr2 = DummyPointer_new()), (Name)"e", 0, DummyPointer_Print, DummyPointer_Copy  );
   coOL_GlobalPointerAppend( data->ol3, (data->addPtr3 = (void*)DummyFunc1), (Name)"e"  );
   coOL_ClassAppend( data->ol0, (data->addPtr0 = DummyClass_new()), (Name)"a"  );
   coOL_ClassAppend( &data->ol1, (data->addPtr1 = DummyClass_new()), (Name)"a"  );
   coOL_PointerAppend( data->ol2, (data->addPtr2 = DummyPointer_new()), (Name)"a", 0, DummyPointer_Print, DummyPointer_Copy  );
   coOL_GlobalPointerAppend( data->ol3, (data->addPtr3 = (void*)DummyFunc1), (Name)"a"  );

   coOL_ClassReplaceAll( data->ol0, DELETE, (data->addPtr0 = DummyClass_new()), (Name)"f"  );
   coOL_ClassReplaceAll( &data->ol1, DELETE, (data->addPtr1 = DummyClass_new()), (Name)"f"  );
   coOL_PointerReplaceAll(
      data->ol2,
      DELETE,
      (data->addPtr2 = DummyPointer_new()),
      "f",
      0,
      DummyPointer_Print,
      DummyPointer_Copy );
   coOL_GlobalPointerReplaceAll(  data->ol3, DELETE, (data->addPtr3 = (void*)DummyFunc6), "f" );
   pcu_check_true(
      data->addPtr0 == coOL_Get( data->ol0, (Name)"f"  ) &&
      data->addPtr0 == coOL_ObjectAt( data->ol0, 0 ) &&
      coOL_Count( data->ol0 ) == 1 );
   pcu_check_true(
      data->addPtr1 == coOL_Get( &data->ol1, (Name)"f"  ) &&
      data->addPtr1 == coOL_ObjectAt( &data->ol1, 0 ) &&
      coOL_Count( &data->ol1 ) == 1 );
   pcu_check_true(
      data->addPtr2 == coOL_Get( data->ol2, (Name)"f"  ) &&
      data->addPtr2 == coOL_ObjectAt( data->ol2, 0 ) &&
      coOL_Count( data->ol2 ) == 1 );
   pcu_check_true(
      data->addPtr3 == coOL_Get( data->ol3, (Name)"f"  ) &&
      data->addPtr3 == coOL_ObjectAt( data->ol3, 0 ) &&
      coOL_Count( data->ol3 ) == 1 );
#endif
}


void ObjectListSuite( coUnit* unit ) {
   coUnit_setData( unit, ObjectListSuiteData );
   coUnit_setFixtures( unit, ObjectListSuite_Setup, ObjectListSuite_Teardown );
   coUnit_addTest( unit, ObjectListSuite_TestAppend );
   coUnit_addTest( unit, ObjectListSuite_TestPrepend );
   coUnit_addTest( unit, ObjectListSuite_TestInsertBefore );
   coUnit_addTest( unit, ObjectListSuite_TestInsertAfter );
   coUnit_addTest( unit, ObjectListSuite_TestReplace );
   coUnit_addTest( unit, ObjectListSuite_TestRemove );
   coUnit_addTest( unit, ObjectListSuite_TestReplaceAll );
}


int main( int argc, char* argv[] ) {
	coUnit* unit = coUnit_new();

	ObjectListSuite( unit );
	coUnit_run( unit );

	coObject_delete( unit );
	return 0;
}
