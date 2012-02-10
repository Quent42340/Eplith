/*---------------------------------------------------------------------------------

    BrainFuck Advanced Intepreter
    Copyright (C) 2012 Quent42340 quent42340@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

---------------------------------------------------------------------------------*/
#include "globalVars.h"
#include "errors.h"

using namespace std;

void berror(int code, string error, const char* file, unsigned int line) {
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
	if(DEBUG) cerr << " *** " << lines[pLine - 1] << " *** " << endl;
	//if(DEBUG) cerr << " *** " << sourceCode[i - 3] << sourceCode[i - 2] << sourceCode[i - 1] << "|" << sourceCode[i] << "|" << sourceCode[i + 1] << sourceCode[i + 2] << sourceCode[i + 3] << " *** ";
	//if(DEBUG) cerr << " *** " << sourceCode[i - 1] << "|" << sourceCode[i] << "|" << sourceCode[i + 1] << " *** ";
	cerr << " *** " << type << ": " << error << " *** " << bFilename << ":" << pLine << ":" << i - pLastLine << " *** "
#if BE_DEBUG
		<< file << ":" << line
#endif
		<< endl;
	cerr << " --- END: Program terminated with error code " << code << ". ---" << endl;
	exit(code);
}

void bwarning(string warning, const char* file, unsigned int line) {
	cerr << " /!\\ " << "WARNING: " << warning << " *** " << bFilename << ":" << pLine << ":" << i - pLastLine << " /!\\ "
#if BF_DEBUG
		<< file << ":" << line
#endif
		<< endl;
}

