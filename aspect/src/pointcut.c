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
#include "Aspects.h"
#include "pointcut.h"

coBool _coAspectImp( const char* funcName, const char* calledByFuncName, unsigned int calledByLine, unsigned int* ep_i, unsigned int* func_i ) {
	/* This can be a little tricky to get one's head around... the aim of this function is to return the
	 * aspect/apectual-class/aspectual-object of the current function (that has been advised into a pointcut). The implementation
	 * makes an assumption - the current function, if advised more than once into pointcut(s), each time will provide the same
	 * aspect pointer. THIS IS A LIMITATION OF THIS IMPLEMENTATION!!! (TODO: can this be done better?). */

	for( *ep_i = 0; *ep_i < coAspects_s->jps->count; (*ep_i)++ ) {
		coFuncJP* ep = coJPs_at( coAspects_s->jps, *ep_i );

		for( *func_i = 0; *func_i < ep->_funcMetas->count; (*func_i)++ ) {
			coAspectFunc* func = (coAspectFunc*)ep->_funcMetas->data[*func_i];

			/* TODO: the following SHOULDNT do a strcmp BUT it seems the code has been changed to not expect func names to be
			 * global strings and hence needing only a pointer comparison. This method should be ok as func names SHOULD ALWAYS
			 * be from a stringification of the function symbol through the advice macro!!! */
			#include <string.h>
			if( func->name == funcName || (0 == strcmp( func->name, funcName )) ) {
				coFirewall(
					func->hasAspect,
					coErrJrn( "pointcut" ),
					"Error: Advised function \"%s\" has call for its aspectual object (the aspect pointer given in an advice), "
					"but no object pointer was given, in function %s, line %u.\n",
					funcName, calledByFuncName, calledByLine );
	    		return coTrue;
			}
		}
	}
	coFirewall(
		*ep_i < coAspects_s->jps->count,
		coErrJrn( "pointcut" ),
		"Error: Function \"%s\" has not been adviced into a pointcut (its not an behavioural aspect), in function %s, line %u.\n",
		funcName, calledByFuncName, calledByLine );
	return coFalse;
}

void* coAspectImp( const char* funcName, const char* calledByFuncName, unsigned int calledByLine ) {
	unsigned int ep_i;
	unsigned int func_i;

	if( _coAspectImp( funcName, calledByFuncName, calledByLine, &ep_i, &func_i ) ) {
		/* The following should be safe as all checks should have occurred */
		/* The compiler version injects this following line... */
		/* TODO: consider how the following could have less pointer de-references */
		return ((coAspectFunc*)(coAspects_s->jps->jp[ep_i]->_funcMetas->data[func_i]))->fromAspect;
	}
	return NULL;
}

void coAdviceImp( const char* pointcutFuncName, const char* prototypeName, coJoinpointBinding binding, void* id, void* func, const char* funcName, const char* calledByFileName, const char* calledByFuncName, unsigned int calledByLine, coAdviceFunctions adviceFunc, int hasAspect, void* aspect ) {
	int alreadyAdviced;

	/* Make sure this advice hasn't been called already (if so, error... not sure I need/want to handle ) */
	alreadyAdviced = coFalse;
	if( hasAspect ) {
		if( coAspects_findAdviceAspect( coAspects_s, calledByFuncName, calledByLine, aspect ) != -1 /* found */ ) {
			alreadyAdviced = coTrue;
		}
	}
	else {
		if( coAspects_findAdvice( coAspects_s, calledByFuncName, calledByLine ) != -1 /* found */ ) {
			alreadyAdviced = coTrue;
		}
	}

	if( alreadyAdviced ) {
		switch( binding ) {
			case coJoinpointStatic:
				coFirewall( 0, coErrJrn( "pointcut" ),
					"%s advice to joinpoint (func/prototype) \"%s/%s\" made more than once... assuming this is an invalid thing at present "
					"in function %s, line %u, aspectual object: %p. %s\n",
					hasAspect ? "Aspectual object" : "Structural",
					pointcutFuncName, prototypeName, calledByFuncName, calledByLine,
					hasAspect ? aspect : NULL,
					hasAspect ? "" : "Ensure advice wasn't supposed to be an aspectual object advice [advice_aspect( ..., aspectPtr )]." );
				break;
			case coJoinpointNamed:
				coFirewall( 0, coErrJrn( "pointcut" ),
					"%s advice to joinpoint (func/prototype/name) \"%s/%s/%s\" made more than once... assuming this is an invalid thing at "
					"present in function %s, line %u, aspectual object: %p. %s\n",
					hasAspect ? "Aspectual object" : "Structural",
					pointcutFuncName, prototypeName, id, calledByFuncName, calledByLine,
					hasAspect ? aspect : NULL,
					hasAspect ? "" : "Ensure advice wasn't supposed to be an aspectual object advice [advice_aspect( ..., aspectPtr )]." );
				break;
			case coJoinpointObject:
				coFirewall( 0, coErrJrn( "pointcut" ),
					"%s advice to joinpoint (func/prototype/object) \"%s/%s/%p\" made more than once... assuming this is an invalid thing at "
					"present in function %s, line %u, aspectual object: %p. %s\n",
					hasAspect ? "Aspectual object" : "Structural",
					pointcutFuncName, prototypeName, id, calledByFuncName, calledByLine,
					hasAspect ? aspect : NULL,
					hasAspect ? "" : "Ensure advice wasn't supposed to be an aspectual object advice [advice_aspect( ..., aspectPtr )]." );
				break;
			default:
				coFirewall(
					binding < coJoinpointInvalid,
					coErrJrn( "pointcut" ),
					"Error: Attempting to create / access a pointcut (function/prototype %s/%s) where 'binding' (%u) value is "
					"invalid in function %s, line %u.\n",
					pointcutFuncName, prototypeName, binding, calledByFuncName, calledByLine );
				break;
		}
	}
	else {
		coFuncJP* entryPoint = coJoinpointNewImp( pointcutFuncName, prototypeName, binding, id, calledByFuncName, calledByLine );

		switch( adviceFunc ) {
			case coAdvicePrepend:
				if( hasAspect )
					coFuncJP_prependWithAspect( entryPoint, func, funcName, calledByFuncName, aspect );
				else
					coFuncJP_prepend( entryPoint, func, funcName, calledByFuncName );
				break;
			case coAdviceAppend:
				if( hasAspect )
					coFuncJP_appendWithAspect( entryPoint, func, funcName, calledByFuncName, aspect );
				else
					coFuncJP_append( entryPoint, func, funcName, calledByFuncName );
				break;
			case coAdviceAlwaysFirst:
				if( hasAspect )
					coFuncJP_prepend_AlwaysFirstWithAspect( entryPoint, func, funcName, calledByFuncName, aspect );
				else
					coFuncJP_prepend_AlwaysFirst( entryPoint, func, funcName, calledByFuncName );
				break;
			case coAdviceAlwaysLast:
				if( hasAspect )
					coFuncJP_append_AlwaysLastWithAspect( entryPoint, func, funcName, calledByFuncName, aspect );
				else
					coFuncJP_append_AlwaysLast( entryPoint, func, funcName, calledByFuncName );
				break;
			case coAdviceAround:
				if( hasAspect )
					coFuncJP_replaceAllWithAspect( entryPoint, func, funcName, calledByFuncName, aspect );
				else
					coFuncJP_replaceAll( entryPoint, func, funcName, calledByFuncName );
				break;
			case coAdviceRemove:
				coFuncJP_remove( entryPoint, funcName );
				break;
			default:
				coFirewall( adviceFunc < coAdviceInvalid, coErrJrn( "pointcut" ),
					"Invalid adviceFunc value \"%d\" in function %s, line %u.\n",
					adviceFunc, calledByFuncName, calledByLine );
				break;
		}

		if( hasAspect ) {
			coAspects_addAdviceAspect( coAspects_s, calledByFileName, calledByFuncName, calledByLine, aspect, binding, id );
		}
		else {
			coAspects_addAdvice( coAspects_s, calledByFileName, calledByFuncName, calledByLine, binding, id );
		}
	}
}

void coAdviceRelImp( char* pointcutFuncName, char* prototypeName, coJoinpointBinding binding, void* id, void* func, char* funcName, const char* calledByFileName, const char* calledByFuncName, unsigned int calledByLine, coAdviceFunctions adviceFunc, char* relName, int hasAspect, void* aspect ) {
	/* Make sure this advice hasn't been called already (if so, error... not sure I need/want to handle ) */
	if( coAspects_findAdvice( coAspects_s, calledByFuncName, calledByLine ) == -1 /* not found */ ) {
		coFuncJP* entryPoint = coJoinpointNewImp( pointcutFuncName, prototypeName, binding, id, calledByFuncName, calledByLine );

		switch( adviceFunc ) {
			case coAdviceInsertBefore:
				if( hasAspect )
					coFuncJP_insertBeforeWithAspect( entryPoint, func, funcName, calledByFuncName, relName, aspect );
				else
					coFuncJP_insertBefore( entryPoint, func, funcName, calledByFuncName, relName );
				break;
			case coAdviceInsertAfter:
				if( hasAspect )
					coFuncJP_insertAfterWithAspect( entryPoint, func, funcName, calledByFuncName, relName, aspect );
				else
					coFuncJP_insertAfter( entryPoint, func, funcName, calledByFuncName, relName );
				break;
			case coAdviceInsertBeforeElseAppend:
				if( coFuncJP_hasFunc( entryPoint, relName ) ) {
					if( hasAspect )
						coFuncJP_insertBeforeWithAspect( entryPoint, func, funcName, calledByFuncName, relName, aspect );
					else
						coFuncJP_insertBefore( entryPoint, func, funcName, calledByFuncName, relName );
				}
				else {
					if( hasAspect )
						coFuncJP_appendWithAspect( entryPoint, func, funcName, calledByFuncName, aspect );
					else
						coFuncJP_append( entryPoint, func, funcName, calledByFuncName );
				}
				break;
			case coAdviceInsertAfterElsePrepend:
				if( coFuncJP_hasFunc( entryPoint, relName ) ) {
					if( hasAspect )
						coFuncJP_insertAfterWithAspect( entryPoint, func, funcName, calledByFuncName, relName, aspect );
					else
						coFuncJP_insertAfter( entryPoint, func, funcName, calledByFuncName, relName );
				}
				else {
					if( hasAspect )
						coFuncJP_prependWithAspect( entryPoint, func, funcName, calledByFuncName, aspect );
					else
						coFuncJP_prepend( entryPoint, func, funcName, calledByFuncName );
				}
				break;
			case coAdviceInsertBeforeElseIgnore:
				if( coFuncJP_hasFunc( entryPoint, relName ) ) {
					if( hasAspect )
						coFuncJP_insertBeforeWithAspect( entryPoint, func, funcName, calledByFuncName, relName, aspect );
					else
						coFuncJP_insertBefore( entryPoint, func, funcName, calledByFuncName, relName );
				}
				break;
			case coAdviceInsertAfterElseIgnore:
				if( coFuncJP_hasFunc( entryPoint, relName ) ) {
					if( hasAspect )
						coFuncJP_insertAfterWithAspect( entryPoint, func, funcName, calledByFuncName, relName, aspect );
					else
						coFuncJP_insertAfter( entryPoint, func, funcName, calledByFuncName, relName );
				}
				break;
			case coAdviceReplace:
				if( hasAspect )
					coFuncJP_replaceWithAspect( entryPoint, func, funcName, calledByFuncName, relName, aspect );
				else
					coFuncJP_replace( entryPoint, func, funcName, calledByFuncName, relName );
				break;
			default:
				coFirewall( adviceFunc > coAdviceInvalid, coErrJrn( "pointcut" ),
					"Invalid adviceFunc value \"%d\" in function %s, line %u.\n",
					adviceFunc, calledByFuncName, calledByLine );
				coFirewall( adviceFunc < coAdviceRelativeInvalid, coErrJrn( "pointcut" ),
					"Invalid adviceFunc value \"%d\" in function %s, line %u.\n",
					adviceFunc, calledByFuncName, calledByLine );
				break;
		}
		coAspects_addAdvice( coAspects_s, calledByFileName, calledByFuncName, calledByLine, binding, id );
	}
	else {
		coFirewall( 0, coErrJrn( "pointcut" ),
			"Advice for %s made more than once... assuming this is an invalid thing at present in function %s, line %u.\n",
			adviceFunc, calledByFuncName, calledByLine );
	}
}

static void PointcutNotFound( coFuncJP* ep, int firewall, const char* pointcutFuncName, const char* prototypeName, coJoinpointBinding binding, void* id, const char* calledByFuncName, unsigned int calledByLine ) {
	if( firewall ) {
		switch( binding ) {
			case coJoinpointStatic:
				coFirewall(
					(long)ep,
					 coErrJrn( "pointcut" ),
					 "Error: No funcs defined for pointcut prototype \"%s\" in function"
					 " \"%s\" as of function %s(), line %u. Might imply at least one"
					 " advice is ill-defined/missing.\n",
					prototypeName, pointcutFuncName, calledByFuncName, calledByLine );
				break;
			case coJoinpointNamed:
				coFirewall(
					(long)ep,
					 coErrJrn( "pointcut" ),
					 "Error: No funcs defined for pointcut prototype \"%s\" in function"
					 " \"%s\" and instance name of '%s' as of function %s(), line %u. Might imply at least one"
					 " advice is ill-defined/missing.\n",
					prototypeName, pointcutFuncName, id, calledByFuncName, calledByLine );
				break;
			case coJoinpointObject:
				coFirewall(
					(long)ep,
					 coErrJrn( "pointcut" ),
					 "Error: No funcs defined for pointcut prototype \"%s\" in function"
					 " \"%s\" and instance object ptr of '%p' as of function %s(), line %u. Might imply at least one"
					 " advice is ill-defined/missing.\n",
					prototypeName, pointcutFuncName, id, calledByFuncName, calledByLine );
				break;
			default:
				coFirewall(
					binding < coJoinpointInvalid,
					coErrJrn( "pointcut" ),
					"Error: Attempting to create / access a pointcut (function/prototype %s/%s) where 'binding' (%u) value is "
					"invalid in function %s, line %u.\n",
					pointcutFuncName, pointcutFuncName, binding, calledByFuncName, calledByLine );
				break;
		}
	}
	else {
		switch( binding ) {
			case coJoinpointStatic:
				coPrintf(
					 coDbgJrn( "pointcut" ),
					 "Warning: No funcs defined for pointcut prototype \"%s\" in function"
					 " \"%s\" as of function %s(), line %u. Might imply at least one"
					 " advice is ill-defined/missing.\n",
					prototypeName, pointcutFuncName, calledByFuncName, calledByLine );
				break;
			case coJoinpointNamed:
				coPrintf(
					 coDbgJrn( "pointcut" ),
					 "Warning: No funcs defined for pointcut prototype \"%s\" in function"
					 " \"%s\" and instance name of '%s' as of function %s(), line %u. Might imply at least one"
					 " advice is ill-defined/missing.\n",
					prototypeName, pointcutFuncName, id, calledByFuncName, calledByLine );
				break;
			case coJoinpointObject:
				coPrintf(
					 coDbgJrn( "pointcut" ),
					 "Warning: No funcs defined for pointcut prototype \"%s\" in function"
					 " \"%s\" and instance object ptr of '%p' as of function %s(), line %u. Might imply at least one"
					 " advice is ill-defined/missing.\n",
					prototypeName, pointcutFuncName, id, calledByFuncName, calledByLine );
				break;
			default:
				coFirewall(
					binding < coJoinpointInvalid,
					coDbgJrn( "pointcut" ),
					"Error: Attempting to create / access a pointcut (function/prototype %s/%s) where 'binding' (%u) value is "
					"invalid in function %s, line %u.\n",
					pointcutFuncName, pointcutFuncName, binding, calledByFuncName, calledByLine );
				break;
		}
	}
}

int coPointcutSizeImp( const char* pointcutFuncName, const char* prototypeName, coJoinpointBinding binding, void* id, const char* calledByFuncName, unsigned int calledByLine ) {
	coFuncJP* ep = coJoinpointGetImp( pointcutFuncName, prototypeName, binding, id, calledByFuncName, calledByLine );

	if( !ep ) {
		PointcutNotFound( ep, 0, pointcutFuncName, pointcutFuncName, binding, id, calledByFuncName, calledByLine );
		return 0;
	}
	else {
		return coFuncJP_funcCount( ep, calledByFuncName );
	}
}


void coPointcutWarnIfEmptyImp( const char* pointcutFuncName, const char* prototypeName, coJoinpointBinding binding, void* id, const char* calledByFuncName, unsigned int calledByLine ) {
	coFuncJP* ep = coJoinpointGetImp( pointcutFuncName, prototypeName, binding, id, calledByFuncName, calledByLine );

	if( !ep ) {
		PointcutNotFound( ep, 0, pointcutFuncName, pointcutFuncName, binding, id, calledByFuncName, calledByLine );
	}
	else {
		coFuncJP_warnIfNoFuncs( ep, calledByFuncName );
	}
}


void coPointcutErrorIfEmptyImp( const char* pointcutFuncName, const char* prototypeName, coJoinpointBinding binding, void* id, const char* calledByFuncName, unsigned int calledByLine ) {
	coFuncJP* ep = coJoinpointGetImp( pointcutFuncName, prototypeName, binding, id, calledByFuncName, calledByLine );

	if( !ep ) {
		PointcutNotFound( ep, 0, pointcutFuncName, pointcutFuncName, binding, id, calledByFuncName, calledByLine );
	}
	else {
		coFuncJP_errorIfNoFuncs( ep, calledByFuncName );
	}
}

