#include "Compass.h"
#include "rose.h"

#include "ObtainValue.h"

using namespace std;

namespace co {

	Value::Value()
		: type( Invalid )
	{
		data.c_ptr = 0;
	}

	void Value::SetStr( const char* _str ) {
		type = Str;
		data.c_str = _str;
	}

	void Value::SetInt( int _int ) {
		type = Int;
		data.c_int = _int;
	}

	void Value::SetSymbol( SgSymbol* _symbol ) {
		type = Symbol;
		data.symbol = _symbol;
	}

	Value::operator const char*() const {
		assert( type == Str );
		return data.c_str;
	}

	Value::operator int() const {
		assert( type == Int );
		return data.c_int;
	}

	Value::operator SgSymbol*() const {
		assert( type == Symbol || (type == Int && data.c_int == 0) );
		return data.symbol;
	}


	const Value ObtainValue( SgExpression* _exp, ObtainWhat what, SgFunctionDeclaration* curFunctionDeclaration ) {
		SgExpression* exp = isSgExpression( _exp ); ROSE_ASSERT( exp );
		SgVarRefExp* ref;
		SgCastExp* cast;
		SgStringVal* strVal;
		SgEnumVal* enumVal;
		SgIntVal* intVal;
		SgUnsignedIntVal* uintVal;
		SgFunctionRefExp* func;
		Value result;

		if( (ref = isSgVarRefExp( exp )) != NULL ) {
			// In here because "exp" is a reference to a variable
			if( ref->get_file_info()->isCompilerGenerated() ) {
				// If the variable is compiler generated... we need to resolve this ourselves
				string str = ref->unparseToString();
				ROSE_ASSERT( str == "__func__" ); // this is the only inbuilt we handle thus far
				ROSE_ASSERT( curFunctionDeclaration); // We need to be AND should be as far as I know inside a function
				switch( what ) {
					case SymbolValue:
						result.SetStr( curFunctionDeclaration->get_name().str() );
						break;
					case SymbolName:
						assert( what != SymbolName /* __func__ hasn't a symbol! */ );
						break;
					case SymbolNode:
						/* Not attempting to handle this case... may not even know how or what it means */
						assert( what != SymbolName );
						break;
					default:
						assert( what < SymbolInvalid );
						break;
				}
			}
			else {
				// Else resolve the variable symbol
				SgSymbol* symbol = ref->get_symbol(); ROSE_ASSERT( symbol );
				switch( what ) {
					case SymbolValue: {
						SgVariableSymbol* varSymbol = isSgVariableSymbol( symbol ); ROSE_ASSERT( varSymbol );
						SgInitializedName* inName = varSymbol->get_declaration(); ROSE_ASSERT( inName );
						SgAssignInitializer* init = isSgAssignInitializer( inName->get_initptr() ); ROSE_ASSERT( init );
						//cout << symbol->get_name().str() << " [" << ref->get_symbol() << "] ";
						result = ObtainValue( init->get_operand(), what, curFunctionDeclaration );
						break;
					}
					case SymbolName:
						result.SetStr( symbol->get_name().str() );
						break;
					case SymbolNode:
						result.SetSymbol( symbol );
						break;
					default:
						assert( what < SymbolInvalid );
						break;
				}
			}
		}
		else if( (cast = isSgCastExp( exp )) != NULL ) {
			// In here because "exp" is a cast ... need to dig deeper (not going to cast - as shouldn't change anything in C)
			//cout << "(cast) ";
			result = ObtainValue( cast->get_operand(), what, curFunctionDeclaration );
		}
		else if( (strVal = isSgStringVal( exp )) != NULL ) {
			// In here because "exp" is an inlined C string (i.e. "")
			//cout << strVal->get_value() << endl;
			switch( what ) {
				case SymbolValue:
					result.SetStr( strVal->get_value().c_str() );
					break;
				case SymbolName:
					/* Not attempting to handle this case... may not even know how or what it means */
					assert( what != SymbolName );
					break;
				case SymbolNode:
					/* Not attempting to handle this case... may not even know how or what it means */
					assert( what != SymbolNode );
					break;
				default:
					assert( what < SymbolInvalid );
					break;
			}
		}
		else if( (func = isSgFunctionRefExp( exp )) != NULL ) {
			SgSymbol* symbol = func->get_symbol(); ROSE_ASSERT( symbol );
			cout << symbol->get_name().str() << "() [" << func->get_symbol() << "]" << endl;
			switch( what ) {
				case SymbolValue:
result.SetSymbol( symbol );
					/* Not attempting to handle this case... may not even know how or what it means */
//					assert( what != SymbolValue );
					break;
				case SymbolName:
					result.SetStr( symbol->get_name().str() );
					break;
				case SymbolNode:
					result.SetSymbol( symbol );
					break;
				default:
					assert( what < SymbolInvalid );
					break;
			}
		}
		else if( (enumVal = isSgEnumVal( exp )) != NULL ) {
			// In here because "exp" is an enum value
			//cout << enumVal->get_value() << endl;
			switch( what ) {
				case SymbolValue:
					result.SetInt( enumVal->get_value() );
					break;
				case SymbolName:
					/* Not attempting to handle this case... may not even know how or what it means */
					assert( what != SymbolName );
					break;
				case SymbolNode:
					/* Not attempting to handle this case... may not even know how or what it means */
					assert( what != SymbolNode );
					break;
				default:
					assert( what < SymbolInvalid );
					break;
			}
		}
		else if( (intVal = isSgIntVal( exp )) != NULL ) {
			// In here because "exp" is an int value
			//cout << intVal->get_value() << endl;
			switch( what ) {
				case SymbolValue:
					result.SetInt( intVal->get_value() );
					break;
				case SymbolName:
					/* Not attempting to handle this case... may not even know how or what it means */
					assert( what != SymbolName );
					break;
				case SymbolNode:
					// "NULL" come up as a "0" ... if was asking for a symbol, saying null is ok
					assert( intVal->get_value() == 0 /* all other cases are not ok */ );
					result.SetSymbol( 0 );
					break;
				default:
					assert( what < SymbolInvalid );
					break;
			}
		}
		else if( (uintVal = isSgUnsignedIntVal( exp )) != NULL ) {
			// In here because "exp" is an unsigned int value
			//cout << uintVal->get_value() << endl;
			switch( what ) {
				case SymbolValue:
					result.SetInt( uintVal->get_value() );
					break;
				case SymbolName:
					/* Not attempting to handle this case... may not even know how or what it means */
					assert( what != SymbolName );
					break;
				case SymbolNode:
					/* Not attempting to handle this case... may not even know how or what it means */
					assert( what != SymbolNode );
					break;
				default:
					assert( what < SymbolInvalid );
					break;
			}
		}
		else {
			// YET TO IMPLEMENT THIS SCENARIO!!
			// Assuming this will be for ptr, int, double, .. values  - else will need to think the above
			cout << "Error: unrecognised expression in compass' weave: " << exp->class_name() << ", for code: " << exp->unparseToString() << endl;
			ROSE_ASSERT( 0 );
		}
		return result;
	}

}
