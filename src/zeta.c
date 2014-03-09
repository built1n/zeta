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
 *  Utility functions, the language implementation is in exec.c
 */
#include <zeta.h>
#include <util.h>
#include <exec.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void printBinary(uint32_t n)
{
  for(int i=0;i<32;++i)
    {
      if((((n>>i)&1))==1)
	printChar('1');
      else
	printChar('0');
    }
}
void badInstr(zeta_ctx* ctx)
{
  printf("Bad instruction at 0x%08X!\n", ctx->regs.pc);
}
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
zeta_ctx* zeta_init(byte* memory, word memsize, word maxstacksize, word sp, bool debug)
{
  zeta_ctx* ctx=malloc(sizeof(zeta_ctx));
  ctx->memory=memory;
  ctx->memsize=memsize;
  ctx->maxstacksize=maxstacksize;
  ctx->regs.sp=sp;
  ctx->regs.pc=0;
  srand(time(0));
  ctx->regs.accl=rand(); // give a random seed
  ctx->done=false;
  ctx->return_value=EXIT_SUCCESS;
  ctx->debug=debug;
  return ctx;
}
word getArg (zeta_ctx* ctx)
{
  if((ctx->regs).pc<(ctx->memsize)-4)
    {
      register word pc = (ctx->regs).pc;
      register word ret = (ctx->memory)[pc + 4];
      ret |= (((ctx->memory)[pc + 3])<<8);
      ret |= (((ctx->memory)[pc + 2])<<16);
      ret |= (((ctx->memory)[pc + 1])<<24);
      return ret;
    }
  else
    badRead(ctx);
}
void zeta_exec (zeta_ctx* ctx)
{
  if((ctx->regs).pc<(ctx->memsize)-4)
    {
      byte opcode = ctx->memory[ctx->regs.pc];
      word arg=getArg(ctx);
      exec_instr(opcode, arg, ctx);
      ctx->regs.pc+=5; // extended opcodes will automatically add one
    }
  else
    {
      ctx->done=true;
    }
}
