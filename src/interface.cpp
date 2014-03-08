// A C++ interface for Zeta
// rather non-optimal code
#include <iostream>
#include <zeta.h>
#include <string>
#include <cstdlib>
#include <vector>
#include <cctype>
using namespace std;
char hex_chars[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
int main()
{
  vector<byte> program;
  cout << "Enter program in hex:" << endl;
  while(cin)
    {
      string line;
      getline(cin, line);
      if(line.length()%2!=0)
	{
	  cerr << "Syntax error!" << endl;
	  return -1;
	}
      for(int i=0;i<line.length();i+=2)
	{
	  byte val;
	  bool good=false;
	  string oneByte=line.substr(i, i+2);
	  for(int j=0;j<16;++j)
	    {
	      if(toupper(oneByte[0])==hex_chars[j])
		{
		  val=j;
		  good=true;
		}
	    }
	  if(!good)
	    {
	      cerr << "Non-hex character found!" << endl;
	      return -1;
	    }
	  good=false;
	  for(int j=0;j<16;++j)
	    {
	      if(toupper(oneByte[1])==hex_chars[j])
		{
		  val|=((j&0xF)<<4);
		  good=true;
		}
	    }
	  if(!good)
	    {
	      cerr << "Non-hex character found!" << endl;
	      return -1;
	    }
	  program.push_back(val);
	}
    }
  byte* p=(byte*)malloc(program.size()+2048); // add 2048 byte stack
  for(int i=0;i<program.size();++i)
    p[i]=program[i];
  zeta_ctx* ctx=zeta_init((byte*)p, program.size()+2048,2048,program.size());
  while(!ctx->done)
    zeta_exec(ctx);
  return ctx->return_value;
}
