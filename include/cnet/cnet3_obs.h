#ifndef CNET_CNET3_OBS_H
#define CNET_CNET3_OBS_H

// CNet/3 subboard structure
struct NewSubboardType
   {
   char   Title[31];      /* 0  */
   char   SubDirName[21]; /* 31 */
   char   DataPath  [40]; /* 52 */
   long   Parts;          /* 92 partitions 0-31 now allowed */

   long   SubOpIDs [6];   /* 96 */

   short   MRewardBytes;  /* 120 % of byte  size to award */
   short   NRewardFiles;  /* 122 # of file creds to award uploader each time his file is downloaded */

   struct   FreeType *free0; /* 124 */
   long   nf_obs;            /* 128 */
   long   nfmax_obs;         /* 132 */

   short   SubOpAccs[6];  /* 136 */

   char   Filler[6];      /* 148 shown during SCAN */

   UBYTE   fdelta;        /* 152 */
   UBYTE   floaded;       /* 154 */

   UBYTE   Direct;        /* 156 Disk-exchange type */
   UBYTE   Vote;          /* 157 May non-sysops add topics? */

   short   FreeDays;      /* 158 when files are auto-free */

   UBYTE   Subdirectory;  // 1==dir, 2==text/door
   UBYTE   Closed;

   long   UnionFlags;     /* 162 Must have all of these flags */
   long   Access;         /* 166 Groups which may ENTER */
   long   PostAccess;     /* 170 Groups which may Post  */
   long   RespondAccess;  /* 174 ... Respond */
   long   UploadAccess;   /* 178 ... Upload */
   long   DownloadAccess; /* 182 ... Download */
   long   ComputerTypes;  /* 186 */
   long   Hours;          /* 190 restricted entry hours */
   long   HourUnionFlags; /* 194 */
   long   HourAccess;     /* 198 groups which may Enter during Hours*/
   long   Baud;           /* 202 minimum Baud rate to enter */
   long   BaudHours;      /* 206 hours to enforce min baud rate */

   UBYTE   Gender;        /* 210 0 or 'M', 'F' */
   UBYTE   AllowAliases;
   UBYTE   PurgeStatus;   /* 212 */
   UBYTE   Youngest;      /* 213 */

   UBYTE   EXTRA;

   UBYTE   Oldest;        /* 215 */

   struct   IsDate   Sent; /* 216 FIDO to track new messages */

   short   MDownBytes;    /* 222 % of download bytes to subtract */
   short   NDownFiles;    /* 224 # of files to subtract per download */
   short   MTimeCredit;   /* 226 % of time to give back for uploads */

   long   MinFreeBytes;   /* 228 to upload in this subboard */
   short   SubSort;       /* 232 */
   short   InactiveDays;  /* 234 */

   UBYTE   RealNames;     /* 236 use real names instead of Handles? */
   UBYTE   Addressees;
   UBYTE   Anonymous;
   UBYTE   override;      /* 239 how to set the use defaults flags in newly uploaded items */

   UBYTE   Verification;  /* 240 */
   UBYTE   DupCheck;
   UBYTE   ShowUnvalidated;
   UBYTE   NoMCI;

   UBYTE   PrivateArea;   /* 244 0/1 No/Yes ... 2==FORCE */
   BYTE   SDownFiles;     /* Which download charge schedules? */
   BYTE   SDownBytes;
   UBYTE   Test;          /* test at upload time */

   UBYTE   Transform;     /* 248 run script at upload time */
   UBYTE   AutoAdopt;     /* Adopt Orphans at a-maint */
   UBYTE   WeedResponses; /* Use inactive days against indiv responses */
   UBYTE   NoSignatures;

   UBYTE   NoReadCharges; /* 252 opt to short circuit accounting system */
   UBYTE   NoWriteCharges; /* 253 for reading/writing messages */

   long   Arcs;           /* 254 which Arcs in BBSARC are allowed uploaded*/

   short   UpRatioBytes;  /* 258 which set of file/byte ratios to use? */
   short   UpChargeBytes; /* 260 which set of charges to use?          */
   short   UpRatioFiles;  /* 262 */
   short   UpChargeFiles; /* 264 */

   UBYTE   Invitation;    /* 266 */
   UBYTE   UserMustJoin;
   UBYTE   DeleteOwn;
   UBYTE   Marker;        /* 0 = subdir,         1 = Messages,  2 = Files,
                             4 = text/door area, 5=Text file, etc,
                             same as "object class" during "AL"
                             &128 = Killed */

   long   Transforms;     /* 270 which BBSARCS to transform */

   long   ExportTo;       /* 274 */

   char   NothingNow[8];  /* 278 */

   ULONG   count;         /* 286 replaces the _COUNT file */
   ULONG   hiwater;       /* 290 */

   UBYTE   NetNumber;     // 294 ->   0 = none, 1=Usenet/NNTP, 2=mailing list 
                          //     ->   type 2 (mailing list) added 25-Jun-97 v4.27c

   UBYTE   Locked;        // 295 Does AMAINT have a lock here?
   UBYTE   DLnotifyULer;  // 296

   char   TransformTo[5]; // 297

   UBYTE   QWKReplies;    // 302
   BYTE   SBaseUse;       // 303  which BBSCHARGE rate to use?
   UBYTE   CarbonCopy;    // 304
   UBYTE   CDROM;         // 305 copy to hard drive before download?

   struct   IsDate LastUpload;
   struct   IsDate LastMessage;

   short   Users;         //   how many people are in this sub
                          //   REAL TIME (not # of members)

   long   Created;        // for membership determination

   short   Parent;        // pointers to other subboard #s
   short   Child;
   short   Next;

   short   TimeLock;      // 330 minutes online before available

   char   ZeroPath[48];   // 332 - part0/direct/CNetCommanderDirect path.
   char   Origin  [60];   // 380

   // if more room is needed, I could put all of this into a pointer to a 'sub-info' structure

   short   MaxItems;      // 440
   UBYTE   Persist;       // 442
   UBYTE   Delay;         // 443

   struct   ItemType3 *Item;
   struct   ItemHeader *ihead;
   struct   MessageType3 *NewMess;
   struct   SignalSemaphore *sem;

   long   nm;             // 460 # of _Messages
   short   rn;            // 464 # of _Items
   short   AllocItems;
   short   nNewMess;

   UBYTE   resetIP;       // 470 - set to TRUE to have CNet reset item pointers

   char   more_stuff[17]; // 471
} __attribute__((packed));


struct OldMessageType
   {
   long   ItemNumber;
   long   ResponseNumber;
   long   ByID;
   long   ToID;

   struct   IsDate PostDate;

   UBYTE   Imported;
   UBYTE   IsFile;
   } __attribute__((packed));

#endif /* CNET_CNET3_OBS_H */
