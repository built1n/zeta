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
