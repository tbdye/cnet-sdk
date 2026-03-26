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
void DoorExit ( void          );
void DoorMain ( char *DosArgs );

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

/* Linker library function prototypes (from libcnet_door.a) */
extern BOOL  IsAbort(void);

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

	LONG zzz=0;

	for( zzz=0 ; (zzz<myp->ns) && IsAbort() ; zzz++ )
		{
		// print subboard info!
		sprintf(z->ABuffer, "\nTITLE        : %su0\n", myp->Subboard[zzz].Title);
		CPutA();
		sprintf(z->ABuffer, "SubDirName   : %s\n", myp->Subboard[zzz].SubDirName);
		CPutA();
		sprintf(z->ABuffer, "DataPath     : %s\n", myp->Subboard[zzz].DataPath);
		CPutA();
		sprintf(z->ABuffer, "Parts        : %ld\n", myp->Subboard[zzz].Parts);
		CPutA();
		sprintf(z->ABuffer, "Filler       : %s\n", myp->Subboard[zzz].Filler);
		CPutA();
		sprintf(z->ABuffer, "Direct       : %d\n", myp->Subboard[zzz].Direct);
		CPutA();
		sprintf(z->ABuffer, "FreeDays     : %d\n", myp->Subboard[zzz].FreeDays);
		CPutA();
		sprintf(z->ABuffer, "SubDirectory : %d\n", myp->Subboard[zzz].Subdirectory);
		CPutA();
		sprintf(z->ABuffer, "Closed       : %d\n", myp->Subboard[zzz].Closed);
		CPutA();
		sprintf(z->ABuffer, "Union Flags  : %ld\n", myp->Subboard[zzz].UnionFlags);
		CPutA();
		sprintf(z->ABuffer, "Access       : %ld\n", myp->Subboard[zzz].Access);
		CPutA();
		sprintf(z->ABuffer, "Post Access  : %ld\n", myp->Subboard[zzz].PostAccess);
		CPutA();
		sprintf(z->ABuffer, "RespondAccess: %ld\n", myp->Subboard[zzz].RespondAccess);
		CPutA();
		sprintf(z->ABuffer, "Upload Access: %ld\n", myp->Subboard[zzz].UploadAccess);
		CPutA();
		sprintf(z->ABuffer, "DLoad Access : %ld\n", myp->Subboard[zzz].DownloadAccess);
		CPutA();
		sprintf(z->ABuffer, "Comp Types   : %ld\n", myp->Subboard[zzz].ComputerTypes);
		CPutA();
		sprintf(z->ABuffer, "Zero Path    : %s\n", myp->Subboard[zzz].ZeroPath);
		CPutA();
		sprintf(z->ABuffer, "Origin       : %s\n", myp->Subboard[zzz].Origin);
		CPutA();
		sprintf(z->ABuffer, "Max Items    : %d\n", myp->Subboard[zzz].MaxItems);
		CPutA();
		sprintf(z->ABuffer, "nm           : %ld\n", myp->Subboard[zzz].nm);
		CPutA();
		sprintf(z->ABuffer, "rn           : %d\n", myp->Subboard[zzz].rn);
		CPutA();
		sprintf(z->ABuffer, "AllocItems   : %d\n", myp->Subboard[zzz].AllocItems);
		CPutA();
		sprintf(z->ABuffer, "nNewMess     : %d\n", myp->Subboard[zzz].nNewMess);
		CPutA();
		}

	return;
}
