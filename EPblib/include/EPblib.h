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
#ifndef EPBLIB_H
#define EPBLIB_H

#include "EPblib/EPb_base.h"
#include "EPblib/EPb_math.h"
#include "EPblib/EPb_string.h"
#include "EPblib/EPb_array.h"
#include "EPblib/EPb_io.h"
#include "EPblib/EPb_system.h"

void EPblib_init() {
	EPblib_initBase();
	EPblib_initMath();
	EPblib_initString();
	EPblib_initArray();
	EPblib_initIO();
	EPblib_initSystem();
}

#endif // EPBLIB_H
