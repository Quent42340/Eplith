#include "globalVars.h"
#include "errors.h"

using namespace std;

void error(int code, string error) {
	string type;
	switch(code) {
		case 1:
			type = "SYNTAX ERROR";
			break;
		
		case 2:
			type = "FATAL ERROR";
			break;
		
		case 3:
			type = "MEMORY ERROR";
			break;
		
		default:
			type = "ERROR";
			break;
	}
	if(DEBUG) cout << sourceCode[i - 1] << sourceCode[i] << sourceCode[i + 1];
	cerr << " *** " << type << ": " << error << " *** " << i << " *** " << endl;
	cout << " --- END --- " << endl;
	exit(code);
}

void warning(string warning) {
	cout << " /!\\ " << "WARNING: " << warning << " *** " << i << " /!\\ " << endl;
}

