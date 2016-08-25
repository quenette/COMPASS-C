/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**
** Copyright (C) Steve Quenette 2011
**
** <license>
**
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**	Interface to store items are categorised into the following types:
**         - Stg_Object.
**           Any instance of Stg_Object or one of its sub-classes contain the name (key) as a property
**           and functions to delete, print and copy itself.
**
**         - C Pointer.
**           A traditional C object, array or multi-dimensional array. It requires a name (key) as well as the
**           functions to delete, print and copy ifself.
**
**         - Global C Pointer.
**           A C pointer, except it is a global object so the responsibility of deleting, printing or copying itself
**           belongs elsewhere. All it requires is the name (key) to be associated with.
**	
**/

#ifndef __Compass_Aspect_OL_h__
#define __Compass_Aspect_OL_h__

	
	/** Textual name of this class */
	extern const char* coOL_Type;

	typedef enum coOL_Former {
		coOL_FormerKeep,
		coOL_FormerDelete,
		coOL_FormerInvalid
	} coOL_Former;

	#define __coObjectList \
		__coObject \
		\
		coObject**       data; \
		unsigned int     count; \
		unsigned int     _size; \
		unsigned int     _delta; \

	typedef struct _coObjectList { __coObjectList } coObjectList;
	
	
	/* Public member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	
	/* Object new and construct interface */
	coObjectList* coOL_new();
	
	/** Append Object instance to list. Returns the index where the new object was inserted (the last element) */
	unsigned int coOL_append( void* objectList, void* objectPtr );
	
#if 0 /*Yet to implement */
	/** Append C pointer to list. Returns the index where the new object was inserted 
	(the last element). See notes in ObjectAdaptor.h about the responsibilities of the
	3 function pointers required - especially the DeletePointer function. */
	unsigned int coOL_pointerAppend(
		void*						objectList, 
		void*						objectPtr, 
		const char*						name,
		Stg_ObjectAdaptor_DeletePointerFunction*	ptrDelete,
		Stg_ObjectAdaptor_PrintPointerFunction*		ptrPrint );
	
	/** Append global C pointer to list. Returns the index where the new object was inserted (the last element) */
	unsigned int coOL_globalPointerAppend( void* objectList, void* objectPtr, const char* name );
#endif
	
	/** Prepend object to list */
	unsigned int coOL_prepend( void* objectList, void* objectPtr );
	
#if 0
	/** Prepend pointer to list */
	unsigned int coOL_pointerPrepend(
		void*						objectList, 
		void*						objectPtr, 
		const char*						name,
		Stg_ObjectAdaptor_DeletePointerFunction*	ptrDelete,
		Stg_ObjectAdaptor_PrintPointerFunction*		ptrPrint );
	
	/** Prepend global pointer to list */
	unsigned int coOL_globalPointerPrepend( void* objectList, void* objectPtr, const char* name );
#endif
	
	/** Replace whole list with a object */
	unsigned int coOL_replaceAll( void* objectList, coOL_Former option, void* objectPtr );
	
#if 0 /*Yet to implement */
	/** Replace whole list with a pointer */
	unsigned int coOL_pointerReplaceAll(
		void*						objectList, 
		coOL_Former				option,
		void*						objectPtr, 
		const char*						name,
		Stg_ObjectAdaptor_DeletePointerFunction*	ptrDelete,
		Stg_ObjectAdaptor_PrintPointerFunction*		ptrPrint );
	
	/** Replace whole list with a global pointer */
	unsigned int coOL_globalPointerReplaceAll( void* objectList, coOL_Former option, void* objectPtr, const char* name );
#endif
	
	/** Replace a specific entry with a object */
	unsigned int coOL_replace( void* objectList, const char* toReplace, coOL_Former option, void* objectPtr );
	
#if 0 /*Yet to implement */
	/** Replace a specific entry with a pointer*/
	unsigned int coOL_pointerReplace(
		void*						objectList, 
		const char*						toReplace,
		coOL_Former				option,
		void*						objectPtr, 
		const char*						name,
		Stg_ObjectAdaptor_DeletePointerFunction*	ptrDelete,
		Stg_ObjectAdaptor_PrintPointerFunction*		ptrPrint );
	
	/** Replace a specific entry with a global pointer */
	unsigned int coOL_globalPointerReplace( void* objectList, const char* toReplace, coOL_Former option, void* objectPtr, const char* name );
#endif
	
	/** Insert object before a specific entry */
	unsigned int coOL_insertBefore( void* objectList, const char* reference, void* objectPtr );
	
#if 0 /*Yet to implement */
	/** Insert pointer before a specific entry */
	unsigned int coOL_pointerInsertBefore(
		void*						objectList, 
		const char*						reference,
		void*						objectPtr, 
		const char*						name,
		Stg_ObjectAdaptor_DeletePointerFunction*	ptrDelete,
		Stg_ObjectAdaptor_PrintPointerFunction*		ptrPrint );
	
	/** Insert global pointer before a specific entry */
	unsigned int coOL_globalPointerInsertBefore( void* objectList, const char* reference, void* objectPtr, const char* name );
#endif

	/** Insert object after a specific entry */ 
	unsigned int coOL_insertAfter( void* objectList, const char* reference, void* objectPtr );
	
#if 0 /*Yet to implement */
	/** Insert pointer after a specific entry */ 
	unsigned int coOL_pointerInsertAfter(
		void*						objectList, 
		const char*						reference,
		void*						objectPtr, 
		const char*						name,
		Stg_ObjectAdaptor_DeletePointerFunction*	ptrDelete,
		Stg_ObjectAdaptor_PrintPointerFunction*		ptrPrint );
	
	/** Insert global pointer after a specific entry */ 
	unsigned int coOL_globalPointerInsertAfter( void* objectList, const char* reference, void* objectPtr, const char* name );
#endif
	
	/** Insert object at a specific index */
	void coOL_insertAtIndex( void* objects, unsigned int index, void* objectPtr );

	/** Removes object from list */
	unsigned int coOL_remove( void* objectList, const char* reference, coOL_Former option ) ;
	
	/** Removes a ptr at a given index */
	void coOL_removeByIndex( void* objects, unsigned int index, coOL_Former option );

	/** Find an object's index in the list, by name. Returns (unsigned)-1 if not found. */
	unsigned int coOL_getIndex( void* objectList, const char* toGet );
	
	/** Get an object's ptr from the list, by name. Returns NULL if not found. */
	void* coOL_get( void* objectList, const char* toGet );
	
	/** Deletes all the objects in the list. */
	void coOL_deleteAllObjects( void* objectList );
	
	/** Prints the names of all the object in the list. */
	void coOL_printAllEntryNames( void* objectList, coStream* stream );
	
	/** Calls Print on all the objects in the list. */
	void coOL_printAllObjects( void* objectList, coStream* stream ) ;
	
	
	#define coOL_atMacro( self, index ) \
		( (self)->data[index] ) 
	void* coOL_atFunc( void* objectList, unsigned int index );
	#ifdef MACRO_AS_FUNC
		/** Obtain the object at a given index in the list. */
		#define coOL_at coOL_atFunc
	#else
		/** Obtain the object at a given index in the list. */
		#define coOL_at coOL_atMacro
	#endif

#if 0 /*Yet to implement */
	/* Caution: using this function implies you _know_ that the stored object at
	 * given index is stored inside an ObjectAdaptor. If this isn't the case, you
	 * should use coOL_At() instead. */
	#define coOL_objectAt( self, index ) \
		( Stg_ObjectAdaptor_Object( (Stg_ObjectAdaptor*)( (self)->data[index] ) ) )
#endif
	
	#define coOL_countMacro( self ) \
		( (self)->count )
	unsigned int coOL_countFunc( void* objectList );
	#ifdef MACRO_AS_FUNC
		/** Obtain the number of objects in the list. */
		#define coOL_count coOL_countFunc
	#else
		/** Obtain the number of objects in the list. */
		#define coOL_count coOL_countMacro
	#endif
	

	/* Private member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	
	/** Delete() implementation. Note that we assume ownership of all objects in this list! */
	void _coOL_delete( void* namedOL );
	
	/** Print() implementation */
	void _coOL_print( void* objectList, coStream* stream, coBool concise );
	
	
#endif /* __Compass_Aspect_OL_h__ */

