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
#ifndef EPLIB_VARIABLE_H
#define EPLIB_VARIABLE_H

#include <vector>
#include "value.h"

class Variable {
	public:
		Variable(std::string name, Value *value, bool tmp = false);
		~Variable();
		
		void print() { m_value->print(); }
		
		Value* value() { return m_value; }
		void value(Value *value);
		
		std::string name() const { return m_name; }
		std::string address() const { return m_address; }
		
		void setElement();
		
		static Variable* findByName(std::string name);
		static bool exists(std::string name);
		static bool erase(std::string name);
		
		static void initNullVar();
		
		static std::vector< std::vector<Variable*> > vars;
		
	private:
		std::string m_name;
		std::string m_address;
		Value* m_value;
		int m_id;
		int m_scope;
};

#endif // EPLIB_VARIABLE_H
