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

#define MEM_SIZE 255

using namespace std;

bool HEX;

vector<unsigned char> memory(MEM_SIZE, 0);

int main(int argc, char* argv[]) {
	HEX = false;
	
	ifstream file(argv[1], ifstream::in);
	
	string line;
	string sourceCode;
	
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
					unsigned int temp(0);
					char tmpChar[3] = "";
					if(sourceCode[i + 1] == '(') {
						i++;
						temp = i + 1;
						for(i++ ; sourceCode[i] != ')' ; i++) {
							tmpChar[i - temp] = sourceCode[i];
						}
						if((int)pointer <= atoi(tmpChar)) {
							if(HEX) pointer += (unsigned int)strtol(tmpChar, NULL, 16);
							else	pointer += atoi(tmpChar);
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
					unsigned int temp(0);
					char tmpChar[3] = "";
					if(sourceCode[i + 1] == '(') {
						i++;
						temp = i + 1;
						for(i++ ; sourceCode[i] != ')' ; i++) {
							tmpChar[i - temp] = sourceCode[i];
						}
						if((int)pointer >= atoi(tmpChar)) {
							if(HEX) pointer -= (unsigned int)strtol(tmpChar, NULL, 16);
							else	pointer -= atoi(tmpChar);
						}
					} else {
						pointer--;
					}
				}
				break;
				
			case '+': {
				unsigned int temp(0);
				char tmpChar[3] = "";
				if(sourceCode[i + 1] == '(') {
					i++;
					temp = i + 1;
					for(i++ ; sourceCode[i] != ')' ; i++) {
						tmpChar[i - temp] = sourceCode[i];
					}
					if(HEX) memory[pointer] += (unsigned int)strtol(tmpChar, NULL, 16);
					else	memory[pointer] += atoi(tmpChar);
				} else {
					memory[pointer]++;
				}
				break;
			}
			
			case '-': {
				unsigned int temp(0);
				char tmpChar[3] = "";
				if(sourceCode[i + 1] == '(') {
					i++;
					temp = i + 1;
					for(i++ ; sourceCode[i] != ')' ; i++) {
						tmpChar[i - temp] = sourceCode[i];
					}
					if(HEX) memory[pointer] -= (unsigned int)strtol(tmpChar, NULL, 16);
					else	memory[pointer] -= atoi(tmpChar);
				} else {
					memory[pointer]--;
				}
				break;
			}
			
			case '.':
				//cout << (int)memory[pointer] << " *** " << pointer << " *** " << i << " *** " << sourceCode[i] << " *** " << memory[pointer] << " *** " << bStack.size() << endl;
				cout << memory[pointer];
				break;
			
			case ':':
				cout << (int)memory[pointer];
				break;
			
			case '_':
				cout << endl;
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
				unsigned int temp(0);
				char tmpChar[3] = "";
				i++;
				if(sourceCode[i] == '(') {
					temp = i + 1;
					for(i++ ; sourceCode[i] != ')' ; i++) {
						tmpChar[i - temp] = sourceCode[i];
					}
					if(HEX) pointer = (unsigned int)strtol(tmpChar, NULL, 16);
					else	pointer = atoi(tmpChar);
				} else {
					cout << " *** ERROR: * operator needs one argument. *** " << endl;
				}
				break;
			}
			
			case '~':
				cout << "Pointer: " << pointer << endl;
				break;
				
			case '#':
				i++; if(sourceCode[i] == '0') {
					i++; if(sourceCode[i] == 'x') {
						HEX = true;
					} else {
						HEX = false;
					} i--;
				}
				break;
			
			default:
				break;
		}
		
		cout.flush();
	}
	
	cout << " --- END --- " << endl;
	
	return 0;
}

