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
#include "value.h"

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

StrValue::StrValue(string *str) {
	m_type = typeStr;
	m_values.s = str;
}

StrValue::~StrValue() {
}

NullValue::NullValue() {
	m_type = typeVoid;
	m_values.v = NULL;
}

NullValue::~NullValue() {
}

