#include "rose.h"

#include <list>
#include <set>
#include <sstream>
#include <string>

#include "InlinePrep.h"

using namespace std;


static std::string IntToStr( int n ) {
	std::ostringstream result;
	result << n;
	return result.str();
}


namespace co {

	InlinePrep::InlinePrep()
		: jps(), curJP( jps.end() ), state( StateInvalid )
	{}


	void InlinePrep::visit( SgNode* astNode ) {
		switch( state ) {
			case Begin: {
				// Ensure the node we're starting on is a "function declaration"
				SgFunctionDeclaration* funcDec = isSgFunctionDeclaration( astNode ); ROSE_ASSERT( funcDec );
				(*curJP).curFunc = (*curJP).funcDecs.insert( (*curJP).funcDecs.end(), funcDec );
				curScope = (*(*curJP).curFunc)->get_definition()->get_body();

				state = Searching;
			}
			case Searching: {
				SgVarRefExp* ref;
				SgInitializedName* iName;

				// Check to see if the current node is a __func__, ...
				if( (ref = isSgVarRefExp( astNode )) != NULL ) {
					if( ref->get_file_info()->isCompilerGenerated() ) {
						// If the variable is compiler generated... we need to resolve this ourselves
						string str = ref->unparseToString();
						SgExpression* newRef;
						if( str == "__func__" ) {
							if( (*curJP).curFuncName == "" ) {
								newRef = SageBuilder::buildStringVal( (*(*curJP).curFunc)->get_name().str() );
							}
							else {
								newRef = SageBuilder::buildStringVal( (*curJP).curFuncName );
							}
							actions.insert( actions.end(), Action( Action::ReplaceExpression, ref, newRef ) );
						}
					}
				}
				else if( (iName = isSgInitializedName( astNode )) != NULL ) {
					SgBasicBlock* varScope = isSgBasicBlock( SageInterface::getScope( iName ) );
					if( varScope == curScope ) {
						actions.insert( actions.end(), Action( Action::UniqueName, iName ) );
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

	void InlinePrep::operate( void ) {
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

		// 2. Do variable renaming to unique names
		i = actions.begin();
		while( i != actions.end() ) {
			switch( (*i).getType() ) {
				case Action::UniqueName: {
					SgInitializedName* iName = isSgInitializedName( (*i).getNode1() ); ROSE_ASSERT( iName );
					SgName name = SgName( iName->get_name().str() );
					SgName finalName;

					// Make sure the name is unique for this (at least to this call to operate)
					int counter = 0;
					finalName = name;
					pair<set<SgName>::iterator,bool> ret = (*curJP).varNames.insert( finalName );
					while( !ret.second ) {
						finalName = name + IntToStr( counter );
						ret = (*curJP).varNames.insert( finalName );
					}

					// Love this... 1liner to rename the variable in the AST!
					if( name != finalName ) {
						SageInterface::set_name( iName, finalName );
					}
					break;
				}
				default:
					break;
			}
			i++;
		}

		actions.clear();
	}


	void InlinePrep::transform( SgNode* afNode, SgBasicBlock* jpBlock, string funcName ) {
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
		(*curJP).curFuncName = funcName;

		// Step 1 - begin the parsing...
		state = Begin;
		traverse( afNode, preorder );

		// Step 2 - make the changes to the AST...
		operate();
	}

	InlinePrep::Action::Action( Type _type, SgNode* _node1, SgNode* _node2 )
		: node1( _node1 ), node2( _node2 ), type( _type )
	{}

	SgNode* InlinePrep::Action::getNode1() const {
		return node1;
	}

	SgNode* InlinePrep::Action::getNode2() const {
		return node2;
	}

	InlinePrep::Action::Type InlinePrep::Action::getType() const {
		return type;
	}

}
