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

template<class T>
class StringExpression : public Expression {
	public:
		StringExpression(Expression *exp, Expression *exp2 = 0) { m_exp = exp; m_exp2 = exp2; }
		~StringExpression() {}
		
		Value *evaluate() { return T::eval(m_exp, m_exp2); }
		void doExp() { T::exec(m_exp, m_exp2); }
		
	private:
		Expression *m_exp;
		Expression *m_exp2;
};

string String_reverse(string s) {
	reverse(s.begin(), s.end());
	return s;
}

string String_rep(string s, int n) {
	string r;
	for(int i = 0 ; i < n ; i++) r += s;
	return r;
}

string String_sub(string s, int i, int j = -1) {
	if(i >= 0) {
		return s.substr(i, j);
	} else {
		return s.substr(s.length() + i, j);
	}
}

EPb_initStringStruct(Len, (int)EPb_getStr(exp).length());
EPb_initStringStruct(Upper, boost::to_upper_copy(EPb_getStr(exp)));
EPb_initStringStruct(Lower, boost::to_lower_copy(EPb_getStr(exp)));
EPb_initStringStruct(Reverse, String_reverse(EPb_getStr(exp)));
EPb_initStringStruct2(Rep, String_rep(EPb_getStr(exp), EPb_getInt(exp2)));
EPb_initStringStruct2(Sub, String_sub(EPb_getStr(exp), EPb_getInt(exp2)));

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

