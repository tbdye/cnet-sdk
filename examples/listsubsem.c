// CNet Amiga/4 Programming examples
// copyright (c) 1998 Ray A. Akey
// Exclusive license granted to ZenMetal Software and CNet door coders.

/* Ported from SAS/C to m68k-amigaos-gcc -noixemul */

#include <stdlib.h>
#include <stdio.h>

#include <exec/types.h>
#include <dos/dos.h>
#include <cnet/cnet.h>

#undef __asm
#include <proto/exec.h>
#include <proto/dos.h>

int main(int argc, char **argv)
{
	struct MainPort *myp;
	struct SubboardType4 *sp;
	BPTR inputfh=Input();
	short i=0;
	char inchar;

	if(myp=(struct MainPort *)FindPort(CNETPORT))
		{
		for( i=0; i<myp->ns; i++ )
			{
			sp = &myp->Subboard[i];

			if(sp->sem->ss_NestCount || sp->sem->ss_Owner)
				{
				printf("\n        Title: %s\n", sp->Title);
				printf("         &sem: %x\n", (unsigned int)sp->sem);
				printf(" ss_NestCount: %x\n", (unsigned int)sp->sem->ss_NestCount);
				printf("     ss_Owner: %x\n", (unsigned int)sp->sem->ss_Owner);
				}

			if(WaitForChar(inputfh, 0L))
				{
				Read(inputfh, (char *)&inchar, 1);
				if(inchar == 27)
					exit(0);
				}
			}
		}

	return 0;
}
