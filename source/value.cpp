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

Value::Value(Type type, boost::any value) {
	m_type = type;
	m_value = value;
}

Value::Value(boost::any *value) {
	m_value = *value;
	
	if(int *pi = boost::any_cast<int>(&m_value)) {
		m_type = typeInt;
	}
	else if(string *pstr = boost::any_cast<string>(&m_value)) {
		m_type = typeStr;
	} else {
		m_type = typeVoid;
	}
}

Value::Value(Variable *var) {
	m_value = var->value()->m_value;
	
	if(int *pi = boost::any_cast<int>(&m_value)) {
		m_type = typeInt;
	}
	else if(string *pstr = boost::any_cast<string>(&m_value)) {
		m_type = typeStr;
	} else {
		m_type = typeVoid;
	}
}

Value::~Value() {
}

void Value::print(Value* value) {
	if(int *pi = boost::any_cast<int>(&value->m_value)) {
		cout << *pi;
	}
	else if(string *pstr = boost::any_cast<string>(&value->m_value)) {
		cout << *pstr;
	}
	else if(value->m_type == typeVoid) {
		cout << "(null)";
	}
}

Value* Value::add(Value *val, Value *val2) {
	//cout << "Test: " << typeInt << " : " << val->m_type << " / " << val2->m_type << endl;
	if((val->m_type != typeInt) || (val2->m_type != typeInt)) {
		string tmp, tmp2;
		if(val->m_type == typeInt) {
			stringstream out;
			out << val->value<int>();
			tmp = out.str();
		} else {
			tmp = val->value<string>();
		}
		if(val2->m_type == typeInt) {
			stringstream out2;
			out2 << val2->value<int>();
			tmp2 = out2.str();
		} else {
			tmp2 = val2->value<string>();
		}
		return new Value(typeStr, tmp + tmp2);
	} else {
		return new Value(typeInt, val->value<int>() + val2->value<int>());
	}
}

IntValue::IntValue(int value) {
	m_type = typeInt;
	m_value = value;
}

IntValue::IntValue(Variable* var) {
	if(var->value()->type() != typeInt) {
		yyerror("Value type error");
	}
	
	m_type = typeInt;
	m_value = var->value()->value<int>();
}

IntValue::~IntValue() {
}

IntValue* IntValue::op(Value *val, int c, Value *val2) {
	if((val->type() != typeInt) || (val2->type() != typeInt)) yyerror("Operator not available with these type");
	int r;
	switch(c) {
		case '-': r = val->value<int>() - val2->value<int>(); break;
		case '*': r = val->value<int>() * val2->value<int>(); break;
		case '/': r = val->value<int>() / val2->value<int>(); break;
		case '^': r = pow(val->value<int>(), val2->value<int>()); break;
		case '<': r = val->value<int>() < val2->value<int>(); break;
		case '>': r = val->value<int>() > val2->value<int>(); break;
		case GE: r = val->value<int>() >= val2->value<int>(); break;
		case LE: r = val->value<int>() <= val2->value<int>(); break;
		case EQ: r = val->value<int>() == val2->value<int>(); break;
		case NE: r = val->value<int>() != val2->value<int>(); break;
	}
	return new IntValue(r); // TO IMPROVE see ~/.xchat2/xchatlogs/NETWORK-##c++.log at line 320: Just avoid passing pointers.
}

StrValue::StrValue(string str) {
	m_type = typeStr;
	m_value = str;
}

StrValue::StrValue(char *str) {
	m_type = typeStr;
	m_value = string(str);
}

StrValue::StrValue(Variable* var) {
	if(var->value()->type() != typeStr) {
		yyerror("Value type error");
	}
	
	m_type = typeStr;
	m_value = var->value()->value<string>();
}

StrValue::~StrValue() {
}

StrValue* StrValue::op(StrValue *val, int c, StrValue *val2) {
	string strr;
	switch(c) {
		case '+': strr = val->value() + val2->value(); break;
	}
	return new StrValue(strr);
}

NullValue::NullValue() {
	m_type = typeVoid;
	m_value = string("(null)");
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

