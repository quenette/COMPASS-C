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


coFuncJP* coJoinpointNewImp( const char* pointcutFuncName, const char* prototypeName, coJoinpointBinding binding, void* id, const char* calledByFuncName, unsigned int calledByLine ) {
	coFuncJP* ep = coJoinpointGetImp( pointcutFuncName, prototypeName, binding, id, calledByFuncName, calledByLine );

	if( ep ) {
		return ep;
	}
	else {
		switch( binding ) {
			case coJoinpointStatic:
				ep = coFuncJP_newImp( pointcutFuncName, coFuncJP_StrPtrNonGlobal, prototypeName, coFuncJP_StrPtrNonGlobal, NULL, coFuncJP_InstanceStatic );
				break;
			case coJoinpointNamed:
				ep = coFuncJP_newImp( pointcutFuncName, coFuncJP_StrPtrNonGlobal, prototypeName, coFuncJP_StrPtrNonGlobal, id, coFuncJP_InstanceNamedNonGlobal );
				break;
			case coJoinpointObject:
				ep = coFuncJP_newImp( pointcutFuncName, coFuncJP_StrPtrNonGlobal, prototypeName, coFuncJP_StrPtrNonGlobal, id, coFuncJP_InstanceObject );
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
		coJPs_add( coAspects_s->jps, ep );
		return ep;
	}
}

coFuncJP* coJoinpointGetImp( const char* pointcutFuncName, const char* prototypeName, coJoinpointBinding binding, void* id, const char* calledByFuncName, unsigned int calledByLine ) {
	unsigned int i;

	switch( binding ) {
		case coJoinpointStatic:
			i = coJPs_find( coAspects_s->jps, pointcutFuncName, prototypeName );
			break;
		case coJoinpointNamed:
			i = coJPs_findNamed( coAspects_s->jps, pointcutFuncName, prototypeName, id );
			break;
		case coJoinpointObject:
			i = coJPs_findObject( coAspects_s->jps, pointcutFuncName, prototypeName, id );
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

	if( i != -1 ) {
		return coJPs_at( coAspects_s->jps, i );
	}
	else {
		return 0;
	}
}
