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
#ifndef VALUE_H
#define VALUE_H

#include <boost/any.hpp>
#include <cmath>

class Variable;

typedef enum {
	typeInt,
	typeFloat,
	typeStr,
	typeArray,
	typeVoid
} Type;

typedef enum {
	noMode,
	modeHex,
	modeSci
} Mode;

#define getNumVal(val) ((val->type() == typeInt) ? val->value<int>() : val->value<double>())
#define setNumVal(val1, val2) { if(val1->type() == typeInt) val1->value<int>(val2); else val1->value<double>(val2); }
#define valPow(val1, val2) pow((double)getNumVal(val1), (double)getNumVal(val2))
#define valNumToBool(val) ((val->any()->type() != typeid(bool)) ? (bool)getNumVal(val) : val->value<bool>())
#define isNum(val) ((val->type() == typeInt || val->type() == typeFloat) ? true : false)

class Value {
	public:
		Value();
		Value(int value, Mode mode = noMode);	// Int type
		Value(double value, Mode mode = noMode);// Float type
		Value(std::string value);				// String type
		Value(char *value);						// String type
		Value(bool value);						// Bool type
		Value(std::vector<Value*> array);		// Array
		Value(Type type, boost::any value, Mode mode = noMode);
		Value(boost::any *value, Mode mode = noMode);
		Value(Variable *var, Mode mode = noMode);
		~Value() {}
		
		void print(std::ostream &out = std::cout, Mode mode = noMode);
		
		template <typename T>
			T* valuePtr() { T *c = boost::any_cast<T>(&m_value); return (c) ? c : 0; }
		template <typename T>
			T value() { return *valuePtr<T>(); }
		template <typename T>
			T value(T value) { m_value = value; }
		
		boost::any* any() { return &m_value; }
		
		Type type() const { return m_type; }
		
		Mode mode() const { return m_mode; }
		void mode(Mode m) { m_mode = m; }
		
		bool hexMode() const { return m_mode == modeHex; }
		void hexMode(bool h) { m_mode = (h) ? modeHex : noMode; }
		
		bool sciMode() const { return m_mode == modeSci; }
		void sciMode(bool s) { m_mode = (s) ? modeSci : noMode; }
		
		Value *valIncr() { m_value = value<int>() + 1; return this; }
		Value *valDecr() { m_value = value<int>() - 1; return this; }
		
		Value *intToBool() { return (m_value.type() == typeid(int)) ? new Value((bool)value<int>()) : new Value(value<bool>()); }
		
	protected:
		Type m_type;
		boost::any m_value;
		Mode m_mode;
};

#endif // VALUE_H
