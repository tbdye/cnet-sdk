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

	if(argc != 2)
		{
		printf("\nUsage: %s <port number>\n\n", argv[0]);
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

		if(myp->PortZ[portnum]->TimeIdle > 600)
			printf("Port %d idle %d.%d hours.\n", portnum, myp->PortZ[portnum]->TimeIdle/600, (myp->PortZ[portnum]->TimeIdle%600)/60 );
		else
			printf("Port %d idle %d.%d minutes.\n", portnum, myp->PortZ[portnum]->TimeIdle/10, myp->PortZ[portnum]->TimeIdle%10);
		}

	return 0;
}
