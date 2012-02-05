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
#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstddef>

#define MEM_SIZE 0xFFFF

inline bool isd(char c) { return (c >= '0' && c <= '9'); }
inline bool ish(char c) { return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || isd(c)); }

extern bool HEX, DEBUG;

extern std::vector<unsigned char> memory;

extern std::string sourceCode;

extern unsigned int i;

#endif // GLOBALVARS_H

