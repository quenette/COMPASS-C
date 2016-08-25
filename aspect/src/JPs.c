/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**
** <copyright>
**
** <license>
**
** $Id$
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

#include <string.h>

/* Textual name of this class */
const char* coJPs_type = "coJPs";

#define ES coErrJrn( coFuncJP_type )


/* Public member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


coJPs* coJPs_new( void ) {
	coJPs* self;
	
	self = (coJPs*)coObject_new( sizeof(coJPs), coJPs_type, _coJPs_delete, _coJPs_print, NULL );

	self->count = 0;
	self->_size = 8;
	self->_delta = 8;
	self->jp = coMalloc( sizeof(coFuncJP*) * self->_size, "coJPs->jp" );
	memset( self->jp, 0, sizeof(coFuncJP*) * self->_size );

	return self;
}

unsigned int coJPs_add( void* jps, void* jp ) {
	coJPs* self = (coJPs*)jps;
	unsigned int handle;
	
	if( self->count >= self->_size ) {
		self->_size += self->_delta;
		self->jp = coRealloc( self->jp, sizeof(coFuncJP*) * self->_size );
	}
	
	handle = self->count;
	self->jp[handle] = (coFuncJP*)jp;
	self->count++;
	
	return handle;
}

unsigned int coJPs_find( void* jps, const char* name, const char* prototypeName ) {
	coJPs* self = (coJPs*)jps;
	unsigned int jp_I;
	
	/* Try integer comparisons of the pointers first. This provides a fast implementation as default. */
	for( jp_I = 0; jp_I < self->count; jp_I++ ) {
		if( self->jp[jp_I]->name == name &&
			self->jp[jp_I]->_option == coFuncJP_InstanceStatic &&
			self->jp[jp_I]->_prototypeName == prototypeName ) {
			return jp_I;
		}
	}

	/* Else try string comparisons */
	for( jp_I = 0; jp_I < self->count; jp_I++ ) {
		if( strcmp( self->jp[jp_I]->name, name ) == 0 &&
			self->jp[jp_I]->_option == coFuncJP_InstanceStatic &&
		    strcmp( self->jp[jp_I]->_prototypeName, prototypeName ) == 0 )
		{
			return jp_I;
		}
	}
	return (unsigned)-1;
}

unsigned int coJPs_findNamed( void* jps, const char* name, const char* prototypeName, char* optionalName ) {
	coJPs* self = (coJPs*)jps;
	unsigned int jp_I;

	/* Try integer comparisons of the pointers first. This provides a fast implementation as default. */
	for( jp_I = 0; jp_I < self->count; jp_I++ ) {
		if( self->jp[jp_I]->name == name &&
			self->jp[jp_I]->_prototypeName == prototypeName &&
			(self->jp[jp_I]->_option == coFuncJP_InstanceNamedGlobal ||
			 self->jp[jp_I]->_option == coFuncJP_InstanceNamedNonGlobal) &&
			self->jp[jp_I]->_optionalName == optionalName )
		{
			return jp_I;
		}
	}

	/* Else try string comparisons */
	for( jp_I = 0; jp_I < self->count; jp_I++ ) {
		if( strcmp( self->jp[jp_I]->name, name ) == 0 &&
		    strcmp( self->jp[jp_I]->_prototypeName, prototypeName ) == 0 &&
			(self->jp[jp_I]->_option == coFuncJP_InstanceNamedGlobal ||
			 self->jp[jp_I]->_option == coFuncJP_InstanceNamedNonGlobal) &&
			strcmp( self->jp[jp_I]->_optionalName, optionalName ) == 0 )
		{
			return jp_I;
		}
	}
	return (unsigned)-1;
}

unsigned int coJPs_findObject( void* jps, const char* name, const char* prototypeName, void* optionalObject ) {
	coJPs* self = (coJPs*)jps;
	unsigned int jp_I;

	/* Try integer comparisons of the pointers first. This provides a fast implementation as default. */
	for( jp_I = 0; jp_I < self->count; jp_I++ ) {
		if( self->jp[jp_I]->name == name &&
			self->jp[jp_I]->_prototypeName == prototypeName &&
			self->jp[jp_I]->_option == coFuncJP_InstanceObject &&
			self->jp[jp_I]->_optionalObject == optionalObject )
		{
			return jp_I;
		}
	}

	/* Else try string comparisons */
	for( jp_I = 0; jp_I < self->count; jp_I++ ) {
		if( strcmp( self->jp[jp_I]->name, name ) == 0 &&
		    strcmp( self->jp[jp_I]->_prototypeName, prototypeName ) == 0 &&
			self->jp[jp_I]->_option == coFuncJP_InstanceObject &&
/* TODO: ... aren't these pointer values? why doesn't it crash??? */
			strcmp( self->jp[jp_I]->_optionalObject, optionalObject ) == 0 )
		{
			return jp_I;
		}
	}
	return (unsigned)-1;
}

coFuncJP* _coJPs_at( void* jps, unsigned int handle ) {
	coJPs* self = (coJPs*)jps;
	
	return coJPs_at( self, handle );
}


unsigned int coJPs_findByJoinPoint( void* jps, void* jp ) {
	coJPs* self = (coJPs*)jps;
	unsigned int ep_I;
	
	for( ep_I = 0; ep_I < self->count; ep_I++ ) {
		if( self->jp[ep_I] == (coFuncJP*)jp ) {
			return ep_I;
		}
	}
	
	return (unsigned int)-1;
}

void coJPs_reset( void* jps ) {
	coJPs* self = (coJPs*)jps;

	/* Assumes ownerships of the elements */
	if( self->jp ) {
		unsigned int jp_I;

		for( jp_I = 0; jp_I < self->count; jp_I++ ) {
			coObject_delete( self->jp[jp_I] );
			self->jp[jp_I] = 0;
		}
	}
	self->count = 0;
}


/* Private member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


void _coJPs_delete( void* jps ) {
	coJPs* self = (coJPs*)jps;

	/* Assumes ownerships of the elements */
	if( self->jp ) {
		unsigned int jp_I;

		for( jp_I = 0; jp_I < self->count; jp_I++ ) {
			coObject_delete( self->jp[jp_I] );
		}

		coFree( self->jp );
	}

	/* Stg_Class_Delete parent */
	_coObject_delete( self );
}

void _coJPs_print( void* jps, coStream* stream, coBool concise ) {
	coJPs* self = (coJPs*)jps;
	unsigned int jp_I;

	if( concise ) {
		/* Prettier, shorter, within a sentence print... */
		_coObject_print( self, stream, coTrue );
		coPrintf( stream, ": { " );
		coPrintf( stream, "jp[0-%u]: ", self->count );
		for( jp_I = 0; jp_I < self->count; jp_I++ ) {
			coObject_print( self->jp[jp_I], stream, coTrue );
			coPrintf( stream, ", ", jp_I );
		}
		coPrintf( stream, " }" );
	}
	else {
		/* Complete object information print... */
		coPrintf( stream, "coJPs (ptr): %p\n", self );
		_coObject_print( self, stream, coFalse );
		coPrintf( stream, "\tcount: %u\n", self->count );
		coPrintf( stream, "\t_size: %lu\n", self->_size );
		coPrintf( stream, "\t_delta: %lu\n", self->_delta );

		coPrintf( stream, "\tjp (ptr): %p\n", self->jp );
		coPrintf( stream, "\tjp[0-%u]:\n", self->count );
		for( jp_I = 0; jp_I < self->count; jp_I++ ) {
			coPrintf( stream, "jp[%u]: ", jp_I );
			coObject_print( self->jp[jp_I], stream, coFalse );
		}
		coPrintf( stream, "\t]\n" );
	}
}
