/*---------------------------------------------------------------------------------

    BrainFuck Advanced Interpreter
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
	
	Attr attr = {{-1, -1}, false, false, -1};
	
	if(sourceCode[i + 1] == '(') {
		i++;
		unsigned int temp = i + 1;
		
		for(i++ ; sourceCode[i] != ';' ; i++) {
			if(sourceCode[i] == ',') { attr.sized = true; break; }
			if(sourceCode[i] == ')') { attr.alone = true; break; }
			tmpChar[i - temp] = sourceCode[i];
		}
		
		if(!attr.alone) {
			temp = i + 1;
			
			for(i++ ; sourceCode[i] != ')' ; i++) {
				tmpChar2[i - temp] = sourceCode[i];
			}
		}
		
		if(HEX) attr.tab[0] = (unsigned int)strtol(tmpChar, NULL, 16);
		else	attr.tab[0] = atoi(tmpChar);
		
		if(!attr.alone) {
			if(!attr.sized) {
				if(HEX) attr.tab[1] = (unsigned int)strtol(tmpChar2, NULL, 16);
				else	attr.tab[1] = atoi(tmpChar2);
				attr.length = attr.tab[1] - attr.tab[0] + 1;
			} else {
				if(HEX) attr.length = (unsigned int)strtol(tmpChar2, NULL, 16);
				else	attr.length = atoi(tmpChar2);
			}
		} else {
			attr.length = 1;
		}
	}
	
	return attr;
}

int catchValue(bool onlyValue) {
	int value = -1;
	
	while(sourceCode[i] == ' ') i++;
	if((sourceCode[i] == '=') || (onlyValue)) {
		if(!onlyValue) i++;
		while(sourceCode[i] == ' ') i++;
		
		i--;
		unsigned int temp = i + 1;
		char* tmpChar = new char;
		for(i++ ; ish(sourceCode[i]) ; i++) {
			tmpChar[i - temp] = sourceCode[i];
		}
		tmpChar[i - temp] = '\0';
		
		if(HEX) value = (unsigned int)strtol(tmpChar, NULL, 16);
		else	value = atoi(tmpChar);
	}
	
	return value;
}

char* catchStringValue(bool onlyString) {
	char* str = new char;
	
	if((sourceCode[i + 1] == '(') || (onlyString)) {
		if(onlyString) { while(sourceCode[i] == ' ') i++; }
		else i++;
		if((sourceCode[i] == '"') || (sourceCode[i + 1] == '"')) {
			if(!onlyString) i++;
			unsigned int temp = i + 1;
			
			for(i++ ; sourceCode[i] != '"' ; i++) {
				if(sourceCode[i] == '\\' && sourceCode[i + 1] == 'n') {
					str[i - temp] = '\n';
					i++;
				} else {
					str[i - temp] = sourceCode[i];
				}
			}
			str[i - temp] = '\0';
			
			if((sourceCode[i + 1] == ')') || (sourceCode[i + 1] == ';')) {
				i++;
				return str;
			} else {
				return NULL;
			}
		} else {
			i--;
			return NULL;
		}
	} else {
		return NULL;
	}
}

void setDataRangeValue(DataRange &dr, int value, bool noError) {
	dr.value = value;
	
	if(dr.value == -1) {
		if(!noError) error(2, "A value is required after '=' operator.");
	} else {
		int vSize = (int)ceil((float)dr.value / 255);
		
		if(vSize <= dr.addr.length) {
			for(int j = dr.addr.tab[0] ; j < dr.addr.tab[0] + vSize ; j++) {
				if(((dr.value - 255 * (j - dr.addr.tab[0])) - 255) > 0) {
					memory[j] = 255;
				} else {
					memory[j] = (dr.value - 255 * (j - dr.addr.tab[0]));
					break;
				}
			}
			
			for(int j = dr.addr.tab[0] ; j < dr.addr.tab[0] + dr.addr.length ; j++) {
				mem[j] = 1;
			}
		} else {
			if(DEBUG) cout << "vSize: " << vSize << " *** dr.addr.length: " << dr.addr.length << " *** alone: " << dr.addr.alone << endl;
			error(3, "Data range too little for the value.");
		}
	}
}

int getDataRangeValue(DataRange &dr) {
	dr.value = 0;
	
	for(int j = dr.addr.tab[0] ; j < dr.addr.tab[0] + dr.addr.length ; j++) {
		dr.value += memory[j];
	}
	
	return dr.value;
}

void setDataRangeStringValue(DataRange &dr, char* str, bool noError) {
	dr.str = str;
	
	if(dr.str == NULL) {
		if(!noError) error(2, "A value is required after '=' operator.");
	} else {
		int vSize = strlen(dr.str) + 1;
		
		if(vSize <= dr.addr.length) {
			for(int j = dr.addr.tab[0] ; j < dr.addr.tab[0] + vSize ; j++) {
				memory[j] = dr.str[dr.addr.tab[0] - j];
			}
			
			for(int j = dr.addr.tab[0] ; j < dr.addr.tab[0] + dr.addr.length ; j++) {
				mem[j] = 1;
			}
		} else {
			if(DEBUG) cout << "vSize: " << vSize << " *** dr.addr.length: " << dr.addr.length << " *** alone: " << dr.addr.alone << endl;
			error(3, "Data range too little for the string.");
		}
	}	
}

char* getDataRangeStringValue(DataRange &dr) {
	if(dr.str == NULL) dr.str = new char;
	else {
		delete[] dr.str;
		dr.str = new char;
	}
	
	for(int j = dr.addr.tab[0] ; j < dr.addr.tab[0] + dr.addr.length ; j++) {
		dr.str[dr.addr.tab[0] - j] = memory[j];
	}
	
	return dr.str;
}

DataRange catchDataRange(bool onlyDR) {
	Attr addr = catchAttrs();
	DataRange dr = {addr, -1, NULL};
	
	if(onlyDR) { i++; return dr; }
	
	if(addr.tab[0] != -1) {
		int value = catchValue();
		if(value == -1) {
			char* str = catchStringValue(true);
			if(str != NULL) {
				setDataRangeStringValue(dr, str);
			}
		} else setDataRangeValue(dr, value);
	} else {
		error(1, "'&' operator needs at least one argument.");
	}
	
	return dr;
}

