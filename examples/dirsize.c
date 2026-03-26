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
#include <string.h>

#include <exec/types.h>
#include <dos/dos.h>
#include <cnet/cnet.h>
#include <cnet/version.h>

/* Restore __asm after <cnet/cnet.h> redefined it to nothing,
 * so GCC inline stubs (LP* macros) work for library calls. */
#undef __asm

#include <proto/exec.h>
#include <proto/cnet.h>
#include <proto/cnet4.h>
#include <proto/cnetc.h>

/* ---------------------- DEFAULT FUNCTION PROTOTYPES ---------------------- */
void DoorExit ( void          );
void DoorMain ( char *DosArgs );

/* --------------------- GLOBAL VARIABLE/DATA SECTION ---------------------- */

// CNCL_DoorInit() requires that the following 5 variables are declared
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

char                   **bm;                 // pointer to bbstext lines - set by us in DoorMain()
struct Library         *CNet4Base    = NULL; // pointer to cnet4.library lib base - opened by us in DoorMain()
struct Library         *CNetBase     = NULL; // pointer to cnet.library lib base - opened by us in DoorMain()


// these were made globals to avoid subtle quirks..
// you should be able to move them as local variables as long as you use
// the __aligned C keyword to make sure they are LONGWORD aligned.
struct ItemHeader      *MyHead = NULL;
struct SubboardType4   *sp     = NULL;
short                  physnum = 0;


/*
 * FindUnits - convert a byte count into human-readable units.
 *
 * This was a cnet.lib linker library function not available as a
 * shared library call.  Reimplemented here for GCC.
 */
static void FindUnits(ULONG size, char *unit, ULONG chunk, char **bm_unused)
{
	(void)chunk;
	(void)bm_unused;
	if(size >= 1073741824U) {
		sprintf(unit, "%uG", size / 1073741824U);
	} else if(size >= 1048576U) {
		sprintf(unit, "%uM", size / 1048576U);
	} else if(size >= 1024U) {
		sprintf(unit, "%uk", size / 1024U);
	} else {
		sprintf(unit, "%u", size);
	}
}



// **********************************************************************************
// DO NOT ALTER ANYTHING WITHIN THESE LINES OR FUTURE COMPATIBILITY MAY BE SACRIFICED
// VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
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
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// DO NOT ALTER ANYTHING BETWEEN THESE LINES OR FUTURE COMPATIBILITY MAY BE SACRIFICED
// ***********************************************************************************




void SizeItems( struct SubboardType4 *sub )
{
	// only allow usage on file transfer and msg areas..
	if( (sub->Marker != MRK_FILE_TXFER) && (sub->Marker != MRK_MSG_BASE) )
		CPutText(bm[2595]); // "command cannot be used on non-file areas"

	// make sure we don't attempt to do processing on a killed subboard..
	else if(sub->Marker & MRK_SUBBOARD_KILLED)
		{
		// this case should theoretically never happen but..  just in case..
		// Let's lie and say we never found it. 8^P
		sprintf(z->ABuffer, bm[2596], sub->SubDirName); // "could not find sub w/go key "%s"
		CPutA();
		}

	// checks panned out..  attempt to "enter" subboard and do our thing..
	// note the requirement that OneMoreUser() MUST be called before ZGet* functions
	// can be used..
	else if(OneMoreUser(sub, FALSE))
		{
		long  j            = 0L;
		ULONG size         = 0L;
		short nfiles       = 0;
		char  sizestr [9];

		// loop through all subboard items..
		for( j=0; j<sub->rn; j++ )
		   {
		   // Get info for the current item..
		   MyHead = ZGetIHeadPtr( sub, j );

			// note we could have done other processing here such as listing
			// the items etc..  but for simplicity, we only get the file/item
			// sizes and sum them.

			// add item size to the running total.
		   size += (ULONG)MyHead->Size;

			// increment number of files if item has a filesize.
		   nfiles += (MyHead->Size) ? 1:0;
		   }

		// print subboard header text..
		sprintf(z->ABuffer, bm[2597], sub->Title, sub->SubDirName); // "Subboard: %s (%s)"
		CPutA();

		// convert size to k/M/G
		FindUnits(size, sizestr, 1, bm);

		// print stats -> items, files, size and k/M/G size as well..
		sprintf(z->ABuffer, bm[2598],  // "%d item%s, %d file%s; %ld byte%s (%s)"
                          sub->rn,
                          (sub->rn != 1) ? bm[2599]:bm[2600], // "s":""
                          nfiles,
                          (nfiles != 1) ? bm[2599]:bm[2600], // "s":""
                          size,
                          (size != 1) ? bm[2599]:bm[2600], // "s":""
                          sizestr,
                          (size != 1) ? bm[2599]:bm[2600]); // "s":""
		CPutA();

		// "exit" subboard.
		OneLessUser(sub);
		}

	return;
}



// -------------------- YOUR DOOR EXIT/CLEANUP CODE GOES HERE -------------------
void DoorExit( void )
{
	// Place any resource cleanup code, such as freeing memory, closing libs
   // opened by your door (but not cnetc.library, which is already opened) here.
   // These will be freed by the door exit code whenever exit() is called


	// close the libs we opened in DoorMain()
	if(CNetBase)
		CloseLibrary(CNetBase);
	CNetBase = NULL;

	if(CNet4Base)
		CloseLibrary(CNet4Base);
	CNet4Base = NULL;

	return;
}



// -------------- YOUR DOOR CODE EXECUTION BEGINS IN THIS FUNCTION --------------
void DoorMain( char *DosArgs )
{
	// Your door code begins here.
	// DosArgs == NULL if no ** DOS ** args were passed to door from CNet

	bm=z->bm; // set our bbstext lines pointer

	// open required libs..
	// DO NOT open/close cnetc.library..  The door construct/destruct code does that for you..
	if(!(CNetBase = OpenLibrary(CNETLIBNAME, 4L)))
		{
		CPutText("Could not open cnet.library\n");
		return;
		}
	if(!(CNet4Base = OpenLibrary(CNET4LIBNAME, 4L)))
		{
		CPutText("Could not open cnet4.library\n");
		return;
		}

	// if we have AmigaDOS args or CNet args, parse them..
	if(DosArgs || (z->npitems > 1))
		{
		if( ((physnum = CNC_FindForeignArea( DosArgs ? DosArgs:z->pitem[1])) != INVALID_SUB_PHYSNUM) &&
          (physnum != MAIN_SUB_PHYSNUM))
			{
			sp = &myp->Subboard[physnum];
			}
		else
			{
			sprintf(z->ABuffer, bm[2596], DosArgs ? DosArgs:z->pitem[1]); // "Could not find subboard with go key of %s"
			CPutA();
			return;
			}
		}

	// else check if we are IN a subboard now and default to that one..
	else if(z->sb)
		{
		sp = z->sb;
		}

	// else not in a subboard.. print verbose instruction/hint
	else
		{
		CPutText(bm[2601]); // "You must supply a subboard GO arg or be in the subboard you wish to size"
		return;
		}

	// if all went well, proceed to final checks and size function.
	SizeItems(sp);

	return;
}
