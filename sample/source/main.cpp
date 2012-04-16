/*---------------------------------------------------------------------------------

    Eplith sample C++ program
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
#include <EPlib.h>

using namespace std;

int main(int argc, char* argv[]) {
	Variable::initNullVar();
	
	// New variable: 'y = 3'
	Variable *y = new Variable("y", new Value(3));
	
	// New function: 'puts = function(str) { print(str + "\n"); }'
	Expression::scopes++;
	vector<VarExpression*> putsArgs;
	putsArgs.push_back(new VarExpression("str"));
	vector<Expression*> putsStmts;
	putsStmts.push_back(new PrintExpression(new OpExpression(putsArgs[0], '+', new StrExpression("\n"))));
	Expression::scopes--;
	Variable *puts = new Variable("puts", new Value(new Function(&putsArgs, &putsStmts)));
	
	// Read file
	newFile("scripts/sample.ep");
	
	// Parse file
	yyparse();
	
	// Print variable: 'print("x = " + x + "\n")'
	cout << "x = "; Variable::findByName("x")->value()->print(); cout << endl;
	
	// Calls mputs function: 'mputs("All", "is", "OK.");'
	vector<Expression*> args;
	args.push_back(new StrExpression("All"));
	args.push_back(new StrExpression("is"));
	args.push_back(new StrExpression("OK."));
	CallExpression *call = new CallExpression("mputs", &args);
	call->doExp();
	
	delete y;
	delete puts;
	
	return 0;
}

