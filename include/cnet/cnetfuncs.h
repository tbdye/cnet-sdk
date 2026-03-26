#ifndef CNET_CNETFUNCS_H
#define CNET_CNETFUNCS_H

#include <exec/types.h>

void YankQueue( void );
short SkipDrop( short r, short d, short count );
//char RexxSetRootName( char *s );
char *RexxModem( char *s );
void UploadQWK( void );
void ManageFile0( void );
void UnManageFile0( void );
void CopyCDROM( struct SelectType *select, UBYTE create );
void RequestFiles( void );
void ListMembers( void );
void DirectFileLoop( void (*dfunct)( char *name ) );
void PostNetwork( void );
void SetHeaderDates( void );
UBYTE CanPostUUCP( void );
void FormatFidoEd( struct FidoNet *fidonet, struct FidoType *net, short sig, char *area );
void FormatVanillaEd( char *group, char *subject, char *dist, char *organ, UBYTE signum );
short FindItem( long number );
UBYTE TempMoveToSub( short sub, long item, UBYTE show );
void ReturnToSub( short list0, short br0 );
void VDEclose( void );
void VDEapply( void *data0, void *data1 );
void AutoJoinLink( char *dial );
void JoinLink( void );
void DropDTR( void );
UBYTE SaveAccount( struct UserData *user, short id );
UBYTE ReadAccountLock( short id, struct UserData *user );
UBYTE ReadAccount( short id, struct UserData *user );


void Squery( struct RangeContext *rc, char *command );
UBYTE TimedWaitIO( struct IORequest *areq, ULONG wbit, ULONG secs );
void LoadInfo( UBYTE close );

void YMarkRec( BPTR kp, long pos, struct HeaderType *head );
void SetDownloadCredits( void );
void AdoptQWK( void );
short IsAMember( char *name );
void GetBaseUser( short id, struct BaseUser *bp );
void PutBaseUser( short id, struct BaseUser *bp );

BPTR OpenPost( BPTR was );
void EditItems( void );
UBYTE EditOne( long r );
void QuickSearch( void );
UBYTE DupCheck( char *name );

char *RexxGetUser( char *arg );
char *RexxPutUser( char *arg );

UBYTE ReadHeader( BPTR fh, long pos, struct HeaderType *head );
void BaseSaveEditor( BPTR fh, short num );
long SizeOfMessage( short num );
long SaveText( long prev, short sig );
long SaveResponse( void );
long SaveItem( short sig );
long KillResponse( long pos );
long NewText( long pos, short sig );
long EditResponse( long pos );

void MarkReceived( char *path, long pos );
long LocateResponse( BPTR fh, long resp );
void GetAllItems( void );
void AdoptLocalDefaults( void );

void ReadM0( void );
void SetSubStuff( short base );
void DisplaySubPath( void );
void ShowGlobalPath( void );

void ReadMarked( void );
void ExtSetMinFree( long free );
UBYTE ExtSetProtocol( char a );
char *ExtDownload( char *args );
char *ExtUpload( char *args );

void FixCase( ULONG opts );
void ChangeExchange( void );
void SetCapture( char *uucpid);
void OpenCapture( char force, char *uucpid );
short SetSigNumber( short num );
void OpenYank( short parm );
void ReadOne( void );
void GetMessage( long b );
void Rquery( char *command, UBYTE deftype );
UBYTE GoItem( UBYTE nof, UBYTE file );
UBYTE ReadJoined( struct BaseUser **p, short r );
UBYTE ChangeDate( struct IsDate *date );
void ChangeNew( void );
UBYTE Rsetup( UBYTE deftype, UBYTE chrono );
UBYTE Rnext( void );
void AMLogWrite( char *line );
void ArrangeItemPointers( void );
UBYTE SearchOne( void );

void MultiMail( void );
void ExitSubboard( UBYTE quiet );

void SendToMailBox( void );
void ScanForNewFiles( void );
void ScanForNewMessages( void );
void NewScanAtLogon( void );
short VisualDataEditor( char *file, void *data, long size );

void ShowMailHeader( struct MailHeader *m, short number, short of, short samv );
UBYTE SelectAndDownload( char *file, UBYTE flags );

void LoadBaseUser( void );
void SaveBaseUser( void );

void LogOffUser( void );
void TermLink( void );
void DoPortMonitor( void );
void DoTermLink( void );
//void DoRange( struct RangeContext *rc, long min, long max, long defmin, long defmax );
//long GetNext( struct RangeContext *rc );

void SetByAndTo( void );
UBYTE existf( char *name );
void AdoptItemStuff( void );
UBYTE SetItemStuff( UBYTE qwk );
void TestOne( void );
void TestItems( void );
void TransformOne( void );
void TransformItems( void );

void AskAPlan( UBYTE time, UBYTE sysop, char *maildir );
void dispRexxPort( void );
void NormalizeScreen( void );
void CallXPR( void );
UBYTE ChangeDir( char *name );
void BrowseItems( void );
void EraseChat( void );
UBYTE *FilePart( UBYTE *a );
void GetSkyBrush( BPTR fd );
void LoadBBSCharges( void );
short LoadBBSText( char *loc, char **bm, char *name, long size );
void LoadXLation( short t );
void NormalizeColors( void );
void ResetSRead( UBYTE on );
void Rreset( void );
UBYTE RunParagon( char *name );
UBYTE RunC( char *name, char *args );
UBYTE RunCPfile( char *name );
UBYTE RunRexx( char *name );
void sendcanx( void );
void SetCreditStuff( void );
void SetSelect( UBYTE *a );
void SetUtilityClock( long mics );
void ShowAccess( void );
void ShowTimeLeft( void );
char WaitForInput( long mics );
UBYTE YesNo( UBYTE def );
char CheckOLM( void );

void PutError( char *what );
void GotoResponse( long n, BPTR fh );
UBYTE CheckSysPassword( short x, char *banner );
void SilentUpload( char p );
void SilentDownload( char p );
UBYTE Unknown( void );
UBYTE CheckBBSTimes( void );
UBYTE ReadGraphics( char *f, char fnf );
void VerifyOff( void );
void WaitScreen( void );
void ModemCommand( char *s );
UBYTE WaitForCall( void );
void LogonScreen( void );
void ResetLine( void );
void ClearLine( void );
void UserStatus( struct UserData *user, short ii );
short FindAccount( char *text, struct UserData *user, UBYTE quiet );
void UpdatePrivs( struct UserData *u );
void UpdateAccess( BYTE access );
void CheckChanges( void );
long CommonCommands( void );
UBYTE ReadLog( char *f );
//short SendMail( UBYTE edin );
short FollowForward( short id, struct UserData *u );
UBYTE CheckUrgentMail( void );
short CountMail( short id );
void CheckMail( char *mailid, char *foldername );
void ReadMail( short id );
void TimeDate( void );
void Feedback( void );
UBYTE LoginUser( void );
void SetColors( short off );
void OpenChat( void );
void CloseChat( void );
void CloseDisplay( void );
UBYTE OpenDisplay( void );
void WriteSAMSummary( void );
void BadLogonReason( char *s );
void ParseCommandLine( UBYTE numargs );
short FindCommand( short menu );

UBYTE GetFilename( char *action );
void ReadDirectory( short parent );
void EntryPath( UBYTE exit );
void EditEntryFile( UBYTE exit );
void EntryFile( UBYTE exit );
void ListSubboards( void );
long ListSelected( UBYTE optdel );
short FindUpload( char *name );
void NothingSelected( void );
void NotValidated( void );
void Base( UBYTE base );
UBYTE OtherStuff( short r, UBYTE quiet, UBYTE enter, UBYTE ignorejoined );
void MoveToList( short r );
void MoveToArea( short r, UBYTE quiet );
void SetNewMessages( UBYTE quiet );
void SetThePrompt( void );
int GetItemNumber( void );
void PostMessage( UBYTE qwk );
UBYTE OKFilename( char *name );
void UploadFiles( void );
UBYTE FileItem( void );
void NextItem( void );
void LastPost( void );
void NoNumber( void );
void OutScan( void );
void ScanItems( void );
char DoAnonymous( UBYTE to );
void AboutPost( UBYTE new );
UBYTE ValidateOne( UBYTE ud );
void ValidateItems( void );
void PutItem( short b );
void GetItem( short b );
void PrintOption( char *s, UBYTE *o );
void KillItems( void );
void RemovePost( UBYTE killit );
void SaveHeaders( void );
void GrabItems( void );
UBYTE GrabOne( void );
void GrabFile( char *name );
void DownloadFiles( void );
void PostHeader( void );
void MessageHeader( void );
void ReadItems( void );
void AskDescription( void );
void AddDescription( void );
UBYTE PromptFileNote( char *filename );
void LimitEditor( char *loc );
UBYTE SetFrom( void );
UBYTE SetMessageStuff( UBYTE qwk );
void RespondOne( UBYTE qwk );
void RespondItems( void );
int ScanAll( void );
int ScanNew( UBYTE quiet );
int BetweenNew( void );
void ViewSubOps( void );
void DescribeOne( void );
void DescribeItems( void );
void SelectOne( UBYTE remove );
void SelectItems( UBYTE remove );
void RemoveUpload( short i );
void ExtractOne( void );
void ExtractItems( void );
void CheckString( char *string );
void SearchHere( void );
UBYTE SearchText( UBYTE filters );
void SearchEnd( void );
void TraverseAll( int (*Func1)( void ), int(*Func2)( void ) );
int TimeEstimate( int bytes );
void AdoptOrphans( void );
void DescribeBatch( char *shortfile );
UBYTE CheckPrivacy( struct HeaderType *h, UBYTE quiet );
UBYTE PrivateItem( void );
UBYTE VerifyKill( void );
void FindLoc( void );
long FindLargest( UBYTE quiet );

short GetWhichPort( UBYTE MustBeOn);
void SendOLM( void );
void ReadOLM( void );
long ConferenceWait( short r );
int EnterText( char firstkey, short max, short perline, short maxlines );
void WhosHere( void );
void HideOpts( char **text, UBYTE *all, UBYTE value );
void CheckTellMe( char *s );
void InterUserChat( short port );
void ChatRequest( void );
UBYTE CheckIUC( USHORT opts );

UBYTE RunDOS( char *name, char *arguments );
void AddToBuffer( char c );
long FileSize( char *path );
long GrabFileSize( UBYTE quiet );
long ListDir( UBYTE add, UBYTE counttime, struct IsDate *date );
UBYTE OkName( char *n );
void GetInfo( char *name, char *output );

void EditorPrint( char *text );

void MakeEd( char *path );
void DeleteEd( void );
BPTR OpenEd( long mode );
void PrepEditor( BPTR fh );
void CallEditor( short max, short inlines );
void SaveEditor( BPTR fh, UBYTE eof );
char LineEditor( short max, UBYTE flags );
char ExitSaveEditor( void );
void DoMCIRead( void );
void DoZip( void );
UBYTE CJustify( char *prompt, void (*fn)() );
void PutString( short line, short ww );
void InsPoint( short line );
void InsChar( short line, short col, short n );
void DelChar( short line, short col, short n );
void DelPoint( short line );
void FunctLoop( void (*funct)(), short returns );
UBYTE EdFindParts( UBYTE d );
void FList( long line );
void FnRead( long line );
void FEdit( long line );
void FReplace( long line );
void FToggle( long line );
void CDelete( void );
void CBorder( void );
void FCopy( long line );
void CCopy( short *line );
void FLeft( long line );
void FRight( long line );
void FCenter( long line );
void FPacked( long line );
void FIndent( long line );
void FUnindent( long line );
void FExpand( long line );
void FFind( long line );
void CGet( void );
void CPut( void );
void MakePrompt( char *s );

void PrintAGraph( void );
void DisplaySAM( void );
void EditAccount( void );
int EditHelp( UBYTE x );
void EditGroup( void );
void EditPrivs( struct Privs *p );
void EditPrivFlags( struct Privs *p );
void AccountSummary( UBYTE detail );
void PrintSigned( int n, char *s );
UBYTE CheckBalance( short n, short a );
UBYTE AddCharge( short n, short a );
void MyCredits( struct UserData *u );

void ResetServ( UBYTE reset, UBYTE time );
void ConGetChar( void );
void SerGetChar( void );
void ClearBuffer( void );
long GetABunch( char *buffer, long number );
void PutChar( char s );
char GetChar( ULONG mask );
void PrintAnything( char *s );
void DoPause( void );
void PrintSlow( char *wherefrom, short howmany );
void WaitModem( void );
void CursorOff( void );
void CursorOn( void );
void PrintWorkbench( char *where, short howmany );
void PrintPrinter( char *wherefrom, short howmany );
void PrintConsole( char *where, short howmany );
void PrintModem( char *where, short howmany );
char OneKey( void );
void ScreenBS( void );
void DoBackspace( short count );
void ScrollUp( void );
void DoReturn( void );
UBYTE ShowAbortKey( void );
void CheckFlowControl( void );
void PutText( char *wherefrom );
UBYTE PutQ( char *a );
void PutA( void );
void SetBaudRate( ULONG baud );
void Service( void );
void CheckCarrier( void );
void EnterPassword( UBYTE maxlen );
UBYTE DoANSIBox( UBYTE maxlen, USHORT opts, char *prompt, short len );
int EnterLine( UBYTE maxlen, ULONG opts, char *prompt );
UBYTE CheckInputChar( UBYTE gfx );
UBYTE ReadFile( char *filename, UBYTE fnf );
void ReadText( BPTR fh, char *buffer, char *key );
void ReadMessage( BPTR fh, char *key );
void ReadMessagePoint( char *name, long pos );
UBYTE LoadLine( BPTR fh, char *line, short w, short flags );
void EditMessage( char *file );
void EditMessageLines( short max, char *file );
void MakeDate( struct IsDate *date, char *whereto );
void ResetTime( void );
void StatPrint( char *text, int XOffset, int YOffset, UBYTE colors );
void DisplayStatScreen( void );
void ShowCredits( void );
void LoAccess( void );
UBYTE NoMessages( void );
long EnterAccess( char *prompt, long def );
char *PrintAccess( int a, short n );
short DeadKeyConvert( struct IntuiMessage *msg );
void Print40( void );
void ResetEnviron( void );
void ResetFont( void );
void AttemptOpenDisplay( void );

void ListItems( void );
void AddItem( void );
void SaveItemList( void );
void KillItem( short i, UBYTE quiet );
void EditItem( short i );
UBYTE MakeChanges( struct ListItem *f );
void CheckNews( void );
UBYTE ObtainPfile( char *name, UBYTE restrict );
void ReleasePfile( void );
void AlphabetizeItems( void );

void DoMCI( char *string );
void PutNumber( ULONG n );
void SOHelp( short n );
void DoStillOn( void );
void DoCCG( UBYTE c );
void DoANSI( UBYTE n, USHORT a, USHORT b );
void DoANSIOut( UBYTE n );
void EraseLineEnd( void );
void EraseLineBegin( void );
void AScrollUp( short line, short num );
void AScrollDown( short line, short num );
void DoSky( short which );
void FreeSkyBrush( void );

UBYTE DoGeneral( short n );
UBYTE EditProfile( void );
UBYTE DoParams( void );
UBYTE EditParams( void );
void NewUser( void );
UBYTE CheckBadFiles( char *f, char *s, char *d );
short CheckHandle( void );
void EditGeneral( short i );
UBYTE EditPrefs( void );
UBYTE DisplayList( char **text, short n, char *prompt, short def );
void ListComp( void );
void AutoCBV( short force );

void ToggleBott( char mode );
void ToggleStat( char mode );
void DoWEdit( UBYTE c );
void SetDoing( char *what );
void InitMenu( void ); // portcheck: 100=default (no port selected) otherwise port that is highlighted in Control panel
void ControlCommand( short c );

void Terminal( void );
void DoTermMenu( long code );
void AdjustPath( void );

void CarriageReturn( void );
void ClearStr( void );
short Findend( void );
void PutStr1( void );
void GetStr( void );
void Position( void );
void Position2( void );
void OutLine( short col );
void ClearToEnd( void );
char DoVisual( void );
char DrawEditor( void );
void DoInsLine( void );
void DoEOP( void );
void VInsChar( short n );
void SetOut( void );
void GoCorner( void );
void GoHome( UBYTE title );
void SendHome( void );
void GoLeft( void );
void GoRight( void );
void GoUp( void );
void SinsMany( short a );
void SdelMany( short a );
void GoDown( void );
void DoLeft( void );
void DoRight( void );
UBYTE DoBS( void );
void DoCR( void );
void VDelChar( short x );
UBYTE DoDelete( void );
void DoTab( void );
void DoILine( void );
void DoDLine( void );
void Pullups( void );
void RemoveMCI( char *b );

void zprprotocol( char *a );
void printsize( ULONG size );
void zprbytes( long val );
void zprblocks( USHORT b );
void zprblocksize( USHORT b );
void printerrors( long e );
void plotstatus( char *message );
void ploterrors( char *message );
void plotblockcheck( char *message );
void zprname( char *a );
void endxfer( void );
void AddDownload( void );
void DLoadSelect( UBYTE optdel );
void ULoadSelect( void );
void XAddFile( void );
UBYTE CheckTime( void );
UBYTE DCheckOK( struct SelectType *sp, short number );
UBYTE UCheckOK( UBYTE mode );
UBYTE OKProtocol( void );
UBYTE ChooseProtocol( struct UserData *u );
UBYTE MatchProtocol( UBYTE what );
UBYTE XOptions( char p );
void LogTransfer( char type, UBYTE log );
void AddTimeBenefit( void );
void CheckVerification( void );
void CloseTransfer( void );
UBYTE OpenTransfer( void );
void  CapsFirst( char *s );
short instr( char *s, char *t );

/* ZenMetal v4.10 */
APTR InitWinStruct( void );
void CleanUpWindow( struct WinStruct *winstr);
void SetGoExt(short physnum);   // 28-Oct-96
void MailDirRename(char *olduucp, char *newuucp, UBYTE newzhome);

/* ZenMetal v4.11 */
UBYTE LostYanks(char *uucpid);   /* check for lost Yanks/QWK packets and auto-select them */

/* ZenMetal v4.12 17-May-96 */
void                  GetUTP            (time_t *t, struct tm *utp);
short                  GetChargeNum      (struct ChargeSet *chargeptr);
char                  *IDToUUCP         (LONG userid);               /* convert user ID to UUCP name */
short                  UUCPToAccount      (char *uucp);                /* convert UUCP name to account number */
char                  *HNameToUUCP      (char *hname);               /* Accepts handle or real name and returns UUCP name belonging to the name given.  Note that HANDLES are searched first! */
ULONG DisplayNetInfo(char *netaddress ); // added 9-Jun-96

// v4.12d 21-Jun-96
void LogEvent( char *text, BYTE start );

// v4.13  1-Jul-96
BOOL GetFileDiz(char *name);

// v4.21 16-Aug-96
BOOL SaveFileDiz(char *DizText, char *DizLength);

// v4.13c 18-Jul-96
LONG StripDIZ(char *diztext, LONG dizlength);

// v4.20a 11-Aug-96
UBYTE ManageOne( UBYTE DoTest, UBYTE DoTxform );

// v4.24fr2 18-Jan-97
BYTE WriteUKeys( BYTE getsem );   // write the bbs.keys file
                                 // if getsem is TRUE, ObtainSemaphore is done on myp->SEM[1]
                                 // you should NOT use getsem if you have already gotten a semaphore lock on myp->SEM[1]!
                                 // See empty.c for a description of this function's purpose.

// Mail/Folder functions
struct MailToList      *IsFido            (char *net,   USHORT *faddr,   struct CNetNodeDesc *nd, struct MailToList *mailtolist );  /* get node information for net address */
struct MailToList      *IsLocal            (char *name, struct MailToList *mailtolist);
struct MailToList      *IsUUCP            (char *net, struct MailToList *mailtolist );
short                  GetMailHeaders      (short portnum, struct NewMailFolder *currentfolder); /* get mailheaders for folder "currentfolder" */
struct MailHeader4   *GetMailHeader      (short num);                 /* get a pointer to mail number "num" in the header list attached to the current port */
UBYTE                  UpdateMailHeader   (struct MailHeader4 *mhead); /* returns TRUE if success */
void                  DisposeMailHeaders(short portnum);             /* free all mail headers attached to current port */
char                  *GetMailText      (struct MailHeader4 *mhead, struct NewMailFolder *folder ); /* get message text belonging to mail header */
char                  *DisposeMailText   (char *text);                /* free memory used for mail text */
UBYTE                  CreateMailFolder   (char *foldername, char *UUCPName);
void                  MailSendQuick      (LONG fromid, LONG toid, char *subject, char *text); /* send a quick LOCAL mail with no prompting! */
UBYTE                  FMailSend         (struct MailHeader4 *mhead, struct MailToList *mailto, char *text);
UBYTE                  MailMoveToFolder   (struct MailHeader4 *mhead, char *ownerUUCP, struct NewMailFolder *sourcefolder, char *destfoldername);
UBYTE                  RemoveMailHeader   (struct MailHeader4 *mhead);
LONG                  InitializeMailFolders(char *path, char *UUCP); /* search user's FOLDERS directory for available folders */
UBYTE                  FolderToList(struct NewMailFolder *addfolder); /* Attach a new folder name to the list of folders - this function does NOT create the folders but puts a "placeholder" for the folder name in the list */
void                  DisposeMailFolders( void );                    /* free/deallocate memory used for the current port's linked folders list */
short                  BuildDir( char *path );                        /* create full path specified.  Builds nested directories also */
short                  ListMailFolders( UBYTE Quiet );                /* list folders currently attached to the current port's Folders structure - returns the number of folders in the list
                                                                       If Quiet=TRUE, only returns a count of the current folders with no listing */
struct NewMailFolder   *GotoMailFolder( char *uucpid, char *foldername ); /* Calls FindFolder() to see if there is a named folder belonging to the specified user. If no matching folders are found, a temporary folder structure is returned with it's "temporary" variable set to 1. */
UBYTE                  OpenMailFolderRead(struct NewMailFolder *rfolder, UBYTE quiet); /* opens the "_text" and "_header" files for the specified folder structure in READ mode and returns with the filehandle fields of the
                                                                           NewMailFolder structure set to the opened files or sets the filehanbdles to 0 if non-existant.
                                                                           Other errors returned are -1 if the header file was opened succesfully but no matching text/body
                                                                           file was found.  This routine also adds the full path/file names opened to the myp->Locks locked file list. */
UBYTE                  OpenMailFolderWrite(struct NewMailFolder *rfolder); /* opens the "_text" and "_header" files for the specified folder structure in READ mode and returns with the filehandle fields of the
                                                                            NewMailFolder structure set to the opened files or sets the filehanbdles to 0 if non-existant.
                                                                            Other errors returned are -1 if the header file was opened succesfully but no matching text/body file was found.  This routine also adds the
                                                                            full path/file names opened to the myp->Locks locked file list. */
struct NewMailFolder   *FindFolder(char *uucpid, char *foldername, struct NewMailFolder *FolderList);
                          /* finds a folder belonging to the specified uucp id
                             with the folder name specified, in the list
                             passed as FolderList. */
struct NewMailFolder *FolderFromNum(short num);
                          /* finds folder number "num" in the list of folders */
                          /* attached to the current port */
void CloseMailFolder( struct NewMailFolder *afolder);
                          /* close folder files (whether read or write)
                             and deallocate any memory allocated for the
                             folder if it is a "temporary" folder */
UBYTE DestroyMailFolder(struct NewMailFolder *killfolder);
                          /* removes the specified folder from   */
                          /* the user's FOLDERS directory        */
UBYTE RemoveFolderList(struct NewMailFolder *killfolder);
                          /* removes the specified folder from */
                          /* the list of folders attached to   */
                          /* the current port                  */

UBYTE InfoFromShortFile(char *shortfile);   // added 12-Sept-1996

// 28-Jan-97 -> see empty.c for description
BYTE InputSignalInit(char *portname);
void InputSignalDispose( void );


BOOL MoveFile(char *fromfile, char *tofile);

// 01-May-97
UBYTE TrimShort(LONG maxlines, LONG linelen);

// 02-May-97
void EditMailKill( void );            // global mailkill file editing - "MK" command
void EditTwitFilters(UBYTE flags);   // personal mail filter file editing - MAIL;FILTER command

// 29-Sep-97
void NetFinger(char *name);			// internet finger stuff
int sockgetc(int sock);					// socket stuff - get data from stream
void disconnectsocket(int sock);		// socket stuff - close socket connection to host

// 05-10-97
int UploadAttach( char *ulpath, char *pathfile ); // ulpath = path to upload to
                                                  // pathfile = full path/filename of final uploaded file
// 06-10-97
void AdoptAttach(char *ulpath, char *pathfile);

// 07-10-97
void MimeEncode(char *pathfile, char *uutempdir, char *mimename); // mime encode the file specified in pathfile and store in uutempdir.  mimename is filled with the path & filename of the encoded file.
void UUEncode(char *pathfile, char *uutempdir, char *uuename);		// uuencode the file specified in pathfile and store in uutempdir.  uuename is filled with the path & filename of the encoded file.
void CopyFileT(char *pathfile, char *uutempdir, char *destname);	// copy the file specified in pathfile to uutempdir.  destname is filled with the path & filename of the destination file.

// 11-Oct-97
void SignalOpenDisplay(short portnum); // Signal port screen/window to open
UBYTE CheckAndReadOLMS( void );        // Check for OLMs and display using the CNet OLM facilities

// 12-Oct-97
void GetSetFilenote( char *pathfile ); // Get a Description from the user via EnterLine() and add as an AmigaDOS FILENOTE.

// 21-Nov-97
UBYTE SaveQWKMail(short fromaccount, char *touser, char *msg, short len, char *subject);

// 31-Dec-97
struct Screen *OpenCNetScreen( void );

// 07-Feb-98
void ShowPartInfo( struct SubboardType4 *sb);
void ShowUpTime( void );

// 06-Mar-98
UBYTE GetTelnetDMsg( void );
void CleanupTelnet( void );

// 07-Mar-98 - moved from bbs.c module to external prototype
char    *OrdinalText( long o );

// 08-Mar-98
long SockRead(int sock, char *buffer, long readnum, UBYTE dotelnetstates);

short FindForeignArea( char *name );

#endif /* CNET_CNETFUNCS_H */
