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

#include <string.h>

const char* coUnit_type = "coUnit";


/* Public member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


coUnit* coUnit_new( void ) {
	coUnit* self;
	
	/* Allocate memory */
	self = (coUnit*)coObject_new( sizeof(coUnit), coUnit_type, _coUnit_delete, _coUnit_print, NULL );
	self->_setup = NULL;
	self->_teardown = NULL;
	self->_data = NULL;
	self->_count = 0;
	self->_size = 8;
	self->_func = (coUnit_test_FP**)coMalloc( sizeof(coUnit_test_FP*) * self->_size, coUnit_type );

	return self;
}

void _coUnit_setDataImp( void* unit, size_t size ) {
	coUnit* self = (coUnit*)unit;

	if( self->_data ) {
		coFree( self->_data );
	}
	self->_data = coMalloc( size, coUnit_type );
}

void _coUnit_addTestImp( void* unit, coUnit_test_FP* func, const char* name ) {
	/* "name" currently ignored! */
	coUnit* self = (coUnit*)unit;

	assert( self->_count <= self->_size );
	if( self->_count == self->_size ) {
		coUnit_test_FP** newFunc;

		self->_size += 8;
		newFunc = (coUnit_test_FP**)coMalloc( sizeof(coUnit_test_FP*) * self->_size, coUnit_type );
		memset( newFunc, 0, sizeof(coUnit_test_FP*) * self->_size );
		memcpy( newFunc, self->_func, sizeof(coUnit_test_FP*) * self->_count );
		coFree( self->_func );
		self->_func = newFunc;
	}

	self->_func[self->_count] = func;
	self->_count += 1;
}

void _coUnit_setFixturesImp( void* unit, coUnit_fixture_FP* setup, coUnit_fixture_FP* teardown ) {
	coUnit* self = (coUnit*)unit;

	self->_setup = setup;
	self->_teardown = teardown;
}


coBool coUnit_run( void* unit ) {
	coUnit* self = (coUnit*)unit;
	unsigned int i;

	for( i = 0; i < self->_count; i++ ){
		if( self->_setup ) {
			self->_setup( self->_data );
		}

		self->_func[i]( self->_data );

		if( self->_teardown ) {
			self->_teardown( self->_data );
		}
	}

	return coTrue; /* the idea being that a better version of this returns fail if any checks fail */
}


/* Private member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


void _coUnit_delete( void* unit ) {
	coUnit* self = (coUnit*)unit;
	
	coFree( self->_func );
	if( self->_data ) {
		coFree( self->_data );
	}

	_coObject_delete( self );
}


void _coUnit_print( void* unit, coStream* stream, coBool concise ) {
	coUnit* self = (coUnit*)unit;
	unsigned int i;
	
	if( concise ) {
		/* Prettier, shorter, within a sentence print... */
		_coObject_print( self, stream, coTrue );
		coPrintf( stream, " [ " );
		for( i = 0; i < self->_size; i++ ) {
			coPrintf( stream, "(funcPtr):%p, ", self->_func[i] );
		}
		coPrintf( stream, "]" );
	}
	else {
		/* Complete object information print... */
		coPrintf( stream, "coUnit (ptr): %p\n", self );
		coPrintf( stream, "_setup (func ptr): %p\n", self->_setup );
		coPrintf( stream, "_teardown (func ptr): %p\n", self->_teardown );
		coPrintf( stream, "_count: %u\n", self->_count );
		coPrintf( stream, "_size: %p\n", self->_size );
		for( i = 0; i < self->_size; i++ ) {
			coPrintf( stream, "\t_func[%u] (func ptr): %p\n", i, self->_func[i] );
		}
	}
}



