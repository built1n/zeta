/*
 * zeta.h
 *
 *  Created on: Mar 3, 2014
 *      Author: Franklin Wei
 */

#ifndef ZETA_H_
#define ZETA_H_
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
typedef uint8_t byte;
typedef uint32_t word;
typedef uint32_t ptr;
typedef struct zeta_regs {
  word accl, pc, sp;
} zeta_regs;
typedef struct zeta_ctx {
  byte* memory;
  word memsize, maxstacksize;
  word stacksize;
  zeta_regs regs;
  bool done;
} zeta_ctx;
#ifdef __cplusplus
extern "C" {
#endif
zeta_ctx* zeta_init(byte* memory, word memsize, word maxstacksize, word sp);
void zeta_exec(zeta_ctx* ctx);
#ifdef __cplusplus
}
#endif
#endif /* ZETA_H_ */
