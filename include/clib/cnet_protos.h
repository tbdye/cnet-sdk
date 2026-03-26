/* CNET.LIBRARY functions */
short  ReplaceText            ( char *line, char *from, char *with );
long   GiveAmigaDays          ( struct IsDate *date );
void   FormDate               ( struct IsDate *date, long *parts );
void   AlterDate              ( struct IsDate *date, long i );
void   UltimateFindParts      ( char *buffer, long min, long max, long *nparts, long *parts );
void   ExpandFlags            ( long def, char *where );
long   ConvertAccess          ( char *s );
void   OneLessUser            ( struct SubboardType4 *s );
UBYTE  OneMoreUser            ( struct SubboardType4 *s, UBYTE lock );
long   SaveFree               ( struct SubboardType4 *s );
long   FreeText               ( struct SubboardType4 *s, long pos, long length );
long   AllocText              ( struct SubboardType4 *s, long length );
struct UserData *LockAccount  ( short acc );
void   UnLockAccount          ( short acc, UBYTE save );
long   FindPhone              ( short *n, char *phone, short id );
long   FindHandle             ( short *n, char *name,  short id );
void   FormatVanilla          ( BPTR in, BPTR out );
void   MakeDateZone           ( struct IsDate *date, char *whereto, char **text, BYTE tz, BYTE form  );
void   ZPutItem               ( struct ItemType3 *Item0, struct ItemHeader *ihead0, struct SubboardType4 *s, short c );
void   ZGetItem               ( struct ItemType3 *Item0, struct ItemHeader *ihead0, struct SubboardType4 *s, short c );
UBYTE  ZAddItem               ( struct ItemType3 *Item0, struct ItemHeader *ihead0, struct SubboardType4 *s );
void   ZAddMessage            ( struct MessageType3 *Message0, struct SubboardType4 *s );
void   DelayAddItems          ( struct SubboardType4 *s );
void   FlushAddItems          ( struct SubboardType4 *s );
UBYTE  FileOLM                ( struct PortData *z, char *text, short id, long flags );
UBYTE  XAddCharge             ( struct ChargeSet *set, short id, short n, short a, char *info );
void   AddCredits             ( struct ItemType3 *i, short m );
void   WriteLog               ( struct PortData *z, short n, char *text1, char *text2 );
ULONG  DirectorySize          ( char *dir, BPTR lock );
short  FileExists             ( char *filename);
void   CreateMailDir          ( char *uucpid);
UBYTE  CreateFolderName       ( char *destpath, char *UUCP, char *foldername);
short  BuildDir               ( char *path );
short  BitPosition            ( LONG bitpattern);                        // returns the actual position of the highest SET bit in bitpattern
void   MCIFilter              ( char *text, LONG length, short flags);   // flags==1 -> filter CTRL-Y, flags==2 -> filter CTRL-Q, flags==3 -> filter both */
LONG   AccountToID            ( LONG account);
short  IDToAccount            ( LONG userid);
void   StrToUpper             ( char *string);               // 25-Jan-97 -  Converts string to all uppercase
LONG   FileSize               ( char *Name);                 // 25-Jan-97 - Get size of file (requires full path and filename)
BYTE   AddIdentdUser          ( short Port, LONG Socket);    // CNetPort number and Socket as args, return 0 on success
BYTE   RemoveIdentdUser       ( ULONG Socket);               // socket as arg, return 0 on success
short  LookupIdentd           ( ULONG Socket);               // returns CNet PORT number (-1 if not found)
UBYTE  CNetAddressType        ( char *user);                 // returns ADDRESSTYPE_* - see mail.h
short  CNetAddressToAccount   ( char *a );                   // lookup account number by CNet UUCP/MailID
char   *HNameToUUCP           ( char *hname );               // lookup UUCP/MailID by handle/real name
char   *CNetIDToUUCP          ( LONG userid );               // lookup UUCP/MailID from user serial IDNumber
short  NumFromUnique          ( char *gokeyword );           // returns physical subboard number for identical (case insensitive) match of gokeyword
struct Node *FindOrdinal      ( struct List *list, short n); // accepts an Exec list pointer and a number (N) and returns the Nth node in the Exec list
short  InStr                  ( char *string, char *isin );  // check to see if <isin> is a substring of <string>
BOOL   AllDigits              ( char *string);               // is string all numeric characters?? returns TRUE if so.
void   CopyFilterText         ( char *dest, char *source);   // copies source to destination, removing MCI
                                                             // and CR/LF in the process - this is the function
                                                             // used to prepare text to be displayed in CNet GUIs
struct PortData *IsNowOnLine  (struct MainPort *myp, short acc );
void   GetRFCDate             ( char *datedest);
BOOL   AppendToFile           ( char *filename, char *text, BOOL AddCR); // add text to filename with or without CR (newline character)
void   PItemToArgV            ( short portnum, char *argv[] );
struct CNetFileEntry   *CreateDirEntry   (struct FileInfoBlock *fib);   // Allocate memory for a CNetFileEntry structure and return pointer to mem block or NULL if fail.
void                   InsertDirEntry    (struct CNetFileEntry **list, struct CNetFileEntry *newentry); // insert a CNEtFileEntry into a list, sorted alphabetically; DIRS first.
void                   CNetDisposeDir    (struct CNetFileEntry **list); // free the COMPLETE file list as returned by CNetReadDir()
struct CNetFileEntry   *CNetReadDir      (char *path, UBYTE recurse);   // Read a complete directory into a linked list of CNetFileEntry structures with parent and child directory retention; with recurse option.
struct CNetFileEntry   *GetDirEntry      (LONG ordinal, struct CNetFileEntry *list); // Get a pointer to the CNEtFileEntry specified by the ordinal number, with the TOP of the list being entry number ** 1 **
LONG                   CountDirEntries   (struct CNetFileEntry *list);  // Count the number of entries on the current list (not inclusive of parent and child entries)
struct CNetFileEntry   *FindParent       (struct CNetFileEntry *list);  // returns pointer to the PARENT  dir if found; else returns pointer to the same list passed as list arg.
struct CNetFileEntry   *NextDirEntry     (struct CNetFileEntry *list, struct CNetFileEntry *nowentry);
struct CNetFileEntry   *PrevDirEntry     (struct CNetFileEntry *list, struct CNetFileEntry *nowentry);
struct CNetFileEntry   *HeaderEntry      (struct CNetFileEntry *anyentry);
struct CNetFileEntry   *LastEntry        (struct CNetFileEntry *anyentry);
LONG                   EntryOrdinal      (struct CNetFileEntry *entry, struct CNetFileEntry *list);
struct CNetFileEntry   *GetVolAssList    (void);
struct CNetFileEntry   *CNetSearchEntry  (struct CNetFileEntry *list, struct CNetFileEntry **destlist, char *pattern, UBYTE recurse);
char                   *MCIRemove        (char *text);
void                   UCacheDispose     (void);
struct CNUserCache     *UCacheInitUser   (short account);	// initialize cache entry for user
struct CNUserCache     *UCacheLookup     (short account);	// obtain existing cache entry for user
short                  UCacheRead        (short account, struct UserData *user, short numaccounts);
short                  UCacheWrite       (short account, struct UserData *user, short numaccounts);
void                   StartupInitCache  (void);
struct ItemType3       *ZGetItemPtr      (struct SubboardType4 *s, short c );
struct ItemHeader      *ZGetIHeadPtr     (struct SubboardType4 *s, short c );
void                   CNetStripChars    (char *s, char *strip );
/* end of CNET.LIBRARY functions */
