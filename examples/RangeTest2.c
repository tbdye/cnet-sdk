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
extern void  CDoReturn(void);
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

	while(CEnterLine(40, ELINE_INPUTBOX, "Enter a CNet range [RETURN=exit]: "))
		{

		// notice that the range limits are 1 to 100

		if( CNetFindRange(z->InBuffer, 1, 100, &rc) )
			{
			do
				{
				x=CNetNextRange( &rc );
				sprintf( z->ABuffer, "x=%d\n", (int)x );
				CPutA();
				} while( !CNetEndOfRange(&rc) );
			}
		else
			CPutText( "Bad range; ranges between 1 and 100 only!\n" );
		}


	// CNetNextRange() also returns -1 if there are no more elements in the range.
	// Therefore, the following method is another way to handle the above code.

	while( CEnterLine(40, ELINE_INPUTBOX, "Enter a CNet range [RETURN=exit]: ") )
		{

		// notice that the range limits here are 10 to 300

		if( CNetFindRange(z->InBuffer, 10, 300, &rc) )
			{
			while( (x=CNetNextRange(&rc)) != -1 )
				{
				sprintf( z->ABuffer, "x=%d\n", (int)x );
				CPutA();
				}
			}
		else
			CPutText( "Bad range; ranges between 10 and 300 only!\n" );
		}

	// CNetPrevRange() test
	while( CEnterLine(40, ELINE_INPUTBOX, "Enter a CNet range [RETURN=exit]: ") )
		{

		// notice that the range limits here are 10 to 300

		if( CNetFindRange(z->InBuffer, 10, 300, &rc) )
			{
			UBYTE last=FALSE; // flag to break loop if we are at last element

			// do forward direction until x=50
			CPutText("spanning forward in range until (x >= 50) is reached (if range exceeds 50)..\n");
			do
				{
				x=CNetNextRange(&rc);
				sprintf(z->ABuffer, "x=%d", (int)x);
				CPutA();

				if((last=CNetEndOfRange(&rc)))
					CPutText(" is last element in range!\n");

				CDoReturn();
				} while( x<50 && !last );

			CPutText("Now traversing backwards in range from current element to first element..\n");

			// test CNetPrevRange()..
			while( (x=CNetPrevRange(&rc)) != -1 )
				{
				sprintf(z->ABuffer, "x=%d", (int)x);
				CPutA();
				if(CNetStartOfRange(&rc))
					CPutText(" is first element in range!\n");
				CDoReturn();
				}

			}
		}

	return;
}
