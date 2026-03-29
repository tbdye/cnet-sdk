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

int main( int argc, char *argv[] )
{
	struct MainPort *myp;
	int portnum = 0;

	if(argc != 3)
		{
		printf("\nUsage: %s <port number> <new idle time in 10ths of secs>\n\n", argv[0]);
		exit(0);
		}

	portnum = atoi(argv[1]);

	myp=(struct MainPort *)FindPort(CNETPORT);
	if(myp)
		{
		if(myp->PortZ[portnum] == myp->z0)
			{
			printf("\n%s: port not loaded.\n\n", argv[0]);
			exit(0);
			}
		myp->PortZ[portnum]->TimeIdle = atol(argv[2]);
		}

	return 0;
}
