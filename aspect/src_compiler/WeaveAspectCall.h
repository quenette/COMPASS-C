using namespace std;

namespace co {

	//********************************************************************************************************************************
	// ...
	class WeaveAspectCall : public AstSimpleProcessing {
		public:
			// To represent the state of the visit/parse of the AST
			typedef enum { Begin, Searching, Complete, StateInvalid } State;

			// Because we pass through the AST via a visit function, we dont want to be changing the AST until we're finished
			// analysing. Hence we need to create a list of actions we want to perform on the AST, which is then performed in
			// the operate function.
			class Action {
				public:
					enum Type { ReplaceExpression, TypeInvalid };
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

				list<SgFunctionDeclaration*>::iterator curFunc;
			} FunctionJoinpoint;
		private:
			list<FunctionJoinpoint> jps;

			// Working variables...
			list<FunctionJoinpoint>::iterator curJP;
			SgBasicBlock* curScope;
			State state;
			ActionList actions;

		public:
			WeaveAspectCall();

			virtual void visit( SgNode* astNode );

			/** Perform the transformation.
			 **  Takes this form to suite the "visit" pattern of ROSE but first prime the data structures required to maintain
			 **  state between node-visits.
			 **  Replaces call to "traverse". It also calls "operate".
			 ** @param afNode The function definition AST node for the aspectual function that this transformation is being applied
			 **                to. Type: SgNode*.
			 ** @param jpBlock The block "{}" AST node for the pointcut. Type: SgBasicBlock* */
			void transform( SgNode* afNode, SgBasicBlock* jpBlock );

		private:
			virtual void operate( void );
	};

}
