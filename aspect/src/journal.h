/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**
** Copyright (C) Steve Quenette 2011
**
** <license>
**
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef __Compass_Aspect_journal_h__
#define __Compass_Aspect_journal_h__

	#define __coStream \
		FILE*      file; \
        coBool     isDebug; \

	typedef struct _coStream { __coStream } coStream;

	coStream* coErrJrn( const char* type );
	coStream* coOutJrn( const char* type );
	coStream* coDbgJrn( const char* type );


	#ifdef DEBUG
		/** Compass stream printing
		 **
		 **  If the stream is NULL, this will revert to printing to stdout's journal.
		 **  If the stream is a debug stream and DEBUG is NOT set, will only evaluate the check and not actually call printf. */
		#define coPrintf( stream, fmt, ... ) \
			stream ? \
				_coPrintfImp( stream, fmt, ##__VA_ARGS__ ) : \
				_coPrintfImp( coOutJrn( NULL ), fmt, ##__VA_ARGS__ );
	#else
		/** Compass stream printing
		 **
		 **  If the stream is NULL, this will revert to printing to stdout's journal.
		 **  If the stream is a debug stream and DEBUG is NOT set, will only evaluate the check and not actually call printf. */
		#define coPrintf( stream, fmt, ... ) \
			stream ? \
				((coStream*)stream)->isDebug ? 0 : _coPrintfImp( stream, fmt, ##__VA_ARGS__ ) : \
				_coPrintfImp( stream, fmt, ##__VA_ARGS__ );
	#endif
	int _coPrintfImp( void* _stream, const char* const fmt, ... );

	/** Compass firewall (assert & printf)
	 **
	 ** This implementation only evaluates the expression once, hence requires a buffer/temporary variable,
	 ** and in inherently not thread safe.
	 **
	 ** There is not debug equivalent to ensure consistency b/w debug and non-deug code - sorry but if you're firewalling in an
	 ** inner loop you probably have good reason to!
	 **/
	#define coFirewall( expression, stream, ... ) \
		if( !(_coJournal_FW_tmp = (expression) ) ) { \
			_coFirewallImp( _coJournal_FW_tmp, #expression, __FILE__, __func__, __LINE__, stream, ##__VA_ARGS__ ); \
		} \
		else {}
	int _coFirewallImp( int expression, char* expressionText, const char* file, const char* func, int line, void* stream,
			char* fmt, ... );
	extern int _coJournal_FW_tmp;


#endif /* __Compass_Aspect_Object_h__ */

