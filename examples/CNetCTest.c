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

/* Restore __asm after <cnet/cnet.h> redefined it to nothing,
 * so GCC inline stubs (LP* macros) work for library calls. */
#undef __asm

#include <proto/exec.h>
#include <proto/dos.h>
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
extern void CNCL_Printf(char *fmt, ...);

/* C* wrapper function prototypes (from libcnet_door.a) */
extern void  CPutText(char *text);
extern void  CPutA(void);
extern char  COneKey(void);
extern int   CEnterLine(UBYTE len, ULONG flags, char *prompt);
extern UBYTE CPutQ(char *a);
extern UBYTE CPutQMult(char *prompt, char *poss);
extern void  CDoReturn(void);
extern char  CWaitForInput(long mics);
extern void  CCallEditor(short max, short inlines);
extern UBYTE CReadFile(char *path, UBYTE flags);
extern void  CLoadText(BPTR fh);
extern void  CMakeEd(char *path);

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
		CloseLibrary(CNet4Base);

	return;
}



// -------------- YOUR DOOR CODE EXECUTION BEGINS IN THIS FUNCTION --------------
void DoorMain( char *DosArgs )
{
	// Your door code begins here.
	// DosArgs == NULL if no ** DOS ** args were passed to door from CNet

	LONG zzz=0;
	char date[40];

	if(!(CNet4Base = OpenLibrary("cnet4.library", 4L)))
		exit(RETURN_FAIL);

	CPutText("\n");
	sprintf(z->ABuffer, "   Your Handle: %s\n", z->user1.Handle);
	CPutA();
	sprintf(z->ABuffer, "Your Real name: %s\n", z->user1.RealName);
	CPutA();
	sprintf(z->ABuffer, "      Your age: %d\n\n", CalculateAge(&z->user1));
	CPutA();


	// ------------- MAKE PRINTABLE DATE OUT OF CNET IsDate STRUCTURE. --------------

	// CNetPrintDate() is a cnet4.library function.
	//
	CNetPrintDate(&z->user1.LastCall, date, NULL, NULL);
	sprintf(z->ABuffer, "     Last Call: %s\n", date);
	CPutA();
	CNetPrintDate(&z->user1.Birthdate, date, NULL, NULL);
	sprintf(z->ABuffer, "     Birthdate: %s\n\n", date);
	CPutA();


	// --------------------------- SHOW LOADED PORTS. -----------------------------

	CPutText("Loaded ports\n---------------------\n");
	for(zzz=0;zzz<100;zzz++)
		{
		if(IsPortLoaded(zzz))
			{
			sprintf(z->ABuffer, "%3d: %s\n", (int)zzz, IsPortOccupied(zzz) ? myp->PortZ[zzz]->user1.Handle : "[IDLE]");
			CPutA();
			}
		}


	//  ---- SIMPLY PRINT A NEWLINE - SAME AS CPutText("\n") OR CPutText("n1") ---

	CDoReturn();


	// ---------------- ASK USER IF HE/SHE WANTS TO DO AN EDITOR TEST. -------------

	if(CPutQ("Do editor test now? [Y/n] ?1"))
		{
		// YES!

			//  ------ sample editor usage - load systext:menu/main into editor first. ---
			{
			BPTR fh;
			char edfile[128];

			// open existing source file
			if((fh=Open("systext:menu/main", MODE_OLDFILE)))
				{
				// fill editor with contents of file belonging to file handle "fh".

				// CLoadText() loads file contents into the editor buffer.
				CLoadText(fh);

				// Allow user to edit editor file.
				// allow 255 lines and use existing _edbuffX file contents.
				CCallEditor(255, TRUE);

				// create the CNet editor filename for the current port
				// CMakeEd() is a linker library function
				CMakeEd(edfile);

				// read the editor file and print file not found if it does not exist.
				CReadFile(edfile, TRUE);

				// close the source file.
				Close(fh);
				}
			}

		}
	else
		{
		// NO!
		CPutText("Editor test skipped\n");
		}


	// ------------------------------- ENTERLINE USAGE -----------------------
	//
	{
	BOOL done = FALSE;

	while(!done)
		{
		if(CEnterLine(10, ELINE_INPUTBOX, "\nEnter 'YES' or 'NO': ")) // see io.h for enterline flags
			{
			if(!stricmp(z->InBuffer, "YES"))
				{
				CPutText("Alright, you agree - so what?!\n");
				done = TRUE; // flag to quit loop
				}
			else if(!stricmp(z->InBuffer, "NO"))
				{
				CPutText("Awwwwww, you suck!\n");
				done = TRUE; // flag to quit loop
				}
			else
				{
				CPutText("Uhmmm..  You DO know english, correct?\n");
				}
			}
		else
			{
			CPutText("I said 'YES' or 'NO'!!\n");
			}
		}
	}


	// ---------------------------- ONEKEY USAGE ---------------------------
	{
	char vowels[] = "aeiou\0"; // string of characters
	short zzz2=0; // temp variable
	char key;
	char choices[2] = { 0,0 };

	while(vowels[zzz2]) // while a valid character is indexed
		{
		do
			{
			// prepare a string to be printed in z->ABuffer
			sprintf(z->ABuffer, "\nPress '%c': ", vowels[zzz2]);

			// Print z->ABuffer
			CPutA();

			// get a key from the user
			// note that by default, COneKey returns UPPERCASE character input.
         // to make it return lowercase input, set z->NoCaseConvert = TRUE
         // and then set it back after getting your key input.

			z->NoCaseConvert = TRUE;
			key=COneKey();
			z->NoCaseConvert = FALSE;

			} while(key != vowels[zzz2]);
		zzz2++;
		}
	CPutText("\nGood, you got through all the vowels!\n");

	// a similar version of the above code, using CPutQMult()

	zzz2=0;
	while(vowels[zzz2]) // while a valid character is indexed
		{
		// prepare a string to be printed in z->ABuffer
		sprintf(z->ABuffer, "\nPress '%c': ", vowels[zzz2]);
		choices[0] = vowels[zzz2];

		// Print z->ABuffer
		z->NoCaseConvert = TRUE;
		key=CPutQMult(z->ABuffer, choices);

		sprintf(z->ABuffer, "User pressed '%c'", choices[key-1]);
		CPutA();

		z->NoCaseConvert = FALSE;
		zzz2++;
		}
	CPutText("\nGood, you got through all the vowels!\n");
	}


	return;
}
