// CNet Amiga/4 Programming examples
// copyright (c) 1998 Ray A. Akey
// Exclusive license granted to ZenMetal Software and CNet door coders.

/* Ported from SAS/C to m68k-amigaos-gcc -noixemul */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <exec/types.h>
#include <dos/dos.h>
#include <cnet/cnet.h>

#undef __asm
#include <proto/dos.h>

char tnhostfile[] = "sysdata:bbs.tnhost";

int main(int argc, char **argv)
{
	BPTR fh;
	struct TNHostEntry he;
	int i=0;

	if((fh=Open(tnhostfile, MODE_OLDFILE)))
		{
		char hoststring[80];

		while( Read(fh, &he, sizeof(struct TNHostEntry)) )
			{
			i++;
			sprintf(hoststring, "%s:%d", he.he_ip, he.he_port);
			printf("\n%2d. %-24.24s %-20.20s %s", i, he.he_system, hoststring, he.he_data);
			}
		Close(fh);
		}
	printf("\n");

	return 0;
}
