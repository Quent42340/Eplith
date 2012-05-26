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
#include <EPlib.h>
#include "EPb_tools.h"
#include "EPb_base.h"

using namespace std;

void *Base_print(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	Value *out = EPb_getVal((*args)[0]);
	out->print();
	return NULL;
}

void *Base_puts(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	Value *out = EPb_getVal((*args)[0]);
	out->print();
	cout << endl;
	return NULL;
}

string Base_gets(EPb_args *args) {
	EPb_checkArgsNbr(0, 0);
	char *str = new char;
	gets(str);
	return string(str);
}

string Base_type(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	Value *val = EPb_getVal((*args)[0]);
	return val->typeToStr();
}

Value *Base_toNumber(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	Value *val = EPb_getVal((*args)[0]);
	return val->toNum();
}

Value *Base_toString(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	Value *val = EPb_getVal((*args)[0]);
	return val->toStr();
}

EPb_initStruct(Print,, Base_print, 1, "out");
EPb_initStruct(Puts,, Base_puts, 1, "out");
EPb_initStruct(Gets,, Base_gets, 0, "");
EPb_initStruct(BType, Base_type,, 1, "val")
EPb_initStruct(ToNumber, Base_toNumber,, 1, "val")
EPb_initStruct(ToString, Base_toString,, 1, "val")

void EPblib_initBase() {
	EPb_initFunc(Print, print);
	EPb_initFunc(Puts, puts);
	EPb_initFunc(Gets, gets);
	EPb_initFunc(BType, type);
	EPb_initFunc(ToNumber, tonumber);
	EPb_initFunc(ToString, tostring);
	
	Variable *version = new Variable("_VERSION", new Value("0.1a"));
}

