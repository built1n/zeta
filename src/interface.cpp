// A C++ interface for Zeta
// rather buggy code
#include <fstream>
#include <iostream>
#include <zeta.h>
#include <string>
#include <cstdlib>
#include <vector>
#include <cctype>
#include <cstdio>
using namespace std;
char hex_chars[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
int main(int argc, char* argv[])
{
  istream* in=&cin;
  ifstream ifs;
  if(argc>=2)
    {
      ifs.open(argv[1]);
      // read from file
      if(ifs.good())
	in=&ifs;
      else
	{
	  cerr << "Cannot read from file.\n" << endl;
	  return -1;
	}
    }
  bool interactive=false;
  if(in==&cin)
    interactive=true;
  vector<byte> prog;
  while(in->good())
    {
      if(interactive)
	printf("0x%08X:", prog.size()); // really shouldn't mix C-style and stream I/O!
      string line;
      getline(*in, line);
      for(int i=0;i<line.length();i+=2)
	{
	  byte val=0xFF; 
	  for(int j=0;j<16;++j)
	    {
	      if(toupper(line[i+1])==hex_chars[j])
		{
		  val=j;
		}
	    }
	  for(int j=0;j<16;++j)
	    {
	      if(toupper(line[i])==hex_chars[j])
		{
		  val|=((j&0xF)<<4);
		}
	    }
	  prog.push_back(val);
	}
    }
  byte* p=(byte*)malloc(prog.size()+2048);
  for(int i=0;i<prog.size();++i)
    p[i]=prog[i];
  zeta_ctx* ctx=zeta_init((byte*)p, prog.size(),prog.size(),0, true);
  while(!ctx->done)
    zeta_exec(ctx);
  return ctx->return_value;
}
