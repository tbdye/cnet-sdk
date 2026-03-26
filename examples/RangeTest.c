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
extern int   CEnterLine(UBYTE len, ULONG flags, char *prompt);

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

	struct RangeContext rc;
	LONG x;

   if( !(CNet4Base = OpenLibrary("cnet4.library", 4)) )
		exit(RETURN_ERROR);

	while(CEnterLine(40, ELINE_INPUTBOX, "Enter a CNet range: "))
		{
		if(CNetFindRange(z->InBuffer, 1, 100, &rc))
			{
			for(;;)
				{
				x=CNetNextRange(&rc);
				sprintf(z->ABuffer, "value: %4.4d    nparts: %2d    set: %2d    element: %4d\n", (int)x, (int)rc.nparts, (int)rc.set, (int)rc.element);
				CPutA();
				if( (rc.set == (rc.nparts-1)) && (rc.element == (rc.parts[rc.set][1])) )
					{
					CPutText("END OF RANGE!\n");
					exit(0);
					}
				}
			}
		}

	return;
}
