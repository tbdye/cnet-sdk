#ifndef CNET_SUBS_H
#define CNET_SUBS_H

#include <cnet/align.h>

#define HIGH_ITEM_LIMIT 4000 // absolute maximum number of items per subboard
                             // This is a Perspective Software limit that will
                             // be removed in the near future, which is why
                             // it was moved to a #define.

#define SORDER_LIMIT 240     // another KenLimit found
                             // This means 240 subboards allowed per list/subboard
                             // Will be removed for CNet/5 also.


CNET_PACK_BEGIN

// Select list structure
struct SelectType
   {
   long           Size;

   char           Title[32];
   char           Location[96];     // physical path to file
   short          Base;             // physical subboard #

   UBYTE          SDownFiles;       // Which accounting schedule?
   UBYTE          SDownBytes;

   UBYTE          temp_rem;         // TRUE/FALSE - DS from DS list!!
   UBYTE          CDROM;            // TRUE/FALSE - Copy to CDROMpath before download?
   BYTE           FreeStuff;        // TRUE/FALSE - Free file
   UBYTE          AutoKill;         // see ST_AKILL_* flags below

   long           ByteDownload;     // credits to subtract when this
   short          FileDownload;     // item is downloaded ...
                                    // makes * faster by being here
   struct IsDate  ADate;            // utility ... AO adopt dates?

   ULONG          Number;           // For regular selection, this is the same as ItemHeader.Number
                                    // For upload list, this is ordinal item number on current list
                                    // NOTE: Only during uploads is z->Upload[x].Number is set to item number +1.
   };

#define ST_AKILL_AMAINT   1 // mark for amaint deletion when downloaded
#define ST_AKILL_YANK     2 // Yank
#define ST_AKILL_UNSELECT 3 // DELETE when unselected


// SelectAndDownload() flags
#define SAD_FLAG_DLNOW    0x01 // begin downloading immediately
#define SAD_FLAG_AKILL    0x02 // enable AutoKill
#define SAD_FLAG_CDTEMP   0x04 // enable CDROM temp


struct BaseUser
   {
   long   UID;         /* ==IDNumber if user has been here */

   BYTE   Joined;      /* See BUJ_* below */
   BYTE   Invited;     /* See BUJ_* below */
   BYTE   Sort;
   UBYTE   scan_flags; /* used to 'mark' subboards */

   struct   IsDate LastMNew;
   struct   IsDate MNewDate;

   long   Created;     /* when sub was created */

   char   Alias[22];
   BYTE   Carbons;     /* See BUC_* below */
   UBYTE   MNewSave;

   short   tome0;
   short   tome1;      /* while "online" */

   char   expansion[12];
};

#define BUJ_DROPPED -1
#define BUJ_SUBDEF   0
#define BUJ_JOINED   1

#define BUI_DENIED  -1
#define BUI_SUBDEF   0
#define BUI_INVITED  1

#define BUC_NO      -1
#define BUC_SUBDEF   0
#define BUC_YES      1


struct HeaderType
   {
   struct   IsDate ShowDate;   /* for display */
   struct   IsDate EditDate;   /* last edited */
   struct   IsDate PostDate;

   short   ByAccount;

   char   NetAddress[52];      /* May be shortened */
   long   UUCPnumber;

   char   other[16];

   char   By[36];         /* Real name or alias */
   char   ByUser[24];     /* Handle */
   long   ByID;

   char   To[36];
   char   ToUser[24];
   long   ToID;

   UBYTE   ByNotParanoid; /* allow showing real name? */
   UBYTE   ByAnonymous;
   UBYTE   ToAnonymous;
   UBYTE   Private;

   UBYTE   Imported;
   UBYTE   ByAlias;       /* is BY an alais for ByUser? */
   UBYTE   ToNotParanoid; /* allow showing real name? */
   UBYTE   ToAlias;       /* is TO an alias for ToUser? */

   short   ToAccount;     /* for SendToMailBox() */

   ULONG   Magic;         /* to ensure alignment   */
   ULONG   Number;        /* unique message number */

   long   Text;
   long   TextLen;

   char   Organ[31];

   UBYTE   Received;

   short   unknown;

   long   Next;
   long   Previous;
   };



struct   MessageType3
   {
   ULONG   ItemNumber;    /* item serial ID# */
   long   Seek;           /* position in text */
   long   ByID;
   long   ToID;
   ULONG   Number;        /* serial ID# */

   struct   IsDate PostDate;

   UBYTE   Imported;
   UBYTE   IsFile;
   };



struct   ItemType3
   {
   char   Title[42];          /* 0 */
   short   ByAccount;         /* 42 Uploader's account # */

   long   ByID;               /* 44 */
   long   ToID;               /* 48 */

   struct   IsDate UsedDate;  /* 52 new downloads ? */
   short   Part;              /* 58 which partition? */

   long   Downloads;          /* 60 */

   long   ByteCharges;        /* 64 Accounting system charges to the */
   short   FileCharges;       /* 68 uploader ... */

   UBYTE   Private;           /* 70 */
   UBYTE   DLnotifyULer;      /* 71 */

   UBYTE   PleaseKill_obs;    /* 72 */
   UBYTE   Frozen;            /* 73 */
   UBYTE   Free;              /* 74 */
   UBYTE   delta;

   UBYTE   Favorite;          /* 76 */
   UBYTE   Validated;         /* 77 */
   UBYTE   Finished;          /* 78 */
   UBYTE   Described;         /* 79 */

   UBYTE   Transformed;       /* 80 has the script been ran? */
   UBYTE   PurgeKill;         /* 81 delete file when purged? */
   UBYTE   MissingFile;       /* 82 offline? */
   BYTE    Integrity;         // 83 see ITEMTYPE_INTEGRITY_* below

   UBYTE   AutoGrab;          /* 84 ASCII-Grab when read */
   UBYTE   PurgeStatus;       // 85 see ITEMTYPE_PURGE_* below

   struct   IsDate ShowDate;

   short   order;             /*  92 on disk ...*/

   UBYTE   nada[5];           /*  94 reserved */
   UBYTE   VirusChecked;      /*  99 */
   UBYTE   override;          /* 100 */
   UBYTE   loaded;            /* 101 is in memory? */

   short   FilePayBack;       /* 102 after each download */
   long   BytePayBack;        /* 104 Credits to award the uploader */

   long   VoteLink;           /* 108 vote serial# */
   long   SizeTemp;           /* 112 used privately by AT */

   long   ByteRewards;        /* 116 # of bytes awarded so far */
   long   FileRewards;        /* 120 # of files awarded so far */

   long   ByteDownload;       /* 124 Number of byte cred to download */
   short   FileDownload;      /* 128 Number of file cred to download */

   struct   IsDate DLableFrom; /* 130 */
   struct   IsDate DLableTo;   /* 136 */
   struct   IsDate PurgeOn;    /* 142 */

   long   BestCPS;            /* 148 best download rate this file */

   long   First;              /* 152 pointer to post in _Text */
   long   Last;               /* 156 pointer to last response in _Text */

   long   InfoX;              /* 160 pointer into _Short */
   long   InfoLen;            /* 164 */
   };

#define ITEMTYPE_INTEGRITY_UNTESTED 0 // not tested
#define ITEMTYPE_INTEGRITY_PASSED   1 // passed
#define ITEMTYPE_INTEGRITY_FAILED  -1 // failed
#define ITEMTYPE_INTEGRITY_UNKNOWN  2 // not testable

#define ITEMTYPE_PURGE_AUTO      0 // Auto
#define ITEMTYPE_PURGE_ATDL      1 // @DL
#define ITEMTYPE_PURGE_ASK_ATDL  2 // Query@DL
#define ITEMTYPE_PURGE_AT_AMAINT 3 // @Amaint
#define ITEMTYPE_PURGE_PROTECTED 4 // Protected

/* size = 34 */


struct   ItemHeader
{
   ULONG   Number;
   long   Size;              /* ==0 is POST, not file */
   long   Responses;         /* How many responses */

   struct   IsDate PostDate; /* new item ?      */
   struct   IsDate RespDate; /* new responses ? */

   UBYTE   TitleSort[9];
   UBYTE   Killed;
};


struct FreeType
{
   long   start;
   long   length;
   struct   FreeType *next;
};





// CNet/4+ subboard structure
struct SubboardType4
{
   char   Title[60];          // 0   - what user sees
   char   SubDirName[75];     // 60  - also functions as GO argument
   char   DataPath  [95];     // 135 - path to data files - may be different than SubDirName!
   long   Parts;              // 230 -  partitions 0-31 allowed

   long   SubOpIDs [6];       // 234 - unique user IDs of subbies

   short  MRewardBytes;       // 258 % of byte  size to award
   short  NRewardFiles;       // 260 # of file creds to award
                              // uploader each time his file is
                              // downloaded */

   struct FreeType *free0;    // 262 pointer to free text in _text

   // added for v4.22 (beta)
   long   NNTPMaxDupes;       // 266 - maximum number of dupes to track for NNTP articles

   long   subvalid;           // should be 1234567890L if subboard data is not corrupt

   short  SubOpAccs[6];       /* 274 - subop ACCOUNT #s */
   char   Filler[6];          /* 286 shown during SCAN */
   UBYTE  fdelta;             /* 292 */
   UBYTE  floaded;            /* 293 */
   UBYTE  Direct;             /* 294 Disk-exchange type */
   UBYTE  Vote;               /* 295 May non-sysops add topics? */
   short  FreeDays;           /* 296 when files are auto-free */
   UBYTE  Subdirectory;       /* 298 */
   UBYTE  Closed;             /* 299 */
   long   UnionFlags;         /* 300 Must have all of these flags */
   long   Access;             /* 304 Groups which may ENTER */
   long   PostAccess;         /* 308 Groups which may Post  */
   long   RespondAccess;      /* 312 ... Respond */
   long   UploadAccess;       /* 316 ... Upload */
   long   DownloadAccess;     /* 320 ... Download */
   long   ComputerTypes;      /* 324 */
   long   Hours;              /* 328 restricted entry hours */
   long   HourUnionFlags;     /* 332 */
   long   HourAccess;         /* 336 groups which may Enter during Hours*/
   long   Baud;               /* 340 minimum Baud rate to enter */
   long   BaudHours;          /* 344 hours to enforce min baud rate */
   UBYTE  Gender;             /* 348   0 (for unisex) or 'M', 'F' */
   UBYTE  AllowAliases;       /* 349 */
   UBYTE  PurgeStatus;        /* 350 */
   UBYTE  Youngest;           /* 351 */
   UBYTE  EXTRA;              /* 352 */
   UBYTE  Oldest;             /* 353 */

   struct IsDate   Sent;      /* 354 FIDO to track new messages */

   short  MDownBytes;         /* 360 % of download bytes to subtract */
   short  NDownFiles;         /* 362 # of files to subtract per download */
   short  MTimeCredit;        /* 364 % of time to give back for uploads */
   long   MinFreeBytes;       /* 366 to upload in this subboard */
   UBYTE  SubSort;            /* 370 */
   UBYTE   SuperUser_Only;    /* 371 only superusers can enter */
   short  InactiveDays;       /* 372 */

   UBYTE  RealNames;          /* 374 use real names instead of Handles? */
   UBYTE  Addressees;         /* 375 */
   UBYTE  Anonymous;          /* 376 */
   UBYTE  override;           /* 377 how to set the use defaults flags in newly uploaded items */

   UBYTE  Verification;       /* 378 */
   UBYTE  DupCheck;           /* 379 */
   UBYTE  ShowUnvalidated;    /* 380 */
   UBYTE  NoMCI;              /* 381 */

   UBYTE  PrivateArea;        /* 382 0/1 No/Yes ... 2==FORCE */
   BYTE   SDownFiles;         /* 383 Which download charge schedules? */
   BYTE   SDownBytes;         /* 384 */
   UBYTE  Test;               /* 385 test at upload time */

   UBYTE  Transform;          /* 386 run script at upload time */
   UBYTE  AutoAdopt;          /* 387 Adopt Orphans at a-maint */
   UBYTE  WeedResponses;      /* 388 Use inactive days against indiv responses */
   UBYTE  NoSignatures;       /* 389 */

   UBYTE  NoReadCharges;      /* 390 opt to short circuit accounting system */
   UBYTE  NoWriteCharges;     /* 391 for reading/writing messages */

   long   Arcs;               /* 392 which Arcs in BBSARC are allowed uploaded*/

   short  UpRatioBytes;       /* 396 which set of file/byte ratios to use? */
   short  UpChargeBytes;      /* 398 which set of charges to use?          */
   short  UpRatioFiles;       /* 400 */
   short  UpChargeFiles;      /* 402 */

   UBYTE  Invitation;         /* 404 */
   UBYTE  UserMustJoin;       /* 405 */
   UBYTE  DeleteOwn;          /* 406 */
   UBYTE  Marker;             /* 407     0 = Base, 1 = UDBASE, &128 = Killed */

   long   Transforms;         /* 408 which BBSARCS to transform */

   long   ExportTo;           /* 412 */

   ULONG  obits;              // 32 bitflags used for various features
                              // see OF_* below for bitdefs

   // 4 bytes below and the flags they represent combined into oflags 15-01-1999
   //
   //   UBYTE  DIZAlphaNumOnly;    /* 416 - strip everything but alpha and numeric text */
   //   UBYTE  UseDIZStripChars;   /* 417 - strip the characters specified in BBSTEXT line 2150 */
   //   UBYTE  UseDIZStripText;    /* 418 - strip any text matching the contents of any line of sysdata:DIZStripText */
   //   UBYTE  DIZStripCR;         /* 419 - strip carriage return/linefeed from DIZ */

   UBYTE  NNTPMarker;         /* 420 - 0 = no-import/disabled newsgroup
                                       1 = regular NNTPGet newsgroup - full import of header/body
                                       2 = mailing list pseudo-newsgroup
                                       3 = (not yet implemented) NNTPGet headers only - connect to remote host to retrieve body text
                                       4 = (not yet implemented) No NNTPGet - always connect to remote for article list and body text after scanning articles & presenting to user */

   UBYTE  ReformatDIZ;        // 18-Feb-97 reformat DIZ - remove multiple spaces between words,
                              //                          successive newlines, tabs, etc,..

   UBYTE  NoWrapDIZ;          // 15-May-98 do not allow DIZ wrapping..  TRIM at 44 chars..

   UBYTE  nada1001;

   ULONG  count;              /* 424 replaces the _COUNT file */

   ULONG  hiwater;            // 428 - corresponds to last article received.
                              // This value is the GROUP article number as
                              // referenced by the ISP, **NOT** the local
                              // article number/.next value!

   UBYTE  NetNumber;          /* 432 */ 
   UBYTE  Locked;             /* 433 Does AMAINT (or other processes) have a lock here? */
   UBYTE  DLnotifyULer;       /* 434 */

   char   TransformTo[5];     /* 435 */

   UBYTE  QWKReplies;         /* 440 */

   BYTE   SBaseUse;           /* 441  which BBSCHARGE rate to use? */
   UBYTE  CarbonCopy;         /* 442 */
   UBYTE  CDROM;              /* 443 copy to hard drive before download? */

   struct IsDate LastUpload;  /* 444 */
   struct IsDate LastMessage; /* 450*/

   short  Users;              /* 456  how many people are in this sub REAL TIME (not # of members) */

   long   Created;            /* 458  for membership determination */

   short  Parent,             /* 462 - PHYSICAL number of first subboard on the list to which this subboard belongs (all subboards in the ROOT directory should point to the very first physical subboard) */
          Child,              /* 464 - physnum of first subboard in the list to which this subdirectory houses (-1 if none) */
          Next;               /* 466 - physnum of the next subboard on the current list (-1 if none) */

   short  TimeLock;           /* 468 */

   char   ZeroPath[90];       /* 470 */
   char   Origin  [60];       /* 560 */

   /* if more room is needed, I could put all of this into a pointer to a
      'sub-info' structure */

   USHORT MaxItems;           /* 620 */
   UBYTE  Persist;            /* 622 */
   UBYTE  Delay;              /* 623 */

   struct ItemType3 *Item;    /* 624 */
   struct ItemHeader *ihead; 
   struct MessageType3 *NewMess;
   struct SignalSemaphore *sem;

   long   nm;
   USHORT rn;
   USHORT AllocItems;
   USHORT nNewMess;

   UBYTE  resetIP;

   /* diz support! 11-Jun-96 */

   long   DizTypes;           /* 652 - which BBSARCs to Diz process (v4.20) */
   UBYTE  DizSave;            /* 656 - 0=don't save back to archive, 1=save DIZ back to archive after edit/changed */
   UBYTE  UseSentBy;          /* 657 - 0=don't tack on sentby's, 1=tack sentbys on uploads */
   UBYTE  DizEdit;            /* 658 - allow diz'd discriptions to be altered/edited - maint users have override can be turned of in user prefs */

   LONG   NNTPLastImport;     /* 659 - last NNTP news import date/time */

   char   SubDoing[17];       /* 663 - String to be set for the Where/Action/DOING string in WHO display when use enters this subboard object */
   ULONG  SerNum;             // "physical" subboard number
   BYTE   more_stuff[10];
};                     /* total length = ??? */

CNET_PACK_END

// Marker flags

#define MRK_SUBBOARD_KILLED  128 // Marker & SUBBOARD_KILLED = deleted subboard
#define MRK_SUBBOARD_ROOT    64  // Marker & SUBBOARD_ROOT = root (eg. first listed at main menu) subboard.
#define MRK_SUBBOARD_BASE    63

// Marker values (ignoring Marker flags)
// use ACTUAL_MARKER_VAL=(SubboardType4.Marker & MRK_SUBBOARD_BASE) to obtain these values

#define MRK_MSG_BASE           0 // Subdirectories are type MRK_MSG_BASE with
#define MRK_FILE_TXFER         1 // SubboardType4.Subdirectory=1
#define MRK_TEXT_DOOR          3
#define MRK_TEXT_FILE          4 // Direct exchange (pre-Commander type) are
#define MRK_CNETC_DOOR         5 // type MRK_FILE_TXFER with
#define MRK_AREXX_DOOR         6 // SubboardType4.Direct=1
#define MRK_ADOS_DOOR          7
#define MRK_BBS_MACRO          8
#define MRK_DIRECT_COMMANDER   9

#define INVALID_SUB_PHYSNUM    -1
#define MAIN_SUB_PHYSNUM       -2



// oflags bits
#define OFB_SHOWBOWS         0
#define OFB_DIZALNUM         1
#define OFB_DIZSTRIPCHARS    2
#define OFB_DIZSTRIPTEXT     3
#define OFB_DIZSTRIPCR       4

// oflags masks
#define OFF_SHOWBOWS      (1L << OFB_SHOWBOWS)
#define OFF_DIZALNUM      (1L << OFB_DIZALNUM)
#define OFF_DIZSTRIPCHARS (1L << OFB_DIZSTRIPCHARS)
#define OFF_DIZSTRIPTEXT  (1L << OFB_DIZSTRIPTEXT)
#define OFF_DIZSTRIPCR    (1L << OFB_DIZSTRIPCR)

#endif /* CNET_SUBS_H */
