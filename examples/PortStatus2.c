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

/* Restore __asm after <cnet/cnet.h> redefined it to nothing,
 * so GCC inline stubs (LP* macros) work for library calls. */
#undef __asm

#include <proto/exec.h>
#include <proto/cnet4.h>

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
extern short CalculateAge(struct UserData *user);
extern BOOL  IsPortLoaded(short portnum);
extern BOOL  IsPortOccupied(short portnum);

struct Library         *CNet4Base    = NULL; // pointer to cnet4.library lib base

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

	if(CNet4Base)
		{
		CloseLibrary(CNet4Base);
		CNet4Base=NULL;
		}

	return;
}



// -------------- YOUR DOOR CODE EXECUTION BEGINS IN THIS FUNCTION --------------
void DoorMain( char *DosArgs )
{
	// Your door code begins here.
	// DosArgs == NULL if no ** DOS ** args were passed to door from CNet

	LONG zzz=0;

	if( !(CNet4Base = OpenLibrary("cnet4.library", 4)) ) // library required for CalculateAge() and other date-related routines
		exit(RETURN_ERROR);

	CPutText("\n");
	sprintf(z->ABuffer, "   Your Handle: %s\n", z->user1.Handle);
	CPutA();
	sprintf(z->ABuffer, "Your Real name: %s\n", z->user1.RealName);
	CPutA();
	sprintf(z->ABuffer, "      Your age: %d\n\n", CalculateAge(&z->user1));
	CPutA();

	CPutText("Loaded ports\n---------------------\n");
	for(zzz=0;zzz<100;zzz++)
		{
		if(IsPortLoaded(zzz))
			{
			sprintf(z->ABuffer, "%3d: %s\n", (int)zzz, IsPortOccupied(zzz) ? myp->PortZ[zzz]->user1.Handle : "[IDLE]");
			CPutA();
			}
		}

	return;
}
