#include <zeta.h>
#include <exec.h>
#include <util.h>
#include <stdlib.h>
#include <stdio.h>
static inline void writeWord(zeta_ctx* ctx, word addr, word value)
{
  if(likely(addr<ctx->memsize-4))
    {
      ctx->memory[addr+0]=((value&0xFF000000)>>24);
      ctx->memory[addr+1]=((value&0x00FF0000)>>16);
      ctx->memory[addr+2]=((value&0x0000FF00)>>8);
      ctx->memory[addr+3]=(value&0x000000FF);
    }
  else
    badWrite(ctx);
}
static inline word readWord(zeta_ctx* ctx, word addr)
{
  word ret=0;
  if(likely(addr<(ctx->memsize)-4))
    {
      ret|=(ctx->memory[addr+3]);
      ret|=(ctx->memory[addr+2] << 8);
      ret|=(ctx->memory[addr+1] << 16);
      ret|=(ctx->memory[addr+0] << 24);
    }
  else
    badRead(ctx);
  return ret;
}
static inline void pushStack(zeta_ctx* ctx, word value)
{
  if(likely((ctx->stacksize)<=(ctx->maxstacksize)-4))
    {
      writeWord(ctx, ctx->regs.sp, value);
      ctx->regs.sp+=4;
      ctx->stacksize+=4;
    }
  else
    stackOverflow(ctx);
}
static inline word popStack(zeta_ctx* ctx)
{
  if(likely(ctx->stacksize>=4))
    {
      word ret=readWord(ctx, ctx->regs.sp);
      ctx->regs.sp-=4;
      ctx->stacksize-=4;
      return ret;
    }
  else
    stackUnderflow(ctx);
}
static inline void exec_extd(zeta_ctx* ctx, byte opcode, word operand)
{
  word arg=operand; // explicit value
  if(opcode&0x80) // bit 7 set, treat as pointer
    {
      arg=readWord(ctx, operand);
    }
  switch(opcode&0x7F)
    {
    case 0x08: // let increment/decrement be fastest
      ++(ctx->regs.accl);
      break;
    case 0x09:
      --(ctx->regs.accl);
      break;
    case 0x00:
      ctx->regs.accl=arg;
      break;
    case 0x01:
      ctx->regs.accl|=arg;
      break;
    case 0x02:
      ctx->regs.accl=~(ctx->regs.accl);
      break;
    case 0x03:
      ctx->regs.accl&=arg;
      break;
    case 0x04:
      ctx->regs.accl+=arg;
      break;
    case 0x05:
      ctx->regs.accl-=arg;
      break;
    case 0x06:
      ctx->regs.accl*=arg;
      break;
    case 0x07:
      ctx->regs.accl/=arg;
      break;
    case 0x0A:
      // halt
      ctx->done=true;
      ctx->return_value=arg;
      break;
    case 0x0B:
      switch(arg)
	{
	case 0:
	  printInt(ctx->regs.accl);
	  break;
	case 1:
	  printHex_lcase(ctx->regs.accl);
	  break;
	case 2:
	  printHex_ucase(ctx->regs.accl);
	  break;
	case 4:
	  printBinary(ctx->regs.accl);
	  break;
	default:
	  badInstr(ctx);
	  break;
	}
      break;
    case 0x0C: // puts
      {
	word loc=arg;
	if(unlikely(operand&0x80))
	  {
	    loc=ctx->regs.accl;
	  }
	for(word i=loc;ctx->memory[i] && i<ctx->memsize;++i)
	  {
	    printChar(ctx->memory[i]);
	  }
	break;
      }
    default:
      badInstr(ctx);
    }
}
static inline void zeta_exec_00(word arg, zeta_ctx* ctx)
{
  asm("nop");
}
void zeta_exec_01(word arg, zeta_ctx* ctx)
{
  writeWord(ctx, arg, ctx->regs.accl);
}
void zeta_exec_02(word arg, zeta_ctx* ctx)
{
  ctx->regs.accl=readWord(ctx, arg);
}
void zeta_exec_03(word arg, zeta_ctx* ctx)
{
  ctx->regs.pc=arg-5;
}
void zeta_exec_04(word arg, zeta_ctx* ctx)
{
  --(ctx->regs.accl);
  if(unlikely(ctx->regs.accl==arg))
    {
      // peek at the next instruction
      ctx->regs.pc+=5;
      if(unlikely(ctx->memory[ctx->regs.pc+5]==0x0B))
	++(ctx->regs.pc);
    }
}
void zeta_exec_05(word arg, zeta_ctx* ctx)
{
  if(unlikely(ctx->regs.accl==arg))
    {
      // peek at the next instruction
      byte add=5;
      if(unlikely(ctx->memory[ctx->regs.pc+5]==0x0B))
	++add;
      ctx->regs.pc+=add;
    }
}
void zeta_exec_06(word arg, zeta_ctx* ctx)
{
  pushStack(ctx, ctx->regs.pc);
  ctx->regs.pc=arg-5;
}
void zeta_exec_07(word arg, zeta_ctx* ctx)
{
  ctx->regs.pc=popStack(ctx); // no need to add five, zeta_exec does for us
}
void zeta_exec_08(word arg, zeta_ctx* ctx)
{
  pushStack(ctx, ctx->regs.accl);
}
void zeta_exec_09(word arg, zeta_ctx* ctx)
{
  pushStack(ctx, arg);
}
void zeta_exec_0A(word arg, zeta_ctx* ctx)
{
  ctx->regs.accl=popStack(ctx);
}
void zeta_exec_0B(word arg, zeta_ctx* ctx)
{
  // extended opcode
  ++(ctx->regs.pc);
  exec_extd(ctx, (ctx->memory)[ctx->regs.pc], getArg(ctx));
}
void(*exec_table[256])(word, zeta_ctx*)={&zeta_exec_00, &zeta_exec_01, &zeta_exec_02, &zeta_exec_03, &zeta_exec_04, &zeta_exec_05, &zeta_exec_06, &zeta_exec_07, &zeta_exec_08, &zeta_exec_09, &zeta_exec_0A, &zeta_exec_0B};
void exec_instr(byte opcode, word arg, zeta_ctx* ctx)
{
  if(!ctx->done)
    {
      if(exec_table[opcode])
	exec_table[opcode](arg, ctx);
    }
  return;
}
