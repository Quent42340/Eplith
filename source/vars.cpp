#include "globalVars.h"
#include "errors.h"
#include "vars.h"

using namespace std;

vector<uInt> vars;

uInt uIntNULL = {NULL};

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

bool varExist(uInt var) {
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
	if(!varExist(temp)) {
		while(sourceCode[i] == ' ') i++;
		if(sourceCode[i] == '=') {
			i++;
			while(sourceCode[i] == ' ') i++;
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
			error(1, "Needs an '=' operator here.");
		}
	} else {
		uInt &tmp = findVar(temp.name);
		
		while(sourceCode[i] == ' ') i++;
		if(sourceCode[i] == '=') {
			i++;
			while(sourceCode[i] == ' ') i++;
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
		} else {
			error(1, "Needs an '=' operator here.");
		}
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

