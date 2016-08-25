/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**
** Copyright (C) Steve Quenette 2011
**
** <license>
**
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef __Compass_Aspect_memory_h__
#define __Compass_Aspect_memory_h__

	/** Compass memory allocation */
	#define coMalloc( size, label ) _coMallocImp( size, label, __FILE__, __func__, __LINE__ )
	void* _coMallocImp( 
		size_t size, 
		const char* const label, 
		const char* const fileName, 
		const char* const funcName, 
		int lineNumber );

	/** Compass memory allocation */
	#define coRealloc( ptr, newSize ) _coReallocImp( ptr, newSize, __FILE__, __func__, __LINE__ )
	void* _coReallocImp( 
		void* ptr,
		size_t newSize, 
		const char* const fileName, 
		const char* const funcName, 
		int lineNumber );

	/** Compass memory free */
	#define coFree( ptr ) _coFreeImp( ptr, __FILE__, __func__, __LINE__ )
	void _coFreeImp( 
		void* ptr,
		const char* const fileName, 
		const char* const funcName, 
		int lineNumber );
 
	/** Compass string duplicate */
	char* coStrdup( const char* const src );

	/** Compass string printf and allocate */
	int coAsprintf( char** string, char* format, ... );

#endif /* __Compass_Aspect_Object_h__ */

