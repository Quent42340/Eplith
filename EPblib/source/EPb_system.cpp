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
#include "EPb_tools.h"
#include "EPb_system.h"

using namespace std;

double System_time(EPb_args *args) {
	return time(NULL);
}

int System_execute(EPb_args *args) {
	string cmd;
	if(EPb_argsNbr(args) == 1) {
		cmd = EPb_getStr((*args)[0]);
	} else {
		cmd = "";
	}
	return system(cmd.c_str());
}

void *System_exit(EPb_args *args) {
	int status;
	if(EPb_argsNbr(args) == 1) {
		status = EPb_getInt((*args)[0]);
	} else {
		status = 0;
	}
	exit(status);
	return NULL;
}

EPb_initStruct(Time, System_time,, 0, "");
EPb_initStruct(Execute,, System_execute, 1, "cmd=-1");
EPb_initStruct(Exit,, System_exit, 1, "status=-1");

void EPblib_initSystem() {
	map<string, Value*> System_elements;
	
	EPb_initElemFunc(System_elements, Time, time);
	EPb_initElemFunc(System_elements, Execute, execute);
	EPb_initElemFunc(System_elements, Exit, exit);
	
	Variable *System = new Variable("System", new Value(System_elements));
}

