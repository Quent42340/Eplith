//----------------------------------
//
// BFI - BrainFuck Interpretor
// by Quent42340
// https://gist.github.com/1622410
// https://github.com/FabianM/Brainfuck/blob/master/src/interpreter.c
//
//----------------------------------
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>

#define MEM_SIZE 0xFFFF

using namespace std;

bool HEX, DEBUG;

vector<unsigned char> memory(MEM_SIZE, 0);

bool isd(char c) { return (c >= '0' && c <= '9'); }
bool ish(char c) { return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || isd(c)); }

string sourceCode;

void error(unsigned int i, int code, string error) {
	string type;
	switch(code) {
		case 1:
			type = "SYNTAX ERROR";
			break;
		
		case 2:
			type = "FATAL ERROR";
			break;
		
		case 3:
			type = "MEMORY ERROR";
			break;
		
		default:
			type = "ERROR";
			break;
	}
	cerr << " *** " << type << ": " << error << " *** " << i << " *** " << endl;
	exit(code);
}

int catchAttr(unsigned int &i) {
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

typedef struct {
	int tab[2];
	bool sized;
	int length;
} Attr;

Attr catchAttrs(unsigned int &i) {
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
			length = attr.tab[1] - attr.tab[0];
		} else {
			if(HEX) length = (unsigned int)strtol(tmpChar2, NULL, 16);
			else	length = atoi(tmpChar2);
		}
	}
	return attr;
}

int catchValue(unsigned int &i) {
	int value = -1;
	
	while(sourceCode[i] != '=') {
		if(sourceCode[i] == ';') {
			cerr << " *** SYNTAX ERROR: Needs an '=' operator. *** " << i << " *** " << endl;
			exit(1);
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

typedef struct {
	Attr addr;
	int value;
} DatArea;

DatArea getDatArea(unsigned int &i) {
	Attr addr = catchAttrs(i);
	DatArea da = {addr, -1};
	
	if(addr.tab[0] != -1) {
		da.value = catchValue(i);
		
		if(da.value == -1) {
			error(i, 2, "A value is required after '=' operator.");
		} else {
			int vSize = (int)ceil((float)da.value / 255);
			
			if(vSize < da.addr.length) {
				for(int j = da.addr.tab[0] ; j < da.addr.tab[0] + vSize ; j--) {
					if((da.value - 255) > 0) {
						da.value -= 255;
						memory[j] = 255;
					} else {
						memory[j] = da.value;
					}
				}
			} else {
				error(i, 3, "Data area too little for the value.");
			}
		}
	} else {
		error(i, 1, "'&' operator needs at least one argument.");
	}
	
	return da;
}

typedef struct {
	char* name;
	DatArea da;
} uInt;

char* catchVarName(unsigned int &i) {
	char* name = new char;
	
	if(sourceCode[i + 1] == '(') {
		i++;
		unsigned int temp = i + 1;
		
		for(i++ ; sourceCode[i] != ')' ; i++) {
			name[i - temp] = sourceCode[i];
		}
	}
	
	return name;
}

uInt* vars;

/* Tester make pour voir si ça compile ou pas.
 * Faire la fonction catchVar() avec var = value, var = addr et var = addr = value
 * Faire les tableau des variables
 * Faire la recherche de mémoire pour les variables
 * Mettre des warnings pour des zones mémoires non-associées à des variables mais != 0
 * Mettre des erreurs pour des zones mémoires réservées à des variables
 * /

int main(int argc, char* argv[]) {
	HEX = false;
	DEBUG = false;
	
	ifstream file(argv[1], ifstream::in);
	
	string line;
	
	while(getline(file, line)) {
		sourceCode += line;
	}
	
	unsigned int pointer(0);
	unsigned int loop(0);
	char c;
	
	cout << " --- START --- " << endl;
	
	for(unsigned int i = 0 ; i < sourceCode.size() ; i++) {
		switch(sourceCode[i]) {
			case '>':
				if(pointer == MEM_SIZE) {
					pointer = 0;
				} else {
					int attr = catchAttr(i);
					if(attr != -1) {
						if((int)pointer <= attr) {
							pointer += attr;
						}
					} else {
						pointer++;
					}
				}
				break;
				
			case '<':
				if(pointer == 0) {
					pointer = MEM_SIZE;
					break;
				} else {
					int attr = catchAttr(i);
					if(attr != -1) {
						if((int)pointer >= attr) {
							pointer -= attr;
						}
					} else {
						pointer--;
					}
				}
				break;
				
			case '+': {
				int attr = catchAttr(i);
				if(attr != -1) {
					memory[pointer] += attr;
				} else {
					memory[pointer]++;
				}
				break;
			}
			
			case '-': {
				int attr = catchAttr(i);
				if(attr != -1) {
					memory[pointer] -= attr;
				} else {
					memory[pointer]--;
				}
				break;
			}
			
			case '.':
				if(DEBUG) cout << (int)memory[pointer] << " *** " << pointer << " *** " << i << " *** " << sourceCode[i] << " *** " << memory[pointer] << endl;
				else	  cout << memory[pointer];
				break;
			
			case ',':
				cin >> memory[pointer];
				break;
			
			case '[':
				if(!memory[pointer]) {
					loop = 1;
					while (loop > 0) {
						c = sourceCode[++i];
						if (c == '[') {
							loop++;
						}
						else if (c == ']') {
							loop--;
						}
					}
				}
				break;
				
			case ']':
				loop = 1;
				while (loop > 0) {
					c = sourceCode[--i];
					if (c == '[') {
						loop--;
					}
					else if (c == ']') {
						loop++;
					}
				}
				i--;
				break;
			
			case '*': {
				int attr = catchAttr(i);
				if(attr != -1) {
					pointer = attr;
				} else {
					error(i, 1, "'*' operator needs an argument.")
				}
				break;
			}
			
			// À TESTER !!!
			if(DEBUG) {
				case '~':
					cout << "Pointer: " << pointer << endl;
					break;
				
				case ':':
					cout << (int)memory[pointer];
					break;
				
				case '_':
					cout << endl;
					break;
			}
			
			case '#': {
				int a = 0;
				for(i++ ; a == 0 ; i++) {
					switch(sourceCode[i]) {
						case 'H':
							HEX = true;
							break;
						
						case 'D':
							DEBUG = true;
							break;
						
						default:
							i--;
							a = 1;
							break;
					}
				}
				break;
			}
			
			// VARIABLES
			case '&': // Plage de données
				getDatArea(i);
				break;
			
			case '@': // Entier de taille comprise entre un char et un int
				break;
			
			case '$': // Équivalent d'un char*
				break;
				
			// FONCTIONS - À FAIRE !!!
			case '{': // Ouverture de la fonction
				break;
				
			case '}': // Fermeture de la fonction
				break;
			
			case '?': // Appel de la fonction
				break;
			
			default:
				break;
		}
		
		cout.flush();
	}
	
	cout << " --- END --- " << endl;
	
	return 0;
}

