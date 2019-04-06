/*---------------------------------------------------------------------------------

    Eplith Intepreter
    Copyright (C) 2012 Quent42340 <quent42340@gmail.com>

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
#include <EPlib.h>
#include <EPblib.h>
#include <ctime>

using namespace std;

int main(int argc, char* argv[]) {
	// Set default filename to "Eplith" to make the error messages before launching a script much more pretty
	EP_filename = "Eplith";

	// Set default value for the inline interpreter
	EP_inlineInterpreter = false;

	#ifdef TYPES_DEBUG // Debug information of types enum
		edbg("Types: Int: " << typeInt << " | Float: " << typeFloat << " | String: " << typeStr << " | Array: " << typeArray << " | Function: " << typeFunc << " | File: " << typeFile << " | Void: " << typeVoid);
	#endif

	// Initalize Eplith libs and default variables
	Variable::initNullVar();
	EPblib_init();

	// Verify the arguments
	if(argc > 2) {
		yyerror("Too much arguments");
	}
	else if(argc == 2) {
		// Initialize the file
		newFile(argv[1]);
		EP_filename = argv[1];
	} else {
		EP_inlineInterpreter = true;
	}

	clock_t beginTime = clock();

	// Run the script
	yyparse();

	clock_t endTime = clock();

#ifdef EXEC_TIME // Display execution time of the script
	edbg("Exec time: " << (float(endTime - beginTime)) / CLOCKS_PER_SEC << "s.");
#endif

	return 0;
}

