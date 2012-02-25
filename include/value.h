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

#include <boost/any.hpp>

class Variable;

typedef enum {
	typeInt,
	typeStr,
	typeVoid
} Type;

class Value {
	public:
		Value();
		Value(Type type, boost::any value);
		Value(boost::any *value);
		Value(Variable *var);
		~Value();
		
		static void print(Value* value);
		
		template <typename T>
			T value() { return *boost::any_cast<T>(&m_value); }
		
		Type type() { return m_type; }
		
		static Value* add(Value *val, Value *val2);
		
	protected:
		Type m_type;
		boost::any m_value;
};

class IntValue : public Value {
	public:
		IntValue(int value);
		IntValue(Variable *var);
		~IntValue();
		
		int value() { return *boost::any_cast<int>(&m_value); }
		
		static IntValue* op(Value *val, int c, Value *val2);
		static IntValue* op(Value *val, int c) { int r = -val->value<int>(); return new IntValue(r); }
};

class StrValue : public Value {
	public:
		StrValue(std::string str);
		StrValue(char *str);
		StrValue(Variable *var);
		~StrValue();
		
		std::string value() { return *boost::any_cast<std::string>(&m_value); }
		
		static StrValue* op(StrValue *val, int c, StrValue *val2);
};

class NullValue : public Value {
	public:
		NullValue();
		~NullValue();
		
		std::string value() { return "(null)"; }
};

std::ostream &operator<<(std::ostream &out, IntValue *val);
std::ostream &operator<<(std::ostream &out, StrValue *str);
std::ostream &operator<<(std::ostream &out, NullValue *null);

#endif // VALUE_H
