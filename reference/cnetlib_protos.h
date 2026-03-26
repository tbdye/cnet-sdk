
// Utility functions
BOOL         checkspaces              ( UBYTE *str, LONG checklen );
BOOL         DupeUUCPName             ( char *uucpname, short id );
short        CalculateAge             ( struct UserData *user );
short        compstra                 ( char *s, char *t );
short        FindPhysNum              ( struct SubboardType4 *sp);
struct       PortData *IsOnLine       ( short acc );
UBYTE        *RemoveLeadingSpace      ( UBYTE *str );
UBYTE        DupeSubDirName           ( char *name, short skipphysnum);
UBYTE        HasMaint                 ( void );
UBYTE        IsNorthAm                ( char *u );
UBYTE        IsPrivate                ( void );
UBYTE        IsUnvalidated            ( void );
UBYTE        NewItem                  ( struct IsDate *date, struct IsDate *base );
UBYTE        TextReload               ( void );
ULONG        TimeTilCutoff            ( void );
void         AddFileNote              ( char *file );
void         AddSAM                   ( short samidx, long value );
void         AddSAM4                  ( struct MainPort *mainport, short samidx, long value );
void         DelSAM                   ( short samidx, long value );
void         DelSAM4                  ( struct MainPort *mainport, short samidx, long value );
void         DisplayGoKey             ( short base );
void         DoRange                  ( struct RangeContext *rc, long min, long max, long defmin, long defmax );
void         FindUnits                ( ULONG size, char *unit, ULONG chunk, char **bm );
UBYTE        GetReplyTo               ( struct UserData *u, char *replytostr);
void         NoteTimeTilCutoff        ( void );
void         remMCI                   ( char *t );
void         RemoveNonAlNum           ( char *a );
void         SaveAlpha                ( void );
void         SaveNums                 ( void );
void         SaveReplyTo              ( struct UserData *u, char *replytostr);
void         SaveSAM                  ( void );
void         SetTheDoing              ( struct SubboardType4 *sb);
void         SToLower                 ( char *s );
void         SToUpper                 ( char *s );
void         SuggestUUCPName          ( char *uucpout, char *realname, short id);
BOOL         IsControlLocked          ( void );
void         MungeMCI                 ( char *b );
BOOL         IsPortLoaded             ( short portnum );
BOOL         IsPortOccupied           ( short portnum );
BYTE         WriteUKeys               ( BYTE getsem1 );
void         SaveAccessData           ( void );
short        ParseToPItem             ( char *input, short port );
char         *CNetTokenExpand         ( char *tokstring );

// I/O
BOOL         IsAbort                  ( void );
void         AddKeyBuffer             ( char *c );
UBYTE        PutQMult                 ( char *prompt, char *possibilities);

// File management
BPTR         OpenAppend               ( char *name );
UBYTE        GetFileDate              ( char *file, struct IsDate *ModDate );
UBYTE        GetFileDateY2K           ( char *file, struct IsDate5 *ModDate );
void         CopyMessage              ( BPTR fh, BPTR th );
void         CopyText                 ( BPTR fh, BPTR th );
void         DeleteAFile              ( char *name );
UBYTE        ExtractFile              ( char *name );
short        FindArc                  ( char *file );
short        FindSelect               ( char *name );
void         LoadSelectList           ( void );
void         MakePath                 ( char *where );
void         NoteSelected             ( void );
void         RemoveSelect             ( short i );
void         RemoveSelectLock         ( short i );
void         SaveSelectList           ( void );
void         SaveSelectListLock       ( void );
void         SelectFile               ( UBYTE counttime, UBYTE remove );
UBYTE        SelectFileB              ( short Account, char *pathfile, UBYTE remove );
ULONG        CopyFile                 ( char *fromfile, char *tofile);
void         AppendFile               ( char *dest, char *source );
void         GetInfo                  ( char *name, char *output );
UBYTE        InitNUpload              ( void );
void         DisposeNUpload           ( void );
void         SetFileNote              ( char *file, char *FileNote );
long         GetFree                  ( char *name, UBYTE quiet, char *deststring, char **bm, short part );

// Clipboard
char         *TextFromClipboard       ( ULONG unit );
int          ClipboardQueryFTXT       ( struct IOClipReq *ior );
int          ClipboardReadLong        ( struct IOClipReq *ior, ULONG *ldata );
int          ClipboardWriteFTXT       ( struct IOClipReq *ior, char *string);
int          ClipboardWriteLong       ( struct IOClipReq *ior, long *ldata);
struct       cbbuf *ClipboardReadCHRS ( struct IOClipReq *ior );
struct       cbbuf *FillClipboardData ( struct IOClipReq *ior, ULONG size );
struct       IOClipReq *ClipboardOpen ( ULONG unit);
void         ClipboardClose           ( struct IOClipReq *ior);
void         ClipboardFreeBuf         ( struct cbbuf *buf );
void         ClipboardReadDone        ( struct IOClipReq *ior );
void         TextToClipboard          ( ULONG unit, char *text);

// Resource (de)allocation
void         SafeFreeMem              ( void *b, ULONG s );
void         SafeFreeVec              ( void *b );

// Subboard
short        LoadSubboards            ( char *subboards_file, struct SubboardType4 **Subboard, long *gcnsub );
void         WriteASubboard           ( short t );
void         WriteAllSubboards        ( char *subboard_file, struct SubboardType4 *subboard_head, short nsub);
void         WriteSubboard            ( void );
void         WriteBSubboard           ( struct SubboardType4 *sbp, short physnum );
void         MakeLoc                  ( struct SubboardType4 *sub4, char *deststr, short p );

// Ranges
long         GetNext                  ( struct RangeContext *rc );

// Accounts
UBYTE        KillAccount              ( short accountnum );
UBYTE        ReadAccount              ( short id, struct UserData *user );
UBYTE        SaveAccount              ( struct UserData *user, short id );
void         ReadUUCP                 ( UBYTE ShowNewsHead );

// Sorting
void         MyStringSort             ( char **strptrs, short count);

// Date/time
void         GetDate                  ( struct IsDate *dest );
void         PutDateHere              ( struct IsDate *dest );
void         LongToDate               ( char *date, char *t, ULONG was );

// Exec lists
void         free_list                ( struct List *list );
struct List  *init_list               ( UBYTE public );

// Editor
void         TextToEditor             ( char *text );


// Socket support
void         SockPrintf               ( int sock, char *fmt, ... );
void         TNSockWrite              ( int sock, char *buff, LONG length );
int          SockStatus               ( int sock );
int          SockQueue                ( int sock, int number );
int          SockGetC                 ( int sock );
ULONG        CNL_SockWait             ( ULONG waitmask, int sock, ULONG secs, ULONG mics );
BOOL         SockCarrier              ( int sock );


// The following functions are used by/for CNetC doors and use code
// that will crash anything other than a CNetC door
BOOL         CNCL_DoorInit            ( LONG *myargs );
void         CNCL_DoorCleanup         ( void );
void         CNCL_Printf              ( char *fmt, ... );

void         GetGUIConfig             ( char *ConfigFile, struct IBox *pbox );
void         SaveGUIConfig            ( char *ConfigFile, struct IBox *pbox );
short        FindPosition             ( struct List *list, struct Node *node );
void         LoadArcs                 ( long *p_narc, struct ArcType4 **p_arc );

short        GoKeyToSubNum            ( char *name, BOOL accesscheck, BOOL ignorejoined );
UBYTE        CheckJoined              ( struct BaseUser **p, short r );

char         *stristr                 ( char *sourcestr, char *findstr );

struct IMPStruct *FindIMPNode         ( char *gokeyword, ULONG processtype, struct MainPort *p_MainPort );
UBYTE        CheckFido                ( char *net );
UBYTE        CheckUUCP                ( char *net );
UBYTE        CheckLocal               ( char *name );
//UBYTE        SendMail                 ( char *from, char *to, char *subject, char *text, LONG smflags);
