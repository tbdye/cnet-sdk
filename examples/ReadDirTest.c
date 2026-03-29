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
#include <cnet/version.h>

#undef __asm
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/cnet.h>

struct Library        *CNetBase=NULL;
struct CNetFileEntry  *filelist;


void DoDisplay(struct CNetFileEntry *list, UBYTE depth)
{
	char buff[20];
	struct CNetFileEntry *fptr=list;
	UBYTE i;

	depth++;
	while((fptr=NextDirEntry(HeaderEntry(list), fptr)))
		{
		if(fptr->size)
			sprintf(buff, "(%ld bytes)", (long)fptr->size);

		for(i=0;i<depth;i++)
			Printf("\t");

		Printf("%3ld. %s%s %s\n", (long)EntryOrdinal(fptr, HeaderEntry(list)), (long)(fptr->ftype ? "[DIR]":""), (long)fptr->filename, (long)(fptr->size ? buff:""));
		if(fptr->ftype)
			{
			DoDisplay(fptr->child, depth);
			}
		}
}

int main( int argc, char *argv[])
{
	if(argc < 2)
		{
		Printf("\nSyntax: %s <path> [TRUE:FALSE]\n\n", (long)argv[0]);
		exit(RETURN_WARN);
		}

	if((CNetBase=OpenLibrary(CNETLIBNAME, CNETLIBVERSION)))
		{
		if((filelist=CNetReadDir(argv[1], argc > 2))) // presence of ANY 2nd argument signifies recurse..
			{
			struct CNetFileEntry *fptr;
			if((fptr=HeaderEntry(filelist))) // the header entry contains info
			                               // about the directory of files
                                        // attached to the linked list
				{
				Printf("Directory of: %s\n", (long)fptr->filename);
				DoDisplay(fptr, 0);
				}
			CNetDisposeDir(&filelist);
			}
		CloseLibrary(CNetBase);
		}
	exit(RETURN_OK);

	return 0;
}
