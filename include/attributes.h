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
#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

typedef struct {
	int tab[2];
	bool sized;
	bool alone;
	int length;
} Attr;

typedef struct {
	Attr addr;
	int value;
} DataRange;

extern std::vector<unsigned char> mem;

int catchAttr();
Attr catchAttrs();
int catchValue(bool onlyValue = false);
char* catchString();
void setDataRangeValue(DataRange &dr, int value, bool noErrors = false);
int getDataRangeValue(DataRange &dr);
DataRange catchDataRange(bool onlyDR = false);

#endif // ATTRIBUTES_H
