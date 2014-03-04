#ifndef UTIL_H_
#define UTIL_H_
#include <zeta.h>
word getArg(zeta_ctx* ctx);
void badWrite(zeta_ctx* ctx);
void badRead(zeta_ctx* ctx);
void stackOverflow(zeta_ctx* ctx);
void stackUnderflow(zeta_ctx* ctx);
#endif
