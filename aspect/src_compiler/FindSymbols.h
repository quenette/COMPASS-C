using namespace std;

namespace co {

	//********************************************************************************************************************************
	// Rose-based parser to obtain the "advice" and "pointcut" symbols
	class FindSymbols : public AstSimpleProcessing {
		private:
			SgFunctionSymbol* adviceSymbol;		// the symbol of the advice function declaration
			SgFunctionSymbol* pointcutSymbol;	// the symbol of the pointcut function declaration (=joinpoint+run)

		public:
			FindSymbols();
	
			void visit ( SgNode* astNode );
	
			SgFunctionSymbol* getAdviceSymbol( void );
			SgFunctionSymbol* getPointcutSymbol( void );
	};

}
