/*****************************************************************************
 *                            CNetC library examples                         *
 *                         copyright (c) ZenMetal Software                   *
 *                              All rights reserved.                         *
 *                                                                           *
 *                 Special thanks to Kelly Cochran for his initial           *
 *                 conversion of empty.c code to a shared library.           *
 *****************************************************************************/

/* Ported from SAS/C to m68k-amigaos-gcc -noixemul */

#include <stdlib.h>
#include <stdio.h>

#include <exec/types.h>
#include <dos/dos.h>
#include <cnet/cnet.h>

/* -------------------------- FUNCTION PROTOTYPES -------------------------- */
void DoorExit      ( void          );
void DoorMain      ( char *DosArgs );

/* --------------------- GLOBAL VARIABLE/DATA SECTION ---------------------- */

// CNCL_DoorInit() requires that the following variables are declared
extern struct RDArgs          *myrdargs;
extern struct Library         *CNetCBase;
extern struct CNetCContext    *context;
extern struct PortData        *z;
extern struct MainPort        *myp;

extern BOOL CNCL_DoorInit(int argc, char **argv);
extern void CNCL_DoorCleanup(void);

/* C* wrapper function prototypes (from libcnet_door.a) */
extern void  CPutText(char *text);
extern void  CPutA(void);

/* --------------------- BEGINNING OF DOOR FUNCTIONS ----------------------- */

void GetOut(void)
{
	DoorExit();
	CNCL_DoorCleanup();
}


int main( int argc, char **argv )
{
   // CNCL_DoorInit() is in libcnet_door.a
	CNCL_DoorInit( argc, argv );

	// execute your door code
   DoorMain(NULL);

   // exit back to CNet - exit trap automatically calls GetOut()
   exit(RETURN_OK);

   return 0;
}


// *******************************************************************************
// DO NOT ALTER ANYTHING ABOVE THIS LINE OR FUTURE COMPATIBILITY MAY BE SACRIFICED
// *******************************************************************************



// -------------------- YOUR DOOR EXIT/CLEANUP CODE GOES HERE -------------------
void DoorExit( void )
{
	// Place any resource cleanup code, such as freeing memory, closing libs
   // opened by your door (but not cnetc.library, which is already opened) here.
   // These will be freed by the door exit code whenever exit() is called

	return;
}



// -------------- YOUR DOOR CODE EXECUTION BEGINS IN THIS FUNCTION --------------
void DoorMain( char *DosArgs )
{
	// Your door code begins here.
	// DosArgs == NULL if no ** DOS ** args were passed to door from CNet

	if(myp->MPE->idd)
		{
		struct IdentdData *iddptr=myp->MPE->idd;
		// walk the list of identd data and print specifics
		while(iddptr)
			{
			sprintf(z->ABuffer, "Socket number: %u, CNet Owner Port: %d\n", iddptr->socket, iddptr->port);
			CPutA();
			iddptr=iddptr->next;
			}
		}
	else
		{
		CPutText("No identd entries\n");
		}

	return;
}
