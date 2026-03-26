/************************************************************************/
/*                               WaitForC.c                             */
/*             pause until the users presses the required key           */
/************************************************************************/

/* Ported from SAS/C to m68k-amigaos-gcc -noixemul */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <exec/types.h>
#include <dos/dos.h>
#include <cnet/cnet.h>

#undef __asm
#include <proto/exec.h>

void		CallHost( UBYTE c );
void		PutText( char *text );
char		OneKey( void );
void		ShutDown( char *spwan );
void		GetOut( void );
void		LoadError( void );
BOOL		CarrierCheck( void );

/* amiga.lib convenience functions */
struct MsgPort *CreatePort(STRPTR name, LONG pri);
void DeletePort(struct MsgPort *port);

struct	MsgPort  *replyp;				/* Some commnunication details ...      */
struct	CPort    *cport;
struct	CMessage  cmess;
struct	MainPort  *myp;				/* Pointer to CNet port--ALL info!      */
struct	PortData  *z;
struct	SignalSemaphore *SEM;

unsigned	int	m;

char		**bm;
char		key;

int main( int argc, char **argv )
{
char WaitKey;

if( argc<2 || !(cport = (struct CPort *)FindPort( argv[1] )) )
	{
	printf("Must be ran as a C-Net Pfile!\n");
	exit(0);
	}
if( !(replyp = CreatePort( 0,0 ) ))
	exit( 0 );
cmess.cn_Message.mn_ReplyPort   = replyp;
cmess.cn_Message.mn_Length      = sizeof( struct CMessage );
cmess.cn_Message.mn_Node.ln_Name= "cstuff";
if( cport->ack != 30 )
	{	/* right CNet version running? */
	cport->ack = 1;
	LoadError();
	}
cport->ack = 0;

z		=	cport->zp;	/* this ports PortData */
myp	=	cport->myp;	/* the system's MainPort */
SEM	=	myp->SEM;	/* short hand if used often */
bm		=	z->bm;

                       /* place your code here ! */

WaitKey = 'C';
if(argc > 2)
	{
	WaitKey = toupper(argv[2][0]);
	}

sprintf(z->ABuffer, "c4Press `%c' to continue..q1", WaitKey);
PutText(z->ABuffer);
while( (key=OneKey()) != WaitKey)
	{
	if(!CarrierCheck()) GetOut();
	}

PutText("h9}h9}h5}");
GetOut();

return 0;
}

void CallHost( UBYTE c )  /* Generic interface routine used by all below */
{
	cmess.command = c;
	PutMsg  ( (struct MsgPort *)cport, (struct Message *)&cmess );
	WaitPort( replyp );
	GetMsg  ( replyp );
}

	/* in your final code, you'll want to remove any of the following */
	/* routines that you don't actually use to save some memory */

void PutText( char *text )
{
	cmess.arg1 = (ULONG)text;	/* text to print		*/
	CallHost( 1 );
}

void GetOut( void )              /* exit pfile with no spawn */
{
CallHost( 0 );
DeletePort( replyp );
exit(0);
}

void LoadError( void )
{
DeletePort( replyp );
exit(0);
}

char OneKey(void)				/* Stop until a key is pressed	*/
{
	CallHost( 3 );
	return( (char)cmess.result );
}

BOOL CarrierCheck( void )
{
if( !z->Carrier ) return FALSE;
if( !z->TimeLeft ) return FALSE;
if( z->Dumped ) return FALSE;
return TRUE;
}
