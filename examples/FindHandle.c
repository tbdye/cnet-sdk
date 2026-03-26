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

/* Restore __asm after <cnet/cnet.h> redefined it to nothing,
 * so GCC inline stubs (LP* macros) work for library calls. */
#undef __asm

#include <proto/exec.h>
#include <proto/cnet.h>

/* --------------------- GLOBAL VARIABLE/DATA SECTION ---------------------- */

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

struct Library         *CNetBase     = NULL; // pointer to cnet.library lib base

/* --------------------- BEGINNING OF DOOR FUNCTIONS ----------------------- */

void GetOut(void)
{
   /* ------------- YOUR CLEANUP CODE STARTS HERE - IF ANY ----------------- */
   if(CNetBase)
      {
      CloseLibrary(CNetBase);
      CNetBase=NULL;
      }

   /* -------------- CNETC DOOR SHUTDOWN PROCEDURE BEGINS ------------------ */
   CNCL_DoorCleanup();
}



int main(int argc, char **argv)
{
	short index;
	char handlestr[22];

   /* ------------------ CHECK IF RAN AS A CNETC DOOR ---------------------- */
   if(!CNCL_DoorInit(argc, argv))
      {
      printf("This is a CNetC Program!\n");
      return 0;
      }
   atexit(GetOut);

   if(!(CNetBase = OpenLibrary("cnet.library", 4)))
      exit(RETURN_FAIL);

   /* --------------------- YOUR CODE BEGINS HERE -------------------------- */


	CEnterLine(21, 0, "Enter handle to find: ");
	strcpy(handlestr, z->InBuffer);
	CEnterLine(4, 0, "Enter optional serial id: ");
	if(FindHandle(&index, handlestr, atoi(z->InBuffer)))
		{
		sprintf(z->ABuffer, "\nindex: %d\n", index);
		CPutA();
		sprintf(z->ABuffer, "\nmyp->IName[%d]: %d\n", index, myp->IName[index]);
		CPutA();

		sprintf(z->ABuffer, "\nmyp->IName[%d]: %d\n", index, myp->IName[index]);
		CPutA();

		sprintf(z->ABuffer, "\nmyp->Key[ %d ].Handle: %s\n", (int)(myp->IName[index]-1), myp->Key[ myp->IName[index]-1 ].Handle);
		CPutA();
		}
	else
		{
		CPutText("\nNot found!\n");
		}




   /* --------------------- END OF YOUR MAIN CODE -------------------------- */
   return 0;
}
