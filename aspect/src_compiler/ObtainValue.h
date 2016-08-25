using namespace std;

namespace co {

	class Value {
		public:
			typedef enum { Ptr, Str, Int, Symbol, Invalid } Type;
			typedef union {
				void* c_ptr;
				const char* c_str;
				int c_int;
				SgSymbol* symbol;
			} Data;
		private:
			Type type;
			Data data;

		public:
			Value();

			void SetPtr( void* _ptr );
			void SetStr( const char* _str );
			void SetInt( int _int );
			void SetSymbol( SgSymbol* _symbol );

			operator const char*() const;
			operator int() const;
			operator SgSymbol*() const;
	};

	typedef enum { SymbolValue, SymbolName, SymbolNode, SymbolInvalid } ObtainWhat;

	const Value ObtainValue( SgExpression* _exp, ObtainWhat what, SgFunctionDeclaration* curFunctionDeclaration );
}
