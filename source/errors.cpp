#include "globalVars.h"
#include "errors.h"

using namespace std;

void berror(int code, string error) {
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
	cerr << " *** " << type << ": " << error << " *** " << i << " *** " << endl;
	exit(code);
}

