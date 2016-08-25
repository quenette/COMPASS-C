#include "Compass.h"
#include "rose.h"

#include "FindSymbols.h"
#include "ObtainValue.h"
#include "CompareAssignment.h"
#include <list>
#include "InlinePrep.h"
#include "WeaveAspectCall.h"
#include "WeavePointcutPtr.h"
#include "Weaver.h"

using namespace std;

static std::string IntToStr( int n ) {
	std::ostringstream result;
	result << n;
	return result.str();
}

namespace co {

	Weaver::Weaver( SgProject* _project, FindSymbols* _declarations )
		: project( _project ), declarations( _declarations ), curFunctionDeclaration( NULL ), uniqueFuncCounter( 0 )
	{}


	Weaver::ActionList& Weaver::getActions() {
		return actions;
	}



	Weaver::Action::Action( Type _type, SgNode* _node1, SgNode* _node2, SgNode* _node3, bool _before )
		: node1( _node1 ), node2( _node2 ), node3( _node3 ), before( _before ), type( _type )
	{}

	SgNode* Weaver::Action::getNode1() const {
		return node1;
	}

	SgNode* Weaver::Action::getNode2() const {
		return node2;
	}

	SgNode* Weaver::Action::getNode3() const {
		return node3;
	}

	bool Weaver::Action::getBefore() const {
		return before;
	}

	Weaver::Action::Type Weaver::Action::getType() const {
		return type;
	}


	// On each node visit, look out for function declaration & call
	void Weaver::visit( SgNode* astNode ) {
		// This requires that the advice and pointcut symbols have been obtained
		//ROSE_ASSERT( declarations->getAdviceSymbol() && declarations->getPointcutSymbol() );

		switch( astNode->variantT() ) {
			// On function declarations, keep reference to so we can resolve "__func__" if needed.
			case V_SgFunctionDeclaration: {
				curFunctionDeclaration = isSgFunctionDeclaration( astNode );
				break;
			}

			// On function calls...
			case V_SgFunctionCallExp: {
				SgFunctionCallExp* functionCallExp = isSgFunctionCallExp( astNode );
				SgExpression* function = functionCallExp->get_function();
				ROSE_ASSERT( function );

				switch( function->variantT() ) {
					case V_SgFunctionRefExp: {
						SgFunctionRefExp* functionRefExp = isSgFunctionRefExp( function );
						SgFunctionSymbol* symbol = functionRefExp->get_symbol(); ROSE_ASSERT( symbol );
						SgFunctionDeclaration* functionDeclaration = symbol->get_declaration(); ROSE_ASSERT( functionDeclaration );

						/* If this is the "advice" function declaration
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
						//if( symbol == declarations->getAdviceSymbol() ) {
						if( symbol->get_name() == "coAdviceImp" ) {
							SgExprListExp* args = functionCallExp->get_args(); ROSE_ASSERT( args );
							SgExpressionPtrList& argList = args->get_expressions();
							SgExpressionPtrList::iterator i = argList.begin();

							const char* pointcutFuncName = ObtainValue( *i++, SymbolValue, curFunctionDeclaration ); // is a result of stringify
							const char* prototypeName = ObtainValue( *i++, SymbolValue, curFunctionDeclaration ); // is a result of stringify
							coJoinpointBinding binding  = (coJoinpointBinding)(int)ObtainValue( *i++, SymbolValue, curFunctionDeclaration );
							SgSymbol* id = ObtainValue( *i++, SymbolNode, curFunctionDeclaration ); // <== not sure!!! how do I get a ptr value???
							SgSymbol* func = ObtainValue( *i++, SymbolValue, curFunctionDeclaration );
							const char* funcName = ObtainValue( *i++, SymbolValue, curFunctionDeclaration ); // is a result of stringify
							const char* calledByFileName = ObtainValue( *i++, SymbolValue, curFunctionDeclaration ); // is a result of __FILE__
							const char* calledByFuncName = ObtainValue( *i++, SymbolValue, curFunctionDeclaration ); // is a result of __func__
							unsigned int calledByLine = (int)ObtainValue( *i++, SymbolValue, curFunctionDeclaration ); // is a result of __LINE__
							coAdviceFunctions adviceFunc = (coAdviceFunctions)(int)ObtainValue( *i++, SymbolValue, curFunctionDeclaration );
							int hasAspect = ObtainValue( *i++, SymbolValue, curFunctionDeclaration );
							SgSymbol* aspect = NULL; i++;  // This is a run-time variable
							ROSE_ASSERT( i == argList.end() ); // Ensure no more arguments!

							cout << "Found function call \"" << symbol->get_name().str()
								<< "\" ( \"" << pointcutFuncName
								<< "\", " << prototypeName
								<< ", " << funcName
								<< "() [" << (SgSymbol*)func << "] )" << endl;

							// Add the advice...
							coAdviceImp( pointcutFuncName, prototypeName, binding, id, func, funcName,
									calledByFileName, calledByFuncName, calledByLine, adviceFunc, hasAspect, aspect );

							/* The following is removed... I'm thinking we want to keep it in the code for:
							 * (a) run-time information (although it could be done via the compiler - this way saves work)
							 * (b) because there will clearly be intermixed run-time and compile time weaving!
							 */
							/* Remove the statement from the code...
							SgExprStatement* exprSmt = isSgExprStatement( functionCallExp->get_parent() ); ROSE_ASSERT( exprSmt );
							SgBasicBlock* block = isSgBasicBlock( exprSmt->get_parent() ); ROSE_ASSERT( block );
							actions.insert( actions.end(), Action( exprSmt, Action::Remove, block ) );*/
						}

						// If this is the "pointcut" function call
						//else if( symbol == declarations->getPointcutSymbol() ) {
						else if( symbol->get_name() == "coJoinpointNewImp" ) {
							SgExprListExp* args = functionCallExp->get_args(); ROSE_ASSERT( args );
							SgExpressionPtrList& argList = args->get_expressions();
							SgExpressionPtrList::iterator i = argList.begin();

							const char* pointcutFuncName = ObtainValue( *i++, SymbolValue, curFunctionDeclaration ); // is a result of stringify
							const char* prototypeName = ObtainValue( *i++, SymbolValue, curFunctionDeclaration ); // is a result of stringify
							coJoinpointBinding binding  = (coJoinpointBinding)(int)ObtainValue( *i++, SymbolValue, curFunctionDeclaration );
							SgSymbol* id = ObtainValue( *i++, SymbolNode, curFunctionDeclaration ); // <== not sure!!! how do I get a ptr value???
							const char* calledByFuncName = ObtainValue( *i++, SymbolValue, curFunctionDeclaration ); // is a result of __func__
							unsigned int calledByLine = (int)ObtainValue( *i++, SymbolValue, curFunctionDeclaration ); // is a result of __LINE__

							const char emptyStr[] = "";
							const char nullStr[] = "NULL";
							const char* idStr = (binding == coJoinpointObject ? (id ? id->get_name().str() : nullStr) : emptyStr);
							cout << "Found function call \"" << symbol->get_name().str()
								<< "\" ( [\"" << pointcutFuncName
								<< "\",\"" << prototypeName
								<< "\",object=\"" << idStr
								<< "\"], ...  )" << endl;

							// Replace the pointcut with the hooks...
							/* Implements the "pointcut" macro here in the compiler...
							 * #define pointcut( proto, ... ) {\
							 * 	coAspects_pcPush( coAspects_s, coJoinpointNew( (char*)__func__, proto ) );\
							 * 	coFuncJP_run( coAspects_pcTop( coAspects_s ), proto, __VA_ARGS__ );\
							 * 	coAspects_pcPop( coAspects_s ); \
							 * }
							 *
							 * Where...
							 * #define coJoinpointNew( pc, proto ) coJoinpointNewImp( pc, #proto, coJoinpointStatic, NULL, (char*)__func__, __LINE__ )
							 *  coFuncJP* coJoinpointNewImp(
							 *  	const char* pointcutFuncName,
							 *  	const char* prototypeName,
							 *  	coJoinpointBinding binding,
							 *  	void* id,
							 *  	const char* calledByFuncName,
							 *  	unsigned int calledByLine );
							 *
							 * Where when USE_PROFILE is not defined...
							 * #define coFuncJP_run1( funcJP, proto, ... ) {\
							 *	unsigned int _coFuncJP_i;\
							 *	for( _coFuncJP_i = 0; _coFuncJP_i < funcJP->_funcMetas->count; _coFuncJP_i++ ) {\
							 *		proto* _coFuncJP_p = funcJP->_funcs[_coFuncJP_i];\
							 *		_coFuncJP_p( __VA_ARGS__ );\
							 *	}\
							 * }
							 *
							 * What we currently have is the AST node - function call expression to coJoinpointNew... need to
							 * work back the AST to the "for loop" we want to replace!
							*/

							SgExprListExp* pushArgs = isSgExprListExp( functionCallExp->get_parent() ); ROSE_ASSERT( pushArgs );
							SgFunctionCallExp* pushCall = isSgFunctionCallExp( pushArgs->get_parent() ); ROSE_ASSERT( pushCall );
							SgExprStatement* pushSmt = isSgExprStatement( pushCall->get_parent() ); ROSE_ASSERT( pushSmt );
							SgNode* curInsertAfterNode = pushSmt;
							SgBasicBlock* pcBlock = isSgBasicBlock( pushSmt->get_parent() ); ROSE_ASSERT( pcBlock );
							SgBasicBlock* pcContextBlock = isSgBasicBlock( pcBlock->get_parent() ); ROSE_ASSERT( pcBlock );


							// The following is because I can't seem to create a new statement without copying one
							SgStatementPtrList pcBlockSmts = pcBlock->get_statements();
							SgStatementPtrList::iterator l = pcBlockSmts.begin();
							l++; // want 2nd item of l
							SgBasicBlock* runBlock = isSgBasicBlock( *l ); ROSE_ASSERT( runBlock );
							SgStatementPtrList runBlockSmts = runBlock->get_statements();
							SgStatementPtrList::iterator j = runBlockSmts.begin();
							j++; // want 2nd item of j
							SgForStatement* forSmt = isSgForStatement( *j ); ROSE_ASSERT( forSmt );
							SgBasicBlock* forBlock = isSgBasicBlock( forSmt->get_loop_body() ); ROSE_ASSERT( forBlock );
							SgStatementPtrList forBlockSmts = forBlock->get_statements();
							SgStatementPtrList::iterator k = forBlockSmts.begin();
							k++; // want 2nd item of k
							SgExprStatement* srcExprSmt = isSgExprStatement( *k ); ROSE_ASSERT( srcExprSmt );
							SgFunctionCallExp* srcFuncCallExp = isSgFunctionCallExp( srcExprSmt->get_expression() ); ROSE_ASSERT( srcFuncCallExp );
							SgExprListExp* srcArgs = isSgExprListExp( srcFuncCallExp->get_args() ); ROSE_ASSERT( srcArgs );
							SgExpressionPtrList& srcArgList = srcArgs->get_expressions();
							l++; // want 3rd item of l
							SgExprStatement* popSmt = isSgExprStatement( *l ); ROSE_ASSERT( popSmt );

							coFuncJP* _pointcut_jp = coJoinpointNewImp( pointcutFuncName, prototypeName, binding, id, calledByFuncName, calledByLine );
							unsigned int _coFuncJP_i;
							for( _coFuncJP_i = 0; _coFuncJP_i < _pointcut_jp->_funcMetas->count; _coFuncJP_i++ ) {
								// Important - do this such that new node aren't parsed (apparently ok to reuse across nodes)
								Sg_File_Info* newInfo = new Sg_File_Info(); ROSE_ASSERT( newInfo );
								newInfo->set_isPartOfTransformation( true );

								// Copy symbol
								SgFunctionSymbol* hookSymbol = isSgFunctionSymbol( (SgSymbol*)_pointcut_jp->_funcs[_coFuncJP_i] ); ROSE_ASSERT( hookSymbol );
								SgFunctionDeclaration* hookDec = hookSymbol->get_declaration(); ROSE_ASSERT( hookDec );
								SgFunctionSymbol* newSymbol = new SgFunctionSymbol( hookDec );
								// Copy type
								SgFunctionType* hookType = hookDec->get_type(); ROSE_ASSERT( hookType );
								SgFunctionType* newType = new SgFunctionType( hookType, false ); ROSE_ASSERT( newType );
								// Copy args
								SgTreeCopy newCopyHelp;
								SgExprListExp* newArgs = new SgExprListExp( newInfo );
								SgExpressionPtrList::iterator m = srcArgList.begin();
								while( m != srcArgList.end() ) {
									SgExpression* newExp = isSgExpression( (*m++)->copy( newCopyHelp ) ); ROSE_ASSERT( newExp );
									newArgs->append_expression( newExp );
								}

								// Build function call expression of the particular (ith) function pointcut aspect
								SgFunctionRefExp* newFuncRefExp = new SgFunctionRefExp( newInfo, newSymbol, newType ); ROSE_ASSERT( newFuncRefExp );
								SgFunctionCallExp* newFuncCallExp = new SgFunctionCallExp( newInfo, newFuncRefExp, newArgs, newType ); ROSE_ASSERT( newFuncCallExp );
								SgExprStatement* newExprSmt = isSgExprStatement( srcExprSmt->copy( newCopyHelp ) ); ROSE_ASSERT( newExprSmt );
								delete newExprSmt->get_expression();
								newExprSmt->set_expression( newFuncCallExp );
								newFuncCallExp->set_parent( newExprSmt );

								// Instruct the weaving - insertion & optimisation of the function pointcut aspect
								actions.insert( actions.end(), Action( Action::Add, newExprSmt, pcBlock, curInsertAfterNode, false /* i.e. after */ ) );
								actions.insert( actions.end(), Action( Action::Inline, newExprSmt, id ) );
								curInsertAfterNode = newExprSmt;

								cout << "Injected hook: " << hookSymbol->get_name().str() << endl;
							}
	
							actions.insert( actions.end(), Action( Action::Remove, pcBlock, pushSmt ) );
							actions.insert( actions.end(), Action( Action::Remove, pcBlock, runBlock ) );
							actions.insert( actions.end(), Action( Action::Remove, pcBlock, popSmt ) );
							actions.insert( actions.end(), Action( Action::Unstack, pcBlock, pcContextBlock ) );
							actions.insert( actions.end(), Action( Action::ReduceNames, pcContextBlock ) );
							actions.insert( actions.end(), Action( Action::RemoveUnusedLabels, pcContextBlock ) );
						}
						// If this is the "weave" function call
						else if( symbol->get_name() == "coWeave" ) {
							cout << "Found function call \"" << symbol->get_name().str() << endl;
							coWeave();
						}
						break;
					}
					case V_SgPointerDerefExp: {
						// Not worried about this case - coAdviceImp and coJoinpointImp are not going to be
						// found this way (at least as I understand it).
						break;
					}
					default: {
						cerr << "Warning: unrecognized variant in compass' weave: " << function->class_name() << endl;
						break;
					}
				}
				break;
			}
	
			default: {
				// No other special cases
				break;
			}
		}
	}


	void Weaver::operate( void ) {
		ActionList::iterator i = actions.begin();
		ActionList newActions;

		// In this order...

		// 1. Insert new statements (i.e. inject hooks)
		while( i != actions.end() ) {
			switch( (*i).getType() ) {
				case Action::Add: {
					SgBasicBlock* block = isSgBasicBlock( (*i).getNode2() ); ROSE_ASSERT( block );
					SgStatement* smt = isSgStatement( (*i).getNode1() ); ROSE_ASSERT( smt );
					SgStatement* relative = isSgStatement( (*i).getNode3() ); ROSE_ASSERT( relative );
					block->insert_statement( relative, smt, (*i).getBefore() );
					break;
				}
				default:
					break;
			}
			i++;
		}

		// 2. Remove (replaced) statements (i.e. remove the pointcut line)
		i = actions.begin();
		while( i != actions.end() ) {
			switch( (*i).getType() ) {
				case Action::Remove: {
					SgBasicBlock* block = isSgBasicBlock( (*i).getNode1() ); ROSE_ASSERT( block );
					SgStatement* smt = isSgStatement( (*i).getNode2() ); ROSE_ASSERT( smt );
					block->remove_statement( smt );
					delete smt;
					break;
				}
				default:
					break;
			}
			i++;
		}

		// 3. Inline (rose inline - i.e. replace all hooks with inlined block statements)
		i = actions.begin();
		InlinePrep ilp;
		WeaveAspectCall wac;
		while( i != actions.end() ) {
			switch( (*i).getType() ) {
				case Action::Inline: {
					SgExprStatement* smt = isSgExprStatement( (*i).getNode1() ); ROSE_ASSERT( smt );
					SgFunctionCallExp* functionCall = isSgFunctionCallExp( smt->get_expression() ); ROSE_ASSERT( functionCall );
					SgSymbol* pcPtrSym = NULL;
					if( (*i).getNode2() ) {
						pcPtrSym = isSgSymbol( (*i).getNode2() ); ROSE_ASSERT( pcPtrSym );
					}

					// To not to have to change the rose inliner, we need to grab the "ith" # of this inlined
					//  statement, and we'll assume the rose inliner always inlines by replacing with a block
					// statement, which should then always be replaced at the same "ith" statement of the parent
					// block.
					SgBasicBlock* block = isSgBasicBlock( smt->get_parent() ); ROSE_ASSERT( block );
					SgStatementPtrList smts = block->get_statements();
					SgStatementPtrList::iterator j = smts.begin();
					int k = 0;
					SgStatement* cur;
					while( (cur = (*j)) != smt ) {
						ROSE_ASSERT( j != smts.end() );
						k += 1;
						j++;
					}
					cout << "inline statement #: " << k << endl;

					// Do inlining prep... need to resolve __func__, etc.
					cout << "Inline step: copy the function..." << endl;
					SgFunctionDeclaration* funcDec = functionCall->getAssociatedFunctionDeclaration(); ROSE_ASSERT( funcDec );
					SgFunctionDeclaration* newFuncDec = isSgFunctionDeclaration( SageInterface::copyStatement( funcDec ) );
					string xxx = funcDec->get_name().str();
					xxx += "_co" + IntToStr( uniqueFuncCounter++ );
					newFuncDec->set_name( xxx );
					cout << "creating temporary symbol: " << xxx << endl;

					// Insert it to a scope
					SgGlobal* glb = SageInterface::getFirstGlobalScope( project );
					SageInterface::appendStatement( newFuncDec,glb );
					SgFunctionSymbol* func_symbol =  glb->lookup_function_symbol( xxx, newFuncDec->get_type() );
					if( func_symbol == NULL ) {
						func_symbol = new SgFunctionSymbol( newFuncDec );
						glb->insert_symbol( xxx, func_symbol );
					}

					cout << "Inline step: aspect transform..." << endl;
					SgFunctionRefExp* newFuncRefExp = isSgFunctionRefExp( functionCall->get_function() );
					newFuncRefExp->set_symbol( func_symbol );
					ilp.transform( newFuncDec, block, funcDec->get_name().str() );
					wac.transform( newFuncDec, block );

					// Do the inlining
					cout << "Inline step: inline..." << endl;
					bool sucessfullyInlined = doInline( functionCall, false );

					// Grab the inlined block for and add an action to unwrap it from a stack/block
					smts = block->get_statements();
					j = smts.begin();
					for( int l = 0; sucessfullyInlined && l < k; l++ ) {
						j++;
					}
					newActions.insert( newActions.end(), Action( Action::Unstack, *j, block, pcPtrSym ) );

					break;
				}
				default:
					break;
			}
			i++;
		}

		ActionList::iterator x = newActions.begin();

		// 4. Unstack/unblock the inlining (of advice bodies in the poincut block)
		x = newActions.begin();
		WeavePointcutPtr wpp;
		while( x != newActions.end() ) {
			switch( (*x).getType() ) {
				case Action::Unstack: {
					SgBasicBlock* block = isSgBasicBlock( (*x).getNode2() ); ROSE_ASSERT( block );
					SgBasicBlock* smt = isSgBasicBlock( (*x).getNode1() ); ROSE_ASSERT( smt );
					SgSymbol* pcPtrSym = NULL;
					if( (*x).getNode3() ) {
						pcPtrSym = isSgSymbol( (*x).getNode3() ); ROSE_ASSERT( pcPtrSym );
					}

					cout << "Inline step: unwrapping of inlined block..." << endl;
					SgStatementPtrList smts = smt->get_statements();
					SgStatementPtrList::iterator j = smts.begin();
					SgStatementPtrList::iterator k = j;

					// Obtaining "k" - where the variable declarations stop (from a C perspective)
					while( j != smts.end() && isSgVariableDeclaration( *j ) ) {
						//TODO: mangle the names!
						j++;
						k = j;
					}

					// Obtaining "l" - where in the parent block the sub block is
					SgStatementPtrList::iterator l = block->get_statements().begin();
					while( (*l) != smt ) {
						ROSE_ASSERT( l != block->get_statements().end() );
						l++;
					}

					// Done in the following order such that inserts are on an otherwise un-modified list...

					// Move all the non-variable-declarations from the subblock to the parent block
					while( j != smts.end() ) {
						// Move into parent block
						(*j)->set_parent( block );
						block->insert_statement( (*l), (*j), false );
						//TODO: change to use the mangled variable names!
						l = j;
						j++;
					}

					// Move all the variable declarations from the subblock to the top of the parent block
					j = smts.begin();
					while( j != k ) {
						ROSE_ASSERT( j != smts.end() );
						//Move variables into top of parent block.
						(*j)->set_parent( block );
						block->insert_statement( (*block->get_statements().begin()), (*j), true );
						//TODO: change to use the mangled variable names!
						j++;
					}

					//Remove sub-block
					block->remove_statement( smt );
					//delete smt; TODO ... cant do without letting smt know its no longer owner of the moved statements

					cout << "Inline step: pointcut object transform..." << endl;
					wpp.transform( block, pcPtrSym );

					break;
				}
				default:
					break;
			}
			x++;
		}

		// 5. Unstack/unblock the pointcuts/blocks
		i = actions.begin();
		while( i != actions.end() ) {
			switch( (*i).getType() ) {
				case Action::Unstack: {
					SgBasicBlock* block = isSgBasicBlock( (*i).getNode2() ); ROSE_ASSERT( block );
					SgBasicBlock* smt = isSgBasicBlock( (*i).getNode1() ); ROSE_ASSERT( smt );

					cout << "Inline step: unwrapping of (pointcut) block..." << endl;
					SgStatementPtrList smts = smt->get_statements();
					SgStatementPtrList::iterator j = smts.begin();
					SgStatementPtrList::iterator k = j;

					// Obtaining "k" - where the variable declarations stop (from a C perspective)
					while( j != smts.end() && isSgVariableDeclaration( *j ) ) {
						//TODO: mangle the names!
						j++;
						k = j;
					}

					// Obtaining "l" - where in the parent block the sub block is
					SgStatementPtrList::iterator l = block->get_statements().begin();
					while( (*l) != smt ) {
						ROSE_ASSERT( l != block->get_statements().end() );
						l++;
					}

					// Done in the following order such that inserts are on an otherwise un-modified list...

					// Move all the non-variable-declarations from the subblock to the parent block
					while( j != smts.end() ) {
						// Move into parent block
						(*j)->set_parent( block );
						block->insert_statement( (*l), (*j), false );
						//TODO: change to use the mangled variable names!
						l = j;
						j++;
					}

					// Move all the variable declarations from the subblock to the top of the parent block
					j = smts.begin();
					while( j != k ) {
						ROSE_ASSERT( j != smts.end() );
						//Move variables into top of parent block.
						(*j)->set_parent( block );
						block->insert_statement( (*block->get_statements().begin()), (*j), true );
						//TODO: change to use the mangled variable names!
						j++;
					}

					//Remove sub-block
					block->remove_statement( smt );
					//delete smt; TODO ... cant do without letting smt know its no longer owner of the moved statements

					cout << "Inline done!" << endl;
					break;
				}
				default:
					break;
			}
			i++;
		}

// TODO: implement!
		i = actions.begin();
		while( i != actions.end() ) {
			switch( (*i).getType() ) {
				case Action::ReduceNames: {
					SgBasicBlock* block = isSgBasicBlock( (*i).getNode1() ); ROSE_ASSERT( block );

					cout << "Inline step: reducing unnecessary variables..." << endl;
					SgStatementPtrList smts = block->get_statements();
					SgStatementPtrList::iterator j = smts.begin();
					SgStatementPtrList::iterator k = j;
					SgStatementPtrList::iterator l = k;

					// Obtaining "k" - where the variable declarations stop (from a C perspective)
					// And - building the set of local variable names
					// set of < varName, AST, list of varNames with same assignment >
					set< pair<SgName,pair<SgAssignInitializer*,list<SgName>* > > > vars;
					set< pair<SgName,pair<SgAssignInitializer*,list<SgName>* > > >::iterator vars_i;
					list<SgName>::iterator same_i;
					SgVariableDeclaration* curVarDec;
					while( j != smts.end() && (curVarDec = isSgVariableDeclaration( *j )) ) {
						SgInitializedNamePtrList nameList = curVarDec->get_variables();
						SgInitializedNamePtrList::iterator nl_i;
						for( nl_i = nameList.begin(); nl_i != nameList.end(); nl_i++ ) {
							SgInitializedName* iName = isSgInitializedName( (*nl_i) );
							SgName name = SgName( iName->get_name().str() );
							SgAssignInitializer* ai = isSgAssignInitializer( iName->get_initializer() );
							vars.insert( pair<SgName,pair<SgAssignInitializer*,list<SgName>* > >( name, pair<SgAssignInitializer*,list<SgName>* >( ai, new list<SgName>() ) ) );
						}
						j++;
						k = j;
						l = k;
					}
					cout << "Inlined variables: ";
					for( vars_i = vars.begin(); vars_i != vars.end(); vars_i++ ) {
						cout << (*vars_i).first.str() << ", ";
					}
					cout << endl;

					// once we have all the names, find those who have the same initialisation.
					for( vars_i = vars.begin(); vars_i != vars.end(); vars_i++ ) {
						set< pair<SgName,pair<SgAssignInitializer*,list<SgName>* > > >::iterator vars_j;
						vars_j = vars_i;
						for( vars_j++; vars_j != vars.end(); vars_j++ ) {
							bool result = CompareAssignment( (*vars_i).second.first, (*vars_j).second.first );
							//cout << (*vars_i).first.str() << " vs " << (*vars_j).first.str() << " : " << result << endl;
							if( result ) {
								list<SgName>* varSameList = (*vars_i).second.second;
								varSameList->push_back( (*vars_j).first );
							}
						}
					}
					cout << "Inlined variables with same assignment: ";
					for( vars_i = vars.begin(); vars_i != vars.end(); vars_i++ ) {
						cout << (*vars_i).first.str() << "= [";
						list<SgName>* varSameList = (*vars_i).second.second;
						for( same_i = varSameList->begin(); same_i != varSameList->end(); same_i++ ) {
							cout << (*same_i).str() << ",";
						}
						cout << "], ";
					}
					cout << endl;


					// of those, keep only those who are not the recipient of assign in the rest of the code
					set<SgName> assVars;
					set<SgName>::iterator assVar_i;
					while( k != smts.end() ) {
						Rose_STL_Container<SgNode*> vres = NodeQuery::querySubTree( (*k), V_SgVarRefExp );
						Rose_STL_Container<SgNode*>::iterator vre_i;
						for( vre_i = vres.begin(); vre_i != vres.end(); vre_i++ ) {
							SgVarRefExp* vre = isSgVarRefExp( (*vre_i) ); ROSE_ASSERT( vre );
							SgAssignOp* ao = isSgAssignOp( vre->get_parent() );
							SgPlusAssignOp* pao = isSgPlusAssignOp( vre->get_parent() );
							if( ao ) {
								SgSymbol* s = isSgSymbol( vre->get_symbol() ); ROSE_ASSERT( s );
								assVars.insert( s->get_name() );
							}
							else if( pao ) {
								SgSymbol* s = isSgSymbol( vre->get_symbol() ); ROSE_ASSERT( s );
								assVars.insert( s->get_name() );
							}
						}
						k++;
					}
					cout << "Assigned vars: ";
					for( assVar_i = assVars.begin(); assVar_i != assVars.end(); assVar_i++ ) {
						cout << (*assVar_i).str() << ", ";
					}
					cout << endl;

					// reduce repeated vars to 1
					// TODO...
					// In reverse order os vars
					set< pair<SgName,pair<SgAssignInitializer*,list<SgName>* > > >::reverse_iterator rvars_i;
					for( rvars_i = vars.rbegin(); rvars_i != vars.rend(); rvars_i++ ) {
						//    if var is not in assigned list
						bool found = false;
						for( assVar_i = assVars.begin(); assVar_i != assVars.end(); assVar_i++ ) {
							if( (*rvars_i).first == (*assVar_i) ) {
								found = true;
							}
						}
						if( !found ) {
							//    for all "same" vars
							list<SgName>* varSameList = (*rvars_i).second.second;
							for( same_i = varSameList->begin(); same_i != varSameList->end(); same_i++ ) {
								//      if same var is not in assigned list
								bool found2 = false;
								for( assVar_i = assVars.begin(); assVar_i != assVars.end(); assVar_i++ ) {
									if( (*same_i) == (*assVar_i) ) {
										found2 = true;
									}
								}
								if( !found ) {
									//         find all symbols that is to "same var"
									SgStatementPtrList::iterator m = l;
									while( m != smts.end() ) {
										Rose_STL_Container<SgNode*> vres = NodeQuery::querySubTree( (*m), V_SgVarRefExp );
										Rose_STL_Container<SgNode*>::iterator vre_i;
										for( vre_i = vres.begin(); vre_i != vres.end(); vre_i++ ) {
											SgVarRefExp* vre = isSgVarRefExp( (*vre_i) ); ROSE_ASSERT( vre );
											SgVariableSymbol* vs = isSgVariableSymbol( vre->get_symbol() ); ROSE_ASSERT( vs );
											if( vs->get_name() == (*same_i) ) {
												//             - queue replace symbol with that of "var"
												cout << "Replacing symbol \"" << (*same_i).str() << "\" with \"" << (*rvars_i).first.str() << "\"" << endl;
//												SgVariableSymbol* to_vs = SageInterface::lookupVariableSymbolInParentScopes( (*rvars_i).first, SageInterface::getScope( vre ) ); ROSE_ASSERT( to_vs );
												SgInitializedName* in = isSgInitializedName( (*rvars_i).second.first->get_parent() ); ROSE_ASSERT( in );
												SgVariableSymbol* to_vs = isSgVariableSymbol( in->get_symbol_from_symbol_table() ); ROSE_ASSERT( to_vs );
												vre->set_symbol( to_vs );
											}
										}
										m++;
									}
									//         queue - remove declaration of "same var"
								}
							}
						}
					}
					//         recall operate

					cout << "Inline done!" << endl;
					break;
				}
				default:
					break;
			}
			i++;
		}

		i = actions.begin();
		while( i != actions.end() ) {
			switch( (*i).getType() ) {
				case Action::RemoveUnusedLabels: {
					SgBasicBlock* block = isSgBasicBlock( (*i).getNode1() ); ROSE_ASSERT( block );

					SageInterface::removeUnusedLabels( block );
				}
				default:
					break;
			}
			i++;
		}
	}
}
