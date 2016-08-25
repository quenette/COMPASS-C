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
#include "AspectFunc.h"

#include <assert.h>

const char* coAspectFunc_type = "coAspectFunc";


coAspectFunc* coAspectFunc_new( const char* name, void* funcPtr, const char* fromFunc ) {
	coAspectFunc* self;

	self = (coAspectFunc*)coObject_new( sizeof(coAspectFunc), coAspectFunc_type, _coAspectFunc_delete, _coAspectFunc_print, name );
	self->funcPtr = funcPtr;
	self->fromFunc = coStrdup( fromFunc );
	self->hasAspect = coFalse;
	self->fromAspect = NULL;

	return self;
}


coAspectFunc* coAspectFunc_newWithAspect( const char* name, void* funcPtr, const char* fromFunc, void* fromAspect ) {
	coAspectFunc* self;

	self = (coAspectFunc*)coObject_new( sizeof(coAspectFunc), coAspectFunc_type, _coAspectFunc_delete, _coAspectFunc_print, name );
	self->funcPtr = funcPtr;
	self->fromFunc = coStrdup( fromFunc );
	self->hasAspect = coTrue;
	self->fromAspect = fromAspect;

	return self;
}


/* Private member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


void _coAspectFunc_delete( void* aspectFunc ) {
	coAspectFunc* self = (coAspectFunc*)aspectFunc;
	assert( self );
	
	coFree( self->fromFunc );
	/* Stg_Class_Delete parent */
	_coObject_delete( self );
}

void _coAspectFunc_print( void* aspectFunc, coStream* stream, coBool concise ) {
	coAspectFunc* self = (coAspectFunc*)aspectFunc;
	assert( self );
	
	if( concise ) {
		/* Prettier, shorter, within a sentence print... */
		_coObject_print( self, stream, coTrue );
		coPrintf( stream, " [%s", self->fromFunc );
		if( self->hasAspect ) {
			coPrintf( stream, "(ptr)%p", self->fromAspect );
		}
		coPrintf( stream, "]" );
	}
	else {
		/* Complete object information print... */
		coPrintf( stream, "coAspectFunc (ptr) - %p:\n", self );
		_coObject_print( self, stream, coFalse );
		coPrintf( stream, "\tfuncPtr (func ptr): %p\n", self->funcPtr );
		coPrintf( stream, "\tfromFunc: %s\n", self->fromFunc );
		coPrintf( stream, "\thasAspect (ptr): %p\n", self->hasAspect );
		if( self->hasAspect ) {
			coPrintf( stream, "\tfromAspect (ptr): %p\n", self->fromAspect );
		}
	}
}

