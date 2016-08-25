/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**
** Copyright (C) Steve Quenette 2011
**
** <license>
**
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "memory.h"
#include "journal.h"
#include "Object.h"
#include "ObjectList.h"

#include <assert.h>
#include <string.h>


const char* coOL_type = "coObjectList";

#define ES coErrJrn( coOL_type )

static unsigned int _append( void* objectList, void* objectPtr );
static unsigned int _prepend( void* objectList, void* objectPtr );
static unsigned int _replaceAll( void* objectList, coOL_Former option, void* objectPtr );
static unsigned int _replace( void* objectList, const char* toReplace, coOL_Former option, void* objectPtr );
static unsigned int _insertBefore( void* objectList, const char* reference, void* objectPtr );
static unsigned int _insertAfter( void* objectList, const char* reference, void* objectPtr );
static unsigned int _remove( void* objectList, const char* reference, coOL_Former option );
static unsigned int _getIndex( void* objectList, const const char* toGet );
static void* _get( void* objectList, const const char* toGet );
static void _deleteAllObjects( void* objectList );
static void _allocMoreMemory( void* objectList );
static void _insertAtIndex( void* objects, unsigned int index, void* objectPtr );
static void _removeByIndex( void* objects, unsigned int index, coOL_Former option );

#if 0 /*Yet to implement */
static void GlobalPrint( void* ptr, coStream* stream );
#endif


/* Public member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


coObjectList* coOL_new() {
	coObjectList* self;
	
	self = (coObjectList*)coObject_new( sizeof(coObjectList), coOL_type, _coOL_delete, _coOL_print, NULL );
	self->count = 0;
	self->_size = 16;
	self->_delta = 16;
	self->data = (coObject**)coMalloc( sizeof(coObject*) * self->_size, "coObjectList->data" );

	return self;
}


unsigned int coOL_append( void* objectList, void* objectPtr ) {
	coObjectList* self = (coObjectList*) objectList;
	
	return _append( self, objectPtr );
}	

#if 0 /*Yet to implement */
unsigned int coOL_pointerAppend(
		void*						objectList, 
		void*						objectPtr, 
		const char*						name,
		Stg_ObjectAdaptor_DeletePointerFunction*	ptrDelete,
		Stg_ObjectAdaptor_PrintPointerFunction*		ptrPrint )
{
	coObjectList* self = (coObjectList*) objectList;
	
	return coOL_Append( self, Stg_ObjectAdaptor_NewOfPointer( objectPtr, name, True, False, ptrDelete, ptrPrint, ptrCopy ) );
}

unsigned int coOL_globalPointerAppend( void* objectList, void* objectPtr, const char* name ) {
	coObjectList* self = (coObjectList*) objectList;
	
	return coOL_Append( self, Stg_ObjectAdaptor_NewOfPointer( objectPtr, name, False, True, 0, GlobalPrint, 0 ) );
}
#endif


unsigned int coOL_prepend( void* objectList, void* objectPtr ) {
	coObjectList* self = (coObjectList*) objectList;
	
	return _prepend( self, objectPtr );
}

#if 0 /*Yet to implement */
unsigned int coOL_pointerPrepend(
		void*						objectList, 
		void*						objectPtr, 
		const char*						name,
		Stg_ObjectAdaptor_DeletePointerFunction*		ptrDelete,
		Stg_ObjectAdaptor_PrintPointerFunction*		ptrPrint )
{
	coObjectList* self = (coObjectList*) objectList;
	
	return coOL_Prepend( self, Stg_ObjectAdaptor_NewOfPointer( objectPtr, name, True, False, ptrDelete, ptrPrint, ptrCopy ) );
}

unsigned int coOL_globalPointerPrepend( void* objectList, void* objectPtr, const char* name ) {
	coObjectList* self = (coObjectList*) objectList;
	
	return coOL_Prepend( self, Stg_ObjectAdaptor_NewOfPointer( objectPtr, name, False, True, 0, GlobalPrint, 0 ) );
}
#endif


unsigned int coOL_replaceAll( void* objectList, coOL_Former option, void* objectPtr ) {
	coObjectList* self = (coObjectList*) objectList;
	
	return _replaceAll( self, option, objectPtr );
}

#if 0 /*Yet to implement */
unsigned int coOL_pointerReplaceAll(
		void*						objectList, 
		coOL_Former				option,
		void*						objectPtr, 
		const char*						name,
		Stg_ObjectAdaptor_DeletePointerFunction*		ptrDelete,
		Stg_ObjectAdaptor_PrintPointerFunction*		ptrPrint)
{
	coObjectList* self = (coObjectList*) objectList;
	
	return coOL_ReplaceAll(
		self, 
		option, 
		Stg_ObjectAdaptor_NewOfPointer( objectPtr, name, True, False, ptrDelete, ptrPrint, ptrCopy ) );
}

unsigned int coOL_globalPointerReplaceAll( void* objectList, coOL_Former option, void* objectPtr, const char* name ) {
	coObjectList* self = (coObjectList*) objectList;
	
	return coOL_ReplaceAll( self, option, Stg_ObjectAdaptor_NewOfPointer( objectPtr, name, False, True, 0, GlobalPrint, 0 ) );
}
#endif


unsigned int coOL_replace(
		void*						objectList, 
		const char*						toReplace,
		coOL_Former				option,
		void*						objectPtr )
{
	coObjectList* self = (coObjectList*) objectList;
	
	return _replace( self, toReplace, option, objectPtr );
}

#if 0 /*Yet to implement */
unsigned int coOL_pointerReplace(
		void*						objectList, 
		const char*						toReplace,
		coOL_Former				option,
		void*						objectPtr, 
		const char*						name,
		Stg_ObjectAdaptor_DeletePointerFunction*		ptrDelete,
		Stg_ObjectAdaptor_PrintPointerFunction*		ptrPrint )
{
	coObjectList* self = (coObjectList*) objectList;
	
	return coOL_Replace(
		self, 
		toReplace, 
		option, 
		Stg_ObjectAdaptor_NewOfPointer( objectPtr, name, True, False, ptrDelete, ptrPrint, ptrCopy ) );
}

unsigned int coOL_globalPointerReplace(
		void*						objectList, 
		const char*						toReplace,
		coOL_Former				option,
		void*						objectPtr, 
		const char*						name )
{
	coObjectList* self = (coObjectList*) objectList;
	
	return coOL_Replace(
		self, 
		toReplace, 
		option, 
		Stg_ObjectAdaptor_NewOfPointer( objectPtr, name, False, True, 0, GlobalPrint, 0 ) );
}
#endif


unsigned int coOL_insertBefore( void* objectList, const char* reference, void* objectPtr ) {
	coObjectList* self = (coObjectList*) objectList;
	
	return _insertBefore( self, reference, objectPtr );
}

#if 0 /*Yet to implement */
unsigned int coOL_pointerInsertBefore(
		void*						objectList, 
		const char*						reference,
		void*						objectPtr, 
		const char*						name,
		Stg_ObjectAdaptor_DeletePointerFunction*		ptrDelete,
		Stg_ObjectAdaptor_PrintPointerFunction*		ptrPrint )
{
	coObjectList* self = (coObjectList*) objectList;
	
	return coOL_InsertBefore(
		self, 
		reference, 
		Stg_ObjectAdaptor_NewOfPointer( objectPtr, name, True, False, ptrDelete, ptrPrint, ptrCopy ) );
}

unsigned int coOL_globalPointerInsertBefore( void* objectList, const char* reference, void* objectPtr, const char* name ) {
	coObjectList* self = (coObjectList*) objectList;
	
	return coOL_InsertBefore(
		self, 
		reference, 
		Stg_ObjectAdaptor_NewOfPointer( objectPtr, name, False, True, 0, GlobalPrint, 0 ) );
}
#endif


unsigned int coOL_insertAfter( void* objectList, const char* reference, void* objectPtr ) {
	coObjectList* self = (coObjectList*) objectList;
	
	return _insertAfter( self, reference, objectPtr );
}


#if 0 /*Yet to implement */
unsigned int coOL_pointerInsertAfter(
		void*						objectList, 
		const char*						reference,
		void*						objectPtr, 
		const char*						name,
		Stg_ObjectAdaptor_DeletePointerFunction*		ptrDelete,
		Stg_ObjectAdaptor_PrintPointerFunction*		ptrPrint )
{
	coObjectList* self = (coObjectList*) objectList;
	
	return coOL_InsertAfter(
		self, 
		reference, 
		Stg_ObjectAdaptor_NewOfPointer( objectPtr, name, True, False, ptrDelete, ptrPrint, ptrCopy ) );
}

unsigned int coOL_globalPointerInsertAfter( void* objectList, const char* reference, void* objectPtr, const char* name ) {
	coObjectList* self = (coObjectList*) objectList;
	
	return coOL_InsertAfter(
		self, 
		reference, 
		Stg_ObjectAdaptor_NewOfPointer( objectPtr, name, False, True, 0, GlobalPrint, 0 ) );
}
#endif


void coOL_insertAtIndex( void* objects, unsigned int index, void* objectPtr ) {
	coObjectList* self = (coObjectList*) objects;

	/* Runs error checking and then inserts */
	assert( objectPtr );
	assert( self->count <= self->_size );
	if ( self->count == self->_size ) {
		_allocMoreMemory( self );
	}

	_insertAtIndex( self, index, objectPtr );
}


unsigned int coOL_remove( void* objectList, const char* reference, coOL_Former option ) {
	coObjectList* self = (coObjectList*) objectList;

	return _remove( self, reference, option );
}

void coOL_removeByIndex( void* objects, unsigned int index, coOL_Former option ) {
	coObjectList* self = (coObjectList*) objects;

	return _removeByIndex( self, index, option );
}


unsigned int coOL_getIndex( void* objectList, const const char* toGet ) {
	coObjectList* self = (coObjectList*) objectList;

	return _getIndex( self, toGet );
}


void* coOL_get( void* objectList, const const char* objectName ) {
	coObjectList* self = (coObjectList*) objectList;

	return _get( self, objectName );
}


void coOL_deleteAllObjects( void* objectList ) {
	coObjectList* self = (coObjectList*) objectList;

	_deleteAllObjects( self );
}


void coOL_printAllEntryNames( void* objectList, coStream* stream ) {
	coObjectList* self = (coObjectList*)objectList;
	unsigned int object_I;

	coPrintf( stream, "[" );
	for( object_I = 0 ; object_I < self->count - 1 ; object_I++ ) {
		coPrintf( stream, "\"%s\"(%s), ", self->data[ object_I ]->name, self->data[ object_I ]->type );
	}
	if( self->count >= 0 ) {
		coPrintf( stream, "\"%s\"(%s)", self->data[ object_I ]->name, self->data[ object_I ]->type );
	}
	coPrintf( stream, "]" );
}


void coOL_printAllObjects( void* objectList, coStream* stream ) {
	coObjectList* self = (coObjectList*)objectList;
	unsigned int objectIndex = 0;
	
	for( objectIndex = 0; objectIndex < self->count; objectIndex++ ) {
		coObject_print( self->data[ objectIndex ], stream, coFalse ) ;
	}
}


void* coOL_atFunc( void* objectList, unsigned int index ) {
	coObjectList* self = (coObjectList*)objectList;
	
	return coOL_atMacro( self, index );
}


unsigned int coOL_countFunc( void* objectList ) {
	coObjectList* self = (coObjectList*)objectList;
	
	return coOL_countMacro( self );
}


/* Private member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


static unsigned int _append( void* objectList, void* objectPtr ) {
	coObjectList* self = (coObjectList*) objectList;

	assert( objectPtr );
	assert( self->count <= self->_size );
	if( self->count == self->_size ) {
		_allocMoreMemory( self );
	}

	self->data[self->count] = (coObject*)objectPtr;

	return (self->count)++;
}


static unsigned int _prepend( void* objectList, void* objectPtr ) {
	coObjectList* self = (coObjectList*) objectList;

	assert( objectPtr );
	assert( self->count <= self->_size );
	if( self->count == self->_size ) {
		_allocMoreMemory( self );
	}

	_insertAtIndex( self, 0, objectPtr );
	return 0;
}


static unsigned int _replaceAll( void* objectList, coOL_Former option, void* objectPtr ) {
	coObjectList* self = (coObjectList*) objectList;
	unsigned int objectIndex;

	assert( objectPtr );
	for( objectIndex = 0; objectIndex < self->count; objectIndex++ ) {
		if ( coOL_FormerDelete == option ) {
			coObject_delete( self->data[objectIndex] );
		}
		self->data[objectIndex] = 0;
	}
	self->data[0] = (coObject*) objectPtr;
	self->count = 1;
	return 0;
}


static unsigned int _replace( void* objectList, const char* toReplace, coOL_Former option, void* objectPtr ) {
	coObjectList* self = (coObjectList*) objectList;
	unsigned int objectIndex;

	assert( objectPtr );
	for( objectIndex = 0; objectIndex < self->count; objectIndex++ ) {
		if ( toReplace == self->data[objectIndex]->name ) {
			if ( coOL_FormerDelete == option ) {
				coObject_delete( self->data[objectIndex] );
			}
			self->data[objectIndex] = (coObject*) objectPtr;
			return objectIndex;
		}
	}
	for( objectIndex = 0; objectIndex < self->count; objectIndex++ ) {
		if (0 == strcmp( toReplace, self->data[objectIndex]->name ) ) {
			if ( coOL_FormerDelete == option ) {
				coObject_delete( self->data[objectIndex] );
			}
			self->data[objectIndex] = (coObject*) objectPtr;
			return objectIndex;
		}
	}

	fprintf( stderr, "Error: %s(): specified object \"%s\" to replace not found.\n",
		__func__, toReplace ); 
	assert( 0 );
	return (unsigned int) -1;
}


static unsigned int _insertBefore( void* objectList, const char* reference, void* objectPtr ) {
	coObjectList* self = (coObjectList*) objectList;
	unsigned int objectIndex;

	assert( objectPtr );
	assert( self->count <= self->_size );
	if ( self->count == self->_size ) {
		_allocMoreMemory( self );
	}

	for( objectIndex = 0; objectIndex < self->count; objectIndex++ ) {
		if ( reference == self->data[objectIndex]->name ) {
			_insertAtIndex( self, objectIndex, objectPtr );
			return objectIndex;
		}
	}
	for( objectIndex = 0; objectIndex < self->count; objectIndex++ ) {
		if (0 == strcmp( reference, self->data[objectIndex]->name ) ) {
			_insertAtIndex( self, objectIndex, objectPtr );
			return objectIndex;
		}
	}

	fprintf( stderr, "Error: %s(): specified object \"%s\" to insert before not found.\n",
		__func__, reference ); 
	assert( 0 );
	return (unsigned int) -1;
}


static unsigned int _insertAfter( void* objectList, const char* reference, void* objectPtr ) {
	coObjectList* self = (coObjectList*) objectList;
	unsigned int objectIndex;

	assert( objectPtr );
	assert( self->count <= self->_size );
	if ( self->count == self->_size ) {
		_allocMoreMemory( self );
	}

	for( objectIndex = 0; objectIndex < self->count; objectIndex++ ) {
		if ( reference == self->data[objectIndex]->name ) {
			_insertAtIndex( self, objectIndex+1, objectPtr );
			return objectIndex;
		}
	}
	for( objectIndex = 0; objectIndex < self->count; objectIndex++ ) {
		if (0 == strcmp( reference, self->data[objectIndex]->name ) ) {
			_insertAtIndex( self, objectIndex+1, objectPtr );
			return objectIndex;
		}
	}

	fprintf( stderr, "Error: %s(): specified object \"%s\" to insert after not found.\n",
		__func__, reference ); 
	assert( 0 );
	return (unsigned int) -1;
}

static unsigned int _remove( void* objectList, const char* reference, coOL_Former option ) {
	coObjectList* self        = (coObjectList*) objectList;
	unsigned int            objectIndex;

	objectIndex = _getIndex( self, reference );

	coFirewall( objectIndex != (unsigned int)-1, ES,
			"Error: %s(): specified object \"%s\" to remove not found.\n", __func__, reference ); 
	
	_removeByIndex( self, objectIndex, option );

	return objectIndex;
}


static unsigned int _getIndex( void* objectList, const char* toGet ) {
	coObjectList* self = (coObjectList*) objectList;
	unsigned int objectIndex;
	
	/* Find the object and return it */
	for( objectIndex = 0; objectIndex < self->count; objectIndex++ ) {
		if ( toGet == self->data[objectIndex]->name ) return objectIndex;
	}
	for( objectIndex = 0; objectIndex < self->count; objectIndex++ ) {
		if (0 == strcmp( toGet, self->data[objectIndex]->name ) ) return objectIndex;
	}
			
	return (unsigned int) -1;
}


static void* _get( void* objectList, const char* toGet ) {
        coObjectList* self = (coObjectList*) objectList;
        unsigned int objectIndex;
                                                                                                                                    
        /* Find the object and return it */
        for( objectIndex = 0; objectIndex < self->count; objectIndex++ ) { 
			if ( toGet == self->data[objectIndex]->name ) { 
#if 0 /*Yet to implement */
				if ( self->data[objectIndex]->type == Stg_ObjectAdaptor_type ) {
					return Stg_ObjectAdaptor_Object( (Stg_ObjectAdaptor*)self->data[objectIndex] ); 
				} 
				else { 
					return self->data[objectIndex]; 
				} 
#else
				return self->data[objectIndex];
#endif
			}
        }
        for( objectIndex = 0; objectIndex < self->count; objectIndex++ ) { 
			if (0 == strcmp( toGet, self->data[objectIndex]->name ) ) { 
#if 0 /*Yet to implement */
				if ( self->data[objectIndex]->type == Stg_ObjectAdaptor_type ) {
					return Stg_ObjectAdaptor_Object( (Stg_ObjectAdaptor*)self->data[objectIndex] ); 
				} 
				else { 
					return self->data[objectIndex]; 
				} 
#else
				return self->data[objectIndex];
#endif
			}
        }
                                                                                                                                    
        return NULL;
}

static void _deleteAllObjects( void* objectList ) {
	coObjectList* self = (coObjectList*) objectList;
	unsigned int objectIndex;
	
	for( objectIndex = 0; objectIndex < self->count; objectIndex++ ) {
		if( self->data[objectIndex] ) {
			coObject_delete( self->data[objectIndex] );
			self->data[objectIndex] = 0;
		}
	}
	self->count = 0;
}


static void _allocMoreMemory( void* objectList ) {
	coObjectList* self = (coObjectList*) objectList;

	self->_size += self->_delta;
	self->data = coRealloc( self->data, sizeof(coObject*) * self->_size );
	assert( self->data );
}



static void _insertAtIndex( void* objects, unsigned int index, void* objectPtr ) {
	coObjectList* self = (coObjectList*) objects;
	
	/* Move current content down one position, and add  */
	if (index != self->count) {
		memmove( &self->data[index+1], &self->data[index], sizeof(coObject*) * (self->count - index) );
	}

	self->data[index] = (coObject*) objectPtr;
	self->count++;
}

static void _removeByIndex( void* objects, unsigned int index, coOL_Former option ) {
	coObjectList* self = (coObjectList*) objects;
	
	assert( index < self->count );

	if ( option == coOL_FormerDelete )
		coObject_delete( self->data[index] );
	
	/* Move current content up one position */
	if (index != self->count - 1) {
		memmove( &self->data[index], &self->data[index+1], sizeof(coObject*) * (self->count - index - 1) );
	}

	self->count--;
}

/* Private member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if 0 /*Yet to implement */
/* Print method for "global pointers" that are being wrapped as an object */
static void GlobalPrint( void* ptr, coStream* stream ) {
	coPrintf( stream, "(ptr): %p\n", ptr );
}
#endif

/* Delete implementation */
void _coOL_delete( void* objectList ) {
	coObjectList* self = (coObjectList*) objectList;

	coOL_deleteAllObjects( self );
	coFree( self->data );

	_coObject_delete( self );
}


/* Print implementation */
void _coOL_print( void* objectList, coStream* stream, coBool concise ) {
	coObjectList* self = (coObjectList*) objectList;

	if( concise ) {
		_coObject_print( self, stream, coTrue );
		coPrintf( stream, ": { " );
		coOL_printAllEntryNames( self, stream );
		coPrintf( stream, " }" );
	}
	else {
		coPrintf( stream, "coObjectList (ptr):%p\n", (void*)self );
		_coObject_print( self, stream, coFalse );
		coPrintf( stream, "_size: %u\n", self->_size );
		coPrintf( stream, "_delta: %u\n", self->_delta );
		coPrintf( stream, "count: %u\n", self->count );
		coPrintf( stream, "data[0-%d]:\n", self->count-1 );
		coOL_printAllObjects( self, stream );
	}
}



