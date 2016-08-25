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

#include <assert.h>
#include <string.h>


const char* coObject_type = "coObject";

static unsigned int _coObject_Counter = 0;
static const char _coObject_Unnamed[] = "Unnamed";


/* Public member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


coObject* coObject_new( 
		size_t _sizeOfSelf,
		const char* type, 
		coObject_delete_FP* _delete, 
		coObject_print_FP* _print, 
		const char* name ) 
{
	coObject* self;
	
	/* Allocate memory */
	assert( _sizeOfSelf >= sizeof(coObject) );
	self = (coObject*)coMalloc( _sizeOfSelf, type );
	memset( self, 0, _sizeOfSelf );
	self->type = type;
	self->_sizeOfSelf = _sizeOfSelf;
	self->_deleteSelf = coTrue;
	self->_delete = _delete;
	self->_print = _print;
	if( !name || strlen( name ) == 0 ) {
		coAsprintf( &self->name, "%s-%u", _coObject_Unnamed, _coObject_Counter );
		_coObject_Counter += 1;
	}
	else {
		self->name = coStrdup( name );
	}
	return self;
}


void coObject_delete( void* object ) {
	coObject* self = (coObject*)object;

	self->_delete( self );
}


void coObject_print( void* object, coStream* stream, coBool concise ) {
	coObject* self = (coObject*)object;

	self->_print( self, stream, concise );
}


const char* coObject_getType( void* object ) {
	coObject* self = (coObject*)object;

	return self->type;
}


void coObject_setName( void* object, const char* name ) {
	coObject* self = (coObject*)object;
	
	coFree( self->name );
	self->name = coStrdup( name );
}


const char* coObject_getName( void* object ) {
	coObject* self = (coObject*)object;

	return self->name;
}


/* Private member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


void _coObject_delete( void* object ) {
	coObject* self = (coObject*)object;
	
	coFree( self->name );
	if( self->_deleteSelf ) {
		coFree( self );
	}
}


void _coObject_print( void* object, coStream* stream, coBool concise ) {
	coObject* self = (coObject*)object;

	if( concise ) {
		/* Prettier, shorter, within a sentence print... */
		coPrintf( stream, "\"%s\" (%p(ptr))", self->name, self );
	}
	else {
		/* Complete object information print... */
		coPrintf( stream, "coObject (ptr): %p\n", self );
		coPrintf( stream, "_sizeOfSelf: %lu\n", self->_sizeOfSelf );
		coPrintf( stream, "_deleteSelf: %s\n", self->_deleteSelf ? "Yes" : "No" );
		coPrintf( stream, "type: %s\n", self->type );
		coPrintf( stream, "_delete (func ptr): %p\n", self->_delete );
		coPrintf( stream, "_print (func ptr): %p\n", self->_print );
		coPrintf( stream, "name: %s\n", self->name );
	}
}
