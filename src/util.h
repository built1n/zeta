/*
 *  Zeta emulation library
 *  Copyright (C) 2014 Franklin Wei
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  The emulator is implemented here
 */
#ifndef UTIL_H_
#define UTIL_H_
#include <zeta.h>
word getArg(zeta_ctx* ctx);
void badWrite(zeta_ctx* ctx);
void badRead(zeta_ctx* ctx);
void stackOverflow(zeta_ctx* ctx);
void stackUnderflow(zeta_ctx* ctx);
void badInstr(zeta_ctx*);

#define printInt(a) printf("%d", (a))
#define printHex_lcase(a) printf("%x", (a))
#define printHex_ucase(a) printf("%X", (a))
#define printString(a) printf("%s", (a))
#define printChar(a) putchar(a)
void printBinary(uint32_t);
#define likely(x) __builtin_expect((x),1)
#define unlikely(x) __builtin_expect((x),0)
#endif
