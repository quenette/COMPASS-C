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
#include "debug.h"


void coReset( void ) {
	coAspectReset();
}

void coAspectReset( void ) {
	coAspects_reset( coAspects_s );
}


void coWeave( void ) {
	coAspectWeave();
}

void coAspectWeave( void ) {
	coAspects_weave( coAspects_s );
}
