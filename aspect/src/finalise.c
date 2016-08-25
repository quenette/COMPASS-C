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
#include "finalise.h"

coBool coFinalise( void ) {
	return coAspectFinalise();
}

coBool coAspectFinalise( void ) {
	/* The pointcut/advice system requires a singleton */
	coObject_delete( coAspects_s );

	return coTrue;
}
