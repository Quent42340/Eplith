/*---------------------------------------------------------------------------------

    WordSphere Intepreter
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
#include "header.h"
#include "variable.h"
#include "function.h"
#include "parser.tab.hpp"

using namespace std;

Value::Value() {
}

Value::Value(Type type, Values values) {
	m_type = type;
	m_values = values;
}

Value::~Value() {
}

void Value::print() {
	if(m_type == typeInt) {
		cout << m_values.i;
	}
	else if(m_type == typeStr) {
		cout << m_values.s;
	}
	else if(m_type == typeVoid) {
		if(m_values.v == NULL) {
			cout << "(null)";
		} else {
			cout << m_values.v;
		}
	}
}

IntValue::IntValue(int value) {
	m_type = typeInt;
	m_values.i = value;
}

IntValue::~IntValue() {
}

IntValue* IntValue::op(IntValue *val, int c, IntValue *val2) {
	int r;
	switch(c) {
		case '+': r = val->value() + val2->value(); break;
		case '-': r = val->value() - val2->value(); break;
		case '*': r = val->value() * val2->value(); break;
		case '/': r = val->value() / val2->value(); break;
		case '^': r = pow(val->value(), val2->value()); break;
		case '<': r = val->value() < val2->value(); break;
		case '>': r = val->value() > val2->value(); break;
		case GE: r = val->value() >= val2->value(); break;
		case LE: r = val->value() <= val2->value(); break;
		case EQ: r = val->value() == val2->value(); break;
		case NE: r = val->value() != val2->value(); break;
	}
	return new IntValue(r);
}

StrValue::StrValue(string *str) {
	m_type = typeStr;
	m_values.s = str;
}

StrValue::StrValue(char *str) {
	m_type = typeStr;
	m_values.s = new string(str);
}

StrValue::~StrValue() {
}

string strr;
StrValue* StrValue::op(StrValue *val, int c, StrValue *val2) {
	switch(c) {
		case '+': strr = val->value() + val2->value(); break;
	}
	return new StrValue(&strr);
}

NullValue::NullValue() {
	m_type = typeVoid;
	m_values.v = NULL;
}

NullValue::~NullValue() {
}

ostream &operator<<(ostream &out, IntValue *val) {
    out << val->value();
    return out;
}

ostream &operator<<(ostream &out, StrValue *str) {
    out << str->value();
    return out;
}

ostream &operator<<(ostream &out, NullValue *null) {
    out << null->value();
    return out;
}

