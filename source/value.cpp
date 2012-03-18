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

Value::Value(vector<Value*> array) {
	m_type = typeArray;
	m_value = array;
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
	else if(vector<Value*> *pa = valuePtr< vector<Value*> >()) {
		m_type = typeArray;
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
	else if(vector<Value*> *pa = valuePtr< vector<Value*> >()) {
		m_type = typeArray;
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
	else if(vector<Value*> *pa = valuePtr< vector<Value*> >()) {
		for(unsigned int i = 0 ; i < pa->size() ; i++) {
			(*pa)[i]->print(out);
		}
	}
	else if(m_type == typeVoid) {
		out << "(null)";
	}
}

