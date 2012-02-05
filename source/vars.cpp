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
#include "vars.h"

using namespace std;

vector<uInt> vars;
vector<String> strings;

uInt uIntNULL = {NULL};
String StringNULL = {NULL};

char* catchVarName() {
	char* name = new char;
	
	if(sourceCode[i + 1] == '(') {
		i++;
		unsigned int temp = i + 1;
		
		for(i++ ; sourceCode[i] != ')' ; i++) {
			name[i - temp] = sourceCode[i];
		}
		name[i - temp] = '\0';
		
		i++;
		return name;
	} else {
		return NULL;
	}
}

bool varExists(uInt var) {
	for(unsigned int j = 0 ; j < vars.size() ; j++) {
		if(strcmp(vars[j].name, var.name) == 0) {
			return true;
		}
	}
	
	return false;
}

uInt catchVar() {
	uInt temp = {NULL};
	temp.name = catchVarName();
	
	uInt &tmp = findVar(temp.name);
	
	while(sourceCode[i] == ' ') i++;
	if(sourceCode[i] == '=') {
		i++;
		while(sourceCode[i] == ' ') i++;
		if(!varExists(temp)) {
			if(sourceCode[i] == '&') {
				temp.dr = catchDataRange(true);
				setDataRangeValue(temp.dr, catchValue(), true);
				if(sourceCode[i] == ';') {
					vars.push_back(temp);
					return temp;
				} else {
					error(1, "Expecting a ';' here.");
				}
			} else {
				error(1, "A data range is expected here.");
			}
		} else {
			if(sourceCode[i] == '&') {
				warning("Variable already assigned.");
				tmp.dr = catchDataRange(true);
				tmp.dr.value = getDataRangeValue(tmp.dr);
				if(sourceCode[i] == ';') {
					return tmp;
				} else {
					error(1, "Expecting a ';' here.");
				}
			} else {
				setDataRangeValue(tmp.dr, catchValue(true));
				while(sourceCode[i] == ' ') i++;
				if(sourceCode[i] == ';') {
					return tmp;
				} else {
					error(1, "Expecting a ';' here.");
				}	
			}
		}
	} else {
		error(1, "Needs an '=' operator here.");
	}
	
	return temp;
}

uInt& findVar(char* name) {
	for(unsigned int j = 0 ; j < vars.size() ; j++) {
		if(strcmp(vars[j].name, name) == 0) {
			return vars[j];
		}
	}
	
	return uIntNULL;
}

bool stringExists(String str) {
	for(unsigned int j = 0 ; j < strings.size() ; j++) {
		if(strcmp(strings[j].name, str.name) == 0) {
			return true;
		}
	}
	
	return false;
}

String catchString() {
	String temp = {NULL};
	temp.name = catchVarName();
	
	String &tmp = findString(temp.name);
	
	while(sourceCode[i] == ' ') i++;
	if(sourceCode[i] == '=') {
		i++;
		while(sourceCode[i] == ' ') i++;
		if(!stringExists(temp)) {
			if(sourceCode[i] == '&') {
				temp.dr = catchDataRange(true);
				setDataRangeStringValue(temp.dr, catchStringValue(true), true);
				if(sourceCode[i] == ';') {
					strings.push_back(temp);
					return temp;
				} else {
					error(1, "Expecting a ';' here.");
				}
			} else {
				error(1, "A data range is expected here.");
			}
		} else {
			if(sourceCode[i] == '&') {
				warning("Variable already assigned.");
				tmp.dr = catchDataRange(true);
				tmp.dr.str = getDataRangeStringValue(tmp.dr);
				if(sourceCode[i] == ';') {
					return tmp;
				} else {
					error(1, "Expecting a ';' here.");
				}
			} else {
				setDataRangeStringValue(tmp.dr, catchStringValue(true));
				while(sourceCode[i] == ' ') i++;
				if(sourceCode[i] == ';') {
					return tmp;
				} else {
					error(1, "Expecting a ';' here.");
				}	
			}
		}
	} else {
		error(1, "Needs an '=' operator here.");
	}
	
	return temp;
}

String& findString(char* name) {
	for(unsigned int j = 0 ; j < strings.size() ; j++) {
		if(strcmp(strings[j].name, name) == 0) {
			return strings[j];
		}
	}
	
	return StringNULL;
}

