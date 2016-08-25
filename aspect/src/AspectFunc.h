/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**
** Copyright (C) Steve Quenette 2011
**
** <license>
**
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef __Compass_Aspect_AspectFunc_h__
#define __Compass_Aspect_AspectFunc_h__
	
	/** Textual name of this class */
	extern const char* coAspectFunc_type;

	#define __coAspectFunc \
		__coObject \
		\
		void*  funcPtr;     /** The function pointer to the adviced function. */ \
		char*  fromFunc;    /** The name of the function advicing. c-string that is copied & freed. */ \
		coBool hasAspect;   /** Whether the advice was with an aspectual object/ptr. */ \
		void*  fromAspect;  /** The ascpectual object/ptr if provided. */

	/** coAspectFunc class */
	typedef struct _coAspectFunc { __coAspectFunc } coAspectFunc;
	

	/* Public member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


	/** Create a new coAspectFunc. Note programmatically there's no reason to not allow a user to provide an aspect of
	 *   NULL, but we have to know when a user asks for an aspect when none is provided. For this reason we have to two styles
	 *   constructors. This one hasn't an aspect provided. */
	coAspectFunc* coAspectFunc_new( const char* name, void* funcPtr, const char* fromFunc );
	
	/** Create a new coAspectFunc. Note programmatically there's no reason to not allow a user to provide an aspect of
	 *   NULL, but we have to know when a user asks for an aspect when none is provided. For this reason we have to two styles
	 *   constructors. This one has an aspect provided. */
	coAspectFunc* coAspectFunc_newWithAspect( const char* name, void* funcPtr, const char* fromFunc, void* fromAspect );
	

	/* Private member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


	/** Delete implementation */
	void _coAspectFunc_delete( void* aspectFunc );
	
	/** Print implementation */
	void _coAspectFunc_print( void* aspectFunc, coStream* stream, coBool concise );

	
#endif /* __Compass_Aspect_AspectFunc_h__ */

