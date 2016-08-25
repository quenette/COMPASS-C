#include "Compass.h"
#include "rose.h"

#include <list>
#include <set>
#include <sstream>
#include <string>

#include "ObtainValue.h"
#include "WeaveAspectCall.h"

using namespace std;


#if 0
static std::string IntToStr( int n ) {
	std::ostringstream result;
	result << n;
	return result.str();
}
#endif

namespace co {

	WeaveAspectCall::WeaveAspectCall()
		: jps(), curJP( jps.end() ), state( StateInvalid )
	{}


	void WeaveAspectCall::visit( SgNode* astNode ) {
		switch( state ) {
			case Begin: {
				// Ensure the node we're starting on is a "function declaration"
				SgFunctionDeclaration* funcDec = isSgFunctionDeclaration( astNode ); ROSE_ASSERT( funcDec );
				(*curJP).curFunc = (*curJP).funcDecs.insert( (*curJP).funcDecs.end(), funcDec );
				curScope = (*(*curJP).curFunc)->get_definition()->get_body();

				state = Searching;
			}
			case Searching: {
				switch( astNode->variantT() ) {
					case V_SgFunctionCallExp: {
						SgFunctionCallExp* functionCallExp = isSgFunctionCallExp( astNode );
						SgExpression* function = functionCallExp->get_function();
						ROSE_ASSERT( function );

						switch( function->variantT() ) {
							case V_SgFunctionRefExp: {
								SgFunctionRefExp* functionRefExp = isSgFunctionRefExp( function );
								SgFunctionSymbol* symbol = functionRefExp->get_symbol(); ROSE_ASSERT( symbol );
								SgFunctionDeclaration* functionDeclaration = symbol->get_declaration(); ROSE_ASSERT( functionDeclaration );

								if( symbol->get_name() == "coAspectImp" ) {
									SgExprListExp* args = functionCallExp->get_args(); ROSE_ASSERT( args );
									SgExpressionPtrList& argList = args->get_expressions();
									SgExpressionPtrList::iterator i = argList.begin();

									unsigned int ep_i;
									unsigned int func_i;
									const char* funcName = ObtainValue( *i++, SymbolValue, functionDeclaration ); // is a result of stringify
									const char* calledByFuncName = ObtainValue( *i++, SymbolValue, functionDeclaration ); // is a result of __func__
									unsigned int calledByLine = (int)ObtainValue( *i++, SymbolValue, functionDeclaration ); // is a result of __LINE__
									ROSE_ASSERT( i == argList.end() ); // Ensure no more arguments!

									cout << "Found function call \"" << symbol->get_name().str()
										<< "\" ( \"" << funcName
										<< "\" )" << endl;

									// Resolve as much of the aspect reference that we can at compile time.
									// ep_i and func_i need to be resolved run-time
									_coAspectImp( funcName, calledByFuncName, calledByLine, &ep_i, &func_i );

									SgVarRefExp* coAspects_s_node = SageBuilder::buildVarRefExp( "coAspects_s" );
									SgVarRefExp* jps_node = SageBuilder::buildVarRefExp( "jps" );
									SgArrowExp* arrow0_node = SageBuilder::buildArrowExp( coAspects_s_node, jps_node );
									SgVarRefExp* jp_node = SageBuilder::buildVarRefExp( "jp" );
									SgArrowExp* arrow1_node = SageBuilder::buildArrowExp( arrow0_node, jp_node );
									SgIntVal* ep_i_node = SageBuilder::buildIntVal( ep_i );
									SgPntrArrRefExp* array0_node = SageBuilder::buildPntrArrRefExp( arrow1_node, ep_i_node );
									SgVarRefExp* funcMetas_node = SageBuilder::buildVarRefExp( "_funcMetas" );
									SgArrowExp* arrow2_node = SageBuilder::buildArrowExp( array0_node, funcMetas_node );
									SgVarRefExp* data_node = SageBuilder::buildVarRefExp( "data" );
									SgArrowExp* arrow3_node = SageBuilder::buildArrowExp( arrow2_node, data_node );
									SgIntVal* func_i_node = SageBuilder::buildIntVal( func_i );
									SgPntrArrRefExp* array1_node = SageBuilder::buildPntrArrRefExp( arrow3_node, func_i_node );
									SgGlobal* gs = SageInterface::getGlobalScope( functionCallExp );
									SgType* coAspectFunc_type = SageInterface::lookupNamedTypeInParentScopes( "coAspectFunc", gs ); ROSE_ASSERT( coAspectFunc_type );
									SgPointerType* coAspectFuncPtr_type = SageBuilder::buildPointerType( coAspectFunc_type );
									SgCastExp* cast_node = SageBuilder::buildCastExp( array1_node, coAspectFuncPtr_type, SgCastExp::e_C_style_cast );
									SgVarRefExp* fromAspect_node = SageBuilder::buildVarRefExp( "fromAspect" );
									SgArrowExp* arrow4_node = SageBuilder::buildArrowExp( cast_node, fromAspect_node );

									actions.insert( actions.end(), Action( Action::ReplaceExpression, functionCallExp, arrow4_node ) );
								}
								break;
							}
							default: {
								// Ignore when its not this case
								break;
							}
						}
						break;
					}
					default: {
						// Ignore when its not this case
						break;
					}
				}
			}
			case Complete:
				// Ignore remaining nodes
			default:
				ROSE_ASSERT( state < StateInvalid );
				break;
		}
	}

	void WeaveAspectCall::operate( void ) {
		ActionList::iterator i;

		// In this order...

		// 1. Do expression replacements
		i = actions.begin();
		while( i != actions.end() ) {
			switch( (*i).getType() ) {
				case Action::ReplaceExpression: {
					SgExpression* oldExp = isSgExpression( (*i).getNode1() ); ROSE_ASSERT( oldExp );
					SgExpression* newExp = isSgExpression( (*i).getNode2() ); ROSE_ASSERT( newExp );
					SageInterface::replaceExpression( oldExp, newExp, false /*keepOld=*/ );
					break;
				}
				default:
					break;
			}
			i++;
		}

		actions.clear();
	}


	void WeaveAspectCall::transform( SgNode* afNode, SgBasicBlock* jpBlock ) {
		// Set the current joint point to the one provided.
		bool found = false;
		for( list<FunctionJoinpoint>::iterator j = jps.begin(); j != jps.end(); j++ ) {
			if( (*j).jpBlock == jpBlock ) {
				found = true;
				curJP = j;
			}
		}
		if( !found ) {
			FunctionJoinpoint newFJP;
			newFJP.jpBlock = jpBlock;
			curJP = jps.insert( jps.end(), newFJP );
		}

		// Step 1 - begin the parsing...
		state = Begin;
		traverse( afNode, preorder );

		// Step 2 - make the changes to the AST...
		operate();
	}

	WeaveAspectCall::Action::Action( Type _type, SgNode* _node1, SgNode* _node2 )
		: node1( _node1 ), node2( _node2 ), type( _type )
	{}

	SgNode* WeaveAspectCall::Action::getNode1() const {
		return node1;
	}

	SgNode* WeaveAspectCall::Action::getNode2() const {
		return node2;
	}

	WeaveAspectCall::Action::Type WeaveAspectCall::Action::getType() const {
		return type;
	}

}
