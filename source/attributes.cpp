#include "globalVars.h"
#include "errors.h"
#include "attributes.h"

using namespace std;

vector<unsigned char> mem(MEM_SIZE, 0);

int catchAttr() {
	char* tmpChar = new char;
	int attr = -1;
	if(sourceCode[i + 1] == '(') {
		i++;
		unsigned int temp = i + 1;
		for(i++ ; sourceCode[i] != ')' ; i++) {
			tmpChar[i - temp] = sourceCode[i];
		}
		if(HEX) attr = (unsigned int)strtol(tmpChar, NULL, 16);
		else	attr = atoi(tmpChar);
	}
	return attr;
}

Attr catchAttrs() {
	char* tmpChar = new char;
	char* tmpChar2 = new char;
	Attr attr = {{-1, -1}, false, -1};
	if(sourceCode[i + 1] == '(') {
		i++;
		unsigned int temp = i + 1;
		for(i++ ; sourceCode[i] != ';' ; i++) {
			if(sourceCode[i] == ',') attr.sized = true; break;
			tmpChar[i - temp] = sourceCode[i];
		}
		temp = i + 1;
		for(i++ ; sourceCode[i] != '(' ; i++) {
			tmpChar2[i - temp] = sourceCode[i];
		}
		if(HEX) attr.tab[0] = (unsigned int)strtol(tmpChar, NULL, 16);
		else	attr.tab[0] = atoi(tmpChar);
		if(!attr.sized) {
			if(HEX) attr.tab[1] = (unsigned int)strtol(tmpChar2, NULL, 16);
			else	attr.tab[1] = atoi(tmpChar2);
			attr.length = attr.tab[1] - attr.tab[0];
		} else {
			if(HEX) attr.length = (unsigned int)strtol(tmpChar2, NULL, 16);
			else	attr.length = atoi(tmpChar2);
		}
	}
	return attr;
}

int catchValue() {
	int value = -1;
	
	while(sourceCode[i] != '=') {
		if(sourceCode[i] == ';') {
			berror(1, "Needs an '=' operator.");
		}
		i++;
	}
	
	while(sourceCode[i] == ' ') i++;
	
	unsigned int temp = i + 1;
	char* tmpChar = new char;
	for(i++ ; isd(sourceCode[i]) || ish(sourceCode[i]) ; i++) {
		tmpChar[i - temp] = sourceCode[i];
	}
	
	if(HEX) value = (unsigned int)strtol(tmpChar, NULL, 16);
	else	value = atoi(tmpChar);
	
	return value;
}

char* catchString() {
	char* name = new char;
	
	if(sourceCode[i + 1] == '(') {
		i++;
		if(sourceCode[i + 1] == '"') {
			i++;
			unsigned int temp = i + 1;
			
			for(i++ ; sourceCode[i] != '"' ; i++) {
				name[i - temp] = sourceCode[i];
			}
			
			if(sourceCode[i + 1] == ')') {
				i++;
				return name;
			} else {
				return NULL;
			}
		} else {
			return NULL;
		}
	} else {
		return NULL;
	}
}

DataRange catchDataRange() {
	Attr addr = catchAttrs();
	DataRange dr = {addr, -1};
	
	if(addr.tab[0] != -1) {
		dr.value = catchValue();
		
		if(dr.value == -1) {
			berror(2, "A value is required after '=' operator.");
		} else {
			int vSize = (int)ceil((float)dr.value / 255);
			
			if(vSize < dr.addr.length) {
				for(int j = dr.addr.tab[0] ; j < dr.addr.tab[0] + vSize ; j--) {
					if((dr.value - 255) > 0) {
						dr.value -= 255;
						memory[j] = 255;
					} else {
						memory[j] = dr.value;
					}
				}
				
				for(int j = dr.addr.tab[0] ; j < dr.addr.tab[0] + dr.addr.length ; j++) {
					mem[j] = 1;
				}
			} else {
				berror(3, "Data area too little for the value.");
			}
		}
	} else {
		berror(1, "'&' operator needs at least one argument.");
	}
	
	return dr;
}

