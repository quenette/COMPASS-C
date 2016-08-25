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


int main( int argc, char* argv[] ) {
	coObject* obj1;
	coStream* stream = NULL;

	obj1 = coObject_new( sizeof(coObject), coObject_type, _coObject_delete, _coObject_print, "test" );
	coObject_print( obj1, stream, coTrue );
	coPrintf( stream, "\n" );
	coObject_print( obj1, stream, coFalse );

	coObject_delete( obj1 );
	return 0;
}
