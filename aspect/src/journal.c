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
#include "journal.h"

#include <stdarg.h>


static coStream _coErrJrn;
coStream* coErrJrn( const char* type ) {
	/* TODO: This is a poxy implementation but will do for now. */
	_coErrJrn.file = stderr;
	_coErrJrn.isDebug = coFalse;
	return &_coErrJrn;
}


static coStream _coOutJrn;
coStream* coOutJrn( const char* type ) {
	/* TODO: This is a poxy implementation but will do for now. */
	_coOutJrn.file = stdout;
	_coOutJrn.isDebug = coFalse;
	return &_coOutJrn;
}


static coStream _coDbgJrn;
coStream* coDbgJrn( const char* type ) {
	/* TODO: This is a poxy implementation but will do for now. */
	_coDbgJrn.file = stdout;
	_coDbgJrn.isDebug = coTrue;
	return &_coDbgJrn;
}


int _coPrintfImp( void* _stream, const char* const fmt, ... ) {
	coStream* stream = (coStream*)_stream;
	size_t result;
	va_list ap;
	FILE* file;

	if( stream ) {
		file = stream->file;
	}
	else {
		_coDbgJrn.file = stdout; /* TODO: remove once streams implemented properly */
		file = _coDbgJrn.file;
	}

	va_start( ap, fmt );
	result = vfprintf( file, fmt, ap );
	va_end( ap );

	return result;
}


int _coJournal_FW_tmp; /* used by coFirewall macro */
int _coFirewallImp( int expression, char* expressionText, const char* fileName, const char* func, int line, void* _stream,
		char* fmt, ... )
{
	coStream* stream = (coStream*)_stream;
	int result;
	va_list ap;
	FILE* file;

	if( stream ) {
		file = stream->file;
	}
	else {
		_coErrJrn.file = stderr; /* TODO: remove once streams implemented properly */
		file = _coErrJrn.file;
	}

	if( expression ) {
		/* Every thing is OK! Back to work as normal */
		result = 0;
	}
	else {
		va_start( ap, fmt );
		coPrintf( stream, "Firewall: %s. ", expressionText );
		coPrintf( stream, fmt, ap );
		coPrintf( stream, " In file %s:%s:%u.\n", fileName, func, line );
		fflush( stream->file );
		va_end( ap );
		exit( EXIT_FAILURE );
		result = -1;
	}
	return result;
}

