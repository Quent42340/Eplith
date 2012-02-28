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
#ifndef VARIABLE_H
#define VARIABLE_H

#include <vector>
#include "value.h"

class Variable {
	public:
		Variable(std::string name, Value *value);
		~Variable();
		
		void print() { Value::print(m_value); }
		
		Value* value() { return m_value; }
		std::string name() const { return m_name; }
		
		static Variable* findByName(std::string name);
		
		static void initNullVar();
		
		static std::vector<Variable*> vars;
		
	private:
		std::string m_name;
		Value* m_value;
};

#endif // VARIABLE_H
