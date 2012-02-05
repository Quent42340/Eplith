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
#include "operators.h"

using namespace std;

unsigned int pointer(0);

unsigned int loop(0);

char c;

char* tempString = new char;

void exec() {
	for(i = 0 ; i < sourceCode.size() ; i++) {
		switch(sourceCode[i]) {
			case '>':
				if(pointer == MEM_SIZE) {
					pointer = 0;
				} else {
					int attr = catchAttr();
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
					int attr = catchAttr();
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
				int attr = catchAttr();
				if(attr != -1) {
					memory[pointer] += attr;
				} else {
					memory[pointer]++;
				}
				break;
			}
			
			case '-': {
				int attr = catchAttr();
				if(attr != -1) {
					memory[pointer] -= attr;
				} else {
					memory[pointer]--;
				}
				break;
			}
			
			case '.': {
				char* out = catchStringValue();
				
				if(out == NULL) {
					if(sourceCode[i + 1] == '$') {
						i++;
						cout << tempString;
					}
					else if(sourceCode[i + 1] == '!') {
						i++;
						cout << endl;
					}
					else if(sourceCode[i + 1] == '(') {
						i++;
						if(sourceCode[i + 1] == '@') {
							i++;
							uInt &var = findVar(catchVarName());
							if(var.name != NULL) {
								cout << var.dr.value;
							} else {
								error(2, "Please enter a correct variable.");
							}
						}
						else if(sourceCode[i + 1] == '$') {
							i++;
							String &str = findString(catchVarName());
							if(str.name != NULL) {
								cout << str.dr.str;
							} else {
								error(2, "Please enter a correct variable.");
							}
						}
					} else {
						cout << memory[pointer];
					}
				} else {
					cout << out;
				}
				
				break;
			}
			
			case ',':
				if(sourceCode[i + 1] == '$') {
					i++;
					cin >> tempString;
				} else {
					cin >> memory[pointer];
				}
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
				int attr = catchAttr();
				if(attr != -1) {
					pointer = attr;
				} else {
					error(1, "'*' operator needs an argument.");
				}
				break;
			}
			
			// À TESTER !!!
			if(DEBUG) {
				case '~':
					cout << "Pointer: " << pointer << endl;
					break;
				
				case '_':
					cout << (int)memory[pointer] << endl;
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
						
						case '!':
							i++;
							while(1) {
								if((sourceCode[i] == '!') && (sourceCode[i + 1] == '#')) {
									break;
								} else {
									i++;
								}
							}
							break;
							
						
						default:
							i -= 2;
							a = 1;
							break;
					}
				}
				break;
			}
			
			// VARIABLES
			case '&': // Plage de données
				catchDataRange();
				break;
			
			case '@': // Entier de taille comprise entre un char et un int
				catchVar();
				break;
			
			case '$': // Équivalent d'un char*
				catchString();
				break;
				
			// FONCTIONS - À FAIRE !!!
			case '{': // Ouverture de la fonction
				break;
				
			case '}': // Fermeture de la fonction
				break;
			
			case ':': // Appel de la fonction
				break;
			
			default:
				break;
		}
		
		cout.flush();
	}

}

