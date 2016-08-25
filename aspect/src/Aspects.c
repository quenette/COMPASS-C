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
#include "joinpoint.h"
#include "mode.h"
#include "Aspects.h"

#include <string.h>

/* Textual name of this class */
const char* coAspects_type = "coAspects";

coAspects* coAspects_s = NULL;


/* Public member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


coAspects* coAspects_new( unsigned int mode ) {
	coAspects* self;
	
	self = (coAspects*)coObject_new( sizeof(coAspects), coAspects_type, _coAspects_delete, _coAspects_print, NULL );
	self->mode = mode;
	self->count = 0;
	self->_size = 8;
	self->_delta = 8;
	self->advices = coMalloc( sizeof(coAdviceEntry) * self->_size, "coAspects->advices" );
	memset( self->advices, 0, sizeof(coAdviceEntry) * self->_size );
	self->jps = coJPs_new();
	coObject_setName( self->jps, "jps" );
	self->hasWeaved = coFalse;
	self->_pcStackTop = 0;
	self->_pcStackSize = 8;
	self->_pcStackDelta = 8;
	self->_pcStack = coMalloc( sizeof(coFuncJP*) * self->_pcStackSize, "coAspects->_pcStack" );
	memset( self->_pcStack, 0, sizeof(coFuncJP*) * self->_pcStackSize );

	return self;
}

unsigned int coAspects_addAdvice( void* aspects, const char* file, const char* func, unsigned int line, coJoinpointBinding binding, void* id ) {
	coAspects* self = (coAspects*)aspects;
	unsigned int handle;
	
	/* If the mode is static weave, then ensure the weave has NOT happened */
	coFirewall(
			self->mode & coStaticWeave ? !self->hasWeaved : 1,
			coErrJrn( "coAspects" ),
			"Error: \"advice\" called but \"static weave\" has already happened yet! In %s, %s:%u\n", func, file, line );

	if( self->count >= self->_size ) {
		self->_size += self->_delta;
		self->advices = coRealloc( self->advices, sizeof(coAdviceEntry) * self->_size );
	}
	
	handle = self->count;
	self->advices[handle].file = coStrdup( file );
	self->advices[handle].func = coStrdup( func );
	self->advices[handle].line = line;
	self->advices[handle].aspect = NULL;
	self->advices[handle].binding = binding;
	self->advices[handle].id = id;
	self->count++;
	
	return handle;
}

unsigned int coAspects_addAdviceAspect( void* aspects, const char* file, const char* func, unsigned int line, void* aspect, coJoinpointBinding binding, void* id ) {
	coAspects* self = (coAspects*)aspects;
	unsigned int handle;

	/* If the mode is static weave, then ensure the weave has NOT happened */
	coFirewall(
			self->mode & coStaticWeave ? !self->hasWeaved : 1,
			coErrJrn( "coAspects" ),
			"Error: \"advice\" called but \"static weave\" has already happened yet! In %s, %s:%u\n", func, file, line );

	/* If the mode is static advice, then an aspectual advice is not permitted */
	coFirewall(
			(self->mode & coStaticAdvice) == 0,
			coErrJrn( "coAspects" ),
			"Error: Aspectual \"advice\" called but the mode is for static advice only! In %s, %s:%u\n", func, file, line );

	if( self->count >= self->_size ) {
		self->_size += self->_delta;
		self->advices = coRealloc( self->advices, sizeof(coAdviceEntry) * self->_size );
	}

	handle = self->count;
	self->advices[handle].file = coStrdup( file );
	self->advices[handle].func = coStrdup( func );
	self->advices[handle].line = line;
	self->advices[handle].aspect = aspect;
	self->advices[handle].binding = binding;
	self->advices[handle].id = id;
	self->count++;

	return handle;
}

unsigned int coAspects_findAdvice( void* aspects, const char* func, unsigned int line ) {
	coAspects* self = (coAspects*)aspects;
	unsigned int advice_I;
	
	for( advice_I = 0; advice_I < self->count; advice_I++ ) {
		if(
			strcmp( self->advices[advice_I].func, func ) == 0 &&
			self->advices[advice_I].line == line &&
			self->advices[advice_I].aspect == NULL )
		{
			return advice_I;
		}
	}
	return (unsigned)-1;
}

unsigned int coAspects_findAdviceAspect( void* aspects, const char* func, unsigned int line, void* aspect ) {
	coAspects* self = (coAspects*)aspects;
	unsigned int advice_I;

	for( advice_I = 0; advice_I < self->count; advice_I++ ) {
		if(
			strcmp( self->advices[advice_I].func, func ) == 0 &&
			self->advices[advice_I].line == line &&
			self->advices[advice_I].aspect == aspect )
		{
			return advice_I;
		}
	}
	return (unsigned)-1;
}

coAdviceEntry _coAspects_adviceAt( void* aspects, unsigned int handle ) {
	coAspects* self = (coAspects*)aspects;
	
	return coAspects_adviceAt( self, handle );
}


coFuncJP* _coAspects_pcPush( void* aspects, coFuncJP* pc, const char* file, const char* func, unsigned int line ) {
	coAspects* self = (coAspects*)aspects;

	/* If the mode is static weave, then ensure the weave has happened */
	coFirewall(
			self->mode & coStaticWeave ? self->hasWeaved : 1,
			coErrJrn( "coAspects" ),
			"Error: \"pointcut\" called but \"weave\" has not happened yet! In %s, %s:%u\n", func, file, line );

	/* If the mode is static pointcut, mimic compiler optimisation and don't actually push */
	if( self->mode & coStaticPointcut ) {
		self->_pcStackTop += 1;
		coFirewall(
				self->_pcStackTop <= 1,
				coErrJrn( "coAspects" ),
				"Error: Recursive/embedded \"pointcut\" calls in static pointcut mode! In %s, %s:%u\n", func, file, line );
		self->_pcStack[self->_pcStackTop] = pc;

		return coAspects_pcTop( self );
	}
	else {
		if( (self->_pcStackTop+1) >= self->_pcStackSize ) {
			self->_pcStackSize += self->_pcStackDelta;
			self->_pcStack = coRealloc( self->_pcStack, sizeof(coFuncJP*) * self->_pcStackSize );
		}

		self->_pcStackTop += 1;
		self->_pcStack[self->_pcStackTop] = pc;

		return coAspects_pcTop( self );
	}
}

coFuncJP* coAspects_pcPop( void* aspects ) {
	coAspects* self = (coAspects*)aspects;

	/* Note - If the mode is static pointcut, mimic the compiler optimisation and don't actually pop */
	if( self->mode & coStaticPointcut ) {
		self->_pcStackTop = 0;
		return coAspects_pcTop( self );
	}
	else {
		if( self->_pcStackTop > 0 ) {
			self->_pcStack[self->_pcStackTop] = NULL;
			self->_pcStackTop -= 1;
		}
		return coAspects_pcTop( self );
	}
}

coFuncJP* _coAspects_pcTop( void* aspects ) {
	coAspects* self = (coAspects*)aspects;

	return coAspects_pcTop( self );
}


void coAspects_weave( void* aspects ) {
	coAspects* self = (coAspects*)aspects;
	unsigned int advice_I;
	unsigned int jp_I;

	/* If the mode is static weave - ensure the weave hasn't happened already. */
	coFirewall(
			self->mode & coStaticWeave ? !self->hasWeaved : 1,
			coErrJrn( "coAspects" ),
			"Error: \"weave\" called more than once!\n" );

	/* TODO: write this out to a stream/file as determined by (for e.g.) command line param */
	coPrintf( NULL, "Advices:\n" );
	for( advice_I = 0; advice_I < self->count; advice_I++ ) {
		coPrintf(
				NULL,
				"function:\"%s\", line:\"%u\", aspect:%p (ptr), binding:\"%u\", id:%p(ptr)\n",
				self->advices[advice_I].func, self->advices[advice_I].line, self->advices[advice_I].aspect,
				self->advices[advice_I].binding, self->advices[advice_I].id );
	}
	coPrintf( NULL, "(Advised) joinpoints:\n" );
	for( jp_I = 0; jp_I < self->jps->count; jp_I++ ) {
		coPrintf(
				NULL,
				"function name:\"%s\", prototype:\"%s\", binding:\"%u\", id:%p(ptr)\n",
				self->jps->jp[jp_I]->name, self->jps->jp[jp_I]->_prototypeName, self->jps->jp[jp_I]->_option,
				self->jps->jp[jp_I]->_optionalObject );
	}
	coPrintf( NULL, "\n" );

	self->hasWeaved = coTrue;
}


void coAspects_reset( void* aspects ) {
	coAspects* self = (coAspects*)aspects;

	/* Assumes ownerships of the elements */
	if( self->advices ) {
		unsigned int advice_I;

		for( advice_I = 0; advice_I < self->count; advice_I++ ) {
			coFree( self->advices[advice_I].file );
			self->advices[advice_I].file = NULL;
			coFree( self->advices[advice_I].func );
			self->advices[advice_I].func = NULL;
			self->advices[advice_I].line = (unsigned)-1;
			self->advices[advice_I].aspect = NULL;
		}
	}
	self->count = 0;

	self->_pcStackTop = 0;

	coJPs_reset( self->jps );
	self->hasWeaved = coFalse;
}


/* Private member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


void _coAspects_delete( void* aspects ) {
	coAspects* self = (coAspects*)aspects;

	/* Assumes ownerships of the elements */
	if( self->advices ) {
		unsigned int advice_I;

		/* If the mode is static weave - BECAUSE we dont want to put this check in the pointcut code because it will slow down the
		 * run-time version, here at finalise, error if the "weave" hasn't been called (but dont bother if no advices have been made).
		 */
		coFirewall(
				self->count > 0 ? (self->mode & coStaticWeave ? self->hasWeaved : 1) : 1,
				coErrJrn( "coAspects" ),
				"Error: \"weave\" not called!\n" );

		for( advice_I = 0; advice_I < self->count; advice_I++ ) {
			coFree( self->advices[advice_I].file );
			coFree( self->advices[advice_I].func );
		}

		coFree( self->advices );
	}

	coFree( self->_pcStack );
	coObject_delete( self->jps );

	/* Delete parent */
	_coObject_delete( self );
}

void _coAspects_print( void* aspects, coStream* stream, coBool concise ) {
	coAspects* self = (coAspects*)aspects;
	unsigned int advice_I;

	if( concise ) {
		/* Prettier, shorter, within a sentence print... */
		_coObject_print( self, stream, coTrue );
		coPrintf( stream, ": { " );
		coPrintf( (void*) stream, "advice[0-%u]: [", self->count - 1 );
		for( advice_I = 0; advice_I < self->count; advice_I++ ) {
			coPrintf(
					(void*) stream,
					" file:\"%s\", function:\"%s\", line:\"%u\", aspect:%p (ptr), binding:\"%u\", id:%p(ptr);",
					self->advices[advice_I].file, self->advices[advice_I].func, self->advices[advice_I].line,
					self->advices[advice_I].aspect, self->advices[advice_I].binding, self->advices[advice_I].id );
		}
		coPrintf( (void*) stream, " ], " );
		coObject_print( self->jps, stream, coTrue );
		coPrintf( stream, " }" );
	}
	else {
		/* Complete object information print... */
		coPrintf( (void*) stream, "coAspects (ptr): %p\n", self );
		_coObject_print( self, stream, coFalse );
		coPrintf( (void*) stream, "\tcount: %u\n", self->count );
		coPrintf( (void*) stream, "\t_size: %lu\n", self->_size );
		coPrintf( (void*) stream, "\t_delta: %lu\n", self->_delta );

		coPrintf( (void*) stream, "\tadvice (ptr): %p\n", self->advices );
		coPrintf( (void*) stream, "\tadvice[0-%u]: [\n", self->count );
		for( advice_I = 0; advice_I < self->count; advice_I++ ) {
			coPrintf(
					(void*) stream,
					"\t\tadvice[%u]: file:\"%s\", function:\"%s\", line:\"%u\", aspect:%p (ptr), binding:\"%u\", id:%p(ptr)\n",
					advice_I, self->advices[advice_I].file, self->advices[advice_I].func, self->advices[advice_I].line,
					self->advices[advice_I].aspect, self->advices[advice_I].binding, self->advices[advice_I].id );
		}
		coPrintf( (void*) stream, "\t]\n" );
	}
}

