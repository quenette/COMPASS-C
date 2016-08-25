/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**
** Copyright (C) Steve Quenette 2011
**
** <license>
**
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include <stdlib.h>
#include "types.h"
#include "memory.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

void* _coMallocImp( 
		size_t size, 
		const char* const label, 
		const char* const fileName, 
		const char* const funcName, 
		int lineNumber )
{
	return malloc( size );
}


void* _coReallocImp( 
		void* ptr,
		size_t newSize, 
		const char* const fileName, 
		const char* const funcName, 
		int lineNumber )
{
	return realloc( ptr, newSize );
}


void _coFreeImp(
		void* ptr,
		const char* const fileName, 
		const char* const funcName, 
		int lineNumber )
{
	free( ptr );	
}


char* coStrdup( const char* const src ) {
	char* result;

        if( src ) {
		result = coMalloc( sizeof(char) * (strlen( src ) + 1), __func__ );
		strcpy( result, src );
	}
	else {
		result = NULL;
	}

	return result;
}


int coAsprintf( char** string, char* format, ... ) {
	int       sizeOfString;
	va_list   ap;
	int       allocStringSize = 100;
	va_list   apCopy;

	coBool      trying = coTrue;
	
	va_start( ap, format );

	*string = coMalloc( sizeof(char) * allocStringSize, __func__ );
	while( trying ) {
		/* See is we can print to this string with this size */
		#ifdef NO_VA_COPY
			apCopy=ap;
		#else
			va_copy( apCopy, ap );
		#endif
		sizeOfString = vsnprintf( *string, allocStringSize, format, apCopy );
		va_end( apCopy );

		/* Check if string was truncated */
		if( sizeOfString > -1 && sizeOfString < allocStringSize ) {
			return sizeOfString;
		}
		/* String was truncated - resize array and try again */
		if( sizeOfString > -1 ) {
			/* glibc > 2.1 */
			allocStringSize = sizeOfString + 1;
		}
		else {
			/* glibc <= 2.0 */
			allocStringSize = sizeOfString * 2;
		}
		*string = coRealloc( *string, sizeof(char) * allocStringSize );
		if( string == NULL ) {
			assert( string != NULL );
			trying = coFalse;
		}
	}
	va_end( ap );

	return sizeOfString;
}

