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
#include "variable.h"

using namespace std;

vector<Variable*> Variable::vars;

Variable::Variable(string name, Value *value) {
	m_name = name;
	m_value = new Value(*value);
	
	vars.push_back(this);
	
#ifdef VAR_DEBUG
	cout << "Var name: " << m_name << " | Value: "; Value::print(value); cout << " | Nb of vars: " << vars.size() - 1 << endl;
#endif
}

Variable::~Variable() {
}

Variable* Variable::findByName(std::string name) {
	for(int i = 1 ; i < vars.size() ; i++) {
		if(vars[i]->name() == name) {
			return vars[i];
		}
	}
	return vars[0];
}

void Variable::initNullVar() {
	NullValue *nullValue = new NullValue();
	Variable *nullVar = new Variable("<null>", nullValue);
}

