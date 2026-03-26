
#ifndef CNET_USERS
#define CNET_USERS

#include "dates.h"


struct Privs {             /* each user&group has one */
   long   MBaseFlags;      /* 580 */
   long   FBaseFlags;      /* 584 Can user reach which subs ? */
   long   LBaseFlags;      /* 588 Gfiles/Pfiles/Telnet/Other flags */

   ULONG   ABits;          /* 592 see bitdefs above */

   long   DailyDownBytes;  /* 596 */
   long   DailyUpBytes;    /* 600 */
   long   XFreeBytes;      /* 604 !!! NOT USED ANYMORE !!! */
   long   XFreeFiles;      /* 608 SEE GLOBAL DEFAULT IN CONFIG1 */

   short   Calls;          /* 612 */
   short   CallMinutes;    /* 614 */
   short   DailyMinutes;   /* 616 */
   short   DailyDownloads; /* 618 per 24 hr DAY */
   short   DailyUploads;   /* 620 same */
   short   Messages;       /* 622 */
   short   Feedbacks;      /* 624 */
   short   EditorLines;    /* 626 */
   short   Idle;           /* 628 */
   short   MaxMailKBytes;  /* 630 */
   short   PurgeDays;      /* 632 */

   UBYTE   FileRatio;      /* 634 */
   UBYTE   ByteRatio;
   UBYTE   FileRatio2;
   UBYTE   ByteRatio2;
   UBYTE   FileRatio3;
   UBYTE   ByteRatio3;

   ULONG   ABits2;         /* 640 now gives 64 flags! */

   short   SigLines;       /* 644 how many lines per signature */

   UBYTE   AllowAliases;   /* 646 new tri-state variables */
   UBYTE   DeleteOwn;
   UBYTE   Anonymous;
   UBYTE   PrivateArea;

   short   DailyPfileMinutes; /* 650 */
   short   LogToMail;         /* account # to send log */

   long   LogFlags;        /* 654 */

   UBYTE   CallBack;       /* 658 Tri-state */
   UBYTE   TermLink;       /* 659 */
   UBYTE   CallerID;       /* 660 */
   UBYTE   PageSysop;      /* 661 */

   LONG   notused;

   char   exp[ 2 ];

   short   Alias;          /* 668 */
   short   Dictionary;     /* 670 */
} __attribute__((packed));

/* LONG Aligned, total length==672 */


struct UserData {               /* each user account */
   long   IDNumber;             /* 0 unique (serial) ID # */
   char   Handle[21];           /* 4 */
   char   RealName[26];         /* 25 */
   char   Address[31];          /* 51 */
   char   CityState[31];        /* 82 */
   char   ZipCode[11];          /* 113 */
   char   Country[8];           /* 124 */
   char   PhoneNo[17];          /* 132 Euro 8-7 format POSSIBLE!! */
   char   PassWord[15];         /* 149 */
   char   Comments[34];         /* 164 */
   char   Macro[3][36];         /* 198 user's defined macro keys */
   char   Organ[31];            /* 306 */
   UBYTE   PhoneVerified;       /* 337 */

   struct   PortData *z;        /* for lock account */

   struct   IsDate Birthdate;   /* 342 */
   struct   IsDate FirstCall;   /* 348 */
   struct   IsDate LastCall;    /* 354 */

   ULONG   EMailFlags;          /* 360 */
   
   USHORT   InterfaceType;      // 364 - 0: CNet/4 Type -  number entered = read message x
                                //       1: CNet/3 Type - number entered = move to subboard x

   struct   IsDate ConnectDate; /* 366 - date of connection */
   struct   IsDate YankDate;    /* 372 - date of last yank */

   BYTE   Access;               /* 378 - current access level */
   BYTE   ExpireAccess;         /* 379 - access level to expire to when "ExpireDate" is reached */

   UBYTE ShowNewsHead;          // TRUE = Show news header when reading UUNEWS items. 26-Sept-97
   BYTE   PfileTimeToday;

   UBYTE SplitChatFlags;        // &1 = Use split-screen chat for CC

   BYTE   AutoDecodeMail;       // auto-decode MIME in EMail

   BYTE   TextSet;              // 384 - which BBSTEXT/MENU translation
   BYTE   TimeForm;             // which time form to use
   BYTE   MoreMode;             // more? mode
   BYTE   LineFeeds;            // user requires line feeds
   BYTE   FracBal;              // 1/100ths of a cent
   BYTE   TimeZone;             // time zone (relative to local system time)
   BYTE   Colors;               // can user display ANSI colors?
   BYTE   Tabs;                 // can user do ANSI tabs?
   BYTE   ANSI;                 // 392 - user's ANSI type
   BYTE   Sex;                  // user's gender
   BYTE   AutoMore;             // More? and CLS between messages/responses?
   BYTE   TermType;             // Terminal emulation
   BYTE   HelpLevel;            // 396 - novice, intermediate etc..
   BYTE   CompType;             // computer type - relative to top of section 30; in BBSMENU
   BYTE   DefProtocol;          // 398 - user's default txfer protocol
   BYTE   TermWidth;            // number of characters per line
   BYTE   TermLength;           // 400 - number of lines for full screen
   BYTE   MailBoxOpen;          // 401 - mailbox open or closed
   short  MailForward;          // 402 - mail forward to account stored here - if 0, no forward

   long   TotalCalls;           /* 404 */
   long   PubMessages;          /* 408 */
   long   PriMessages;          /* 412 */
   long   UpBytes;              /* 416 KILO bytes */
   long   UpFiles;              /* 420 */
   long   DownBytes;            /* 424 KILO bytes */
   long   DownFiles;            /* 428 */

   long   FileCredits;          /* 432 CREDITS used now! */
   long   ByteCredits;          /* 436 */
   long   TimeCredits;          /* 440 */

   long   Balance;              /* 444 */
   long   NetCredits;           /* 448 */
   long   DoorPoints;           /* 452 */

   long   DayUpBytes;           /* 456 */
   long   DayDownBytes;         /* 460 */
   short   DayUpFiles;          /* 464 */
   short   DayDownFiles;        /* 466 */

   short   CallsToday;          /* 468 */
   short TimeToday;             /* 470 */
   short   LogonAttempts;       /* 472 */

   USHORT   BaudRate;           /* 474 */
   USHORT   HighBaud;           /* 476 */

   char   UUCP[11];             /* 478 - valid length of UUCP name is still 9 characters with one termination \0 to make 10 total */

   UBYTE   PName;               /* 489 flags, are these data     */
   UBYTE   PAge;                /* items to be kept private? */
   UBYTE   PAddress;            /* street address only */
   UBYTE   PVoice;              /* phone numbers */
   UBYTE   PData;

   char   Banner[43];           /* 494 */

   char   VoiceNo[17];          /* 537 Euro 8-7 format POSSIBLE!! */

   short   YankCount;           /* 554 */

   struct   IsDate ExpireDate;  /* 556 */
   struct   IsDate FNewDate;    /* 562 */

   char   more[7];              /* 568 reserved */

   UBYTE   YankCR;              /* 575 add CR's to yank text? */
   UBYTE   YankArc;             /* which archiver to use? */
   BYTE   DefEditor;            /* 577 */
   BYTE   AutoHide;
   BYTE   MailSort;             /* 579 */
   struct   Privs MyPrivs;      /* 580 THESE ACTUALLY USED, not AGC */
} __attribute__((packed));



// bit numbers for ABits2 of struct Privs
#define EDHANDLE_BIT        0
#define EDREALNAME_BIT      1
#define EDADDRESS_BIT       2
#define EDVOICE_BIT         3
#define EDDATA_BIT          4
#define BANNER_BIT          5
#define NOT_USED_BIT        6
#define PORTMONITOR_BIT     7
#define VOTEMAINT_BIT       8
#define ALARMSYSOP_BIT      9
#define OPENSCREEN_BIT     10
#define OPENCAPTURE_BIT    11
#define NETMAIL_BIT        12
#define FREQSEND_BIT       13
#define NETCOSTEXEMPT_BIT  14
#define EXPANSION1_BIT     15
#define EXPANSION2_BIT     16
#define EXPANSION3_BIT     17
#define EXPANSION4_BIT     18
#define UUCPMAIL_BIT       29
#define NETCOSTCREDITS_BIT 20
#define HOLDANDCRASH_BIT   21
#define FEMPTYTRASH_BIT    22
#define BROADCAST_BIT      23
#define SUPERUSER_BIT      24

/* bit defs for ABits2 of struct Privs */
#define EDHANDLE_FLAG       (1L << EDHANDLE_BIT)
#define EDREALNAME_FLAG     (1L << EDREALNAME_BIT)
#define EDADDRESS_FLAG      (1L << EDADDRESS_BIT)
#define EDVOICE_FLAG        (1L << EDVOICE_BIT)
#define EDDATA_FLAG         (1L << EDDATA_BIT)
#define BANNER_FLAG         (1L << BANNER_BIT)
#define NOT_USED_FLAG       (1L << NOT_USED_BIT)
#define PORTMONITOR_FLAG    (1L << PORTMONITOR_BIT)
#define VOTEMAINT_FLAG      (1L << VOTEMAINT_BIT)
#define ALARMSYSOP_FLAG     (1L << ALARMSYSOP_BIT)
#define OPENSCREEN_FLAG     (1L << OPENSCREEN_BIT)
#define OPENCAPTURE_FLAG    (1L << OPENCAPTURE_BIT)
#define NETMAIL_FLAG        (1L << NETMAIL_BIT)
#define FREQSEND_FLAG       (1L << FREQSEND_BIT)
#define NETCOSTEXEMPT_FLAG  (1L << NETCOSTEXEMPT_BIT)
#define EXPANSION1_FLAG     (1L << EXPANSION1_BIT)
#define EXPANSION2_FLAG     (1L << EXPANSION2_BIT)
#define EXPANSION3_FLAG     (1L << EXPANSION3_BIT)
#define EXPANSION4_FLAG     (1L << EXPANSION4_BIT)
#define UUCPMAIL_FLAG       (1L << UUCPMAIL_BIT)
#define NETCOSTCREDITS_FLAG (1L << NETCOSTCREDITS_BIT)
#define HOLDANDCRASH_FLAG   (1L << HOLDANDCRASH_BIT)
#define FEMPTYTRASH_FLAG    (1L << FEMPTYTRASH_BIT)
#define BROADCAST_FLAG      (1L << BROADCAST_BIT)
#define SUPERUSER_FLAG      (1L << SUPERUSER_BIT)


/* bit numbers for ABits of struct Privs */
#define EMAIL_BIT          0
#define PFILE_BIT          1
#define GFILE_BIT          2
#define ULIST_BIT          3
#define SYSOP_BIT          4
#define REWARDS_BIT        5   /* may receive download rewards? */
#define FCREDEX_BIT        6
#define BCREDEX_BIT        7
#define EXPIREMAIL_BIT     8
#define BULKMAIL_BIT       9
#define XXXXXXXXX6_BIT    10
#define URGENTMAIL_BIT    11
#define TIMESBP_BIT       12
#define FILEADD_BIT       13
#define READANY_BIT       14
#define DELETEANY_BIT     15
#define SUSPENDACCT_BIT   16 // account suspended - user cannot login
#define XXXXXXXXX4_BIT    17
#define AUTOVALID_BIT     18
#define XXXXXXXXX2_BIT    19 /* not used */
#define SEEANON_BIT       20
#define XXXXXXXXX3_BIT    21 /* not used */
#define CONF_BIT          22
#define CONFCON_BIT       23
#define MCI1_BIT          24
#define MCI2_BIT          25
#define RELOGON_BIT       26
#define RECEIVEMAIL_BIT   27
#define FORWARD_BIT       28
#define VOTETOPIC_BIT     29
#define VOTECHOICE_BIT    30
#define NOLOCKS_BIT       31


/* bit defs for ABits of struct Privs */
#define EMAIL_FLAG       (1L << EMAIL_BIT)
#define PFILE_FLAG       (1L << PFILE_BIT)
#define GFILE_FLAG       (1L << GFILE_BIT)
#define ULIST_FLAG       (1L << ULIST_BIT)
#define SYSOP_FLAG       (1L << SYSOP_BIT)
#define REWARDS_FLAG     (1L << REWARDS_BIT)
#define FCREDEX_FLAG     (1L << FCREDEX_BIT)
#define BCREDEX_FLAG     (1L << BCREDEX_BIT)
#define EXPIREMAIL_FLAG  (1L << EXPIREMAIL_BIT)
#define BULKMAIL_FLAG    (1L << BULKMAIL_BIT)
#define XXXXXXXXX6_FLAG  (1L << XXXXXXXXX6_BIT)
#define URGENTMAIL_FLAG  (1L << URGENTMAIL_BIT)
#define TIMESBP_FLAG     (1L << TIMESBP_BIT)
#define FILEADD_FLAG     (1L << FILEADD_BIT)
#define READANY_FLAG     (1L << READANY_BIT)
#define DELETEANY_FLAG   (1L << DELETEANY_BIT)
#define SUSPENDACCT_FLAG (1L << SUSPENDACCT_BIT)
#define XXXXXXXXX4_FLAG  (1L << XXXXXXXXX4_BIT)
#define AUTOVALID_FLAG   (1L << AUTOVALID_BIT)
#define XXXXXXXXX2_FLAG  (1L << XXXXXXXXX2_BIT)
#define SEEANON_FLAG     (1L << SEEANON_BIT)
#define XXXXXXXXX3_FLAG  (1L << XXXXXXXXX3_BIT)
#define CONF_FLAG        (1L << CONF_BIT)
#define CONFCON_FLAG     (1L << CONFCON_BIT)
#define MCI1_FLAG        (1L << MCI1_BIT)
#define MCI2_FLAG        (1L << MCI2_BIT)
#define RELOGON_FLAG     (1L << RELOGON_BIT)
#define RECEIVEMAIL_FLAG (1L << RECEIVEMAIL_BIT)
#define FORWARD_FLAG     (1L << FORWARD_BIT)
#define VOTETOPIC_FLAG   (1L << VOTETOPIC_BIT)
#define VOTECHOICE_FLAG  (1L << VOTECHOICE_BIT)
#define NOLOCKS_FLAG     (1L << NOLOCKS_BIT)

// obsolete

struct ULock {                  /* account locking node!!! */
   short   Account;
   short   Wait;

   struct   SignalSemaphore Sem;

   struct   UserData *user1;    // as of 17-Feb-98, pointer into User cache
   struct   UserData *u;

   struct   ULock *Next;
} __attribute__((packed));

struct AccessGroup {
   char   Name[32];             /*   0 */
   struct   Privs DefPrivs;     /*  32 */

   short   ExpireDays;          /*  32+92 */
   UBYTE   ExpireAccess;        /*  32+94 */

   char   exp[29];
} __attribute__((packed));

struct KeyElement3 {
   char   UUCP[8];
   long   IDNumber;             /* high order byte ALWAYS 0 */
   long   phone1, phone2;       /* data phone's area code & local # */
   char   Handle   [21];
   char   RealName [26];
   UBYTE   PName;
} __attribute__((packed));

struct OldKeyElement {
   char   Handle[22];           /* actual user handle */
   short   HandleX;             /* account number where found */
} __attribute__((packed));

struct KeyElement4 {
   char   UUCP[11];             // 10 characters allowed in UUCP name - plus terminating 0 byte
   long   IDNumber;             /* high order byte ALWAYS 0 */
   long   phone1, phone2;       /* data phone's area code & local # */
   char   Handle   [21];
   char   RealName [26];
   UBYTE   PName;

   BYTE Access;                 // user's access group!
} __attribute__((packed));


// CNUserCache is a READ ONLY system structure!!
//
// If you alter (WRITE) this data directly, you are only asking for trouble
// and future incompatibility.
//
// Use the UCacheRead(), UCacheWrite(), LockAccount() and UnLockAccount() functions!
//
struct CNUserCache
   {
   struct UserData *udata;      // cached/dynamic user data.
   short account;               // account number of cached user instance.
   LONG accessed;               // CNet long value representing time data last accessed (read or write)
   struct CNUserCache *next;    // pointer to next instance of CNUserCache
   } __attribute__((packed));

#endif
