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

template<class T>
class ArrayFExpression : public Expression {
	public:
		ArrayFExpression(vector<VarExpression*> *args) { m_args = args; }
		~ArrayFExpression();
		
		Value *evaluate() { return T::eval(m_args); }
		void doExp() { T::exec(m_args); }
		
	private:
		vector<VarExpression*> *m_args;
};

string Array_concat(vector<VarExpression*> *args) {
	if(args->size() > 2 || args->size() < 2) yyerror("Unexpected number of arguments.");
	map<string, Value*> t = EPb_getArray((*args)[0]);
	string sep = EPb_getStr((*args)[1]);
	
	string r;
	for(map<string, Value*>::iterator it = t.begin() ; it != t.end() ; it++) {
		r += EPb_valToStr(it->second);
		if(++it != t.end()) r += sep; it--;
	}
	return r;
}

map<string, Value*> Array_insert(vector<VarExpression*> *args, int pos = -1) {
	if(args->size() > 2 || args->size() < 2) yyerror("Unexpected number of arguments.");
	map<string, Value*> *t = EPb_getArrayPtr((*args)[0]);
	if(!t) yyerror("Bad argument.");
	Value *v = EPb_getVal((*args)[1]);
	
	if(pos == -1) {
		int count = 0;
		for(map<string, Value*>::iterator it = t->begin() ; it != t->end() ; it++) {
			vector<int> r = stoi(it->first.c_str());
			if(r[1] && r[1] != EOF) count++;
		} count++;
		t->insert(t->end(), pair<string, Value*>(itos(count), v));
		return *t;
	}
}

map<string, Value*> Array_remove(vector<VarExpression*> *args, int pos = -1) {
	if(args->size() > 1 || args->size() < 1) yyerror("Unexpected number of arguments.");
	map<string, Value*> *t = EPb_getArrayPtr((*args)[0]);
	if(!t) yyerror("Bad argument.");
	
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

int Array_maxn(vector<VarExpression*> *args) {
	if(args->size() > 1 || args->size() < 1) yyerror("Unexpected number of arguments.");
	map<string, Value*> *t = EPb_getArrayPtr((*args)[0]);
	if(!t) yyerror("Bad argument.");
	
	int count = 0;
	for(map<string, Value*>::iterator it = t->begin() ; it != t->end() ; it++) {
		vector<int> r = stoi(it->first.c_str());
		if(r[1] && r[1] != EOF) count++;
	}
	return --count;
}

EPb_initArrayStruct(Concat, Array_concat,, 2, "t", "s");
EPb_initArrayStruc2(Insert,, Array_insert, 2, "t", "v");
EPb_initArrayStruct(Remove,, Array_remove, 1, "t");
EPb_initArrayStruct(Maxn, Array_maxn,, 1, "t");

void EPblib_initArray() {
	map<string, Value*> Array_elements;
	
	EPb_initElemFunc(Array_elements, Concat, concat);
	EPb_initElemFunc(Array_elements, Insert, push_back);
	EPb_initElemFunc(Array_elements, Remove, pop_back);
	EPb_initElemFunc(Array_elements, Maxn, maxn);
	
	Variable *Array = new Variable("Array", new Value(Array_elements));
}

