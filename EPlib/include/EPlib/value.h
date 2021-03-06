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
#ifndef EPLIB_VALUE_H
#define EPLIB_VALUE_H

#include <boost/any.hpp>
#include <cmath>

class Variable;
class Function;
class ElementExpression;

typedef enum {
	typeInt,
	typeFloat,
	typeStr,
	typeArray,
	typeFunc,
	typeFile,
	typeVoid
} Type;

typedef enum {
	noMode,
	modeHex,
	modeSci
} Mode;

typedef struct {
	FILE *f;
	const char *fname;
} File;

#define getNumVal(val) ((val->type() == typeInt) ? val->value<int>() : val->value<double>())
#define setNumVal(val1, val2) { if(val1->type() == typeInt) val1->value<int>(val2); else val1->value<double>(val2); }
#define valPow(val1, val2) pow((double)getNumVal(val1), (double)getNumVal(val2))
#define valNumToBool(val) ((val->any()->type() != typeid(bool)) ? (bool)getNumVal(val) : val->value<bool>())
#define isNum(val) ((val->type() == typeInt || val->type() == typeFloat) ? true : false)
#define isIndex(val) ((val->type() == typeInt || val->type() == typeStr) ? true : false)
#define getIndexVal(val) ((val->type() == typeInt) ? itos(val->value<int>()) : val->value<std::string>())
#define valToStr(val) ((isIndex(val) ? ((val->type() == typeInt) ? new std::string(itos(val->value<int>())) : new string(val->value<std::string>())) : 0))

class Value;

template<typename T> const std::string GetTypeName();
#define DEFINE_TYPE_NAME(type, name) template<>const std::string GetTypeName<type>() { return std::string(name); }
#define _W(...) __VA_ARGS__

template<>const std::string GetTypeName<void>();
template<>const std::string GetTypeName<void*>();
template<>const std::string GetTypeName<int>();
template<>const std::string GetTypeName<double>();
template<>const std::string GetTypeName<std::string>();
template<>const std::string GetTypeName<bool>();
template<>const std::string GetTypeName<std::map<std::string, Value*> >();
template<>const std::string GetTypeName<Function*>();
template<>const std::string GetTypeName<File*>();

class Value {
	public:
		Value();									// Null
		Value(void*);								// Null
		Value(int value, Mode mode = noMode);		// Int type
		Value(double value, Mode mode = noMode);	// Float type
		Value(std::string value);					// String type
		Value(char *value);							// String type
		Value(bool value);							// Bool type
		Value(std::map<std::string, Value*> array);	// Array
		Value(Function *func);						// Function
		Value(File *file);							// File
		Value(Type type, boost::any value, Mode mode = noMode);
		Value(boost::any *value, Mode mode = noMode);
		Value(Variable *var, Mode mode = noMode);
		~Value() {}

		void print(std::ostream &out = std::cout, Mode mode = noMode);

		template <typename T>
			T* valuePtr() { T *c = boost::any_cast<T>(&m_value); return (c) ? c : 0; }
		template <typename T>
			T value() { T *c = valuePtr<T>(); if(c) return *c; else yyerror(std::string("Value is of type '") + typeToStr() + "' but a '" + GetTypeName<T>() + "' is required"); }
		template <typename T>
			T value(T value) { m_value = value; }

		Value *element(std::vector<std::string> indexTable);
		void element(ElementExpression *element, Value *newValue);

		void copy(Value *value);

		boost::any *any() { return &m_value; }

		Type type() const { return m_type; }

		Mode mode() const { return m_mode; }
		void mode(Mode m) { m_mode = m; }

		bool hexMode() const { return m_mode == modeHex; }
		void hexMode(bool h) { m_mode = (h) ? modeHex : noMode; }

		bool sciMode() const { return m_mode == modeSci; }
		void sciMode(bool s) { m_mode = (s) ? modeSci : noMode; }

		Value *valIncr() { m_value = value<int>() + 1; return this; }
		Value *valDecr() { m_value = value<int>() - 1; return this; }

		bool toBool();

		std::string typeToStr();

		Value *toNum() {
			if(m_type != typeStr && !isNum(this)) yyerror(std::string("tonumber() function not available with type '") + typeToStr() + "'");
			if(m_type == typeStr) {
				std::vector<int> r = stoi(value<std::string>().c_str());
				if(!r[1] || r[1] == EOF) return new Value();
				else return new Value((int)r[0]);
			} else {
				return new Value(getNumVal(this));
			}
		}

		Value *toStr() {
			std::stringstream out;
			print(out, m_mode);
			out << std::ends;
			return new Value(out.str());
		}

	protected:
		Type m_type;
		boost::any m_value;
		Mode m_mode;
};

#endif // EPLIB_VALUE_H
