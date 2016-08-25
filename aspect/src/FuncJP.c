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
#include "AspectFunc.h"
#include "FuncJP.h"

#include <assert.h>
#include <string.h>


const char* coFuncJP_type = "coFuncJP";

#define ES coErrJrn( coFuncJP_type )


/* Public member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


coFuncJP* coFuncJP_newImp(
		const char*                func,
		coFuncJP_StrPtr            funcAT,
		const char*                prototypeName,
		coFuncJP_StrPtr            protoAT,
		void*                      ptr,
		coFuncJP_Instance          option )
{
	coFuncJP* self;
	
	self = (coFuncJP*)coObject_new( sizeof(coFuncJP), coFuncJP_type, _coFuncJP_delete, _coFuncJP_print, func );
	
	self->_prototypeAT = protoAT;
	switch( protoAT ) {
		case coFuncJP_StrPtrGlobal:
			self->_prototypeName = (char*)prototypeName;
			break;
		case coFuncJP_StrPtrNonGlobal:
			self->_prototypeName = coStrdup( prototypeName );
			break;
		default:
			coFirewall( protoAT < coFuncJP_StrPtrInvalid, ES, "Invalid protoAT (%u) to  %s\n", protoAT, __func__ );
			break;
	}
	switch( funcAT ) {
		case coFuncJP_StrPtrGlobal:
			coFirewall( funcAT == coFuncJP_StrPtrGlobal, ES, "Invalid protoAT (%u) to  %s\n", funcAT, __func__ );
			break;
		case coFuncJP_StrPtrNonGlobal:
			/* Ok - this is what coObject_new does */
			break;
		default:
			coFirewall( funcAT < coFuncJP_StrPtrInvalid, ES, "Invalid protoAT (%u) to  %s\n", funcAT, __func__ );
			break;
	}
	switch( option ) {
		case coFuncJP_InstanceStatic:
			self->_option = coFuncJP_InstanceStatic;
			self->_optionalName = NULL;
			self->_optionalNameAT = coFuncJP_StrPtrGlobal;
			self->_optionalObject = NULL;
			break;
		case coFuncJP_InstanceNamedGlobal:
			self->_option = coFuncJP_InstanceNamedGlobal;
			self->_optionalName = ptr;
			self->_optionalNameAT = coFuncJP_StrPtrGlobal;
			self->_optionalObject = NULL;
			break;
		case coFuncJP_InstanceNamedNonGlobal:
			self->_option = coFuncJP_InstanceNamedNonGlobal;
			self->_optionalName = coStrdup( ptr );
			self->_optionalNameAT = coFuncJP_StrPtrNonGlobal;
			self->_optionalObject = NULL;
			break;
		case coFuncJP_InstanceObject:
			self->_option = coFuncJP_InstanceObject;
			self->_optionalName = NULL;
			self->_optionalNameAT = coFuncJP_StrPtrGlobal;
			self->_optionalObject = ptr;
			break;
		default:
			coFirewall( option < coFuncJP_InstanceInvalid, ES, "Invalid option (%u) to  %s\n", option, __func__ );
			break;
	}
	self->_funcMetas = coOL_new();
	coObject_setName( self->_funcMetas, "_funcMetas" );
	self->_funcsSize = self->_funcMetas->_size;
	self->_funcs = coMalloc( sizeof(void*) * self->_funcsSize, "coFuncJP->_funcs" );
	self->_alwaysFirstFunc = NULL;
	self->_alwaysLastFunc = NULL;
	
	return self;
}


static void _coFuncJP_ensureSize( coFuncJP* self ) {
	if( self->_funcMetas->count == self->_funcsSize ) {
		void** newFuncs;
		
		coFirewall( self->_funcMetas->count <= self->_funcsSize, ES, "funcs and funcMetas out of sync in %s\n", __func__ );
		self->_funcsSize += self->_funcMetas->_delta;
		newFuncs = coMalloc( sizeof(void*) * self->_funcsSize, "coFuncJP->_funcs" );
		memcpy( newFuncs, self->_funcs, sizeof(void*) * self->_funcMetas->count );
		coFree( self->_funcs );
		self->_funcs = newFuncs;
	}
}

static void _coFuncJP_insertAt( coFuncJP* self, unsigned int index, void* func, coAspectFunc* meta ) {
	unsigned int afterCount = self->_funcMetas->count - index;
	void** tmpFuncs;

	coFirewall( index <= self->_funcMetas->count, ES, "Insert into index is invalid in %s\n", __func__ );

	_coFuncJP_ensureSize( self );
	if( afterCount ) {
		tmpFuncs = coMalloc( sizeof(void*) * afterCount, "coFuncJP:tmpFuncs" );
		memcpy( tmpFuncs, &(self->_funcs[index]), sizeof(void*) * afterCount );
		memcpy( &(self->_funcs[index+1]), tmpFuncs, sizeof(void*) * afterCount );
		coFree( tmpFuncs );
	}
	self->_funcs[index] = func;
	coOL_insertAtIndex( self->_funcMetas, index, meta );
}

static void _coFuncJP_removeAt( coFuncJP* self, unsigned int index ) {
	unsigned int afterCount = self->_funcMetas->count - index;
	void** tmpFuncs;

	coFirewall( index <= self->_funcMetas->count, ES, "Insert into index is invalid in %s\n", __func__ );

	if( afterCount ) {
		tmpFuncs = coMalloc( sizeof(void*) * afterCount, "coFuncJP:tmpFuncs" );
		memcpy( tmpFuncs, &(self->_funcs[index+1]), sizeof(void*) * afterCount );
		memcpy( &(self->_funcs[index]), tmpFuncs, sizeof(void*) * afterCount );
		coFree( tmpFuncs );
	}
	coOL_removeByIndex( self->_funcMetas, index, coOL_FormerDelete );
}

static coAspectFunc* _coFuncJP_prepend( void* ep, void* func, const char* name, const char* addedBy, coAspectFunc* meta ) {
	coFuncJP* self = (coFuncJP*)ep;
	
	coFirewall( (coBool)self, ES, "Function Joinpoint is NULL in %s\n", addedBy );

	if( self->_alwaysFirstFunc ) {
		unsigned int index;

		index = coOL_getIndex( self->_funcMetas, self->_alwaysFirstFunc->name );
		coFirewall( index != -1, ES, "Func not found in %s\n", addedBy );
		_coFuncJP_insertAt( self, index + 1, func, meta );
	}
	else {
		_coFuncJP_insertAt( self, 0, func, meta );
	}
	return meta;	
}


coAspectFunc* coFuncJP_prepend( void* ep, void* func, const char* name, const char* addedBy ) {
	return _coFuncJP_prepend( ep, func, name, addedBy, coAspectFunc_new( name, func, addedBy ) );
}

coAspectFunc* coFuncJP_prependWithAspect( void* ep, void* func, const char* name, const char* addedBy, void* aspect ) {
	return _coFuncJP_prepend( ep, func, name, addedBy, coAspectFunc_newWithAspect( name, func, addedBy, aspect ) );
}


static coAspectFunc* _coFuncJP_prepend_AlwaysFirst( void* ep, void* func, const char* name, const char* addedBy, coAspectFunc* meta ) {
	coFuncJP* self = (coFuncJP*)ep;
	
	coFirewall( (coBool)self, ES, "Function Joinpoint is NULL in %s\n", addedBy );

	coFirewall( !self->_alwaysFirstFunc, ES,
		"Error: tried to prepend a new func \"%s\" to entry point "
		"%s to always come first, but func \"%s\" already specified to always be first.\n",
		name, self->name, self->_alwaysFirstFunc->name );

	self->_alwaysFirstFunc = _coFuncJP_prepend( self, func, name, addedBy, meta );
	return self->_alwaysFirstFunc;
}

coAspectFunc* coFuncJP_prepend_AlwaysFirst( void* ep, void* func, const char* name, const char* addedBy ) {
	return _coFuncJP_prepend_AlwaysFirst( ep, func, name, addedBy, coAspectFunc_new( name, func, addedBy ) );
}

coAspectFunc* coFuncJP_prepend_AlwaysFirstWithAspect( void* ep, void* func, const char* name, const char* addedBy, void* aspect ) {
	return _coFuncJP_prepend_AlwaysFirst( ep, func, name, addedBy, coAspectFunc_newWithAspect( name, func, addedBy, aspect ) );
}


static coAspectFunc* _coFuncJP_append( void* ep, void* func, const char* name, const char* addedBy, coAspectFunc* meta ) {
	coFuncJP* self = (coFuncJP*)ep;

	coFirewall( (coBool)self, ES, "Function Joinpoint is NULL in %s\n", addedBy );

	if( self->_alwaysLastFunc ) {
		unsigned int index;

		index = coOL_getIndex( self->_funcMetas, self->_alwaysLastFunc->name );
		coFirewall( index != -1, ES, "Func not found in %s\n", addedBy );
		_coFuncJP_insertAt( self, index, func, meta );
	}
	else {
		_coFuncJP_insertAt( self, self->_funcMetas->count, func, meta );
	}
	return meta;
}

coAspectFunc* coFuncJP_append( void* ep, void* func, const char* name, const char* addedBy ) {
	return _coFuncJP_append( ep, func, name, addedBy, coAspectFunc_new( name, func, addedBy ) );
}

coAspectFunc* coFuncJP_appendWithAspect( void* ep, void* func, const char* name, const char* addedBy, void* aspect ) {
	return _coFuncJP_append( ep, func, name, addedBy, coAspectFunc_newWithAspect( name, func, addedBy, aspect ) );
}


static coAspectFunc* _coFuncJP_append_AlwaysLast( void* ep, void* func, const char* name, const char* addedBy, coAspectFunc* meta ) {
	coFuncJP* self = (coFuncJP*)ep;
	
	coFirewall( (coBool)self, ES, "Function Joinpoint is NULL in %s\n", addedBy );

	coFirewall( !(self->_alwaysLastFunc), ES,
		"Error: tried to append a new func \"%s\" to entry point "
		"%s to always come last, but func \"%s\" already specified to always be last.\n",
		name, self->name, self->_alwaysLastFunc->name );

	self->_alwaysLastFunc = _coFuncJP_append( self, func, name, addedBy, meta );
	return self->_alwaysLastFunc;
}

coAspectFunc* coFuncJP_append_AlwaysLast( void* ep, void* func, const char* name, const char* addedBy ) {
	return _coFuncJP_append_AlwaysLast( ep, func, name, addedBy, coAspectFunc_new( name, func, addedBy ) );
}

coAspectFunc* coFuncJP_append_AlwaysLastWithAspect( void* ep, void* func, const char* name, const char* addedBy, void* aspect ) {
	return _coFuncJP_append_AlwaysLast( ep, func, name, addedBy, coAspectFunc_newWithAspect( name, func, addedBy, aspect ) );
}


static coAspectFunc* _coFuncJP_insertBefore( void* ep, void* func, const char* name, const char* addedBy,
		const char* funcToInsertBefore, coAspectFunc* meta )
{
	coFuncJP* self = (coFuncJP*)ep;
	unsigned int index;

	coFirewall( (coBool)self, ES, "Function Joinpoint is NULL in %s\n", addedBy );

	if( self->_alwaysFirstFunc ) {
		coFirewall( strcmp( funcToInsertBefore, self->_alwaysFirstFunc->name ), ES,
			"Error: tried to insert a new func \"%s\" to entry point "
			"%s before a func specified to always be first, \"%s\".\n",
			name, self->name, self->_alwaysFirstFunc->name );
	}

	index = coOL_getIndex( self->_funcMetas, funcToInsertBefore );
	coFirewall( index != -1, ES, "Func not found in %s\n", addedBy );
	_coFuncJP_insertAt( self, index, func, meta );
	return meta;
}

coAspectFunc* coFuncJP_insertBefore( void* ep, void* func, const char* name, const char* addedBy, const char* funcToInsertBefore ) {
	return _coFuncJP_insertBefore( ep, func, name, addedBy, funcToInsertBefore, coAspectFunc_new( name, func, addedBy ) );
}

coAspectFunc* coFuncJP_insertBeforeWithAspect( void* ep, void* func, const char* name, const char* addedBy,
		const char* funcToInsertBefore, void* aspect )
{
	return _coFuncJP_insertBefore( ep, func, name, addedBy, funcToInsertBefore, coAspectFunc_newWithAspect( name, func, addedBy, aspect ) );
}


static coAspectFunc* _coFuncJP_insertAfter( void* ep, void* func, const char* name, const char* addedBy,
		const char* funcToInsertAfter, coAspectFunc* meta )
{
	coFuncJP* self = (coFuncJP*)ep;
	unsigned int index;

	coFirewall( (coBool)self, ES, "Function Joinpoint is NULL in %s\n", addedBy );

	if( self->_alwaysLastFunc ) {
		coFirewall( strcmp( funcToInsertAfter, self->_alwaysLastFunc->name ), ES,
		"Error: tried to insert a new func \"%s\" to entry point "
		"%s after a func specified to always be last, \"%s\".\n",
		name, self->name, self->_alwaysLastFunc->name );
	}
	
	index = coOL_getIndex( self->_funcMetas, funcToInsertAfter );
	coFirewall( index != -1, ES, "Func not found in %s\n", addedBy );
	_coFuncJP_insertAt( self,  index + 1, func, meta );
	return meta;
}

coAspectFunc* coFuncJP_insertAfter( void* ep, void* func, const char* name, const char* addedBy, const char* funcToInsertAfter ) {
	return _coFuncJP_insertAfter( ep, func, name, addedBy, funcToInsertAfter, coAspectFunc_new( name, func, addedBy ) );
}

coAspectFunc* coFuncJP_insertAfterWithAspect( void* ep, void* func, const char* name, const char* addedBy,
		const char* funcToInsertAfter, void* aspect )
{
	return _coFuncJP_insertAfter( ep, func, name, addedBy, funcToInsertAfter, coAspectFunc_newWithAspect( name, func, addedBy, aspect ) );
}


static coAspectFunc* _coFuncJP_replaceAll( void* ep, void* func, const char* name, const char* addedBy, coAspectFunc* meta ) {
	coFuncJP* self = (coFuncJP*)ep;

	coFirewall( (coBool)self, ES, "Function Joinpoint is NULL in %s\n", addedBy );

	/* Purge the EP first. */
	coFuncJP_purge( self );
	return _coFuncJP_append( self, func, name, addedBy, meta );
}

coAspectFunc* coFuncJP_replaceAll( void* ep, void* func, const char* name, const char* addedBy ) {
	return _coFuncJP_replaceAll( ep, func, name, addedBy, coAspectFunc_new( name, func, addedBy ) );

}

coAspectFunc* coFuncJP_replaceAllWithAspect( void* ep, void* func, const char* name, const char* addedBy, void* aspect ) {
	return _coFuncJP_replaceAll( ep, func, name, addedBy, coAspectFunc_newWithAspect( name, func, addedBy, aspect ) );

}


static coAspectFunc* _coFuncJP_replace( void* ep, void* func, const char* name, const char* addedBy, const char* funcToReplace,
		coAspectFunc* meta )
{
	coFuncJP* self = (coFuncJP*)ep;
	unsigned int index;

	coFirewall( (coBool)self, ES, "Function Joinpoint is NULL in %s\n", addedBy );

	if( self->_alwaysFirstFunc && ( 0 == strcmp( funcToReplace, self->_alwaysFirstFunc->name ) ) ) {
		self->_alwaysFirstFunc = NULL;
	}
	else if( self->_alwaysLastFunc && ( 0 == strcmp( funcToReplace, self->_alwaysLastFunc->name ) ) ) {
		self->_alwaysLastFunc = NULL;
	}

	index = coOL_getIndex( self->_funcMetas, funcToReplace );
	coFirewall( index != -1, ES, "Func not found in %s\n", addedBy );
	self->_funcs[index] = func;
	coOL_replace( self->_funcMetas, funcToReplace, coOL_FormerDelete, meta );
	return meta;
}

coAspectFunc* coFuncJP_replace( void* ep, void* func, const char* name, const char* addedBy, const char* funcToReplace ) {
	return _coFuncJP_replace( ep, func, name, addedBy, funcToReplace, coAspectFunc_new( name, func, addedBy ) );
}

coAspectFunc* coFuncJP_replaceWithAspect( void* ep, void* func, const char* name, const char* addedBy, const char* funcToReplace,
		void* aspect )
{
	return _coFuncJP_replace( ep, func, name, addedBy, funcToReplace, coAspectFunc_newWithAspect( name, func, addedBy, aspect ) );
}


void coFuncJP_printConcise( void* ep, coStream* stream ) {
	coFuncJP* self = (coFuncJP*)ep;
	unsigned int funcIndex;
	coAspectFunc* func;

	coFirewall( (coBool)self, ES, "Function Joinpoint is NULL in %s\n", __func__ );

	coPrintf( stream, "\tAspect function: %s\n", self->name );
	for( funcIndex = 0; funcIndex < self->_funcMetas->count; funcIndex++ ) {
		func = (coAspectFunc*) self->_funcMetas->data[funcIndex];
		coPrintf( stream, "\t\tH: \"%s\"", func->name );
		if ( 0 != strcmp( "", func->fromFunc ) ) {
			coPrintf( stream, " (%s)", func->fromFunc );
		}
		coPrintf( stream, "\n" );
	}
}

void coFuncJP_remove( void* ep, const char* name ) {
	coFuncJP* self = (coFuncJP*)ep;
	unsigned int index;

	coFirewall( (coBool)self, ES, "Function Joinpoint is NULL in %s\n", __func__ );

	index = coOL_getIndex( self->_funcMetas, name );
	coFirewall( index != -1, ES, "Func not found in %s\n", __func__ );
	_coFuncJP_removeAt( self, index );
}

void coFuncJP_purge( void* ep ) {
	coFuncJP* self = (coFuncJP*)ep;

	coFirewall( (coBool)self, ES, "Function Joinpoint is NULL in %s\n", __func__ );

	/* Class_Delete the existing func information structures. Note: this won't try and delete the func ptrs themselves */
	coOL_deleteAllObjects( self->_funcMetas );
	self->_alwaysFirstFunc = NULL;
	self->_alwaysLastFunc = NULL;
}

int coFuncJP_hasFunc( void* entryPoint, const char* name ) {
	coFuncJP* self = (coFuncJP*)entryPoint;

	coFirewall( (coBool)self, ES, "Function Joinpoint is NULL in %s\n", __func__ );
	if( coOL_getIndex( self->_funcMetas, name ) == -1 ) {
		return 0;
	}
	else {
		return 1;
	}
}


int coFuncJP_funcCount( void* ep, const char* parentFunction ) {
	coFuncJP* self = (coFuncJP*)ep;

	coFirewall( (coBool)self, ES, "Function Joinpoint is NULL in %s\n", parentFunction );

	return self->_funcMetas->count;
}


void coFuncJP_warnIfNoFuncs( void* ep, const char* parentFunction ) {
	coFuncJP* self = (coFuncJP*)ep;

	coFirewall( (coBool)self, ES, "Function Joinpoint is NULL in %s\n", parentFunction );

	if( !self->_funcMetas->count ) {
		const char* epName = self->name;
		coPrintf( ES, "Warning: No funcs defined for E.P. \"%s\". Calling parent function "
			"%s() usually implies at least one %s func defined.\n",
			epName, parentFunction, epName );
	}
}


void coFuncJP_errorIfNoFuncs( void* ep, const char* parentFunction ) {
	coFuncJP* self = (coFuncJP*)ep;
	const char* epName;

	coFirewall( (coBool)self, ES, "Function Joinpoint is NULL in %s\n", parentFunction );

	epName = self->name;
	coFirewall( self->_funcMetas->count, ES, "Error: No funcs defined for E.P. \"%s\". Calling parent function "
		"%s() requires at least one %s func defined.\n",
		epName, parentFunction, epName );
}



/* Private member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


void _coFuncJP_delete( void* ep ) {
	coFuncJP* self = (coFuncJP*)ep;

	coFirewall( (coBool)self, ES, "Function Joinpoint is NULL in %s\n", __func__ );

	coOL_deleteAllObjects( self->_funcMetas );
	coObject_delete( self->_funcMetas );
	coFree( self->_funcs );
	if( self->_prototypeAT == coFuncJP_StrPtrNonGlobal ) {
		coFree( self->_prototypeName );
	}
	if( self->_optionalNameAT == coFuncJP_StrPtrNonGlobal ) {
		coFree( self->_optionalName );
	}

	/* Delete parent */
	_coObject_delete( self );
}


void _coFuncJP_print( void* ep, coStream* stream, coBool concise ) {
	coFuncJP* self = (coFuncJP*)ep;
	unsigned int i;

	coFirewall( (coBool)self, ES, "Function Joinpoint is NULL in %s\n", __func__ );

	if( concise ) {
		/* Prettier, shorter, within a sentence print... */
		coPrintf( stream, "%s", self->name );
		coPrintf( stream, ",%s", self->_prototypeName );
		switch( self->_option ) {
			case coFuncJP_InstanceStatic:
				break;
			case coFuncJP_InstanceNamedGlobal:
			case coFuncJP_InstanceNamedNonGlobal:
				coPrintf( stream, ",\"%s\"", self->_optionalName );
				break;
			case coFuncJP_InstanceObject:
				coPrintf( stream, ",%p(ptr)", self->_optionalObject );
				break;
			default:
				coFirewall( self->_option < coFuncJP_InstanceInvalid, ES, "Invalid option (%u) to  %s\n", self->_option, __func__ );
				break;
		}
		coPrintf( stream, "(%p(ptr)) : { ", self );
		coObject_print( self->_funcMetas, stream, coTrue );
		coPrintf( stream, " }" );
	}
	else {
		/* Complete object information print... */
		coPrintf( stream, "coFuncJP (ptr) (%p):\n", self );
		coPrintf( stream, "\tprototypeName: %u\n", self->_prototypeName );
		switch( self->_option ) {
			case coFuncJP_InstanceStatic:
				coPrintf( stream, "\toptionalName: N/A\n" );
				coPrintf( stream, "\toptionalObject (ptr): N/A\n" );
				break;
			case coFuncJP_InstanceNamedGlobal:
			case coFuncJP_InstanceNamedNonGlobal:
				coPrintf( stream, "\toptionalName: %s\n", self->_optionalName );
				coPrintf( stream, "\toptionalObject (ptr): N/A\n" );
				break;
			case coFuncJP_InstanceObject:
				coPrintf( stream, "\toptionalName: N/A\n" );
				coPrintf( stream, "\toptionalObject (ptr): %p\n", self->_optionalObject );
				break;
			default:
				coFirewall( self->_option < coFuncJP_InstanceInvalid, ES, "Invalid option (%u) to  %s\n", self->_option, __func__ );
				break;
		}
		coPrintf( stream, "\tfuncs (ptr): { " );
		for( i = 0; i < self->_funcMetas->count; i++ ) {
			coPrintf( stream, "(ptr) %p, ", self->_funcs[i] );
		}
		coPrintf( stream, "}\n" );
		coPrintf( stream, "\tfuncMetas: \n" );
		coObject_print( self->_funcMetas, stream, coFalse );
		coPrintf( stream, "\n" );
		_coObject_print( self, stream, coFalse );
	}
}


