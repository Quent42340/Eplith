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
#include "EPb_array.h"

using namespace std;

string Array_concat(EPb_args *args) {
	map<string, Value*> t = EPb_getArray((*args)[0]);
	string sep = EPb_getStr((*args)[1]);
	
	string r;
	for(map<string, Value*>::iterator it = t.begin() ; it != t.end() ; it++) {
		r += EPb_valToStr(it->second);
		if(++it != t.end()) r += sep; it--;
	}
	return r;
}

map<string, Value*> Array_insert(EPb_args *args) {
	map<string, Value*> *t = EPb_getArrayPtr((*args)[0]);
	if(!t) yyerror("Bad argument.");
	int pos; Value *v;
	
	cdbg(EPb_argsNbr(args));
	if(EPb_argsNbr(args) == 2) {
		v = EPb_getVal((*args)[1]);
		pos = -1;
	} else {
		pos = EPb_getInt((*args)[1]);
		v = EPb_getVal((*args)[2]);
		cdbg(v->type());
	}
	
	int count = 0;
	for(map<string, Value*>::iterator it = t->begin() ; it != t->end() ; it++) {
		if((count - 1) == pos && pos != -1) break;
		vector<int> r = stoi(it->first.c_str());
		if(r[1] && r[1] != EOF) count++;
	} count++;
	cdbg2("Look: " << count << " - ", v->print(), "");
	t->insert(t->end(), pair<string, Value*>(itos(count), v));
	return *t;
}

map<string, Value*> Array_remove(EPb_args *args) {
	map<string, Value*> *t = EPb_getArrayPtr((*args)[0]);
	if(!t) yyerror("Bad argument.");
	int pos = -1;
	
	if(pos == -1) {
		int count = 0;
		for(map<string, Value*>::iterator it = t->begin() ; it != t->end() ; it++) {
			vector<int> r = stoi(it->first.c_str());
			if(r[1] && r[1] != EOF) count++;
		}
		if(!t->erase(itos(count))) yyerror("Trying to clean a void array");
		return *t;
	}
}

int Array_maxn(EPb_args *args) {
	map<string, Value*> *t = EPb_getArrayPtr((*args)[0]);
	if(!t) yyerror("Bad argument.");
	
	int count = 0;
	for(map<string, Value*>::iterator it = t->begin() ; it != t->end() ; it++) {
		vector<int> r = stoi(it->first.c_str());
		if(r[1] && r[1] != EOF) count++;
	}
	return --count;
}

EPb_initStruct(Concat, Array_concat,, 2, "t", "s");
EPb_initStruct(Insert,, Array_insert, 3, "t", "a", "b=-1");
EPb_initStruct(Remove,, Array_remove, 1, "t");
EPb_initStruct(Maxn, Array_maxn,, 1, "t");

void EPblib_initArray() {
	map<string, Value*> Array_elements;
	
	EPb_initElemFunc(Array_elements, Concat, concat);
	EPb_initElemFunc(Array_elements, Insert, insert);
	EPb_initElemFunc(Array_elements, Remove, pop_back);
	EPb_initElemFunc(Array_elements, Maxn, maxn);
	
	Variable *Array = new Variable("Array", new Value(Array_elements));
}

