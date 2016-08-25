/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**
** <copyright>
**
** <license>
**
** $Id$
**
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef __Compass_Aspect_JPs_h__
#define __Compass_Aspect_JPs_h__
	

	/* Textual name of this class */
	extern const char* coJPs_type;
	
	#define __coJPs \
		__coObject \
		\
		unsigned int count; \
		size_t _size; \
		size_t _delta; \
		coFuncJP** jp;
	
	/* Join point collection class */
	typedef struct _coJPs { __coJPs } coJPs;
	

	/* Public member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	
	/** Create a new coJPs */
	coJPs* coJPs_new( void );
	
	/* Add a new elementType */
	unsigned int coJPs_add( void* jps, void* entryPoint );
	
	/** Find the given entry point in the register. If it is not found, return -1. "name" and "prototypeName" parameters
	    are of type "Type", meaning that the find will try an integer comparison of the pointer values before falling back
	    to a string compare. */
	unsigned int coJPs_find( void* jps, const char* name, const char* prototypeName );
	
	/** Find the given entry point in the register. If it is not found, return -1. "name" and "prototypeName" parameters
	    are of type "Type", meaning that the find will try an integer comparison of the pointer values before falling back
	    to a string compare. */
	unsigned int coJPs_findNamed( void* jps, const char* name, const char* prototypeName, char* optionalName );

	/** Find the given entry point in the register. If it is not found, return -1. "name" and "prototypeName" parameters
	    are of type "Type", meaning that the find will try an integer comparison of the pointer values before falling back
	    to a string compare. */
	unsigned int coJPs_findObject( void* jps, const char* name, const char* prototypeName, void* optionalObject );

	/** Find the given entry point (by pointer) in the register. If it is not found, return -1. */
	unsigned int coJPs_findByJoinPoint( void* jps, void* jp );
	
	/* Get an element type from the register... SLOW */
	#define coJPs_at( jps, handle ) ( (jps)->jp[(handle)] )
	coFuncJP* _coJPs_at( void* jps, unsigned int handle );

	/* Reset/restart the EP register mechanism... use in testing rather than creating/destroying */
	void coJPs_reset( void* jps );


	/* Private member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	/* Delete implementation */
	void _coJPs_delete( void* jps );

	/* Print implementation */
	void _coJPs_print( void* jps, coStream* stream, coBool concise );

	
#endif /* __Compass_Aspect_JPs_h__ */

