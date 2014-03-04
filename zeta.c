/*
 * zeta.c
 * Utility functions, the language implementation is in exec.c
 *  Created on: Mar 3, 2014
 *      Author: franklin
 */
#include <zeta.h>
#include <util.h>
#include <exec.h>
#include <stdio.h>
#include <stdlib.h>
void badWrite(zeta_ctx* ctx)
{
  printf("Bad memory write!\n");
}
void badRead(zeta_ctx* ctx)
{
  printf("Bad memory read!\n");
}
void stackOverflow(zeta_ctx* ctx)
{
  printf("Stack overflow!\n");
}
void stackUnderflow(zeta_ctx* ctx)
{
  printf("Stack underflow!\n");
}
zeta_ctx* zeta_init(byte* memory, word memsize, word maxstacksize, word sp)
{
  zeta_ctx* ctx=malloc(sizeof(zeta_ctx));
  ctx->memory=memory;
  ctx->memsize=memsize;
  ctx->maxstacksize=maxstacksize;
  ctx->regs.sp=sp;
  ctx->regs.pc=0;
  ctx->regs.accl=rand(); // give a random seed
  ctx->done=false;
  return ctx;
}
word getArg (zeta_ctx* ctx)
{
  register word pc = ctx->regs.pc;
  register word ret = ctx->memory[pc + 1];
  ret |= ctx->memory[pc + 2];
  ret |= ctx->memory[pc + 3];
  ret |= ctx->memory[pc + 4];
  return ret;
}
void zeta_exec (zeta_ctx* ctx)
{
  byte opcode = ctx->memory[ctx->regs.pc];
  word arg=getArg(ctx);
  exec_opcode(opcode, arg, ctx);
  ctx->regs.pc+=5; // extended opcodes will automatically add one
}
 
