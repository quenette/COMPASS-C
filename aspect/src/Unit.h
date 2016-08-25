/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**
** Copyright (C) Steve Quenette 2011
**
** <license>
**
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef __Compass_Aspect_Unit_h__
#define __Compass_Aspect_Unit_h__

	/** Textual name for coObject class. */
	extern const char* coUnit_type;

	typedef void (coUnit_test_FP) ( void* data );
	typedef void (coUnit_fixture_FP)  ( void* data );
	
	#define __coUnit \
		__coObject; \
		\
		coUnit_fixture_FP* _setup; \
		coUnit_fixture_FP* _teardown; \
		void*              _data; \
		coUnit_test_FP**   _func; \
		unsigned int       _count; \
		unsigned int       _size; \
	
	/** coUnit class - A unit test driver heavily inspired by Pat Sunter's pcu in StGermain (mainly such that the tests here
	    are consistent with the StGermain ones. This implementation shares the interface but is a very simplistic underneath. */
	typedef struct _coUnit { __coUnit } coUnit;
	
	/* Public member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	/* coUnit constructor */
	coUnit* coUnit_new( void );
		
	/** Allocate / set the data to the unit test suite. */
	#define coUnit_setData( unit, type ) _coUnit_setDataImp( unit, sizeof(type) )
	void _coUnit_setDataImp( void* unit, size_t size );
	
	/** Adds a test to the unit test suite. */
	#define coUnit_addTest( unit, func ) _coUnit_addTestImp( unit, (coUnit_test_FP*)func, #func )
	void _coUnit_addTestImp( void* unit, coUnit_test_FP* func, const char* name );

	/** Adds the setup / teardown fixtures to the unit test suite. */
	#define coUnit_setFixtures( unit, setup, teardown ) _coUnit_setFixturesImp( unit, (coUnit_fixture_FP*)setup, (coUnit_fixture_FP*)teardown )
	void _coUnit_setFixturesImp( void* unit, coUnit_fixture_FP* setup, coUnit_fixture_FP* teardown );

	/** Check that two strings are equal. This implementation will assert if the test fails. Simple and clean. */
	#define coUnit_check_streq( strA, strB ) \
	   do { \
	      /* The temporary ptrs are so ++ operations etc inside (strA) or (b) aren't done several times */ \
	      const char* tempStr1 = (strA); \
	      const char* tempStr2 = (strB); \
	      coFirewall( 0 == strcmp( tempStr1, tempStr2 ), coErrJrn( coUnit_type ), "Unit check failed!" );\
	   } while( 0 )


	/** Check the value is true. This implementation will assert if the test fails. Simple and clean. */
	#define coUnit_check_true( expr ) coFirewall( expr, coErrJrn( coUnit_type ), "Unit check failed!" )
		
	/** Run / execute the test suite */
	coBool coUnit_run( void* unit );


	/* Private member functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	/** Delete implementation */
	void _coUnit_delete( void* unit );
	
	/** Print implementation */
	void _coUnit_print( void* unit, coStream* stream, coBool concise );

#endif /* __Compass_Aspect_Object_h__ */

