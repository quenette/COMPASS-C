#include "Compass.h"
#include "rose.h"

#include <list>
#include <set>
#include <sstream>
#include <string>

#include "ObtainValue.h"
#include "WeavePointcutPtr.h"

using namespace std;


#if 0
static std::string IntToStr( int n ) {
	std::ostringstream result;
	result << n;
	return result.str();
}
#endif

namespace co {

	WeavePointcutPtr::WeavePointcutPtr()
		: jps(), curJP( jps.end() ), state( StateInvalid )
	{}


	void WeavePointcutPtr::visit( SgNode* astNode ) {
		switch( state ) {
			case Begin: {
				// Ensure the node we're starting on is a "block" (what a pointcut is)
				SgBasicBlock* pc = isSgBasicBlock( astNode ); ROSE_ASSERT( pc );
//				(*curJP).curFunc = (*curJP).funcDecs.insert( (*curJP).funcDecs.end(), funcDec );
				curScope = isSgBasicBlock( SageInterface::getScope( pc ) ); ROSE_ASSERT( curScope );

				state = Searching;
			}
			case Searching: {
				SgDotExp* arrow3_dot = isSgDotExp( astNode );
				if( arrow3_dot ) {
					SgPointerDerefExp* arrow3_deref = isSgPointerDerefExp( arrow3_dot->get_lhs_operand() );
					SgVarRefExp* var__optionalObject = isSgVarRefExp( arrow3_dot->get_rhs_operand() );
					if( arrow3_deref && var__optionalObject ) {
						SgPntrArrRefExp* array1 = isSgPntrArrRefExp( arrow3_deref->get_operand() );
						if( array1 ) {
							SgArrowExp* arrow1 = isSgArrowExp( array1->get_lhs_operand() );
							SgArrowExp* arrow2 = isSgArrowExp( array1->get_rhs_operand() );
							if( arrow1 && arrow2 ) {
								SgVarRefExp* var_coAspects_s1 = isSgVarRefExp( arrow1->get_lhs_operand() );
								SgVarRefExp* var__pcStack = isSgVarRefExp( arrow1->get_rhs_operand() );
								SgVarRefExp* var_coAspects_s2 = isSgVarRefExp( arrow2->get_lhs_operand() );
								SgVarRefExp* var__pcStackTop = isSgVarRefExp( arrow2->get_rhs_operand() );
								if( var_coAspects_s1 && var__pcStack && var_coAspects_s2 && var__pcStackTop ) {
									// If we're here, the AST form is right, just need to ensure the variable names are...
									//cout << "\"" << var__optionalObject->get_symbol()->get_name().str() << "\"" << endl;
									//cout << "\"" << var_coAspects_s1->get_symbol()->get_name().str() << "\"" << endl;
									//cout << "\"" << var__pcStack->get_symbol()->get_name().str() << "\"" << endl;
									//cout << "\"" << var_coAspects_s2->get_symbol()->get_name().str() << "\"" << endl;
									//cout << "\"" << var__pcStackTop->get_symbol()->get_name().str() << "\"" << endl;
									if( var__optionalObject->get_symbol()->get_name() == "_optionalObject" &&
										var_coAspects_s1->get_symbol()->get_name() == "coAspects_s" &&
										var__pcStack->get_symbol()->get_name() == "_pcStack" &&
										var_coAspects_s2->get_symbol()->get_name() == "coAspects_s" &&
										var__pcStackTop->get_symbol()->get_name() == "_pcStackTop" )
									{
										actions.insert( actions.end(), Action( Action::ReplaceExpression, arrow3_dot, (*curJP).pcPtrRef ) );
									}
								}
							}
						}
					}
				}
				break;
			}
			case Complete:
				// Ignore remaining nodes
			default:
				ROSE_ASSERT( state < StateInvalid );
				break;
		}
	}

	void WeavePointcutPtr::operate( void ) {
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


	void WeavePointcutPtr::transform( SgNode* afNode, SgSymbol* pcPtrSym ) {
		SgBasicBlock* pc = isSgBasicBlock( afNode ); ROSE_ASSERT( pc );
		SgVariableSymbol* sym = NULL;
		if( pcPtrSym ) {
			sym = isSgVariableSymbol( pcPtrSym ); ROSE_ASSERT( sym );
		}

		// Set the current joint point to the one provided.
		bool found = false;
		for( list<FunctionJoinpoint>::iterator j = jps.begin(); j != jps.end(); j++ ) {
			if( (*j).jpBlock == pc ) {
				found = true;
				curJP = j;
			}
		}
		if( !found ) {
			FunctionJoinpoint newFJP;
			newFJP.jpBlock = pc;
			newFJP.pcPtrSym = sym;
			if( sym ) {
				newFJP.pcPtrRef = SageBuilder::buildVarRefExp( sym ); ROSE_ASSERT( newFJP.pcPtrRef );
			}
			else {
				newFJP.pcPtrRef = NULL;
			}
			curJP = jps.insert( jps.end(), newFJP );
		}

		// Step 1 - begin the parsing...
		state = Begin;
		traverse( afNode, preorder );

		// Step 2 - make the changes to the AST...
		operate();
	}

	WeavePointcutPtr::Action::Action( Type _type, SgNode* _node1, SgNode* _node2 )
		: node1( _node1 ), node2( _node2 ), type( _type )
	{}

	SgNode* WeavePointcutPtr::Action::getNode1() const {
		return node1;
	}

	SgNode* WeavePointcutPtr::Action::getNode2() const {
		return node2;
	}

	WeavePointcutPtr::Action::Type WeavePointcutPtr::Action::getType() const {
		return type;
	}

}
