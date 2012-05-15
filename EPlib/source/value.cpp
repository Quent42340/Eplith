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
#include "header.h"
#include "expression.h"
#include "function.h"
#include "parser.tab.hpp"

using namespace std;

Value::Value() {
	m_type = typeVoid;
	m_value = string("(null)");
}

Value::Value(int value, Mode mode) {
	m_type = typeInt;
	m_value = value;
	m_mode = mode;
}

Value::Value(double value, Mode mode) {
	m_type = typeFloat;
	m_value = value;
	m_mode = mode;
}

Value::Value(string value) {
	m_type = typeStr;
	m_value = value;
}

Value::Value(char *value) {
	m_type = typeStr;
	m_value = string(value);
}

Value::Value(bool value) {
	m_type = typeInt;
	m_value = value;
}

Value::Value(map<string, Value*> array) {
	m_type = typeArray;
	m_value = array;
}

Value::Value(Function *func) {
	m_type = typeFunc;
	m_value = func;
}

Value::Value(Type type, boost::any value, Mode mode) {
	m_type = type;
	m_value = value;
	m_mode = mode;
}

Value::Value(boost::any *value, Mode mode) {
	m_value = *value;
	
	if(int *pi = valuePtr<int>()) {
		m_type = typeInt;
	}
	else if(double *pf = valuePtr<double>()) {
		m_type = typeFloat;
	}
	else if(string *pstr = valuePtr<string>()) {
		m_type = typeStr;
	}
	else if(bool *pb = valuePtr<bool>()) {
		m_type = typeInt;
	}
	else if(map<string, Value*> *pa = valuePtr< map<string, Value*> >()) {
		m_type = typeArray;
	}
	else if(Function **func = valuePtr<Function*>()) {
		m_type = typeFunc;
	} else {
		m_type = typeVoid;
	}
	
	m_mode = mode;
}

Value::Value(Variable *var, Mode mode) {
	m_value = var->value()->m_value;
	
	if(int *pi = valuePtr<int>()) {
		m_type = typeInt;
	}
	else if(double *pf = valuePtr<double>()) {
		m_type = typeInt;
	}
	else if(string *pstr = valuePtr<string>()) {
		m_type = typeStr;
	}
	else if(bool *pb = valuePtr<bool>()) {
		m_type = typeInt;
	}
	else if(map<string, Value*> *pa = valuePtr< map<string, Value*> >()) {
		m_type = typeArray;
	}
	else if(Function **func = valuePtr<Function*>()) {
		m_type = typeFunc;
	} else {
		m_type = typeVoid;
	}
	
	m_mode = mode;
}

void Value::print(ostream &out, Mode mode) {
	if(mode == noMode) mode = m_mode;
	if(int *pi = valuePtr<int>()) {
		if(mode == modeHex) out << "0x" << hex;
		out << *pi;
		if(mode == modeHex) out << dec;
	}
	else if(double *pf = valuePtr<double>()) {
		if(mode == modeSci) out << scientific;
		out << *pf;
	}
	else if(string *pstr = valuePtr<string>()) {
		out << *pstr;
	}
	else if(bool *pb = valuePtr<bool>()) {
		out << ((*pb) ? "true" : "false");
	}
	else if(map<string, Value*> *pa = valuePtr< map<string, Value*> >()) {
		out << "{";
		for(map<string, Value*>::iterator it = pa->begin() ; it != pa->end() ; it++) {
			if(atoi(it->first.c_str()) == 0 && it->first != itos(0)) out << it->first << " = ";
			if((*pa)[it->first]->type() == typeStr) out << "\"";
			(*pa)[it->first]->print(out);
			if((*pa)[it->first]->type() == typeStr) out << "\"";
			if(++it != pa->end()) out << ", "; it--;
		}
		out << "}";
	}
	else if(Function **func = valuePtr<Function*>()) {
		out << getPtrAddr((void*)this);
	}
}

Value *Value::element(vector<string> indexTable) {
	if(indexTable.size() > 0) {
		bool colon = false;
		string i = indexTable[indexTable.size() - 1];
		indexTable.pop_back();
		if(i[0] == '*') {
			i.erase(i.begin());
			colon = true;
		}
		Value *v = value< map<string, Value*> >()[i]->element(indexTable);
		if(colon) {
			if(v->type() == typeFunc) {
				v->value<Function*>()->colon(true);
				v->value<Function*>()->mainElement(this);
			} else {
				yyerror("Operator ':' only available with functions");
			}
		}
		cdbg(":" << v->value<Function*>()->args());
		if(v) return v;
		else yyerror("Bad element access");
	} else {
		return this;
	}
}

void Value::copy(Value *value) {
	m_type = value->m_type;
	m_value = value->m_value;
	m_mode = value->m_mode;
}

void mapInsert(map<string, Value*> *m, string index, Value *value) {
	m->insert(m->end(), pair<string, Value*>(index, value));
}

void Value::element(ElementExpression *element, Value *newValue) {
	if(element->index()->size() > 0) {
		string i = (*element->index())[element->index()->size() - 1];
		element->index()->pop_back();
		if(value< map<string, Value*> >()[i] == 0) {
			mapInsert(valuePtr< map<string, Value*> >(), i, new Value());
		}
		value< map<string, Value*> >()[i]->element(element, newValue);
	} else {
		copy(newValue);
	}
}

string Value::typeToStr() {
	switch(m_type) {
		case typeInt:
			if(int *pi = valuePtr<int>()) {
				return "integer";
			} else {
				return "boolean";
			}
		case typeFloat:
			return "float";
		case typeStr:
			return "string";
		case typeArray:
			return "array";
		case typeFunc:
			return "function";
		case typeVoid:
			return "void";
		default:
			yyerror("Unexpected error");
	}
}

bool Value::toBool() {
	switch(m_type) {
		case typeInt:
			if(int *pi = valuePtr<int>()) {
				return (bool)*pi;
			} else {
				return value<bool>();
			}
		case typeFloat:
			return (bool)value<double>();
		case typeStr:
			return true;
		case typeArray:
			return true;
		case typeFunc:
			return value<Function*>()->ret()->toBool();
		case typeVoid:
			return false;
		default:
			yyerror("Unexpected error");
	}
}

