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

Attr attrNULL = {{-1, -1}, false, false, -1};
DataRange drNULL = {attrNULL, -1, NULL};
uInt uIntNULL = {NULL, drNULL};
String StringNULL = {NULL, drNULL};

char* catchVarName() {
	char* name = new char[255];
	
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

void opSwitch(char op, DataRange &dr, int* values, int size) {
	int value = -1;
	switch(op) {
		case '+':
			value = values[0];
			for(int j = 1 ; j < size ; j++) value += values[j];
			setDataRangeValue(dr, value);
			break;
		
		case '-':
			value = values[0];
			for(int j = 1 ; j < size ; j++) value -= values[j];
			setDataRangeValue(dr, value);
			break;
		
		case '*':
			value = values[0];
			for(int j = 1 ; j < size ; j++) value *= values[j];
			setDataRangeValue(dr, value);
			break;
		
		case '/':
			value = values[0];
			for(int j = 1 ; j < size ; j++) value /= values[j];
			setDataRangeValue(dr, value);
			break;
		
		case '%':
			value = values[0];
			for(int j = 1 ; j < size ; j++) value %= values[j];
			setDataRangeValue(dr, value);
			break;
		
		case '^':
			setDataRangeValue(dr, pow(values[0], values[1]));
			break;
			
		default:
			error(2, "Unexpected error.");
			break;
	}
}

void doOperation(DataRange &dr, int value1) {
	int values[255];
	values[0] = value1;
	char op = sourceCode[i];
	i++;
	whilespace();
	if(sourceCode[i] == '@') {
		uInt &var2 = findVar(catchVarName());
		if(var2.name != NULL) {
			values[1] = var2.dr.value;
			whilespace();
			if(sourceCode[i] == ';') {
				opSwitch(op, dr, values, 2);
			} else {
				error(1, "Expecting a ';' here.");
			}
		} else {
			error(2, "This variable doesn't exist");
		}
	}
	else if(ish(sourceCode[i])) {
		values[1] = catchValue(true);
		if(values[1] != -1) {
			whilespace();
			if(sourceCode[i] == ';') {
				opSwitch(op, dr, values, 2);
			} else {
				error(1, "Expecting a ';' here.");
			}
		}
	}
}

void catchOperationResult(uInt &tmp, bool withValueFirst = false, int value = -1) {
	if(!withValueFirst) {
		uInt &var1 = findVar(catchVarName());
		if(var1.name != NULL) {
			whilespace();
			if(sourceCode[i] == ';') {
				setDataRangeValue(tmp.dr, var1.dr.value);
			} else {
				if(isop(sourceCode[i])) {
					doOperation(tmp.dr, var1.dr.value);
				} else {
					error(1, "Expecting a ';' here.");
				}
			}
		} else {
			error(2, "This variable doesn't exist.");
		}
	} else {
		whilespace();
		if(sourceCode[i] == ';') {
			setDataRangeValue(tmp.dr, value);
		} else {
			if(isop(sourceCode[i])) {
				doOperation(tmp.dr, value);
			}
		}
	}
}

uInt catchVar() {
	uInt temp = {NULL};
	temp.name = catchVarName();
	
	uInt &tmp = findVar(temp.name);
	
	whilespace();
	if(sourceCode[i] == '=') { // var = ...
		i++;
		whilespace();
		if(!varExists(temp)) { // If var exists
			if(sourceCode[i] == '&') { // var = dr ...
				temp.dr = catchDataRange(true);
				whilespace();
				if(sourceCode[i] == '=') { // var = dr = ...
					i++;
					whilespace();
					if(sourceCode[i] == '@') { // var = dr = var ...
						catchOperationResult(temp);
						whilespace();
						if(sourceCode[i] == ';') { // var = dr = var ... ;
							vars.push_back(temp);
							return temp;
						} else { // var = dr = var ...
							error(1, "Expecting a ';' here.");
						}
					} else {
						int tmpValue = catchValue(true);
						if(tmpValue != -1) { // var = dr = value ...
							catchOperationResult(temp, true, tmpValue);
						} else { // var = dr =
							error(1, "Expecting a value or a variable.");
						}
						whilespace();
						if(sourceCode[i] == ';') { // var = dr = value ... ;
							vars.push_back(temp);
							return temp;
						} else { // var = dr = value ...
							error(1, "Expecting a ';' here.");
						}
					}
				} else {
					if(sourceCode[i] == ';') { // var = dr;
						vars.push_back(temp);
						return temp;
					} else { // var = dr
						error(1, "Expecting a ';' here.");
					}
				}
			} else { // var = 
				error(1, "A data range is expected here.");
			}
		} else { // If var doesn't exist
			if(sourceCode[i] == '&') { // var = dr ...
				string warn = "Variable @(";
					   warn += tmp.name;
					   warn += ") already assigned.";
				warning(warn);
				tmp.dr = catchDataRange(true);
				tmp.dr.value = getDataRangeValue(tmp.dr);
				whilespace();
				if(sourceCode[i] == ';') { // var = dr;
					return tmp;
				} else {
					if(sourceCode[i] == '=') { // var = dr = ...
						i++;
						whilespace();
						if(sourceCode[i] == '@') { // var = dr = var ...
							catchOperationResult(tmp);
							whilespace();
							if(sourceCode[i] == ';') { // var = dr = var ... ;
								return tmp;
							} else { // var = dr = var ...
								error(1, "Expecting a ';' here.");
							}
						} else { // var = dr = ...
							int tmpValue = catchValue(true);
							if(tmpValue != -1) { // var = dr = value ...
								catchOperationResult(tmp, true, tmpValue);
							} else { // var = dr =
								error(1, "Expecting a value of a variable.");
							}	
							whilespace();
							if(sourceCode[i] == ';') { // var = dr = value ... ;
								return tmp;
							} else { // var = dr = value ...
								error(1, "Expecting a ';' here.");
							}
						}
					} else { // var = dr
						error(1, "Expecting a ';' here.");
					}
				}
			}
			else if(sourceCode[i] == '@') { // var = var ...
				catchOperationResult(tmp);
				return tmp;
			} else { // var = ...
				int tmpValue = catchValue(true);
				if(tmpValue != -1) { // var = value ...
					catchOperationResult(tmp, true, tmpValue);
					whilespace();
					if(sourceCode[i] == ';') { // var = value ... ;
						return tmp;
					} else { // var = value ...
						error(1, "Expecting a ';' here.");
					}
				} else { // var =
					error(1, "Expecting a value of a variable.");
				}
			}
		}
	} else { // var
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
	
	whilespace();
	if(sourceCode[i] == '=') {
		i++;
		whilespace();
		if(!stringExists(temp)) {
			if(sourceCode[i] == '&') {
				temp.dr = catchDataRange(true);
				whilespace();
				if(sourceCode[i] == '=') {
					i++;
					setDataRangeStringValue(temp.dr, catchStringValue(true), true);
				}
				whilespace();
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
				whilespace();
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

