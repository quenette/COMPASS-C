#include "Compass.h"
#include "rose.h"

#include "FindSymbols.h"
#include "Weaver.h"

using namespace std;
using namespace co;

int main( int argc, char * argv[] ) {
	SgProject* project;
	int result;

	coInit( &argc, &argv, 0 );

	// Build ROSE AST
	project = frontend( argc, argv );
	ROSE_ASSERT( project != NULL );

	generateDOT( *project );

	// Find the advice declaration to get the symbol
	FindSymbols declarations;
	declarations.traverseInputFiles( project, preorder );

	// Find advice calls to build the advice lists
	Weaver weaver( project, &declarations );
	weaver.traverseInputFiles( project, preorder );
	weaver.operate();

//	AstTests::runAllTests(project);

	// Generate Code and compile it with backend (vendor) compiler to generate object code
	// or executable (as specified on commandline using vendor compiler's command line).
	// Returns error code form compilation using vendor's compiler.
	result =  backend( project );

	coFinalise();
	return result;
}

