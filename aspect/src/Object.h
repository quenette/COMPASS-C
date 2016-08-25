/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**
** Copyright (C) Steve Quenette 2011
**
** <license>
**
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef __Compass_Aspect_Object_h__
#define __Compass_Aspect_Object_h__

	/** Textual name for coObject class. */
	extern const char* coObject_type;

	typedef void (coObject_delete_FP) ( void* obj );
	typedef void (coObject_print_FP)  ( void* obj, coStream* stream, coBool concise );

	#define __coObject \
		const char*         type;        /**< The object's type (c-string, wont be freed AND only pointer compared) */ \
		size_t              _sizeOfSelf; /**< Final size of this object. */ \
		int                 _deleteSelf; /**< True if is to be deallocated in Delete */ \
		\
		coObject_delete_FP* _delete;     /**< Virtual function for deleting an object. */ \
		coObject_print_FP*  _print;      /**< Virtual function for printing an object. */ \
		\
		char*               name;        /**< The object's name (c-string, will make copy, will use strcmp, will delete. */
	
	/** Object class */
	typedef struct _coObject { __coObject } coObject;
	

	/* Public member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	
	/* Abstract coObject constructor */
	coObject* coObject_new( 
		size_t _sizeOfSelf,
		const char* type, 
		coObject_delete_FP* _delete, 
		coObject_print_FP* _print, 
		const char* name );
		
	/** Delete coObject */
	void coObject_delete( void* object );
	
	/** Print the object. */
	void coObject_print( void* object, coStream* stream, coBool concise );

	/* Get the object's type */
	const char* coObject_getType( void* _class ); 

	/** Sets the name of the object. */
	void coObject_setName( void* object, const char* name );
	
	/** Get the object's name. */
	const char* coObject_getName( void* object );
	

	/* Private member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


	/** coObject delete implementation. */
	void _coObject_delete( void* object );
	
	/** coObject print implementation. */
	void _coObject_print( void* object, coStream* stream, coBool concise );
	

#endif /* __Compass_Aspect_Object_h__ */

