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
 */

#ifndef ZETA_H_
#define ZETA_H_
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint8_t byte;
typedef uint32_t word;

typedef struct zeta_regs {
  word accl, pc, sp;
} zeta_regs;
typedef struct zeta_ctx {
  byte* memory;
  word memsize, maxstacksize;
  word stacksize;
  zeta_regs regs;
  bool done;
  int return_value;
  bool debug;
} zeta_ctx;

#ifdef __cplusplus
extern "C" {
#endif
zeta_ctx* zeta_init(byte* memory, word memsize, word maxstacksize, word sp, bool debug);
void zeta_exec(zeta_ctx*);

#ifdef __cplusplus
}
#endif

#endif /* ZETA_H_ */
