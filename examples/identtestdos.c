// CNet Amiga/4 Programming examples
// copyright (c) 1998 Ray A. Akey
// Exclusive license granted to ZenMetal Software and CNet door coders.

/* Ported from SAS/C to m68k-amigaos-gcc -noixemul */

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

	if(myp=(struct MainPort *)FindPort(CNETPORT))
		{
		if(myp->MPE->idd)
			{
			struct IdentdData *iddptr=myp->MPE->idd;
			// walk the list of identd data and print specifics
			while(iddptr)
				{
				Printf("Socket number: %ld, CNet Owner Port: %ld\n", iddptr->socket, (long)iddptr->port);
				iddptr=iddptr->next;
				}
			}
		else
			{
			Printf("No identd entries\n");
			}
		}

	return 0;
}
