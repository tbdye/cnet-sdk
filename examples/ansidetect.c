/*****************************************************************************
 *                            CNetC library examples                         *
 *                         copyright (c) ZenMetal Software                   *
 *                              All rights reserved.                         *
 *                                                                           *
 *                 Special thanks to Kelly Cochran for his initial           *
 *                 conversion of empty.c code to a shared library.           *
 *****************************************************************************/


/*
 * NOTE: THIS CODE IS 3RD PARTY SOURCE, DISTRIBUTED BY WITH CNET AS EXAMPLE
 *       CODE.  ZENMETAL ASSUMES NO RESPONSIBILITY FOR ITS FUNCTIONALITY
 *       OR NON-FUNCTIONALITY.
 *
 */

/* Ported from SAS/C to m68k-amigaos-gcc -noixemul */

#include <stdlib.h>
#include <stdio.h>

#include <exec/types.h>
#include <dos/dos.h>
#include <cnet/cnet.h>

/* --------------------- GLOBAL VARIABLE/DATA SECTION ---------------------- */

extern struct Library         *CNetCBase;
extern struct CNetCContext    *context;
extern struct PortData        *z;
extern struct MainPort        *myp;

extern BOOL CNCL_DoorInit(int argc, char **argv);
extern void CNCL_DoorCleanup(void);

/* C* wrapper function prototypes (from libcnet_door.a) */
extern void  CPutText(char *text);
extern char  CWaitForInput(long mics);

/* --------------------- BEGINNING OF DOOR FUNCTIONS ----------------------- */

void GetOut(void)
{
   /* ------------- YOUR CLEANUP CODE STARTS HERE - IF ANY ----------------- */




   /* -------------- CNETC DOOR SHUTDOWN PROCEDURE BEGINS ------------------ */
   CNCL_DoorCleanup();
}



int main(int argc, char **argv)
{
	int  count = 0;
	char   key;
	BOOL  DONE = FALSE;

   /* ------------------ CHECK IF RAN AS A CNETC DOOR ---------------------- */
   if(!CNCL_DoorInit(argc, argv))
      {
      printf("This is a CNetC Program!\n");
      return 0;
      }
   atexit(GetOut);

   /* --------------------- YOUR CODE BEGINS HERE -------------------------- */


	z->user1.ANSI = 2;

	if ( z->LocalMode )
		{
		CPutText("n1Local Terminal");
		z->KBuff[ z->KB1 ] = 105;
		z->KB2++;
		}
	else
		{
		CPutText("[6n");
		z->KB2 = z->KB1;
		CPutText("n1Auto-detecting ANSI (SPACE=ABORT).");

		while( !DONE )
			{
			key = CWaitForInput( 200000 );
			if( key != 27 && key != 82 && key != 114 && key >= 1 )
				count--;

			if( count == 12 )
				DONE = TRUE;

			CPutText(".");

			if( key == 32 )
				{
				CPutText("*** ABORT ***");
				DONE = TRUE;
				}

			if( key == 82 || key == 114 )
				{
				CPutText("n1--> ANSI Found!");
				z->KB2 = z->KB1;
				z->KBuff[ z->KB1 ] = 105;
				z->KB2++;
				GetOut();
				}
			count++;
			}

		CPutText("n1--> Could not detect ANSI...n2");

		z->user1.ANSI = 0;
		CPutText("Terminal (A)nsi, [NONE]:");
		z->KB2 = z->KB1;
		}


   /* --------------------- END OF YOUR MAIN CODE -------------------------- */
   return 0;
}
