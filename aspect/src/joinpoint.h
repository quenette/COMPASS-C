/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**
** <copyright>
**
** <license>
**
** $Id$
**
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef __Compass_Aspect_joinpoint_h__
#define __Compass_Aspect_joinpoint_h__

	typedef enum {
		coJoinpointStatic = 0,            /** Joinpoint is functional (not run-time instance). */
		coJoinpointNamed,                     /** Joinpoint is named with a run-time c-string. */
		coJoinpointObject,                     /** Joinpoint is on an object instance. */
		coJoinpointInvalid
	} coJoinpointBinding;


	/** Returns a new entry point if one doesn't already exist for the given pc/prototype.
	 ** @param pc The name of the pointcut (typically a target feature's member). Type: char*.
	 ** @param proto The function prototype of the pointcut. Type: function symbol.
	 ** @returns The new entry point object if it created one or the existing object if it already existed. Type: stgEP*. */
	#define coJoinpointNew( pc, proto ) coJoinpointNewImp( pc, #proto, coJoinpointStatic, NULL, (char*)__func__, __LINE__ )
	coFuncJP* coJoinpointNewImp( const char* pointcutFuncName, const char* prototypeName, coJoinpointBinding binding, void* id, const char* calledByFuncName, unsigned int calledByLine );

	/** Returns a new entry point if one doesn't already exist for the given pc/prototype.
	 ** @param pc The name of the pointcut (typically a target feature's member). Type: char*.
	 ** @param proto The function prototype of the pointcut. Type: function symbol.
	 ** @returns The new entry point object if it created one or the existing object if it already existed. Type: stgEP*. */
	#define coJoinpointNewNamed( pc, proto, name ) coJoinpointNewImp( pc, #proto, coJoinpointNamed, name, (char*)__func__, __LINE__ )

	/** Returns a new entry point if one doesn't already exist for the given pc/prototype.
	 ** @param pc The name of the pointcut (typically a target feature's member). Type: char*.
	 ** @param proto The function prototype of the pointcut. Type: function symbol.
	 ** @returns The new entry point object if it created one or the existing object if it already existed. Type: stgEP*. */
	#define coJoinpointNewObject( pc, proto, object ) coJoinpointNewImp( pc, #proto, coJoinpointObject, object, (char*)__func__, __LINE__ )


	/** Returns the entry point with the given function/static join-point.
	 ** @param pc The name of the pointcut (typically a target feature's member). Type: char*.
	 ** @param proto The function prototype of the pointcut. Type: function symbol.
	 ** @returns The entry point object or 0 if it fails to find it. Type: stgEP*. */
	#define coJoinpointGet( pc, proto ) coJoinpointGetImp( pc, #proto, coJoinpointStatic, NULL, (char*)__func__, __LINE__ )
	coFuncJP* coJoinpointGetImp( const char* pointcutFuncName, const char* prototypeName, coJoinpointBinding binding, void* id, const char* calledByFuncName, unsigned int calledByLine );

	/** Returns the entry point with the given named join-point instance.
	 ** @param pc The name of the pointcut (typically a target feature's member). Type: char*.
	 ** @param proto The function prototype of the pointcut. Type: function symbol.
	 ** @param name The c-string name (will pointer check all, then string compare all). Type: char*.
	 ** @returns The entry point object or 0 if it fails to find it. Type: stgEP*. */
	#define coJoinpointGetNamed( pc, proto, name ) coJoinpointGetImp( pc, #proto, coJoinpointNamed, name, (char*)__func__, __LINE__ )

	/** Returns the entry point with the given object join-point instance.
	 ** @param pc The name of the pointcut (typically a target feature's member). Type: char*.
	 ** @param proto The function prototype of the pointcut. Type: function symbol.
	 ** @param object The object instance that this join-point belongs to. Type: void*.
	 ** @returns The entry point object or 0 if it fails to find it. Type: stgEP*. */
	#define coJoinpointGetObject( pc, proto, object ) coJoinpointGetImp( pc, #proto, coJoinpointObject, object, (char*)__func__, __LINE__ )

#endif /* __Compass_Aspect_joinpoint_h__ */
