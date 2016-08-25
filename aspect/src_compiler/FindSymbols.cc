#include "rose.h"

#include "FindSymbols.h"

using namespace std;

namespace co {

	FindSymbols::FindSymbols()
		: adviceSymbol( NULL ), pointcutSymbol( NULL )
	{}


	SgFunctionSymbol* FindSymbols::getAdviceSymbol( void ) {
		return adviceSymbol;
	}


	SgFunctionSymbol* FindSymbols::getPointcutSymbol( void ) {
		return pointcutSymbol;
	}


	// On each node visit, look out for function declaration & call
	void FindSymbols::visit( SgNode* astNode ) {
		switch( astNode->variantT() ) {
			// On function declarations...
			case V_SgFunctionDeclaration: {
				SgFunctionDeclaration* functionDeclaration = isSgFunctionDeclaration( astNode );
				string functionName = functionDeclaration->get_name().str();

				/* If this is an "advice", we'll see ...
					void coAdviceImp(
						const char* pointcutFuncName,
						const char* prototypeName,
						coJoinpointBinding binding,
						void* id,
						void* func,
						const char* funcName,
						const char* calledByFuncName,
						unsigned int calledByLine,
						coAdviceFunctions adviceFunc,
						int hasAspect,
						void* aspect );
				*/
				if( functionName == "coAdviceImp" ) {
					SgFunctionType* functionType = functionDeclaration->get_type();
					ROSE_ASSERT( functionType != NULL );
					bool foundFunction = false;

					// And if this function has the right prototype...
					if( functionType->get_return_type()->unparseToString() == "void" ) {
						SgTypePtrList& argumentList = functionType->get_arguments();
						SgTypePtrList::iterator i = argumentList.begin();
						if( (*i++)->unparseToString() == "const char *" ) { // pointcutFuncName
							if( (*i++)->unparseToString() == "const char *" ) { // prototypeName
								if( (*i++)->unparseToString() == "coJoinpointBinding" ) { // binding
									if( (*i++)->unparseToString() == "void *" ) { // id
										if( (*i++)->unparseToString() == "void *" ) { // func
											if( (*i++)->unparseToString() == "const char *" ) { // funcName
												if( (*i++)->unparseToString() == "const char *" ) { // calledByFuncName
													if( (*i++)->unparseToString() == "unsigned int" ) { // calledByLine
														if( (*i++)->unparseToString() == "coAdviceFunctions" ) { // adviceFunc
															if( (*i++)->unparseToString() == "int" ) { // hasAspect
																if( (*i++)->unparseToString() == "void *" ) { // aspect
																		foundFunction = true;
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}

					// Once we've found the function declaration, keep track of the function symbol...
					if( foundFunction == true ) {
						// Now get the sysmbol using functionType
						SgScopeStatement* scope = functionDeclaration->get_scope();
						ROSE_ASSERT( scope != NULL );
						adviceSymbol = scope->lookup_function_symbol( functionName, functionType );
						cout << "Found function declaration \"" << functionName << "\", symbol \"" << adviceSymbol << "\"" << endl;
					}
				}
				/* If this is a "pointcut", we'll see within a block, ...
				 *
				 * coFuncJP* coJoinpointNewImp(
				 *  	const char* pointcutFuncName,
				 *  	const char* prototypeName,
				 *  	coJoinpointBinding binding,
				 *  	void* id,
				 *  	const char* calledByFuncName,
				 *  	unsigned int calledByLine );
				 *
				 * Then when USE_PROFILE is not defined...
				 * {
				 *	unsigned int _coFuncJP_i;
				 *	for( _coFuncJP_i = 0; _coFuncJP_i < funcJP->_funcMetas->count; _coFuncJP_i++ ) {
				 *		proto* _coFuncJP_p = funcJP->_funcs[_coFuncJP_i];
				 *		_coFuncJP_p( __VA_ARGS__ );
				 *	}
				 * }
				 *
				 * Remove the block if this is compiled-time weaved!
				 */
				else if( functionName == "coJoinpointNewImp" ) {
					SgFunctionType* functionType = functionDeclaration->get_type();
					ROSE_ASSERT( functionType != NULL );
					bool foundFunction = false;

					// And if this function has the right prototype...
					if( functionType->get_return_type()->unparseToString() == "coFuncJP *" ) {
						SgTypePtrList& argumentList = functionType->get_arguments();
						SgTypePtrList::iterator i = argumentList.begin();
						if( (*i++)->unparseToString() == "const char *" ) { // pointcutFuncName
							if( (*i++)->unparseToString() == "const char *" ) { // prototypeName
								if( (*i++)->unparseToString() == "coJoinpointBinding" ) { // binding
									if( (*i++)->unparseToString() == "void *" ) { // id
										if( (*i++)->unparseToString() == "const char *" ) { // calledByFuncName
											if( (*i++)->unparseToString() == "unsigned int" ) { // calledByLine
												foundFunction = true;
											}
										}
									}
								}
							}
						}
					}
	
					// TODO: check that the "run" is there!!!  And that the block isn't any greater than this!

					// Once we've found the function declaration, keep track of the function symbol...
					if( foundFunction == true ) {
						// BIG TODO: remove the parent block IF its to be compiled-time optimised
						// OR CHEAT and make a "pointcut_o" for now (calls a dummy run func)!
						// Now get the symbol using functionType
						SgScopeStatement* scope = functionDeclaration->get_scope();
						ROSE_ASSERT( scope != NULL );
						pointcutSymbol = scope->lookup_function_symbol( functionName, functionType );
						cout << "Found function declaration \"" << functionName << "\", symbol \"" << pointcutSymbol << "\"" << endl;
					}
				}
				break;
			}
			default: {
				// No other special cases
			}
		}
	}

}
