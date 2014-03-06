// A C++ interface for Zeta
#include <iostream>
#include <zeta.h>
#include <string>
#include <cstdlib>
#include <vector>
using namespace std;
char hex_chars[]="0123456789ABCDEF";
int main()
{
  vector<byte> program;
  cout << "Enter program in hex:" << endl;
  while(cin)
    {
      string line;
      getline(cin, line);
      byte val;
      for(int i=0;i<16;++i)
	{
	  if(line[1]==hex_chars[i])
	    val=i;
	}
      for(int i=0;i<16;++i)
	{
	  if(line[0]==hex_chars[i])
	    val|=(i<<4);
	}
      program.push_back(val);
    }
  byte* p=(byte*)malloc(program.size()+2048); // add 2048 byte stack
  for(int i=0;i<program.size();++i)
    p[i]=program[i];
  zeta_ctx* ctx=zeta_init((byte*)p, program.size()+2048,2048,program.size());
  while(!ctx->done)
    zeta_exec(ctx);
}
