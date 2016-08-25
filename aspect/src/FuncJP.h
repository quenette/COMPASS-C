/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**
** Copyright (C) Steve Quenette 2011
**
** <license>
**
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


#ifndef __Compass_Aspect_FuncJP_h__
#define __Compass_Aspect_FuncJP_h__
	
	/* Templates of virtual functions */
	
	/** Textual name of this class */
	extern const char* coFuncJP_type;
	
	typedef enum {
		coFuncJP_InstanceStatic,                   /** The function joinpoint has no instance name/id. */
		coFuncJP_InstanceNamedGlobal,              /** The function joinpoint has a run-time instance name that is a global string
		                                               (will only copy the pointer & wont delete). Searches will ptr comparison
		                                               first, hence this method is equivalent to the pointer id method in run-time
		                                               work done. */
		coFuncJP_InstanceNamedNonGlobal,           /** The function joinpoint has a run-time instance name that is a run-time /
		                                               local string (will strdup the name & will delete). Searches will revert to
		                                               strcmp to identify this function joinpoint at runtime (i.e. slow!) */
		coFuncJP_InstanceObject,                   /** The function joinpoint has an object run-time instance (a ptr). Run-time resolution
		                                               is a linear pointer comparison. */
		coFuncJP_InstanceInvalid
	} coFuncJP_Instance;

	typedef enum {
		coFuncJP_StrPtrGlobal,                     /** The associated string is a static pointer (valid pointer at point in the
		                                               code). The function joinpoint will only copy the pointer value (i.e. not
		                                               strdup). */
		coFuncJP_StrPtrNonGlobal,                  /** The associated string is a local or dynamics/malloc'd pointer and hence
		                                               the string will be duplicated (strdup). */
		coFuncJP_StrPtrInvalid
	 } coFuncJP_StrPtr;


	 #define __coFuncJP \
		__coObject \
		\
		char*             _prototypeName;   /**< Prototype (as string), with the name & options, is the unique identifier to this EP. */\
		coFuncJP_StrPtr   _prototypeAT;     /**< Whether prototypeName should be strdup and freed or not. */\
		char*             _optionalName;    /**< Optional other id name (as string), with the name & proto, is the unique identifier to this EP. */\
		coFuncJP_StrPtr   _optionalNameAT;  /**< Whether optionalName should be strdup and freed or not. */\
		void*             _optionalObject;  /**< Optional object ptr, with name & proto, is the unique identifier to this EP. */\
		coFuncJP_Instance _option;          /**< Which/whether an optional name or object id is to be used. */\
		void**            _funcs;           /**< Plain c-array of function pointers for the list of funcs to run. */\
		unsigned int      _funcsSize;       /**< Size of funcs array */\
		coObjectList*     _funcMetas;       /**< Meta information for the funcs. */\
		coAspectFunc*     _alwaysFirstFunc; /**< In the case where an always first is needed, func meta is noted here. */\
		coAspectFunc*     _alwaysLastFunc;  /**< In the case where an always last is needed, func meta is noted here. */

	/** Function Join point class */
	typedef struct coFuncJP { __coFuncJP } coFuncJP;
	

	/* Public member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	/** Create a new 'static' function joinpoint, which is identified by the function name and prototype typedef (as a string).
	 *   It hasn't a runtime id. This new method creates 'slow' resolution function joinpoints, which require strcmp to resolve.
	 *   Use coFuncJP_NewOpt for a fast function joinpoint implementation. */
	#define coFuncJP_new( func, proto ) \
			coFuncJP_newImp( func, coFuncJP_StrPtrNonGlobal, #proto, coFuncJP_StrPtrNonGlobal, NULL, coFuncJP_InstanceStatic )
	coFuncJP* coFuncJP_newImp(
			const char*                name,
			coFuncJP_StrPtr            nameAt,
			const char*                prototypeName,
			coFuncJP_StrPtr            protoAT,
			void*                      ptr,
			coFuncJP_Instance          option );

	/** Create a new 'run-time named' function joinpoint, which is identified by the function name, prototype typedef (as a
	 *   string) and a 'name' string. This new method creates 'slow' resolution function joinpoints, which require strcmp to
	 *   resolve. Use coFuncJP_NewOptNamed for a fast function joinpoint implementation. */
	#define coFuncJP_newNamed( func, proto, name ) \
		coFuncJP_newImp( func, coFuncJP_StrPtrNonGlobal, #proto, coFuncJP_StrPtrNonGlobal, name, coFuncJP_InstanceNamedNonGlobal )

	/** Create a new 'object identified' function joinpoint, which is identified by the function name, prototype typedef (as
	 *   a string) and an object instance (pointer). This new method creates 'slow' resolution function joinpoints, which
	 *   require strcmp to resolve. Use coFuncJP_NewOptObject for a fast function joinpoint implementation. */
	#define coFuncJP_newObject( func, proto, object ) \
		coFuncJP_newImp( func, coFuncJP_StrPtrNonGlobal, #proto, coFuncJP_StrPtrNonGlobal, object, coFuncJP_InstanceObject )

	/** Create a new 'static' function joinpoint, which is identified by the function name and prototype typedef (as a string).
	 *   It hasn't a runtime id. This new method creates 'fast' resolution function joinpoints, which use string-pointer
	 *   comparisons resolve, but then assumes 'func' and 'proto' are global strings (and lookup usage use these same strings).
	 *   Use coFuncJP_New for an easier to use function joinpoint implementation. */
	#define coFuncJP_newOpt( func, proto ) \
		coFuncJP_newImp( func, coFuncJP_StrPtrGlobal, proto, coFuncJP_StrPtrGlobal, NULL, coFuncJP_InstanceStatic )
	
	/** Create a new 'run-time named' function joinpoint, which is identified by the function name, prototype typedef (as a
	 *   string) and a 'name' string. This new method creates 'fast' resolution function joinpoints, which use string-pointer
	 *   comparisons resolve, but then assumes 'func' and 'proto' are global strings (and lookup usage use these same strings).
	 *   Use coFuncJP_New for an easier to use function joinpoint implementation. */
	#define coFuncJP_newOptNamed( func, proto, name ) \
		coFuncJP_newImp( func, coFuncJP_StrPtrGlobal, #proto, coFuncJP_StrPtrGlobal, name, coFuncJP_InstanceNamedGlobal )

	/** Create a new 'object identified' function joinpoint, which is identified by the function name, prototype typedef (as a
	 *   string) and an object instance (pointer). This new method creates 'fast' resolution function joinpoints, which use
	 *   string-pointer comparisons resolve, but then assumes 'func' and 'proto' are global strings (and lookup usage use these
	 *   same strings). Use coFuncJP_New for an easier to use function joinpoint implementation. */
	#define coFuncJP_newOptObject( func, proto, object ) \
		coFuncJP_newImp( func, coFuncJP_StrPtrGlobal, #proto, coFuncJP_StrPtrGlobal, object, coFuncJP_InstanceObject )


	/** Run a function joinpoint with no relationship between funcs and no profiling. "coFuncJP_Run"
	    will default to this unless USE_PROFILE is defined) */
	#define coFuncJP_run1( funcJP, proto, ... ) {\
		unsigned int _coFuncJP_i;\
		for( _coFuncJP_i = 0; _coFuncJP_i < funcJP->_funcMetas->count; _coFuncJP_i++ ) {\
			proto* _coFuncJP_p = (proto*)funcJP->_funcs[_coFuncJP_i];\
			_coFuncJP_p( __VA_ARGS__ );\
		}\
	}
	/** Run a function joinpoint with no relationship between funcs and where the local function
	    pointer for the loop ("func") is passed in only use if you know what you're doing
	    In some circumstances may be faster. "func" must be a pointer to the right prototype. */
	#define coFuncJP_run2( funcJP, i, func, count, ... ) {\
		for( i = 0; i < count; i++ ) {\
			func = funcJP->_funcs[i];\
			func( __VA_ARGS__ );\
		}\
	}
	/** Run a function joinpoint with no relationship between funcs with profiling */
	#define coFuncJP_runProfile( funcJP, proto, ... ) {\
		unsigned int _coFuncJP_i;\
		char _coName_p[strlen(funcJP->name)+strlen(funcJP->prototypeName)+1)];\
		strcpy( _coName_p, funcJP->name );\
		strcat( _coName_p, funcJP->prototypeName );\
		coCallGraph_push( stgCallGraph, 0, _coName_p );\
		for( _coFuncJP_i = 0; _coFuncJP_i < funcJP->_funcMetas->count; _coFuncJP_i++ ) {\
			proto* _coFuncJP_p = (proto*)funcJP->_funcs[_coFuncJP_i];\
			coCallGraph_push( stgCallGraph, _coFuncJP_p, funcJP->_funcMetas->data[_coFuncJP_i]->name );\
			_coFuncJP_p( __VA_ARGS__ );\
			coCallGraph_pop( stgCallGraph );\
		}\
		coCallGraph_pop( stgCallGraph );\
	}
	#ifdef USE_PROFILE
		#define coFuncJP_run coFuncJP_runProfile
	#else
		#define coFuncJP_run coFuncJP_run1
	#endif

	/** Operations to perform between funcs when funcs are functions (return a value) and have
	    a relationship. For use with coFuncJP_RunFunc. */
	typedef enum { coRunFuncFirst = 0, coRunFuncLast, coRunFuncMin, coRunFuncMax, coRunFuncInvalid } coRunFuncRelations;

	/** Run a function joinpoint with no relationship between funcs and no profiling. "coFuncJP_Run"
	    will default to this unless USE_PROFILE is defined) */
	#define coFuncJP_runFunc1( funcJP, proto, returnType, result, operation, ... ) {\
		switch( operation ) {\
			case coRunFuncFirst:\
				if( funcJP->_funcMetas->count ) {\
					proto* _coFuncJP_p = funcJP->_funcs[0];\
					result = _coFuncJP_p( __VA_ARGS__ );\
				}\
				break;\
			case coRunFuncLast:\
				if( funcJP->_funcMetas->count ) {\
					proto* _coFuncJP_p = funcJP->_funcs[funcJP->_funcMetas->count-1];\
					result = _coFuncJP_p( __VA_ARGS__ );\
				}\
				break;\
			case coRunFuncMin: {\
				unsigned int _coFuncJP_i;\
				returnType _coTmpResult;\
				if( funcJP->_funcMetas->count ) {\
					proto* _coFuncJP_p = funcJP->_funcs[0];\
					result =_coFuncJP_p( __VA_ARGS__ );\
					for( _coFuncJP_i = 1; _coFuncJP_i < funcJP->_funcMetas->count; _coFuncJP_i++ ) {\
						proto* _coFuncJP_p = funcJP->_funcs[_coFuncJP_i];\
						result = result < (_coTmpResult = _coFuncJP_p( __VA_ARGS__ )) ? result : _coTmpResult;\
					}\
				}\
				break;\
			}\
			case coRunFuncMax: {\
				unsigned int _coFuncJP_i;\
				returnType _coTmpResult;\
				if( funcJP->_funcMetas->count ) {\
					proto* _coFuncJP_p = funcJP->_funcs[0];\
					result = _coFuncJP_p( __VA_ARGS__ );\
					for( _coFuncJP_i = 1; _coFuncJP_i < funcJP->_funcMetas->count; _coFuncJP_i++ ) {\
						proto* _coFuncJP_p = funcJP->_funcs[_coFuncJP_i];\
						result = result > (_coTmpResult = _coFuncJP_p( __VA_ARGS__ )) ? result : _coTmpResult;\
					}\
				}\
				break;\
			}\
			default:\
				coFirewall( \
						operation >= coRunFuncInvalid, \
						coErrJrn( coFuncJP_type ), \
						"Unknown/invalid function joinpoint function operation in %s\n",\
						__func__ );\
				break;\
		}\
	}
	#ifdef USE_PROFILE
		#define coFuncJP_runFunc coFuncJP_runFuncProfile
	#else
		#define coFuncJP_runFunc coFuncJP_runFunc1
	#endif


	/** Print concise function:- print information of interest when not debugging */
	void coFuncJP_printConcise( void* funcJP, coStream* stream );
	
	/** Prepend a new function at the start of a function joinpoint's list of funcs to run. */
	coAspectFunc* coFuncJP_prepend( void* funcJP, void* func, const char* name, const char* addedBy );

	/** Prepend a new function at the start of a function joinpoint's list of funcs to run. Aspectual object provided */
	coAspectFunc* coFuncJP_prependWithAspect( void* funcJP, void* func, const char* name, const char* addedBy, void* aspect );
	
	/** Prepend a new function at the start of a function joinpoint's list of funcs to run, specifying it
	    should always remain the first func. */
	coAspectFunc* coFuncJP_prepend_AlwaysFirst( void* funcJP, void* func, const char* name, const char* addedBy );

	/** Prepend a new function at the start of a function joinpoint's list of funcs to run, specifying it
	    should always remain the first func. Aspectual object provided. */
	coAspectFunc* coFuncJP_prepend_AlwaysFirstWithAspect( void* funcJP, void* func, const char* name, const char* addedBy,
				void* aspect );
	
	/** Add a new function to a function joinpoint, at the end of the current list of funcs. If there is a func
	    already specified to always come last, the new func will be appended to come just before it. */
	coAspectFunc* coFuncJP_append( void* funcJP, void* func, const char* name, const char* addedBy );

	/** Add a new function to a function joinpoint, at the end of the current list of funcs. If there is a func
	    already specified to always come last, the new func will be appended to come just before it. Aspectual object provided.*/
	coAspectFunc* coFuncJP_appendWithAspect( void* funcJP, void* func, const char* name, const char* addedBy, void* aspect );
	
	/** Add a new function to a function joinpoint, at the end of the list of funcs - and make
	    sure the function joinpoint is always kept at the end. If this is called twice on
	    the same function joinpoint, an assert results. */
	coAspectFunc* coFuncJP_append_AlwaysLast( void* funcJP, void* func, const char* name, const char* addedBy );

	/** Add a new function to a function joinpoint, at the end of the list of funcs - and make
	    sure the function joinpoint is always kept at the end. If this is called twice on
	    the same function joinpoint, an assert results. Aspectual object provided. */
	coAspectFunc* coFuncJP_append_AlwaysLastWithAspect( void* funcJP, void* func, const char* name, const char* addedBy,
				void* aspect );

	/** Add a new function to a function joinpoint, just before the specified funcToInsertBefore in the list of funcs.
	    If the specified func to insert before doesn't exist, then asserts with an error. */
	coAspectFunc* coFuncJP_insertBefore( void* funcJP, void* func, const char* name, const char* addedBy,
				const char* funcToInsertBefore );

	/** Add a new function to a function joinpoint, just before the specified funcToInsertBefore in the list of funcs.
	    If the specified func to insert before doesn't exist, then asserts with an error. AspectualObject provided. */
	coAspectFunc* coFuncJP_insertBeforeWithAspect( void* funcJP, void* func, const char* name, const char* addedBy,
				const char* funcToInsertBefore, void* aspect );
	
	/** Add a new function to a function joinpoint, just after the specified funcToInsertAfter in the list of funcs.
	    If the specified func to insert after doesn't exist, then asserts with an error. */
	coAspectFunc* coFuncJP_insertAfter( void* funcJP, void* func, const char* name, const char* addedBy,
				const char* funcToInsertAfter );

	/** Add a new function to a function joinpoint, just after the specified funcToInsertAfter in the list of funcs.
	    If the specified func to insert after doesn't exist, then asserts with an error. Aspectual object provided. */
	coAspectFunc* coFuncJP_insertAfterWithAspect( void* funcJP, void* func, const char* name, const char* addedBy,
				const char* funcToInsertAfter, void* aspect );

	/** Replace a specific named func with a new func function. If the specified func to replace doesn't exist, then asserts with
	 *   an error. */
	coAspectFunc* coFuncJP_replace( void* funcJP, void* func, const char* name, const char* addedBy,
				const char* funcToReplace );
	
	/** Replace a specific named func with a new func function. If the specified func to replace doesn't exist, then asserts with
	 *   an error. Aspectual object provided. */
	coAspectFunc* coFuncJP_replaceWithAspect( void* funcJP, void* func, const char* name, const char* addedBy,
				const char* funcToReplace, void* aspect );
	
	/** Replace all existing funcs of a function joinpoint with a new func. */
	coAspectFunc* coFuncJP_replaceAll( void* funcJP, void* func, const char* name, const char* addedBy );

	/** Replace all existing funcs of a function joinpoint with a new func. Aspectual object provided. */
	coAspectFunc* coFuncJP_replaceAllWithAspect( void* funcJP, void* func, const char* name, const char* addedBy, void* aspect );

	/** Removes a func from a function joinpoint. If the specified func to remove doesn't exist, then asserts with an error. */
	void coFuncJP_remove( void* funcJP, const char* name );

	/** "Purge" the function joinpoint of all existing funcs, leaving it blank. */
	void coFuncJP_purge( void* funcJP );
	
	/** Query if a function joinpoint has a particular func (by name). */
	int coFuncJP_hasFunc( void* funcJP, const char* name );

	/** Return how many funcs this function joinpoint has. */
	int coFuncJP_funcCount( void* funcJP, const char* parentFunc );

	/** Print warning if this function joinpoint has no funcs. */
	void coFuncJP_warnIfNoFuncs( void* funcJP, const char* parentFunc );

	/** Assert/firewall is this function joinpoint has no funcs. */
	void coFuncJP_errorIfNoFuncs( void* funcJP, const char* parentFunction );


	/* Private member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	/** Delete implementation */
	void _coFuncJP_delete( void* funcJP );
	
	/** Print implementation */
	void _coFuncJP_print( void* funcJP, coStream* stream, coBool concise );


#endif /* __Compass_Aspect_FuncJP_h__ */
