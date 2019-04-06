/*---------------------------------------------------------------------------------

    Eplith Intepreter
    Copyright (C) 2012 Quent42340 <quent42340@gmail.com>

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
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <EPlib.h>
#include "EPb_tools.h"
#include "EPb_string.h"

using namespace std;

int String_len(EPb_args *args) {
	string s = EPb_getStr((*args)[0]);
	return (int)s.length();
}

string String_upper(EPb_args *args) {
	string s = EPb_getStr((*args)[0]);
	return boost::to_upper_copy(s);
}

string String_lower(EPb_args *args) {
	string s = EPb_getStr((*args)[0]);
	return boost::to_lower_copy(s);
}

string String_reverse(EPb_args *args) {
	string s = EPb_getStr((*args)[0]);
	reverse(s.begin(), s.end());
	return s;
}

string String_rep(EPb_args *args) {
	string s = EPb_getStr((*args)[0]);
	int n = EPb_getInt((*args)[1]);
	string r;
	for(int i = 0 ; i < n ; i++) r += s;
	return r;
}

string String_sub(EPb_args *args) {
	string s = EPb_getStr((*args)[0]);
	int i = EPb_getInt((*args)[1]);
	int j;

	if(EPb_argsNbr(args) == 3) {
		j = EPb_getInt((*args)[2]);
	} else {
		j = -1;
	}

	if(i >= 0) {
		return s.substr(i, j);
	} else {
		return s.substr(s.length() + i, j);
	}
}

EPb_initStruct(Len, String_len,, 1, "s");
EPb_initStruct(Upper, String_upper,, 1, "s");
EPb_initStruct(Lower, String_lower,, 1, "s");
EPb_initStruct(Reverse, String_reverse,, 1, "s");
EPb_initStruct(Rep, String_rep,, 2, "s", "n");
EPb_initStruct(Sub, String_sub,, 3, "s", "i", "j=-1");

void EPblib_initString() {
	map<string, Value*> String_elements;

	EPb_initElemFunc(String_elements, Len, len);
	EPb_initElemFunc(String_elements, Upper, upper);
	EPb_initElemFunc(String_elements, Lower, lower);
	EPb_initElemFunc(String_elements, Reverse, reverse);
	EPb_initElemFunc(String_elements, Rep, rep);
	EPb_initElemFunc(String_elements, Sub, sub);

	Variable *String = new Variable("String", new Value(String_elements));
}

