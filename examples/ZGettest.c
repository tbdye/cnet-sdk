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
#include <proto/cnet.h>

struct ItemType3 it;
struct ItemHeader ih;

int main(int argc, char *argv[])
{
	struct MainPort *myp=NULL;
	struct Library *CNetBase=NULL;

	if(!(CNetBase = OpenLibrary("cnet.library", 4)))
		{
		printf("No library\n");
		exit(0);
		}


	if(myp=(struct MainPort *)FindPort(CNETPORT))
		{
		printf("Found port!\n");
		if(OneMoreUser(&myp->Subboard[1], FALSE))
			{
			printf("Entered subboard\n");
			ZGetItem(&it, &ih, &myp->Subboard[1], 0);
			if(it.loaded)
				{
				printf("Item Title: %s\n", it.Title);
				}
			else
				{
				printf("Item not loaded!\n");
				}
			OneLessUser(&myp->Subboard[1]);
			}
		}
	CloseLibrary(CNetBase);
	exit(0);

	return 0;
}
