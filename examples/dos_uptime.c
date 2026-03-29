// CNet Amiga/4 Programming examples
// copyright (c) 1998 Ray A. Akey
// Exclusive license granted to ZenMetal Software and CNet door coders.

// note: Original code used NOSTACKCHECK and NOSTARTUP options (SAS/C).
//       GCC with libnix provides startup code, so manual DOSBase
//       handling is removed. The program still opens cnet.library
//       and cnet4.library at runtime.

/* Ported from SAS/C to m68k-amigaos-gcc -noixemul */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <exec/types.h>
#include <dos/dos.h>
#include <cnet/cnet.h>

#undef __asm
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/cnet.h>
#include <proto/cnet4.h>

unsigned long __stack = 65536;

struct Library *CNet4Base=NULL;
struct Library *CNetBase=NULL;

void CleanUp    ( void );
void MCIprintf  ( char *fmt, ...);


int main(int argc, char **argv)
{
   struct MainPort *myp=NULL;
   short i;
   struct PortData *zp;
   char date[24];
   char utstring[128];
   struct RangeContext rc;
   char *argbuff;
   LONG myargs[1] = { 0L };
	struct RDArgs *MyReadArgs=NULL;

   if(!(CNet4Base=OpenLibrary("cnet4.library", 4L)) || !(CNetBase=OpenLibrary("cnet.library", 4L)) )
   	{
   	CleanUp();
      return 0;
      }

	// parse stdin using ReadArgs so that we don't have to link with startup code.

	Forbid();
	myp=(struct MainPort *)FindPort(CNETPORT);
	Permit();

   if(myp)
      {
		MyReadArgs=ReadArgs("PORTS", myargs, NULL);
		if(myargs[0])
			{
	      argbuff = (char *)myargs[0];
	      if(argbuff[0] == '*')
	      	argbuff = "-";
	      }
	   else
	      argbuff = "-";

		// Now do the CNet stuff to list port uptimes.
      memset(&rc, 0, sizeof(struct RangeContext));

      if(CNetFindRange( argbuff, 0, myp->HiPort, &rc))
      	{
			MCIprintf(myp->FirstTT->bm[2425]); // uptime header
			MCIprintf(myp->FirstTT->bm[2426]); // header underline

			// control uptime

			MakeDateZone( &myp->SAMDate[1], date, (char **)&myp->FirstTT->bm[1069], 0, 0 );
			CNetUpTime(utstring, CNetTimeNowDiff(CNetImplodeTime(&myp->SAMDate[1])));
			MCIprintf(myp->FirstTT->bm[2427], date, (*utstring) ? utstring : (char *)myp->FirstTT->bm[2421]);

	      // port uptimes..

			while( (i=CNetNextRange( &rc )) != -1)
				{
				if((zp=myp->PortZ[i]) != myp->z0 )
					{
					MakeDateZone( &zp->BootDate,date, (char **)&myp->FirstTT->bm[1069], 0, 0 );
					CNetUpTime(utstring, CNetTimeNowDiff(CNetImplodeTime(&zp->BootDate)));
					MCIprintf(myp->FirstTT->bm[2428], i, date, *utstring ? utstring : (char *)myp->FirstTT->bm[2421]);
					}
				}
			MCIprintf(myp->FirstTT->bm[2429]); // footer
			}

		FreeArgs(MyReadArgs);
		}
   else
      {
  	   Printf("\nCNet:Control is not running\n\n");
      CleanUp();
      return 0;
      }
   Printf("\n");
   CleanUp();
   return 0;
}



// called before exiting so that resources used may be closed/freed
void CleanUp( void )
{
   if(CNet4Base)
      CloseLibrary(CNet4Base);
   if(CNetBase)
      CloseLibrary(CNetBase);
}



// MCI filtering varargs print routine
void MCIprintf(char *fmt, ...)
{
   va_list ap;
   char sprbuff[256], outbuff[256], *outp=outbuff, *inp;
   UBYTE CQ;
   int num, x;

   va_start(ap, fmt);
   vsprintf(sprbuff, fmt, ap);
   va_end(ap);

   inp=sprbuff;

   while(*inp)
      {
      CQ=FALSE;
      switch(*inp)
         {
         case 17: // ^Q
            CQ=TRUE;
         case 25: // ^Y
            inp++;
            switch(*inp)
               {
               case 'n':
                  inp++;
                  while(*inp == ' ')
                     inp++;
                  num=atoi(inp);
                  for(x=0;x<num;x++)
                     {
                     *outp='\n';
                     outp++;
                     }
                  break;
               default:
                  inp++;
                  break;
               }

            if(CQ)
               {
               char *s;

               if((s=strchr(inp, '}')))
                  inp=s;
               CQ=FALSE;
               }
            break;
         default:
            *outp=*inp;
            outp++;
            break;
         }
      inp++;
      }
   *outp='\0';

   PutStr(outbuff);
}

