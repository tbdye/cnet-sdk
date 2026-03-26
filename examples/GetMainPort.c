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

int main(int argc, char *argv[])
{
	struct MainPort *myp;

	if(myp=(struct MainPort *)FindPort("cnetport"))
		{
		printf("MainPort found!\n");
		}
	else
		{
		printf("Could not find CNet MainPort.\n");
		}
	exit(0);

	return 0;
}
