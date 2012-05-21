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
		ArrayFExpression(Expression *exp = 0, Expression *exp2 = 0) { m_exp = exp; m_exp2 = exp2; }
		~ArrayFExpression();
		
		Value *evaluate() { return T::eval(m_exp, m_exp2); }
		void doExp() { T::exec(m_exp, m_exp2); }
		
	private:
		Expression *m_exp;
		Expression *m_exp2;
};

string Array_concat(map<string, Value*> t, string sep) {
	string r;
	for(map<string, Value*>::iterator it = t.begin() ; it != t.end() ; it++) {
		r += EPb_valToStr(it->second);
		if(++it != t.end()) r += sep; it--;
	}
	return r;
}

map<string, Value*> Array_insert(map<string, Value*> *t, Value *v, int pos = -1) {
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

map<string, Value*> Array_remove(map<string, Value*> *t, int pos = -1) {
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

int Array_maxn(map<string, Value*> *t) {
	int count = 0;
	for(map<string, Value*>::iterator it = t->begin() ; it != t->end() ; it++) {
		vector<int> r = stoi(it->first.c_str());
		if(r[1] && r[1] != EOF) count++;
	}
	return count;
}

EPb_initArrayStruct2(Concat, Array_concat(EPb_getArray(exp), EPb_getStr(exp2)), "array", "string");
EPb_initArrayStructA2(Insert, Array_insert(EPb_getArrayPtr(exp), exp2->evaluate()), "array", "");
EPb_initArrayStructA(Remove, Array_remove(EPb_getArrayPtr(exp)), "array");
EPb_initArrayStruct(Maxn, Array_maxn(EPb_getArrayPtr(exp)), "array");

void EPblib_initArray() {
	map<string, Value*> Array_elements;
	
	EPb_initElemFunc(Array_elements, Concat, concat);
	EPb_initElemFunc(Array_elements, Insert, push_back);
	EPb_initElemFunc(Array_elements, Remove, pop_back);
	EPb_initElemFunc(Array_elements, Maxn, maxn);
	
	Variable *Array = new Variable("Array", new Value(Array_elements));
}

