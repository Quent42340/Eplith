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
#ifndef VARS_H
#define VARS_H

#include "attributes.h"

typedef struct {
	char* name;
	DataRange dr;
} uInt;

typedef struct {
	char* name;
	DataRange dr;
} String;

extern std::vector<uInt> vars;

char* catchVarName();
uInt catchVar();
uInt& findVar(char* name);
String catchString();
String& findString(char* name);

#endif // VARS_H
