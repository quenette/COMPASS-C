/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**
** <copyright>
**
** <license>
**
** $Id$
**
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef __Compass_Aspect_pointcut_h__
#define __Compass_Aspect_pointcut_h__

/* Pointcuts -------------------------------------------------------------------------------------------------------------------*/

typedef enum { coPointcutFirst = 0, coPointcutLast, coPointcutMin, coPointcutMax, coPointcutInvalid } coPointcutRelations;

#define pointcut_ptr() (coAspects_pcTop( coAspects_s )->_optionalObject)

/** A point cut is ...
 ** @param proto The function prototype of the pointcut. Type: function symbol.
 ** @param ... The parameters specified by "proto". */
#define pointcut( proto, ... ) {\
	coAspects_pcPush( coAspects_s, coJoinpointNew( (char*)__func__, proto ) );\
	coFuncJP_run( coAspects_pcTop( coAspects_s ), proto, __VA_ARGS__ );\
	coAspects_pcPop( coAspects_s ); \
}

/** A point cut is ...
 **  a function returning a value which is fed into a rolling operation per advice.
 ** @param proto The function prototype of the pointcut. Type: function symbol.
 ** @param resultType The return type. Must be the same as "proto"s return type. Type: Type.
 ** @param resultPtr The pointer to the variable that the result will be stored into. Type: resultType.
 ** @param operation The operation to perform between pointcut advices. Type: coPointcutRelations.
 ** @param ... The parameters specified by "proto". */
#define pointcutCollab( proto, resultType, resultPtr, operation, ... ) {\
	coAspects_pcPush( coAspects_s, coJoinpointNew( (char*)__func__, proto ) );\
	coFuncJP_runFunc( coAspects_pcTop( coAspects_s ), proto, resultType, resultPtr, operation, __VA_ARGS__ );\
	coAspects_pcPop( coAspects_s ); \
}

/** A point cut is ...
 **  This variant provides a run-time instance "name" to the pointcut - each unique name is a unique pointcut.
 ** @param proto The function prototype of the pointcut. Type: function symbol.
 ** @param name The instance name of the pointcut (will pointer check all, then string compare all). Type: char*.
 ** @param ... The parameters specified by "proto". */
#define pointcut_named( proto, name, ... ) {\
	coAspects_pcPush( coAspects_s, coJoinpointNewNamed( (char*)__func__, proto, name ) );\
	coFuncJP_run( coAspects_pcTop( coAspects_s ), proto, __VA_ARGS__ );\
	coAspects_pcPop( coAspects_s ); \
}

/** A point cut is ...
 **  This variant allows for a pointcut chosen collaboration between the aspects on the pointcut. The advice is implied to be
 **  a function returning a value which is fed into a rolling operation per advice.
 **  This variant provides a run-time instance "name" to the pointcut - each unique name is a unique pointcut.
 ** @param name The instance name of the pointcut (will pointer check all, then string compare all). Type: char*.
 ** @param proto The function prototype of the pointcut. Type: function symbol.
 ** @param resultType The return type. Must be the same as "proto"s return type. Type: Type.
 ** @param resultPtr The pointer to the variable that the result will be stored into. Type: resultType.
 ** @param operation The operation to perform between pointcut advices. Type: coPointcutRelations.
 ** @param ... The parameters specified by "proto". */
#define pointcutCollab_named( proto, name, resultType, resultPtr, operation, ... ) {\
	coAspects_pcPush( coAspects_s, coJoinpointNewNamed( (char*)__func__, proto, name ) );\
	coFuncJP_runFunc( coAspects_pcTop( coAspects_s ), proto, resultType, resultPtr, operation, __VA_ARGS__ );\
	coAspects_pcPop( coAspects_s ); \
}

/** A point cut is ...
 **  This variant provides a run-time instance "object" to the pointcut - each unique object (ptr) is a unique pointcut.
 ** @param object The object instance of the pointcut. Type: object ptr.
 ** @param proto The function prototype of the pointcut. Type: function symbol.
 ** @param ... The parameters specified by "proto". */
#define pointcut_object( proto, object, ... ) {\
	coAspects_pcPush( coAspects_s, coJoinpointNewObject( (char*)__func__, proto, object ) );\
	coFuncJP_run( coAspects_pcTop( coAspects_s ), proto, __VA_ARGS__ );\
	coAspects_pcPop( coAspects_s ); \
}

/** A point cut is ...
 **  This variant allows for a pointcut chosen collaboration between the aspects on the pointcut. The advice is implied to be
 **  a function returning a value which is fed into a rolling operation per advice.
 **  This variant provides a run-time instance "object" to the pointcut - each unique object (ptr) is a unique pointcut.
 ** @param object The object instance of the pointcut. Type: object ptr.
 ** @param proto The function prototype of the pointcut. Type: function symbol.
 ** @param resultType The return type. Must be the same as "proto"s return type. Type: Type.
 ** @param resultPtr The pointer to the variable that the result will be stored into. Type: resultType.
 ** @param operation The operation to perform between pointcut advices. Type: coPointcutRelations.
 ** @param ... The parameters specified by "proto". */
#define pointcutCollab_object( proto, object, resultType, resultPtr, operation, ... ) {\
	coAspects_pcPush( coAspects_s, coJoinpointNewObject( (char*)__func__, proto, object ) );\
	coFuncJP_runFunc( coAspects_pcTop( coAspects_s ), proto, resultType, resultPtr, operation, __VA_ARGS__ );\
	coAspects_pcPop( coAspects_s ); \
}

/* Pointcut Run Time Information Interface -------------------------------------------------------------------------------------*/

#define pointcut_size( pointcutFunc, proto ) coPointcutSizeImp( #pointcutFunc, #proto, coJoinpointStatic, NULL, (char*)__func__, __LINE__ )
int coPointcutSizeImp( const char* pointcutFuncName, const char* prototypeName, coJoinpointBinding binding, void* id, const char* calledByFuncName, unsigned int calledByLine );

#define pointcut_named_size( pointcutFunc, proto, name ) coPointcutSizeImp( #pointcutFunc, #proto, coJoinpointNamed, name, (char*)__func__, __LINE__ )

#define pointcut_object_size( pointcutFunc, proto, object ) coPointcutSizeImp( #pointcutFunc, #proto, coJoinpointObject, object, (char*)__func__, __LINE__ )


#define pointcut_warnIfEmpty( pointcutFunc, proto ) coPointcutWarnIfEmptyImp( #pointcutFunc, #proto, coJoinpointStatic, NULL, (char*)__func__, __LINE__ )
void coPointcutWarnIfEmptyImp( const char* pointcutFuncName, const char* prototypeName, coJoinpointBinding binding, void* id, const char* calledByFuncName, unsigned int calledByLine );

#define pointcut_named_warnIfEmpty( pointcutFunc, proto, name ) coPointcutWarnIfEmptyImp( #pointcutFunc, #proto, coJoinpointNamed, name, (char*)__func__, __LINE__ )

#define pointcut_object_warnIfEmpty( pointcutFunc, proto, object ) coPointcutWarnIfEmptyImp( #pointcutFunc, #proto, coJoinpointObject, object, (char*)__func__, __LINE__ )


#define pointcut_errorIfEmpty( pointcutFunc, proto ) coPointcutErrorIfEmptyImp( #pointcutFunc, #proto, coJoinpointStatic, NULL, (char*)__func__, __LINE__ )
void coPointcutErrorIfEmptyImp( const char* pointcutFuncName, const char* prototypeName, coJoinpointBinding binding, void* id, const char* calledByFuncName, unsigned int calledByLine );

#define pointcut_named_errorIfEmpty( pointcutFunc, proto, name ) coPointcutErrorIfEmptyImp( #pointcutFunc, #proto, coJoinpointNamed, name, (char*)__func__, __LINE__ )

#define pointcut_object_errorIfEmpty( pointcutFunc, proto, object ) coPointcutErrorIfEmptyImp( #pointcutFunc, #proto, coJoinpointObject, object, (char*)__func__, __LINE__ )


/* Advices ---------------------------------------------------------------------------------------------------------------------*/

/** Provides the pointer to the aspectual object that the advice was made from.
 ** @returns The pointer or NULL if no aspect was provided by advice_aspect. Type: pointer. */
#define aspect_ptr() coAspectImp( __func__, __func__, __LINE__ )
coBool _coAspectImp( const char* funcName, const char* calledByFuncName, unsigned int calledByLine, unsigned int* ep_i, unsigned int* func_i );
void* coAspectImp( const char* funcName, const char* calledByFuncName, unsigned int calledByLine );

/** Macro to stringify symbols.
 ** @param sym The symbol to stringify. Type: symbol.
 ** @returns The string. Type: c-string. */
#define stringify( sym ) (char*)#sym

typedef enum {
			coAdvicePrepend = 0,            /** Weave as the first aspect at the pointcut. */
			coAdviceAppend,                 /** Weave as the last aspect at the pointcut. */
			coAdviceAlwaysFirst,            /** Guarantee to weave as the first aspect at the pointcut. */
			coAdviceAlwaysLast,             /** Guarantee to weave as the last aspect at the pointcut. */
			coAdviceAround,                 /** Weave around the pointcut (replace all aspects). */
			coAdviceRemove,                 /** If exists in pointcut, un-weave (remove) given aspect. */
			coAdviceInvalid,
			coAdviceInsertBefore,           /** Relative operation - Weave before given aspect. */
			coAdviceInsertAfter,            /** Relative operation - Weave after given aspect. */
			coAdviceInsertBeforeElseAppend, /** Relative operation - If exists in pointcut, weave before given aspect, else append.  */
			coAdviceInsertAfterElsePrepend, /** Relative operation - If exists in pointcut, weave after given aspect, else prepend. */
			coAdviceInsertBeforeElseIgnore, /** Relative operation - If exists in pointcut, weave before given aspect, else don't weave. */
			coAdviceInsertAfterElseIgnore,  /** Relative operation - If exists in pointcut, weave after given aspect, else don't weave.  */
			coAdviceReplace,                /** Relative operation - If exists in pointcut, weave around (replace) given aspect. */
			coAdviceRelativeInvalid
} coAdviceFunctions;


/* TODO: UNDO THIS... A note on advices: "name"s are strings rather than symbols because its envision that pointcuts may be defined by an explicit
   name or possibly even "renamed" in the future, even though this functionality doesn't exist right now.
   ... Causes "stringify" EVERYWHERE and the aforementioned name is now an additional, optional argument */


/** An advice is ...
 ** @param pointcutFunc The function the pointcut is within. Type: function symbol.
 ** @param proto The function prototype of the pointcut. Type: function prototype.
 ** @param func The function to advise into the pointcut. Type: function symbol.
 ** @param relation The relation this function has relative to other functions in the pointcut. Type: coAdviceFunc. */
#define advice( pointcutFunc, proto, func, adviceFunc ) {\
	proto* _coCheckFuncProto_p = func;\
	coAdviceImp( #pointcutFunc, #proto, coJoinpointStatic, NULL, _coCheckFuncProto_p, #func, (char*)__FILE__, (char*)__func__, __LINE__, adviceFunc, 0, NULL );\
}
void coAdviceImp( const char* pointcutFuncName, const char* prototypeName, coJoinpointBinding binding, void* id, void* func, const char* funcName, const char* calledByFileName, const char* calledByFuncName, unsigned int calledByLine, coAdviceFunctions adviceFunc, int hasAspect, void* aspect );

/** An advice is ...
 ** This variant allows an aspectual object (the 'this/self' of the object/function calling this advice) to be associated.
 ** @param pointcutFunc The function the pointcut is within. Type: function symbol.
 ** @param proto The function prototype of the pointcut. Type: function prototype.
 ** @param func The function to advise into the pointcut. Type: function symbol.
 ** @param relation The relation this function has relative to other functions in the pointcut. Type: coAdviceFunc.
 ** @param aspect The aspectual object that this pointcut advice belongs to (i.e. this advice is part of an object, this is how
 **    to pass self/this to the adviced function. Type: pointer. */
#define advice_aspect( pointcutFunc, proto, func, adviceFunc, aspect ) {\
	proto* _coCheckFuncProto_p = func;\
	coAdviceImp( #pointcutFunc, #proto, coJoinpointStatic, NULL, _coCheckFuncProto_p, #func, (char*)__FILE__, (char*)__func__, __LINE__, adviceFunc, 1, aspect );\
}

/** An advice is ...
 ** This variant also uses a c-string to uniquely identify the joinpoint/pointcut instance (run-time eval).
 ** @param pointcutFunc The function the pointcut is within. Type: function symbol.
 ** @param proto The function prototype of the pointcut. Type: function prototype.
 ** @param name The instance name of the pointcut (will pointer check all, then string compare all). Type: char*.
 ** @param func The function to advise into the pointcut. Type: function symbol.
 ** @param relation The relation this function has relative to other functions in the pointcut. Type: coAdviceFunc.*/
#define advice_named( pointcutFunc, proto, name, func, adviceFunc ) {\
	proto* _coCheckFuncProto_p = func;\
	coAdviceImp( #pointcutFunc, #proto, coJoinpointNamed, name, _coCheckFuncProto_p, #func, (char*)__FILE__, (char*)__func__, __LINE__, adviceFunc, 0, NULL );\
}

/** An advice is ...
 ** This variant also uses a c-string to uniquely identify the joinpoint/pointcut instance (run-time eval).
 ** This variant allows an aspectual object (the 'this/self' of the object/function calling this advice) to be associated.
 ** @param pointcutFunc The function the pointcut is within. Type: function symbol.
 ** @param proto The function prototype of the pointcut. Type: function prototype.
 ** @param name The instance name of the pointcut (will pointer check all, then string compare all). Type: char*.
 ** @param func The function to advise into the pointcut. Type: function symbol.
 ** @param relation The relation this function has relative to other functions in the pointcut. Type: coAdviceFunc.
 ** @param aspect The aspectual object that this pointcut advice belongs to (i.e. this advice is part of an object, this is how
 **    to pass self/this to the adviced function. Type: pointer. */
#define advice_aspect_named( pointcutFunc, proto, name, func, adviceFunc, aspect ) {\
	proto* _coCheckFuncProto_p = func;\
	coAdviceImp( #pointcutFunc, #proto, coJoinpointNamed, name, _coCheckFuncProto_p, #func, (char*)__FILE__, (char*)__func__, __LINE__, adviceFunc, 1, aspect );\
}


/** An advice is ...
 ** This variant also uses an object to uniquely identify the joinpoint/pointcut instance (run-time eval).
 ** @param pointcutFunc The function the pointcut is within. Type: function symbol.
 ** @param proto The function prototype of the pointcut. Type: function prototype.
 ** @param object The object instance of the pointcut. Type: object ptr.
 ** @param func The function to advise into the pointcut. Type: function symbol.
 ** @param relation The relation this function has relative to other functions in the pointcut. Type: coAdviceFunc.*/
#define advice_object( pointcutFunc, proto, object, func, adviceFunc ) {\
	proto* _coCheckFuncProto_p = func;\
	coAdviceImp( #pointcutFunc, #proto, coJoinpointObject, object, _coCheckFuncProto_p, #func, (char*)__FILE__, (char*)__func__, __LINE__, adviceFunc, 0, NULL );\
}

/** An advice is ...
 ** This variant also uses an object to uniquely identify the joinpoint/pointcut instance (run-time eval).
 ** This variant allows an aspectual object (the 'this/self' of the object/function calling this advice) to be associated.
 ** @param pointcutFunc The function the pointcut is within. Type: function symbol.
 ** @param proto The function prototype of the pointcut. Type: function prototype.
 ** @param object The object instance of the pointcut. Type: object ptr.
 ** @param func The function to advise into the pointcut. Type: function symbol.
 ** @param relation The relation this function has relative to other functions in the pointcut. Type: coAdviceFunc.
 ** @param aspect The aspectual object that this pointcut advice belongs to (i.e. this advice is part of an object, this is how
 **    to pass self/this to the adviced function. Type: pointer. */
#define advice_aspect_object( pointcutFunc, proto, object, func, adviceFunc, aspect ) {\
	proto* _coCheckFuncProto_p = func;\
	coAdviceImp( #pointcutFunc, #proto, coJoinpointObject, object, _coCheckFuncProto_p, #func, (char*)__FILE__, (char*)__func__, __LINE__, adviceFunc, 1, aspect );\
}

/** An advice is ...
 ** This variant also uses an object to uniquely identify the joinpoint/pointcut instance (run-time eval).
 ** This variant allows an aspectual object (the 'this/self' of the object/function calling this advice) to be associated.
 ** In this variant, the object and aspectual object are the same - i.e. the object is advicing itself.
 ** @param pointcutFunc The function the pointcut is within. Type: function symbol.
 ** @param proto The function prototype of the pointcut. Type: function prototype.
 ** @param aspectualObject The object instance of the pointcut and the aspectual object that this pointcut advice belongs to
 **    (i.e. this advice is part of an object, this is how to pass self/this to the adviced function. Type: object ptr.
 ** @param func The function to advise into the pointcut. Type: function symbol.
 ** @param relation The relation this function has relative to other functions in the pointcut. Type: coAdviceFunc. */
#define advice_self( pointcutFunc, proto, apectualObject, func, adviceFunc ) {\
	proto* _coCheckFuncProto_p = func;\
	coAdviceImp( #pointcutFunc, #proto, coJoinpointObject, apectualObject, _coCheckFuncProto_p, #func, (char*)__FILE__, (char*)__func__, __LINE__, adviceFunc, 1, apectualObject );\
}



/** A relative advice is ... 
 ** This variant is for advice operations with that are relative to given previous advices.
 ** @param pointcutFunc The function the pointcut is within. Type: function symbol.
 ** @param proto The function prototype of the pointcut. Type: function prototype.
 ** @param func The function to advise into the pointcut. Type: function symbol.
 ** @param relation The relation this function has relative to other functions in the pointcut. Type: coAdviceFunc.
 ** @param relName The name of the function this function is relative to. Type: char*. */
#define radvice( pointcutFunc, proto, func, adviceFunc, relName ) {\
	proto* _coCheckFuncProto_p = func;\
	coAdviceRelImp( #pointcutFunc, #proto, coJoinpointStatic, NULL, _coCheckFuncProto_p, #func, (char*)__FILE__, (char*)__func__, __LINE__, adviceFunc, relName, 0, NULL );\
}
void coAdviceRelImp( char* pointcutFuncName, char* prototypeName, coJoinpointBinding binding, void* id, void* func, char* funcName, const char* calledByFileName, const char* calledByFuncName, unsigned int calledByLine, coAdviceFunctions adviceFunc, char* relName, int hasAspect, void* aspect );

/** A relative advice is ...
 ** This variant is for advice operations with that are relative to given previous advices.
 ** This variant allows an aspectual object (the 'this/self' of the object/function calling this advice) to be associated.
 ** @param pointcutFunc The function the pointcut is within. Type: function symbol.
 ** @param proto The function prototype of the pointcut. Type: function prototype.
 ** @param func The function to advise into the pointcut. Type: function symbol.
 ** @param relation The relation this function has relative to other functions in the pointcut. Type: coAdviceFunc.
 ** @param relName The name of the function this function is relative to. Type: char*.
 ** @param aspect The aspectual object that this pointcut advice belongs to (i.e. this advice is part of an object, this is how
 **    to pass self/this to the adviced function. Type: pointer. */
#define radvice_aspect( pointcutFunc, proto, func, adviceFunc, relName, aspect ) {\
	proto* _coCheckFuncProto_p = func;\
	coAdviceRelImp( #pointcutFunc, #proto, coJoinpointStatic, NULL, _coCheckFuncProto_p, #func, (char*)__FILE__, (char*)__func__, __LINE__, adviceFunc, relName, 1, aspect );\
}

/** An advice is ...
 ** This variant is for advice operations with that are relative to given previous advices.
 ** This variant also uses a c-string to uniquely identify the joinpoint/pointcut instance (run-time eval).
 ** @param pointcutFunc The function the pointcut is within. Type: function symbol.
 ** @param proto The function prototype of the pointcut. Type: function prototype.
 ** @param name The instance name of the pointcut (will pointer check all, then string compare all). Type: char*.
 ** @param func The function to advise into the pointcut. Type: function symbol.
 ** @param relation The relation this function has relative to other functions in the pointcut. Type: coAdviceFunc.
 ** @param relName The name of the function this function is relative to. Type: char*. */
#define radvice_named( pointcutFunc, proto, name, func, adviceFunc, relName ) {\
	proto* _coCheckFuncProto_p = func;\
	coAdviceRelImp( #pointcutFunc, #proto, coJoinpointNamed, name, _coCheckFuncProto_p, #func, (char*)__FILE__, (char*)__func__, __LINE__, adviceFunc, relName, 0, NULL );\
}

/** An advice is ...
 ** This variant is for advice operations with that are relative to given previous advices.
 ** This variant also uses a c-string to uniquely identify the joinpoint/pointcut instance (run-time eval).
 ** This variant allows an aspectual object (the 'this/self' of the object/function calling this advice) to be associated.
 ** @param pointcutFunc The function the pointcut is within. Type: function symbol.
 ** @param proto The function prototype of the pointcut. Type: function prototype.
 ** @param name The instance name of the pointcut (will pointer check all, then string compare all). Type: char*.
 ** @param func The function to advise into the pointcut. Type: function symbol.
 ** @param relation The relation this function has relative to other functions in the pointcut. Type: coAdviceFunc.
 ** @param relName The name of the function this function is relative to. Type: char*.
 ** @param aspect The aspectual object that this pointcut advice belongs to (i.e. this advice is part of an object, this is how
 **    to pass self/this to the adviced function. Type: pointer. */
#define radvice_aspect_named( pointcutFunc, proto, name, func, adviceFunc, relName, aspect ) {\
		proto* _coCheckFuncProto_p = func;\
		coAdviceRelImp( #pointcutFunc, #proto, coJoinpointNamed, named, _coCheckFuncProto_p, #func, (char*)__FILE__, (char*)__func__, __LINE__, adviceFunc, relName, 1, aspect );\
}

/** An advice is ...
 ** This variant is for advice operations with that are relative to given previous advices.
 ** This variant also uses an object to uniquely identify the joinpoint/pointcut instance (run-time eval).
 ** @param pointcutFunc The function the pointcut is within. Type: function symbol.
 ** @param proto The function prototype of the pointcut. Type: function prototype.
 ** @param object The object instance of the pointcut. Type: object ptr.
 ** @param func The function to advise into the pointcut. Type: function symbol.
 ** @param relation The relation this function has relative to other functions in the pointcut. Type: coAdviceFunc.
 ** @param relName The name of the function this function is relative to. Type: char*. */
#define radvice_object( pointcutFunc, proto, object, func, adviceFunc, relName ) {\
		proto* _coCheckFuncProto_p = func;\
		coAdviceRelImp( #pointcutFunc, #proto, coJoinpointObject, object, _coCheckFuncProto_p, #func, (char*)__FILE__, (char*)__func__, __LINE__, adviceFunc, relName, 0, NULL );\
}

/** An advice is ...
 ** This variant is for advice operations with that are relative to given previous advices.
 ** This variant also uses an object to uniquely identify the joinpoint/pointcut instance (run-time eval).
 ** This variant allows an aspectual object (the 'this/self' of the object/function calling this advice) to be associated.
 ** @param pointcutFunc The function the pointcut is within. Type: function symbol.
 ** @param proto The function prototype of the pointcut. Type: function prototype.
 ** @param object The object instance of the pointcut. Type: object ptr.
 ** @param func The function to advise into the pointcut. Type: function symbol.
 ** @param relation The relation this function has relative to other functions in the pointcut. Type: coAdviceFunc.
 ** @param relName The name of the function this function is relative to. Type: char*.
 ** @param aspect The aspectual object that this pointcut advice belongs to (i.e. this advice is part of an object, this is how
 **    to pass self/this to the adviced function. Type: pointer. */
#define radvice_aspect_object( pointcutFunc, proto, object, func, adviceFunc, relName, aspect ) {\
		proto* _coCheckFuncProto_p = func;\
		coAdviceRelImp( #pointcutFunc, #proto, coJoinpointObject, object, _coCheckFuncProto_p, #func, (char*)__FILE__, (char*)__func__, __LINE__, adviceFunc, relName, 1, aspect );\
}

/** An advice is ...
 ** This variant is for advice operations with that are relative to given previous advices.
 ** This variant also uses an object to uniquely identify the joinpoint/pointcut instance (run-time eval).
 ** This variant allows an aspectual object (the 'this/self' of the object/function calling this advice) to be associated.
 ** @param pointcutFunc The function the pointcut is within. Type: function symbol.
 ** @param proto The function prototype of the pointcut. Type: function prototype.
 ** @param aspectualObject The object instance of the pointcut and the aspectual object that this pointcut advice belongs to
 **    (i.e. this advice is part of an object, this is how to pass self/this to the adviced function. Type: object ptr.
 ** @param func The function to advise into the pointcut. Type: function symbol.
 ** @param relation The relation this function has relative to other functions in the pointcut. Type: coAdviceFunc.
 ** @param relName The name of the function this function is relative to. Type: char*. */
#define radvice_self( pointcutFunc, proto, aspectualObject, func, adviceFunc, relName ) {\
		proto* _coCheckFuncProto_p = func;\
		coAdviceRelImp( #pointcutFunc, #proto, coJoinpointObject, aspectualObject, _coCheckFuncProto_p, #func, (char*)__FILE__, (char*)__func__, __LINE__, adviceFunc, relName, 1, aspectualObject );\
}

#endif /* __Compass_Aspect_pointcut_h__ */
