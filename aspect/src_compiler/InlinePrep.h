using namespace std;

namespace co {

	//********************************************************************************************************************************
	// ...
	class InlinePrep : public AstSimpleProcessing {
		public:
			// To represent the state of the visit/parse of the AST
			typedef enum { Begin, Searching, Complete, StateInvalid } State;

			// Because we pass through the AST via a visit function, we dont want to be changing the AST until we're finished
			// analysing. Hence we need to create a list of actions we want to perform on the AST, which is then performed in
			// the operate function.
			class Action {
				public:
					enum Type { ReplaceExpression, UniqueName };
				private:
					SgNode* node1;
					SgNode* node2;
					Type type;

				public:
					Action( Type _type, SgNode* _node1 = NULL, SgNode* _node2 = NULL );
					SgNode* getNode1() const;
					SgNode* getNode2() const;
					Type getType() const;
			};
			typedef Rose_STL_Container<Action> ActionList;

			// Working structure for inlining functional aspects into a function joinpoint
			typedef struct {
				SgBasicBlock* jpBlock; // joinpoint block (used as an id to the joinpoint)
				list<SgFunctionDeclaration*> funcDecs; // function aspects on this joinpoint
				set<SgName> varNames; // unique varNames across the joinpoint

				list<SgFunctionDeclaration*>::iterator curFunc;
				string curFuncName; // used if __func__ needs to be resolved
			} FunctionJoinpoint;

		private:
			list<FunctionJoinpoint> jps;

			// Working variables...
			list<FunctionJoinpoint>::iterator curJP;
			SgBasicBlock* curScope;
			State state;
			ActionList actions;

		public:
			InlinePrep();

			virtual void visit( SgNode* astNode );

			void transform( SgNode* afNode, SgBasicBlock* jpBlock, string funcName = "" ); // traverse and operate

		private:
			virtual void operate( void );
	};

}
