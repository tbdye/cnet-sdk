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
#include <proto/exec.h>
#include <proto/bsdsocket.h>

char *StatTypes[] =
	{
	"WAITING",
	"ACCEPTED",
	"STALLED",
	"ABORTED"
	};

struct Library					*SocketBase				=	NULL;


int main(int argc, char **argv)
{
	struct MainPort *myp;

	if(SocketBase = (struct Library *)OpenLibrary("bsdsocket.library", 2))
		{
		if(myp = (struct MainPort *)FindPort(CNETPORT))
			{
			struct Node *work;
			struct DCCStruct *reqptr;
			char argbuffer[80];
			char addrbuffer[40];
			struct in_addr remote;
			ULONG TempLong;
			int xxx=0;
			char status[10];

			ObtainSemaphore(&myp->MPE->dccsem);

			if( !((struct List *)&myp->MPE->DCCList) || IsListEmpty((struct List *)&myp->MPE->DCCList) )
				printf("\nNo DCC requests pending.\n");
			else
				{
				printf("\n[1;4m## Nick            Address         Type Info                 Status  ID             UserID   [0m\n");
				xxx=0;
				work = (struct Node *)(myp->MPE->DCCList.lh_Head);

				while( work->ln_Succ )
					{
					reqptr = (struct DCCStruct *)work;

					if(reqptr->size)
						sprintf(argbuffer, "%s/%ld bytes", reqptr->arg, reqptr->size);
					else
						sprintf(argbuffer, "%s", reqptr->arg);

					// convert senderaddr to one usable in a socket connect
					TempLong = atol(reqptr->senderaddr);
					remote.s_addr = htonl(TempLong);
					strcpy(addrbuffer, Inet_NtoA(remote.s_addr));

					printf("%2d %-15.15s %-15.15s %-4.4s %-20.20s %-7.7s %-14.14s %ld\n", xxx+1, reqptr->nickname, addrbuffer, reqptr->Type, argbuffer, StatTypes[reqptr->status], reqptr->ID, reqptr->UserID);
					xxx++;
					work=(struct Node *)work->ln_Succ;
					}
				printf("end of list.\n");
				}

			ReleaseSemaphore(&myp->MPE->dccsem);
			}
		CloseLibrary(SocketBase);
		}
	printf("\n");

	return 0;
}
