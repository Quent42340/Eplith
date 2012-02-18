/*---------------------------------------------------------------------------------

    WordSphere Intepreter
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
#include "header.h"

bVar* vars;
int sizeVars = 0;
bool debug = false;

bVar iVar(char* name, int value) {
	bVar var;
	
	var.name = name;
	var.type = VAR_INT;
	var.iValue = value;
	
	vars[sizeVars] = var;
	sizeVars++;
	
	return var;
}

bVar sVar(char* name, char* value) {
	bVar var;
	
	var.name = name;
	var.type = VAR_CHAR;
	var.sValue = value;
	
	vars[sizeVars] = var;
	sizeVars++;
	
	return var;
}

char* var_name(bVar var) {
	return var.name;
}

char* var_type(bVar var) {
	return var.type;
}

int var_iValue(bVar var) {
	return var.iValue;
}

char* var_sValue(bVar var) {
	return var.sValue;
}

bVar bVarNULL = {"NULL", "void", -1};

bVar var_find(char* name) {
	int i;
	for(i = 0 ; i < sizeVars ; i++) {
		if(!strcmp(vars[i].name, name)) {
			return vars[i];
		}
	}
	
	return bVarNULL;
}

