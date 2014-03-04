#include <zeta.h>
#include <exec.h>
#include <util.h>
#include <stdlib.h>
static inline void writeWord(zeta_ctx* ctx, word addr, word value)
{
  if(addr<ctx->memsize-4)
    {
      ctx->memory[addr]=  (value&0xFF000000);
      ctx->memory[addr+1]=(value&0x00FF0000);
      ctx->memory[addr+2]=(value&0x0000FF00);
      ctx->memory[addr+3]=(value&0x000000FF);
    }
  else
    badWrite(ctx);
}
static inline word readWord(zeta_ctx* ctx, word addr)
{
  word ret=0;
  if(addr<ctx->memsize-4)
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
  if(ctx->stacksize<=ctx->maxstacksize-4)
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
  if(ctx->stacksize>=4)
    {
      word ret=readWord(ctx, ctx->regs.sp);
      ctx->regs.sp-=4;
      ctx->stacksize-=4;
      return ret;
    }
  else
    stackUnderflow(ctx);
}
inline void exec_extd(zeta_ctx* ctx, byte opcode, word arg)
{
  word operand=arg; // explicit value
  if(opcode&0x80) // bit 7 set, treat as pointer
    {
      operand=readWord(ctx, arg);
    }
  switch(opcode)
    {
      // give incr/decr priority
    case 0x08:
      ++(ctx->regs.accl);
      break;
    case 0x09:
      --(ctx->regs.accl);
      break;
    case 0x01:
      ctx->regs.accl|=operand;
      break;
    case 0x02:
      ctx->regs.accl=~(ctx->regs.accl);
      break;
    case 0x03:
      ctx->regs.accl&=operand;
      break;
    case 0x04:
      ctx->regs.accl+=operand;
      break;
    case 0x05:
      ctx->regs.accl-=operand;
      break;
    case 0x06:
      ctx->regs.accl*=operand;
      break;
    case 0x07:
      ctx->regs.accl/=operand;
      break;
    case 0x0A:
      // halt
      ctx->done=true;
      break;
    }
}
inline void zeta_exec_00(word arg, zeta_ctx* ctx)
{
  asm("nop");
}
inline void zeta_exec_01(word arg, zeta_ctx* ctx)
{
  writeWord(ctx, arg, ctx->regs.accl);
}
inline void zeta_exec_02(word arg, zeta_ctx* ctx)
{
  ctx->regs.accl=readWord(ctx, arg);
}
inline void zeta_exec_03(word arg, zeta_ctx* ctx)
{
  ctx->regs.pc=arg-5;
}
inline void zeta_exec_04(word arg, zeta_ctx* ctx)
{
  --(ctx->regs.accl);
  if(ctx->regs.accl==arg)
    {
      // peek at the next instruction
      byte add=5;
      if(ctx->memory[ctx->regs.pc+5]==0x0B)
	++add;
      ctx->regs.pc+=add;
    }
}
inline void zeta_exec_05(word arg, zeta_ctx* ctx)
{
  if(ctx->regs.accl==arg)
    {
      // peek at the next instruction
      byte add=5;
      if(ctx->memory[ctx->regs.pc+5]==0x0B)
	++add;
      ctx->regs.pc+=add;
    }
}
inline void zeta_exec_06(word arg, zeta_ctx* ctx)
{
  pushStack(ctx, ctx->regs.pc);
  ctx->regs.pc=arg-5;
}
inline void zeta_exec_07(word arg, zeta_ctx* ctx)
{
  ctx->regs.pc=popStack(ctx); // no need to add five, zeta_exec does for us
}
inline void zeta_exec_08(word arg, zeta_ctx* ctx)
{
  pushStack(ctx, ctx->regs.accl);
}
inline void zeta_exec_09(word arg, zeta_ctx* ctx)
{
  pushStack(ctx, arg);
}
inline void zeta_exec_0A(word arg, zeta_ctx* ctx)
{
  ctx->regs.accl=popStack(ctx);
}
inline void zeta_exec_0B(word arg, zeta_ctx* ctx)
{
  // extended opcode
  ++ctx->regs.pc;
  exec_extd(ctx, ctx->memory[ctx->regs.pc], getArg(ctx));
}
void(*exec_table[256])(word, zeta_ctx*)={&zeta_exec_00, &zeta_exec_01, &zeta_exec_02, &zeta_exec_03, &zeta_exec_04, &zeta_exec_05, &zeta_exec_06, &zeta_exec_07, &zeta_exec_08, &zeta_exec_09, &zeta_exec_0A, &zeta_exec_0B};
void exec_opcode(byte opcode, word arg, zeta_ctx* ctx)
{
  printf("Executing opcode 0x%x\n", opcode);
  if(!ctx->done)
    {
      if(exec_table[opcode])
	exec_table[opcode](arg, ctx);
    }
}
