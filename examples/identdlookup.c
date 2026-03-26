/************************************************************************
 *    CNet/3 and CNet/4  C language interface routines and examples     *
 *                                                                      *
 *  copyright (c) 1996 ZenMetal Software ... this code may be freely    *
 *  distributed to and used by registered CNet owners EXCLUSIVELY.      *
 *  Other distribution is in violation of copyright laws.               *
 ************************************************************************/

/* Ported from SAS/C to m68k-amigaos-gcc -noixemul */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <exec/types.h>
#include <exec/lists.h>
#include <exec/ports.h>
#include <exec/semaphores.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include <graphics/gfx.h>
#include <graphics/rastport.h>
#include <graphics/text.h>
#include <intuition/intuition.h>
#include <libraries/iffparse.h>

/* SAS/C compat macros for CNet struct headers */
#ifndef __saveds
#define __saveds __attribute__((saveds))
#endif
#ifndef __asm
#define __asm
#endif
#ifndef __regargs
#define __regargs
#endif
#ifndef __stdargs
#define __stdargs
#endif

/* CNet data structure headers (individual, not cnet.h master,
 * to avoid pulling in inline library stubs that conflict with
 * this file's own CallHost-based wrapper functions) */
#include "bbslist.h"
#include "bbstext.h"
#include "io.h"
#include "subs.h"
#include "charges.h"
#include "config.h"
#include "control.h"
#include "dates.h"
#include "dcc.h"
#include "doors.h"
#include "editor.h"
#include "fido.h"
#include "files.h"
#include "ftp.h"
#include "getusers.h"
#include "internet.h"
#include "manager.h"
#include "mci.h"
#include "olms.h"
#include "other.h"
#include "ranges.h"
#include "rooms.h"
#include "users.h"
#include "vde.h"
#include "yank.h"
#include "ports.h"
#include "winscreen.h"
#include "telnet.h"
#include "news.h"
#include "convert.h"
#include "sysfiles.h"
#include "smtp.h"
#include "clipboard.h"
#include "menu.h"

#undef __asm
#include <proto/exec.h>
#include <proto/dos.h>

/* This file defines its own CallHost-based wrappers for many cnet.library
 * functions (ConvertAccess, MakeDate, etc.), so we cannot include
 * proto/cnet.h which would define conflicting macros.  Instead, we
 * manually declare only the cnet.library functions actually needed. */
#ifndef __NOLIBBASE__
extern struct Library *CNetBase;
#endif
#include <inline/macros.h>
#ifndef CNET_BASE_NAME
#define CNET_BASE_NAME CNetBase
#endif
#define LookupIdentd(Socket) \
	LP1(0x114, short, LookupIdentd, ULONG, Socket, d0, \
	, CNET_BASE_NAME)

/* amiga.lib convenience functions */
struct MsgPort *CreatePort(STRPTR name, LONG pri);
void DeletePort(struct MsgPort *port);

/************************************************************************
 *                          Function prototypes                         *
 ************************************************************************/
void		CallHost( UBYTE c );
void		PutText( char *text );
void		PutA( void );
int		EnterLine( UBYTE len, ULONG flags, char *prompt );
void		ShutDown( char *spawn );
void		GetOut( void );
int		intGetOut( void );
void		LoadError( void );

void DisplayIdentD( void );


/************************************************************************
 *                           Global Variables                           *
 ************************************************************************/
struct MsgPort         *replyp;	/* Some communication details ...	*/
struct CPort           *cport;
struct CMessage        cmess;
struct MainPort        *myp;		/* Pointer to CNet port--ALL info!	*/
struct PortData        *z;
char	                 **bm;
struct Library         *CNetBase = NULL;
struct SignalSemaphore *SEM;



/************************************************************************
 *                             Main routine                             *
 ************************************************************************/
int main( int argc, char **argv )
{
	Forbid();
	if(argc > 1) cport = (struct CPort *)FindPort( argv[1] );
	Permit();

	if( argc<2 || !(cport) ) {
		printf("This is a CNet C program.\n");
		exit(0);
		}

	if( !(replyp = CreatePort( 0,0 )))
		exit(0);

	cmess.cn_Message.mn_ReplyPort   = replyp;
	cmess.cn_Message.mn_Length      = sizeof( struct CMessage );
	cmess.cn_Message.mn_Node.ln_Name= "cstuff";

	// CNet version check
	if( cport->ack != 30 ) {
		cport->ack = 1;
		LoadError();
		}

	cport->ack = 0;

	z    = cport->zp;
	myp  = cport->myp;
	SEM  = myp->SEM;
	bm   = z->bm;

	if( !(CNetBase = OpenLibrary( "cnet.library", 4 )) ) // <- NOTE version 4 - can be changed to 3 to make compatible with older CNet releases!
		LoadError();

	atexit(GetOut); // inserts an exit trap.  When exit() is called, the
                   // code in GetOut() is invoked.

	// SAS/C onbreak() replaced by signal handler or simply removed.
	// The atexit(GetOut) handler covers cleanup on exit.

/************************************************************************
 *           End of CNet setup - YOUR CUSTOM CODE STARTS HERE           *
 ************************************************************************/

	DisplayIdentD();

	if(EnterLine(8, ELINE_INPUTBOX, "Socket number to lookup: "))
		{
		short port=-1;
		port=LookupIdentd(atol(z->InBuffer));
		sprintf(z->ABuffer, "Socket %s used by %d\n", z->InBuffer, port);
		PutA();
		}

/************************************************************************
 *                         exit back to CNet!
 ************************************************************************/

	// note that an exit trap calling GetOut() has been inserted previously to
	// facilitate resource cleanup..
	exit(0);

	return 0;
}


/**************************************************************************
 *       Routine called if Load error (wrong CNet version, etc,...        *
 **************************************************************************/
void LoadError( void )
{
	if(replyp)
		{
		// Use while(GetMsg(replyp) here to get any pending msgs before deleting port!
		DeletePort( replyp );
		}
	exit(0);
}



/**************************************************************************
 *       An added function to call GetOut() for break support             *
 **************************************************************************/
int intGetOut( void )
{
	exit(0);
	return 0;
}


/************************************************************************
 *                           Generic EXIT code                          *
 ************************************************************************/
void GetOut( void )
{
	ShutDown( NULL );

	// clean up message ports opened by this door.
	if(replyp)
		{
		// delete the port that CNet doors open for CallHost() messages
		DeletePort( replyp );
		}

}


/**************************************************************************
 *                         another file to run?                           *
 **************************************************************************/
void ShutDown( char *spawn )
{
	/* spawn = full path/file to run */
	if( spawn )
		strcpy( z->CSpawn, spawn );

	CallHost( 0 );
}


void CallHost( UBYTE c )
{
	cmess.command = c;
	PutMsg  ( (struct MsgPort *)cport, (struct Message *)&cmess );
	WaitPort( replyp );
	GetMsg  ( replyp );
}

void PutText( char *text )
{
	cmess.arg1 = (ULONG)text;	/* text to print		*/
	CallHost( 1 );
}

void PutA( void )
{
	PutText( z->ABuffer );
}


/**************************************************************************
 *    ENTERLINE FLAGS:                                                    *
 *    Construct by ORing the following bit defines:                       *
 *                                                                        *
 * ie. EnterLine(3, ELINE_ALLCAPS|ELINE_INPOUTBOX, "My Prompt>");         *
 *                                                                        *
 *     FLAGS:                                                             *
 *     ------                                                             *
 *     ELINE_ALLCAPS       = All capitalized                              *
 *     ELINE_FILENAME      = FILENAME.  Don't allow =":; or asterisk      *
 *     ELINE_USEINBUFF     = Begin with existing z.InBuffer               *
 *     ELINE_NOLEADSPACE   = Chop leading spaces                          *
 *     ELINE_CAPWORDSTART  = Force 1st letter of word caps                *
 *     ELINE_RESTWORDLOWER = Force all others lower case                  *
 *     ELINE_NUMBERSONLY   = Numeric input only                           *
 *     ELINE_INPUTBOX      = Print input box (terminated with . )         *
 *     ELINE_ALLOWMCI      = DO allow MCI                                 *
 *     ELINE_HANDLESPECIAL = HANDLES/SPECIAL.  Don't allow ^_`{|}~@       *
 *     ELINE_XSLASHSTART   = Exit for . or / at beginning of line         *
 *     ELINE_XBKSPACESTART = Exit for backspace at beginning of line      *
 *     ELINE_NOOLMS        = Do not allow OLM's to appear while editing   *
 *     ELINE_ALLOWCHAT     = Allow Chat break in at this prompt. COMMAND  *
 *                           PROMPT.                                      *
 *     ELINE_NOSPACES      = Don't allow SPACES                           *
 *     ELINE_NOCURSORMOVE  = DON'T ALLOW MOVEMENT                         *
 *     ELINE_NOSLASHES     = Don't allow forward slash                    *
 *                                                                        *
 *  NOTE: The old "numeric" flag values still work 100% ..                *
 **************************************************************************/
int EnterLine( UBYTE len, ULONG flags, char *prompt )
{
	cmess.arg1 = (ULONG)len;     /* how many chars max to input	*/
	cmess.arg2 = (ULONG)flags;   /* flags describing required input handling */
	cmess.arg3 = (ULONG)prompt;  /* text to print before input	*/
	CallHost( 2 );               /* result is in z->InBuffer	*/

	return( (int)strlen( z->InBuffer ));
}


/**************************************************************************
 *                       Stop until a key is pressed                      *
 **************************************************************************/
char OneKey( void )
{
	CallHost( 3 );
	return( (char)cmess.result );	/* function returns ASCII value of key pressed */
}


/**************************************************************************
 *
 **************************************************************************/
void EnterPassword( UBYTE len )
{
	cmess.arg1 = (ULONG)len;	/* max number of characters */
	CallHost( 4 );
}

/**************************************************************************
 *                 Check z->InBuffer for Chat, OLM, etc                   *
 **************************************************************************/
long CommonCommands( void )
{
	CallHost( 5 );
	return( (long)cmess.result );
}


/**************************************************************************
 *
 **************************************************************************/
UBYTE ReadFile( char *path, UBYTE flags )
{
	cmess.arg1 = (ULONG)path;
	cmess.arg2 = (ULONG)flags;          /* 1 = print File Not Found */
	CallHost( 6 );
	return( (UBYTE)cmess.result );      /* returns FALSE if File Not Found */
}

/**************************************************************************
 *        Sets the "Action" or "Where" field for the user's port          *
 *      ** remember to retain and restore the previous z->DOING **        *
 **************************************************************************/
void SetDoing( char *what )
{
	cmess.arg1 = (ULONG)what;
	CallHost( 7 );
}


/**************************************************************************
 *Invokes the CNet editor according to user's preference of line or Visual*
 **************************************************************************/
void CallEditor( short max, short inlines )
{
	cmess.arg1 = (ULONG)max;	/* Maximum number of lines (250)*/
	cmess.arg2 = (ULONG)inlines;	/* TRUE/FALSE use existing _edbuff? */
	CallHost( 8 );
}

/**************************************************************************
 *             Read text file with MCI/graphic interpretation             *
 **************************************************************************/
UBYTE ReadGraphics( char *path, char flags )
{
	cmess.arg1 = (ULONG)path;
	cmess.arg2 = (ULONG)flags;	/* 1 = print File Not Found	*/
	CallHost( 9 );
	return( (UBYTE)cmess.result );		/* FALSE if File Not Found	*/
}

/**************************************************************************
 *  Construct a date in CNet format - like that seen in the port titlebar *
 **************************************************************************/
void MakeDate( struct IsDate *date, char *output )
{
	cmess.arg1 = (ULONG)date;
	cmess.arg2 = (ULONG)output;
	CallHost( 10 );
}


/**************************************************************************
 *                     Load userdata for account "id"                     *
 **************************************************************************/
UBYTE ReadAccount( short id, struct UserData *user )
{
	cmess.arg1 = (ULONG)id;
	cmess.arg2 = (ULONG)user;
	CallHost( 11 );
	return( (UBYTE)cmess.result );
}

/**************************************************************************
 *           Saves an account after editing/changing attributes           *
 **************************************************************************/
UBYTE SaveAccount( struct UserData *user, short id )
{
	cmess.arg1 = (ULONG)user;
	cmess.arg2 = (ULONG)id;
	CallHost( 12 );
	return( (UBYTE)cmess.result );
}

/**************************************************************************
 *         add charge amount "a" to charge schedule number "n"            *
 **************************************************************************/
UBYTE AddCharge( short n, short a )
{
	cmess.arg1 = (ULONG)n;
	cmess.arg2 = (ULONG)a;
	CallHost( 13 );
	return( (UBYTE)cmess.result );
}

/**************************************************************************
 *
 **************************************************************************/
UBYTE CheckBalance( short n, short a )
{
	cmess.arg1 = (ULONG)n;
	cmess.arg2 = (ULONG)a;
	CallHost( 14 );
	return( (UBYTE)cmess.result );
}

/***************************************************************************
 * Get text from user.  Results are placed in z.GBuffer[] array of strings *
 * note that there are 16 lines MAX and each line may hold up to 80        *
 * characters                                                              *
 ***************************************************************************/
int EnterText( char firstchar, short maxchars, short perline, short maxlines )
{
	cmess.arg1 = (ULONG)firstchar;
	cmess.arg2 = (ULONG)maxchars;
	cmess.arg3 = (ULONG)perline;
	cmess.arg4 = (ULONG)maxlines;
	CallHost( 15 );
	return( (int)cmess.result );
}

/**************************************************************************
 *
 **************************************************************************/
long ConferenceWait( short a )
{
	cmess.arg1 = (ULONG) a;
	CallHost( 16 );
	return( (long)cmess.result );
}


/****************************************************************************
 * Update the current time, load the user's bbstext translation, charges    *
 * If the user has little time remaining, print "you have xx minutes left", *
 * force Events to be checked for pending event execution..                 *
 ****************************************************************************/
void CheckChanges( void )
{
	CallHost( 17 );
}


/**************************************************************************
 *      Converts a CNet access/range string to a packed LONG value        *
 **************************************************************************/
long ConvertAccess( char *s )
{
	cmess.arg1 = (ULONG)s;
	CallHost( 18 );
	return( (long)cmess.result );
}


/**************************************************************************
 * return the number of bytes free on device specified by s               *
 * if q==0, the result is displayed to the user by GetFree()              *
 **************************************************************************/
long GetFree( char *s, UBYTE q )
{
	cmess.arg1 = (ULONG)s;
	cmess.arg2 = (ULONG)q;
	CallHost( 19 );
	return( (long)cmess.result );
}

/**************************************************************************
 *  Forces CNet to flush/check it's buffers and parse/update the keyboard *
 *  buffers                                                               *
 **************************************************************************/
void CheckFlowControl( void )
{
	CallHost( 21 );
}

/**************************************************************************
 * list the contents of a directory (the user will be prompted for        *
 * directory name).                                                       *
 *                                                                        *
 * options: a = allow selection of files to user's select list            *
 *                                                                        *
 *          b = 1: select and download immediately                        *
 *              2: allow wildcard pattern                                 *
 *                                                                        *
 *          c = list files newer than the date specified by c             *
 *              c is a properly filled out Cnet IsDate structure          *
 **************************************************************************/
long ListDir( UBYTE a, UBYTE b, struct IsDate *c )
{
	cmess.arg1 = (ULONG)a;
	cmess.arg2 = (ULONG)b;
	cmess.arg3 = (ULONG)c;
	CallHost( 22 );
	return( (long)cmess.result );
}


/**************************************************************************
 * Read the next base/udbase message                                      *
 **************************************************************************/
UBYTE Rnext( void )
{
	CallHost( 24 );
	return( (UBYTE)cmess.result );
}

/***************************************************************************
 * Parse the contents of z->InBuffer into z->pitems.  Parses up to numargs *
 * items                                                                   *
 ***************************************************************************/
void ParseCommandLine( UBYTE numargs )
{
	cmess.arg1 = (ULONG)numargs;
	CallHost( 25 );
}


/**************************************************************************
 *  Takes the contents of z->InBuffer and searches for a matching command *
 *  in menu "num" of BBSMENU.                                             *
 **************************************************************************/
short FindCommand( short num )
{
	cmess.arg1 = (ULONG) num;
	CallHost( 26 );
	return( (short)cmess.result );
}

/**************************************************************************
 * Open the filename specified by a, seek to position b in the file       *
 * and display the contents of the file from point b to EOF               *
 **************************************************************************/
void ReadMessagePoint( char *a, long b )
{
	cmess.arg1 = (ULONG) a;
	cmess.arg2 = (ULONG) b;
	CallHost( 27 );
}

/**************************************************************************
 * Use the CNet editor to edit the file specified by "file"               *
 **************************************************************************/
void EditMessage( char *file )
{
	cmess.arg1 = (ULONG) file;
	CallHost( 28 );
}

/***************************************************************************
 * Load the text from the file-handle given and insert it into the current *
 * port's editor file                                                      *
 ***************************************************************************/
void LoadText( BPTR fh )
{
	cmess.arg1 = (ULONG) fh;
	CallHost( 29 );
}


/**************************************************************************
 *	1000000 mics = 1 second                                                *
 **************************************************************************/
char WaitForInput( long mics )
{
	cmess.arg1 = (ULONG) mics;
	CallHost( 31 );
	return( (char)cmess.result );
}


/**************************************************************************
 *                       Select and download a file.                      *
 *                                                                        *
 * file  = full path/filename                                             *
 *                                                                        *
 * flags = 0 -> select without immediate download                         *
 *       = 1 -> select and download immediately                           *
 *                                                                        *
 * flag values below are new for v4.11                                    *
 *                                                                        *
 *       = 2 -> delete file after downloading/no immediate download       *
 *       = 3 -> delete file after downloading/download immediate.         *
 **************************************************************************/
UBYTE SelectAndDownload( char *file, UBYTE flags )
{
	cmess.arg1 = (ULONG)file;
	cmess.arg2 = (ULONG)flags;
	CallHost( 39 );
	return( (UBYTE)cmess.result );
}


/**************************************************************************
 * file: the ".vde" filename, without the ".vde"!                         *
 * data: pointer to the structure you are going to edit                   *
 * size: structure length in bytes                                        *
 *                                                                        *
 * returns: TRUE  if structure has been changed                           *
 *          FALSE otherwise                                               *
 **************************************************************************/
short VisualDataEditor( char *file, void *data, long size )
{
	cmess.arg1 = (ULONG)file;
	cmess.arg2 = (ULONG)data;
	cmess.arg3 = (ULONG)size;
	CallHost( 40 );
	return( (short)cmess.result );
}

/**************************************************************************
 * In preparation for an ExtUpload, this function                         *
 * sets the minimum number of free bytes to maintain on the	drive.        *
 **************************************************************************/
void ExtSetMinFree( long free )
{
	cmess.arg1 = (ULONG)free;
	CallHost( 42 );
}


/**************************************************************************
 * In preparation for an ExtDownload or an ExtUpload, this function       *
 * sets the protocol to be used.  If you send NULL, it will allow the     *
 * user to choose his OWN protocol.                                       *
 *                                                                        *
 * Otherwise, you may select 'a' to be the first letter of a valid        *
 * system protocol (from BBSPROTO file), such as 'x', 'z', etc.           *
 *                                                                        *
 * TRUE will be returned if a protocol is selected and ready, FALSE       *
 * if there is a problem.                                                 *
 **************************************************************************/
UBYTE ExtSetProtocol( char a )
{
	cmess.arg1 = (ULONG)a;
	CallHost( 43 );
	return( (UBYTE)cmess.result );
}



/**************************************************************************
 * This routine allows the user to download the SINGLE file specified     *
 * by the FULL PATH 'args'.                                               *
 *                                                                        *
 *	Currently, NULL is always returned.                                    *
 **************************************************************************/
char *ExtDownload( char *args )
{
	cmess.arg1 = (ULONG)args;
	CallHost( 44 );
	return( (char *)cmess.result );
}


/**************************************************************************
 * This routine allows the user to upload the file specified by           *
 * 'args'.  The path for uploading will be taken from the path            *
 * in 'args'.  If you do NOT specify a path, the file(s) will             *
 * appear in the user's HOME directory.                                   *
 *                                                                        *
 * Note that with batch protocols like ZMODEM, the filename(s) are        *
 * taken from the header packet information, and may NOT be the           *
 * same as what you have requested the user upload.  For this reason,     *
 * you should have uploads occur in a TEMP directory, and search that     *
 * directory yourself for new files.                                      *
 *                                                                        *
 * Currently, NULL is always returned.                                    *
 **************************************************************************/
char *ExtUpload( char *args )
{
	cmess.arg1 = (ULONG)args;
	CallHost( 45 );
	return( (char *)cmess.result );
}

/**************************************************************************
 * Write myp->Key[] to bbs.ukeys in a CNet friendly manner
 * getsem: 0 = do not lock the key semaphore
 *         1 = lock the key semaphore before writing
 *
 * the key semaphore is myp->SEM[1] so, if you have already performed
 * ObtainSemaphore(myp->SEM[1]), then use WriteBBSKeys(0)
 * if you have not locked the semaphore, use WriteBBSKeys(1)
 *
 * notes:
 *
 * if a user's account data is altered, you must also copy the
 * handle, MailID (AKA UUCP ID), Real Name, etc, to myp->Key[AccountNumber]
 *
 * Please see the structure definition for KeyElement4 in users.h to
 * see what data is saved to myp->Key[x]
 *
 * After doing this, the key array must be written to bbs.ukeys.  This
 * function facilitates all the grunt work of writing the key array
 * in the proper size.
 *
 **************************************************************************/
BYTE WriteUKeys( BYTE getsem )
{
	CallHost( 58 );
	return( (BYTE)cmess.result );
}

void DoANSI( UBYTE n, USHORT a, USHORT b)
{
	cmess.arg1 = (ULONG)n;
	cmess.arg2 = (ULONG)a;
	cmess.arg3 = (ULONG)b;
	CallHost( 59 );
	return;
}

void DoANSIOut( UBYTE n)
{
	cmess.arg1 = (ULONG)n;
	CallHost( 60 );
	return;
}


/**************************************************************************
 * Print the prompt string passed as "a" and return the user's            *
 * YES or NO equivalent result.  1=YES, 2=NO                              *
 **************************************************************************/
UBYTE PutQ( char *a )
{
	PutText( a );
	return (UBYTE)(z->MCIcreg[0][0]=='1') ;
}


/**************************************************************************
 * Isn't it obvious enough? ;-)
 **************************************************************************/
void DoReturn( void )
{
	PutText("\n");
}


/**************************************************************************
 * Create the editor filename used for the current port and place it      *
 * in the string array passed as "path"                                   *
 **************************************************************************/
void MakeEd( char *path )
{
	sprintf( path, "%s_edbuff%d", myp->gc.ZIPpath, z->InPort );
}


/**************************************************************************
 * Delete the editor file                                                 *
 **************************************************************************/
void DeleteEd( void )
{
	char	filename[80];

	MakeEd    ( filename ) ;
	DeleteFile( filename ) ;
}

/**************************************************************************
 * Open the file used for the editor and return the filehandle (BPTR)     *
 **************************************************************************/
BPTR OpenEd( long mode )
{
	char	filename[80];

	MakeEd( filename );

	return Open( filename, mode );
}

/**************************************************************************
 * insert the contents of the file belonging to filehandle "fp" into      *
 * the current port's editor file                                         *
 **************************************************************************/
void PrepEditor( BPTR fp )
{
	BPTR	kp;
	char	buff[100];

	if( fp ) {
		if( kp = OpenEd( MODE_NEWFILE ) ) {
			while( FGets( fp, buff, 82 ) && buff[0]!=26 )
			       FPuts( kp, buff     ) ;

			Close( kp );
		}
	}
	else	DeleteEd();
}

/**************************************************************************
 * Save the contents of the current port's editor file into the file      *
 * belonging to filehandle "fp"                                           *
 **************************************************************************/
void SaveEditor( BPTR fp, UBYTE eof )
{
	BPTR	kp;
	char	buff[100];

	if( kp = OpenEd( MODE_OLDFILE ) ) {
		while( FGets( kp, buff, 82 ) && buff[0]!=26 )
		       FPuts( fp, buff     ) ;

		Close( kp );

		DeleteEd();
	}

	if( eof ) FPuts( fp, "\032\n" );
}


void DisplayIdentD( void )
{
	if(myp->MPE->idd)
		{
		struct IdentdData *iddptr=myp->MPE->idd;
		// walk the list of identd data and print specifics
		while(iddptr)
			{
			sprintf(z->ABuffer, "Socket number: %ld, CNet Owner Port: %ld\n", iddptr->socket, (long)iddptr->port);
			PutA();
			iddptr=iddptr->next;
			}
		}
	else
		{
		PutText("No identd entries\n");
		}
}
