#include "Compass.h"
#include "rose.h"

#include "CompareAssignment.h"

using namespace std;

namespace co {

	bool CompareAssignment( SgNode* first, SgNode* second ) {
		SgAssignInitializer* ai1;
		SgAssignInitializer* ai2;
		SgPointerDerefExp* pde1;
		SgPointerDerefExp* pde2;
		SgVarRefExp* vre1;
		SgVarRefExp* vre2;
		SgSymbol* s1;
		SgSymbol* s2;
		SgName* n1;
		SgName* n2;
		SgArrowExp* ae1;
		SgArrowExp* ae2;
		SgPntrArrRefExp* pare1;
		SgPntrArrRefExp* pare2;
		SgDotExp* de1;
		SgDotExp* de2;
		SgCastExp* ce1;
		SgCastExp* ce2;
		SgType* t1;
		SgType* t2;
		SgStringVal* sv1;
		SgStringVal* sv2;
		SgEnumVal* e1;
		SgEnumVal* e2;
		SgIntVal* i1;
		SgIntVal* i2;
		SgUnsignedIntVal* ui1;
		SgUnsignedIntVal* ui2;

		if( (first == NULL) && (second == NULL) ) {
			return true;
		}
		else if( first == NULL || second == NULL ) {
			return false;
		}
		if( (ai1 = isSgAssignInitializer( first )) != NULL ) {
			if( (ai2 = isSgAssignInitializer( second )) != NULL ) {
				return CompareAssignment( ai1->get_operand(), ai2->get_operand() );
			}
			else {
				return false;
			}
		}
		if( (pde1 = isSgPointerDerefExp( first )) != NULL ) {
			if( (pde2 = isSgPointerDerefExp( second )) != NULL ) {
				return CompareAssignment( pde1->get_operand(), pde2->get_operand() );
			}
			else {
				return false;
			}
		}
		else if( (vre1 = isSgVarRefExp( first )) != NULL ) {
			if( (vre2 = isSgVarRefExp( second )) != NULL ) {
				return CompareAssignment( vre1->get_symbol(), vre2->get_symbol() );
			}
			else {
				return false;
			}
		}
		else if( (s1 = isSgSymbol( first )) != NULL ) {
			if( (s2 = isSgSymbol( second )) != NULL ) {
				SgName a = s1->get_name();
				SgName b = s2->get_name();
				return CompareAssignment( &a, &b );
			}
			else {
				return false;
			}
		}
		else if( (n1 = isSgName( first )) != NULL ) {
			if( (n2 = isSgName( second )) != NULL ) {
				return ( (*n1) == (*n2) );
			}
			else {
				return false;
			}
		}
		else if( (ae1 = isSgArrowExp( first )) != NULL ) {
			if( (ae2 = isSgArrowExp( second )) != NULL ) {
				if( CompareAssignment( ae1->get_lhs_operand(), ae2->get_lhs_operand() ) ) {
					return CompareAssignment( ae1->get_rhs_operand(), ae2->get_rhs_operand() );
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else if( (pare1 = isSgPntrArrRefExp( first )) != NULL ) {
			if( (pare2 = isSgPntrArrRefExp( second )) != NULL ) {
				if( CompareAssignment( pare1->get_lhs_operand(), pare2->get_lhs_operand() ) ) {
					return CompareAssignment( pare1->get_rhs_operand(), pare2->get_rhs_operand() );
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else if( (de1 = isSgDotExp( first )) != NULL ) {
			if( (de2 = isSgDotExp( second )) != NULL ) {
				if( CompareAssignment( de1->get_lhs_operand(), de2->get_lhs_operand() ) ) {
					return CompareAssignment( de1->get_rhs_operand(), de2->get_rhs_operand() );
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else if( (ce1 = isSgCastExp( first )) != NULL ) {
			if( (ce2 = isSgCastExp( second )) != NULL ) {
				if( CompareAssignment( ce1->get_type(), ce2->get_type() ) ) {
					return CompareAssignment( ce1->get_operand(), ce2->get_operand() );
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else if( (t1 = isSgType( first )) != NULL ) {
			if( (t2 = isSgType( second )) != NULL ) {
				return (SageInterface::get_name( t1 ) == SageInterface::get_name( t2 ) );
			}
			else {
				return false;
			}
		}
		else if( (sv1 = isSgStringVal( first )) != NULL ) {
			if( (sv2 = isSgStringVal( second )) != NULL ) {
				return (sv1->get_value() == sv2->get_value() );
			}
			else {
				return false;
			}
		}
		else if( (e1 = isSgEnumVal( first )) != NULL ) {
			if( (e2 = isSgEnumVal( second )) != NULL ) {
				return (e1->get_value() == e2->get_value() );
			}
			else {
				return false;
			}
		}
		else if( (i1 = isSgIntVal( first )) != NULL ) {
			if( (i2 = isSgIntVal( second )) != NULL ) {
				return (i1->get_value() == i2->get_value() );
			}
			else {
				return false;
			}
		}
		else if( (ui1 = isSgUnsignedIntVal( first )) != NULL ) {
			if( (ui2 = isSgUnsignedIntVal( second )) != NULL ) {
				return (ui1->get_value() == ui2->get_value() );
			}
			else {
				return false;
			}
		}
		else {
			// YET TO IMPLEMENT THIS SCENARIO!!
			// Assuming this will be for ptr, int, double, .. values  - else will need to think the above
			cout << "Error: unrecognised expression in compass' weave: " << first->class_name() << ", for code: " << first->unparseToString() << endl;
			ROSE_ASSERT( 0 );
			return false;
		}
	}
}
