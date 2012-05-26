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
#include <EPlib.h>
#include "EPb_tools.h"
#include "EPb_math.h"

#undef PI
#define PI (3.14159265358979323846)
#define RADIANS_PER_DEGREE (PI/180.0)

using namespace std;

double Math_cos(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	double x = EPb_getNum((*args)[0]);
	return cos(x);
}

double Math_sin(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	double x = EPb_getNum((*args)[0]);
	return sin(x);
}

double Math_tan(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	double x = EPb_getNum((*args)[0]);
	return tan(x);
}

double Math_acos(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	double x = EPb_getNum((*args)[0]);
	return acos(x);
}

double Math_asin(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	double x = EPb_getNum((*args)[0]);
	return asin(x);
}

double Math_atan(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	double x = EPb_getNum((*args)[0]);
	return atan(x);
}

double Math_atan2(EPb_args *args) {
	EPb_checkArgsNbr(2, 2);
	double y = EPb_getNum((*args)[0]);
	double x = EPb_getNum((*args)[1]);
	return atan2(y, x);
}

double Math_cosh(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	double x = EPb_getNum((*args)[0]);
	return cosh(x);
}

double Math_sinh(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	double x = EPb_getNum((*args)[0]);
	return sinh(x);
}

double Math_tanh(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	double x = EPb_getNum((*args)[0]);
	return tanh(x);
}

double Math_sqrt(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	double x = EPb_getNum((*args)[0]);
	return sqrt(x);
}

double Math_exp(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	double x = EPb_getNum((*args)[0]);
	return exp(x);
}

double Math_ldexp(EPb_args *args) {
	EPb_checkArgsNbr(2, 2);
	double x = EPb_getNum((*args)[0]);
	int exp = EPb_getInt((*args)[1]);
	return ldexp(x, exp);
}

double Math_log(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	double x = EPb_getNum((*args)[0]);
	return log(x);
}

double Math_log10(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	double x = EPb_getNum((*args)[0]);
	return log10(x);
}

double Math_ceil(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	double x = EPb_getNum((*args)[0]);
	return ceil(x);
}

double Math_floor(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	double x = EPb_getNum((*args)[0]);
	return floor(x);
}

double Math_fabs(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	double x = EPb_getNum((*args)[0]);
	return fabs(x);
}

double Math_fmod(EPb_args *args) {
	EPb_checkArgsNbr(2, 2);
	double numerator = EPb_getNum((*args)[0]);
	double denominator = EPb_getNum((*args)[1]);
	return fmod(numerator, denominator);
}

double Math_deg(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	double x = EPb_getNum((*args)[0]);
	return x / RADIANS_PER_DEGREE;
}

double Math_rad(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	double x = EPb_getNum((*args)[0]);
	return x * RADIANS_PER_DEGREE;
}

double Math_rand(EPb_args *args) {
	EPb_checkArgsNbr(0, 0);
	return rand();
}

double Math_srand(EPb_args *args) {
	EPb_checkArgsNbr(1, 1);
	double x = EPb_getNum((*args)[0]);
	srand(x); rand();
	return x;
}

EPb_initStruct(Cos, Math_cos,, 1, "x");
EPb_initStruct(Sin, Math_sin,, 1, "x");
EPb_initStruct(Tan, Math_tan,, 1, "x");
EPb_initStruct(aCos, Math_acos,, 1, "x");
EPb_initStruct(aSin, Math_asin,, 1, "x");
EPb_initStruct(aTan, Math_atan,, 1, "x");
EPb_initStruct(aTan2, Math_atan2,, 2, "y", "x");
EPb_initStruct(Cosh, Math_cosh,, 1, "x");
EPb_initStruct(Sinh, Math_sinh,, 1, "x");
EPb_initStruct(Tanh, Math_tanh,, 1, "x");
EPb_initStruct(Sqrt, Math_sqrt,, 1, "x");
EPb_initStruct(Exp, Math_exp,, 1, "x");
EPb_initStruct(Ldexp, Math_ldexp,, 2, "x", "exp");
EPb_initStruct(Log, Math_log,, 1, "x");
EPb_initStruct(Log10, Math_log10,, 1, "x");
EPb_initStruct(Ceil, Math_ceil,, 1, "x");
EPb_initStruct(Floor, Math_floor,, 1, "x");
EPb_initStruct(Abs, Math_fabs,, 1, "x");
EPb_initStruct(Fmod, Math_fmod,, 2, "numerator", "denominator");
EPb_initStruct(Deg, Math_deg,, 1, "x");
EPb_initStruct(Rad, Math_rad,, 1, "x");
EPb_initStruct(Rand, Math_rand,, 0, "");
EPb_initStruct(Srand, Math_srand,, 1, "x");

void EPblib_initMath() {
	map<string, Value*> Math_elements;
	
	EPb_initElemFunc(Math_elements, Cos, cos);
	EPb_initElemFunc(Math_elements, Sin, sin);
	EPb_initElemFunc(Math_elements, Tan, tan);
	EPb_initElemFunc(Math_elements, aCos, acos);
	EPb_initElemFunc(Math_elements, aSin, asin);
	EPb_initElemFunc(Math_elements, aTan, atan);
	EPb_initElemFunc(Math_elements, aTan2, atan2);
	EPb_initElemFunc(Math_elements, Cosh, cosh);
	EPb_initElemFunc(Math_elements, Sinh, sinh);
	EPb_initElemFunc(Math_elements, Tanh, tanh);
	EPb_initElemFunc(Math_elements, Sqrt, sqrt);
	EPb_initElemFunc(Math_elements, Exp, exp);
	EPb_initElemFunc(Math_elements, Ldexp, ldexp);
	EPb_initElemFunc(Math_elements, Log, log);
	EPb_initElemFunc(Math_elements, Log10, log10);
	EPb_initElemFunc(Math_elements, Ceil, ceil);
	EPb_initElemFunc(Math_elements, Floor, floor);
	EPb_initElemFunc(Math_elements, Abs, abs);
	EPb_initElemFunc(Math_elements, Fmod, fmod);
	EPb_initElemFunc(Math_elements, Deg, deg);
	EPb_initElemFunc(Math_elements, Rad, rad);
	EPb_initElemFunc(Math_elements, Rand, rand);
	EPb_initElemFunc(Math_elements, Srand, srand);
	
	Math_elements.insert(Math_elements.end(), pair<string, Value*>("pi", new Value(PI)));
	
	Variable *Math = new Variable("Math", new Value(Math_elements));
}

