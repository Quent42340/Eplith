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
		Value(int value);
		Value(std::string value);
		Value(char *value);
		Value(bool value);
		Value(Type type, boost::any value);
		Value(boost::any *value);
		Value(Variable *var);
		~Value() {}
		
		void print();
		
		template <typename T>
			T value() const { return *boost::any_cast<T>(&m_value); }
		
		boost::any* any() { return &m_value; }
		Type type() const { return m_type; }
		
	protected:
		Type m_type;
		boost::any m_value;
};

#endif // VALUE_H
