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
	EP_filename = "Eplith";
	
	if(argc < 2) {
		yyerror("Filename not given");
	}
	else if(argc > 2) {
		yyerror("Too much arguments");
	}
	
#ifdef TYPES_DEBUG
	edbg("Types: Int: " << typeInt << " | Float: " << typeFloat << " | String: " << typeStr << " | Array: " << typeArray << " | Function: " << typeFunc << " | File: " << typeFile << " | Void: " << typeVoid);
#endif
	
	newFile(argv[1]);
	
	EP_filename = argv[1];
	
	Variable::initNullVar();
	EPblib_init();
	
	clock_t beginTime = clock();
	
	yyparse();
	
	clock_t endTime = clock();
	
#ifdef EXEC_TIME
	edbg(endTime << " - " << beginTime);
	edbg("Exec time: " << (float(endTime - beginTime)) / CLOCKS_PER_SEC << "s.");
#endif
	
	return 0;
} 

