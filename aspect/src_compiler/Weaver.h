using namespace std;

namespace co {

	//********************************************************************************************************************************
	// Rose-based parser to populate joinpoints from advices
	class Weaver : public AstSimpleProcessing {
		public:
			class Action {
				public:
					enum Type { Add, Remove, Inline, Unstack, ReduceNames, RemoveUnusedLabels };
				private:
					SgNode* node1;					// item (to remove/add, etc)
					SgNode* node2;					// parent block - TODO: this name is inconsistent/poor!!!
					SgNode* node3;				// insertion relative to (can be NULL for removal)
					bool before;					// insert before relative (else after)
					Type type;					// action type (remove/add, etc)

				public:
					Action( Type _type, SgNode* _node1, SgNode* _node2 = NULL, SgNode* _node3 = NULL, bool _before = false );
					SgNode* getNode1() const;
					SgNode* getNode2() const;
					SgNode* getNode3() const;
					bool getBefore() const;
					Type getType() const;
			};
			typedef Rose_STL_Container<Action> ActionList;

		private:
			SgProject* project;
			FindSymbols* declarations;	// the advice declaration (required input)
			SgFunctionDeclaration* curFunctionDeclaration;	// the current function we're in (for __func__ resolution, etc)
			ActionList actions;
			int uniqueFuncCounter;

		public:
			Weaver( SgProject* project, FindSymbols* _declarations );
	
			void visit( SgNode* astNode );
			void operate( void );
	
			ActionList& getActions();
	};

}
