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
		Value(int value, bool hexMode = false);	// Int type
		Value(std::string value);		// String type
		Value(char *value);				// String type
		Value(bool value);				// Bool type
		Value(Type type, boost::any value, bool hexMode = false);
		Value(boost::any *value, bool hexMode = false);
		Value(Variable *var, bool hexMode = false);
		~Value() {}
		
		void print();
		
		template <typename T>
			T value() const { const T *c = boost::any_cast<T>(&m_value); if(c) return *c; }
		
		template <typename T>
			T value(T value) { m_value = value; }
		
		boost::any* any() { return &m_value; }
		
		Type type() const { return m_type; }
		
		bool hexMode() const { return m_hexMode; }
		void hexMode(bool h) { m_hexMode = h; }
		
		Value *valIncr() { m_value = value<int>() + 1; return this; }
		Value *valDecr() { m_value = value<int>() - 1; return this; }
		
	protected:
		Type m_type;
		boost::any m_value;
		bool m_hexMode;
};

#endif // VALUE_H
