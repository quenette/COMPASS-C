/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**
** <copyright>
**
** <license>
**
** $Id$
**
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef __Compass_Aspect_Aspects_h__
#define __Compass_Aspect_Aspects_h__
	

	/* Textual name of this class */
	extern const char* coAspects_type;
	
	/* Run-time advice info. */
	typedef struct {
		char* file;
		char* func;
		unsigned int line;
		void* aspect;
		coJoinpointBinding binding;
		void* id;
	} coAdviceEntry;

	#define __coAspects \
		__coObject \
		\
		unsigned int    count; \
		size_t          _size; \
		size_t          _delta; \
		coAdviceEntry*  advices; \
		coJPs*          jps; \
		coBool          hasWeaved; \
		coFuncJP**      _pcStack; \
		unsigned int    _pcStackTop; \
		size_t          _pcStackSize; \
		size_t          _pcStackDelta; \
		unsigned int    mode;
	
	typedef struct _coAspects { __coAspects } coAspects;
	extern coAspects* coAspects_s; /**< Aspect oriented programming management singleton. */
	
	
	/** Create a new coAspects */
	coAspects* coAspects_new( unsigned int mode );
	
	
	/* Add a new static/structural advice entry/call. Note that func stored only as a copy of the pointer. It is not deleted. Its
	 * is expected to be the result of a __func__ and as such an entry in the string table. */
	unsigned int coAspects_addAdvice( void* aspects, const char* file, const char* func, unsigned int line, coJoinpointBinding binding, void* id );
	
	/* Add a new advice entry/call. Note that func stored only as a copy of the pointer. It is not deleted. Its is expected to
	 * be the result of a __func__ and as such an entry in the string table. This version acknowledges the advice is of an
	 * aspectual object. */
	unsigned int coAspects_addAdviceAspect( void* aspects, const char* file, const char* func, unsigned int line, void* aspect, coJoinpointBinding binding, void* id );

	/** Find the given static/structural advice entry/call in the register. If it is not found, return -1. Only matches when
	 *  when advice is provided without an aspectual object. */
	unsigned int coAspects_findAdvice( void* aspects, const char* func, unsigned int line );
	
	/** Find the given aspectual object advice entry/call in the register. If it is not found, return -1. */
	unsigned int coAspects_findAdviceAspect( void* aspects, const char* func, unsigned int line, void* aspect );

	/* Get an advice entry. */
	#define coAspects_adviceAt( aspects, handle )		((aspects)->advices[(handle)] )
	coAdviceEntry _coAspects_adviceAt( void* aspects, unsigned int handle );

	#define coAspects_pcPush( aspects, pc ) _coAspects_pcPush( aspects, pc, __FILE__, __func__, __LINE__ )
	coFuncJP* _coAspects_pcPush( void* aspects, coFuncJP* pc, const char* file, const char* func, unsigned int line );
	coFuncJP* coAspects_pcPop( void* aspects );
	#define coAspects_pcTop( aspects )      (aspects->_pcStack[aspects->_pcStackTop])
	coFuncJP* _coAspects_pcTop( void* aspects );


	/** Weave actions the weaving process. At this point, it is not clearly defined, but at least the following...
	 **   - Disables further advices
	 **   - Dumps out advice info
	 **   - Required BEFORE a pointcut can be called.
	 **   - is only in effect if the mode include 'coStaticWeave'. If not in this mode - the call will still dump info. */
	void coAspects_weave( void* aspects );

	/* Reset/restart the aspects mechanism... use in testing rather than creating/destroying */
	void coAspects_reset( void* aspects );

	
	/* Private member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


	/* Delete implementation */
	void _coAspects_delete( void* aspects );

	/* Print implementation */
	void _coAspects_print( void* aspects, coStream* stream, coBool concise );


#endif /* __Compass_Aspect_Aspects_h__ */

