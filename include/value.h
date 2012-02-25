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
#ifndef VALUE_H
#define VALUE_H

typedef enum {
	typeInt,
	typeStr,
	typeVoid
} Type;

typedef union {
	int i;
	std::string* s;
	void* v;
} Values;

class Value {
	public:
		Value();
		Value(Type type, Values m_values);
		~Value();
		
		void print();
		
	protected:
		Type m_type;
		Values m_values;
};

class IntValue : public Value {
	public:
		IntValue(int value);
		~IntValue();
		
		int value() { return m_values.i; }
		
		static IntValue* op(IntValue *val, int c, IntValue *val2);
		static IntValue* op(IntValue *val, int c) { int r = -val->value(); return new IntValue(r); }
};

class StrValue : public Value {
	public:
		StrValue(std::string *str);
		StrValue(char *str);
		~StrValue();
		
		std::string value() { return *m_values.s; }
		
		static StrValue* op(StrValue *val, int c, StrValue *val2);
};

class NullValue : public Value {
	public:
		NullValue();
		~NullValue();
		
		void* value() { return m_values.v; }
};

std::ostream &operator<<(std::ostream &out, IntValue *val);
std::ostream &operator<<(std::ostream &out, StrValue *str);

#endif // VALUE_H
