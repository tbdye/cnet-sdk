// CNet Amiga/4 Programming examples
// copyright (c) 1998 Ray A. Akey
// Exclusive license granted to ZenMetal Software and CNet door coders.

/* Ported from SAS/C to m68k-amigaos-gcc -noixemul */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <exec/types.h>
#include <dos/dos.h>
#include <cnet/cnet.h>

/* Restore __asm after <cnet/cnet.h> redefined it to nothing,
 * so GCC inline stubs (LP* macros) work for library calls. */
#undef __asm

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/cnetc.h>

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
extern UBYTE CPutQ(char *a);
extern int   CEnterLine(UBYTE len, ULONG flags, char *prompt);

short                  portnum       = -1;


void DoDOSTOD(int argc, char **argv);


/* --------------------- BEGINNING OF DOOR FUNCTIONS ----------------------- */

void GetOut(void)
{
   /* ------------- YOUR CLEANUP CODE STARTS HERE - IF ANY ----------------- */




   /* -------------- CNETC DOOR SHUTDOWN PROCEDURE BEGINS ------------------ */
   CNCL_DoorCleanup();
}



int main(int argc, char **argv)
{
   char *s;

   if(argc < 2)
      DoDOSTOD(argc, argv);
   /* ------------------ CNETC DOOR SETUP STARTS HERE ---------------------- */
   atexit(GetOut);
   if(!CNCL_DoorInit(argc, argv))
      {
      // WAS NOT RAN AS A CNETC DOOR - Use AmigaDOS shell version of
      // code instead.
      DoDOSTOD(argc, argv);
      }

   /* --------------------- YOUR CODE BEGINS HERE -------------------------- */


   // Parse input whether door is "run" from main or executed from BBSMENU
   if(argc > 2)
      portnum=atoi(argv[2]);         // AmigaDOS command line input
                                     // argv[0]    = door path/name
                                     // argv[1]    = door msg port name
                                     // argv[2]    = user argument 1
                                     // argv[x]    = user argument x
   else if(z->npitems > 1)
      portnum = atoi(z->pitem[1]);   // CNet command line input
                                     // pitem[0] = CNet command name
                                     // pitem[1] = user argument 1
                                     // pitem[x] = user argument x

   else if(z->InBuffer)              // remains of command in z->InBuffer
      {
      // parse the second argument (the first is the door command name)
      // from z->InBuffer by looking for first space
      if((s=strchr(z->InBuffer, ' ')))
         {
         s++;
         portnum = atoi(s);
         }
      }

   // initial parsing didn't reveal user command line input; use EnterLine()
   if(portnum == -1)
      {
      if(!CEnterLine(45, ELINE_INPUTBOX, "Port Number: "))
         exit(0);
      portnum=atoi(z->InBuffer);
      }

   // is screen open already?
   if(myp->PortZ[portnum]->Window)
      {
      // yes, pop to front?
      if(CPutQ("\nScreen is already open; pop to front? [Y/n] ?1"))
         ScreenToFront(myp->PortZ[portnum]->Window->WScreen);
      }
   else
      {
      // trigger screen to open
      CNC_OpenDisplay(portnum);
      sprintf(z->ABuffer, "Triggered port %d screen to open.\n", portnum);
      CPutA();
      }



   /* --------------------- END OF YOUR MAIN CODE -------------------------- */
   return 0;
}


// AmigaDOS version of Port screen open
void DoDOSTOD(int argc, char **argv)
{
   if((myp=(struct MainPort *)FindPort(CNETPORT))) // test for mainport since
      {                                          // it is slightly required here. :)
      if(argc < 2)
         {
         char buffer[10]; // this should be increased to accept a longer value but
                          // we'll assume the user KNOWS CNet only accepts 0-99.
                          // In practice, one should use a routine that LIMITS the
                          // number of input characters to the length of the input
                          // buffer (minus 1)..  but that is beyond the scope of
                          // this example.

         printf("Port number: ");
         fgets(buffer, sizeof(buffer), stdin);
         portnum = atoi(buffer);
         }
      else
         portnum = atoi(argv[1]);

		// Is window already open?
      if(myp->PortZ[portnum]->Window)
         {
         // yes, pop to front?
         printf("Screen is already open; pop to front? [y/n] ");
         if(tolower(getchar()) == 'y')
            {
            printf("\n");
            ScreenToFront(myp->PortZ[portnum]->Window->WScreen);
            }
			else printf("\n");
         }
      else
         {
         // no, trigger it to open! It will open it on the next service
         // call - usually in 3 second intervals.
         myp->pc[ portnum ].open |= 1;
         myp->PortZ[portnum]->WantToOpen = TRUE;

         printf("Triggered port %d screen to open\n", portnum);
         }
      }
   exit(0);
}
