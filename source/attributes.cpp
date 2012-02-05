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

char* catchString() {
	char* str = new char;
	
	if(sourceCode[i + 1] == '(') {
		i++;
		if(sourceCode[i + 1] == '"') {
			i++;
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
			
			if(sourceCode[i + 1] == ')') {
				i++;
				return str;
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

DataRange catchDataRange(bool onlyDR) {
	Attr addr = catchAttrs();
	DataRange dr = {addr, -1};
	
	if(onlyDR) {
		i++;
		return dr;
	}
	
	if(addr.tab[0] != -1) {
		setDataRangeValue(dr, catchValue());
	} else {
		error(1, "'&' operator needs at least one argument.");
	}
	
	return dr;
}

