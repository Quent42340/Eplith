//----------------------------------
//
// BFI - BrainFuck Interpretor
// by Quent42340
// https://gist.github.com/1622410
// https://github.com/FabianM/Brainfuck/blob/master/src/interpreter.c
//
//----------------------------------
#include "globalVars.h"
#include "errors.h"
#include "attributes.h"
#include "operators.h"
#include "vars.h"

using namespace std;

int main(int argc, char* argv[]) {
	HEX = false;
	DEBUG = false;
	
	ifstream file(argv[1], ifstream::in);
	
	string line;
	
	while(getline(file, line)) {
		sourceCode += line;
	}
		
	cout << " --- START --- " << endl;
	
	exec();
	
	cout << " --- END --- " << endl;
	
	return 0;
}

