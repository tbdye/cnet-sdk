
#ifndef CNET_CONTROL
#define CNET_CONTROL

#include "ports.h"
#include "config.h"

/*
 * IMPStruct - IMP (Inter-Module Process) linked list node.
 * Stored in MainPortExtension->IMPList, protected by MPE->sem[3].
 * Field sizes inferred from original SDK examples; the original
 * struct definition was not shipped in public headers.
 */
struct IMPStruct {
   struct Node node;            // Exec list linkage
   char   Name[80];             // IMP entry name
   char   ProcessName[80];      // associated process name
   char   ProcessReply[80];     // reply port name
   UBYTE  dflag;                // display flag
   UBYTE  pflag;                // process flag
} __attribute__((packed));

struct   PortConfig {
   UBYTE   online;         // port loaded or not
   UBYTE   open;           // port screen state - see OPEN_* flags below

   UBYTE   check;          // current/saved states for control panel toggle buttons.
                           // see CHECKBIT_* flags below.. also MainPort->check_latch

   UBYTE   idle;           // clock, SAG, SAM ?

   UBYTE   offline;        // port loaded, but modem offline
   UBYTE   not_used;

   UBYTE   bplanes;
   UBYTE   lace;           // 0=none, 1=24 line, 2=49 line

   USHORT   sizes[8];      // workbench window & zoom size
} __attribute__((packed));

#define SCR_OPEN_PERMANENT  1
#define SCR_OPEN_FORCALL    2
#define SCR_OPEN_WORKBENCH  3

#define CHECKBIT_DOORSCLOSED 0
#define CHECKBIT_FILESCLOSED 1
#define CHECKBIT_MSGSCLOSED  2
#define CHECKBIT_NONUSER     3
#define CHECKBIT_SYSOPIN     4


struct MainPortExtension
   {
   struct SignalSemaphore sem[20];  // sem[0] = lock reload_text variable..
                                    // sem[1] = lock idd (identd data) before insertion/removal
                                    // sem[2] = lock the global MailKill list
                                    // sem[3] = lock IMP list
                                    // sem[4] = lock HOSTNAME/SPAM filter config file before accessing
                                    // sem[5] = lock uumail dir - causes mail-task to "pause" until this semaphore is released
                                    //          mail-task will obtain a lock on sem[5] before it tries to access uumail dir.
                                    // sem[6] = lock global MIME list (myp->MPE->mimelist)
                                    // sem[7] = lock MPE->FTPdList or any process within
                                    // sem[8] = lock MPE->TxProcessList
                                    // sem[9..19] not yet assigned/used.

   UBYTE reload_text;               // set to 1 to cause control to reload text/menu.
                                    // you MUST ObtainSemphore(&myp->MPE->sem[0]) before accessing!

   UBYTE newsman_up;            // see TASK_UP_* defines below..

   struct Task   *newstask;     // pointer to news-task Task structure if running.
   struct IdentdData *idd;      // linked list of identd info for in-use clients
   struct List *MailKillList;   // standard Exec List with Node ln_name(s) containing the list of mail kill patterns
   struct List *CPortList;      // Exec list used for Control listview - PRIVATE - accessing this variable may result in unpredictable results

   UBYTE UpdateGUI_notused;     // ** expansion - not used

   struct ConfigExtension gc2;	// second set of config variables
   										// MainPortExtension variable additions
   										// will be added above here.

   struct List IMPList;         // Exec list of IMPStruct(s) - be sure to
                                //   ObtainSemaphore(&myp->MPE->sem[3])
                                //   before accessing and
                                // ReleaseSemaphore(&myp->MPE->sem[3]) afterwards.

   char *statline;              // pointer to allocated Control stats line

   struct SignalSemaphore *MailSem;   // user account mail semaphores
                                      // pointers to structures for CNet mailbox locking.
                                      // Each array member applies to its ordinal account_number-1.
                                      // eg. MailSem[0] is account 1
                                      //     MailSem[1] is account 2 etc.

   UBYTE smtpd_up;           // see TASK_UP_* defines below..
 
   struct Task *smtpdtask;   // pointer to cnsmtpd Task structure if running.
   
   struct MIMEContentType *mimelist;	// linked list of sysop-configured MIMEContentTypes. (see mail.h)

	// added 04-Nov-97
	struct SignalSemaphore dccsem;

	struct List DCCList;               // list (nodes are struct DCCStruct)
                                      // containing DCC requests for ALL ports.
                                      // List is empty at startup and
                                      // specific ports clear its own DCC
                                      // structures at logoff.  This
                                      // allows users to QUIT CNetIRC and
                                      // restart with DCC requests still
                                      // present from the last session in
                                      // the current call.

   UBYTE telnetd_up;          // see TASK_UP_* defines below..

   struct Task *telnetdtask;  // pointer to telnetd Task structure if running.

	char *pubscreenname;       // pointer to string containing name of public screen used for CNet GUIs.

	struct List FTPdList;		// list of active ftpd processes (struct FTPProcess)

	short telnetd_acnt;			// count of autoloaded telnetd ports

	struct List TxProcessList;	// list of active file test/transform processes
   } __attribute__((packed));

struct MainPort {          // common, public to all ports
                           // the same msgport that Findport("cnetport")
                           // returns a pointer to..

   struct  MsgPort mp;     // 0  - standard EXEC message port
   char    portName[22];   // 34

   char    LastOn[32];     // 56

   long    Nums[5];        // See NUMS_* defines below

   ULONG   SAG[2][72];             // 108 activity graph
   long   SAM[5][15];              // activity monitor

	// Change SAMDate to ULONGS to solve y2k problem!!!!!
   struct   IsDate SAMDate[6];     // SAMDate[1] = Control boot time

   long   um[10];          // for C programmer use.  You can
                           // (typecast) them as you need to.
                           // Initially at bootup == 0

   struct   Window      *PWindow;        // pointer to control window if open; otherwise 0
   struct   TransType   *FirstTT;
   struct   SubboardType4 *Subboard;
   struct   LinkPort   *link;            // all-linked port users!
   struct   KeyElement4   *Key;          // alpha order, etc.
   short      *IName;                    // Indexes into KeyElement4 array
   short      *IPhone;                   // Indexes into KeyElement4 array
   char      *SubTitle;
   struct   MailLock   *mlock;
   struct   ChargeSet   *FirstCharge;

   long   ns;           // number of subs in memory (allocated)

   UBYTE   toss_up;		// see TASK_UP_* defines below..
   UBYTE   yank_up;     // see TASK_UP_* defines below..
   UBYTE   serv_exit;   // internally set as TRUE to tell mail-task, yank-task and any other "server" programs to quit - control is closing down/quitting
   UBYTE   ftpd_up;     // see TASK_UP_* defines below..
   UBYTE   mail_up;		// see TASK_UP_* defines below..
   UBYTE   fileman_up;	// see TASK_UP_* defines below..


   UBYTE useredit_up;   // 29-Jul-97
                        // is UserEd running?  Control cannot exit until UserEd quits

   UBYTE   smtpd_up;		// see TASK_UP_* defines below..

	struct Task *ftpdtask; // FTPd task pointer

   long   OpenPfiles;   // number of currently open doors

   struct   NoiseType   *FirstNT;

   long   ClickMaint;   // can we use local mouse maint?
   long   Verbose;      // do not redirect to NIL: for debug

   long   s1;           // version of CONTROL
   long   s2;
   long   s3;           // serial number of owner 0==DEMO

   struct   BitMap  ClockBrush;   // 40 bytes - for the large port clock on idle ports
   struct   SignalSemaphore SEM[19];

               // for critical structure/file/variable locking..
               // Don't hold these in LOCKED state for long
               // as DEADLOCKS will occur!

               // ObtainSemaphore(SEM[x]) to lock, ReleaseSemaphore(SEM[x]) to unlock
               // SEM[0]  = for locking myp (MainPort) itself!
               // SEM[1]  = User myp->Key[], myp->IName, myp->PName, myp->Phone locking
               // SEM[2]  = z.Upload/z.nuploads/z.NoUploaded locking
               // SEM[3]  = for locking serial device
               // SEM[4]  = myp->NUMS[1] locking (NUMS[1] = number of user account currently active
               // SEM[5]  = net/echomail import/export,  subbards3/4 data file and COMPLETE allocated subboard locking - lock before altering/ordering subboard structures
               // SEM[7]  = conference (join) chat locking
               // SEM[8]  = joinlink/conference (join) ROOM locking
               // SEM[10] = Pfile locking
               // SEM[11] = bbs.udata, bbs.cuser & userlist locking
               // SEM[12] = sysdata:log/%s locking
               // SEM[13] = semaphore for locking LIST items - ie. items in a type "LIST" subboard
               // SEM[14] = vote list locking while reading/editing vote items/results
               // SEM[16] = lock before accessing the CopyCDRom() function
               // SEM[18] = semaphore for locking SAM
                                       

   struct   AccessGroup     AGC[32];   // BBS.ADATA / Access Group stuff

   struct   NewConfig1 gc;             // global configuration defaults

   char   SysPassword[6][21];
   ULONG   IDQ;

   short   root;                       // first area#
   struct   PortConfig  pc   [101];    // For CONTROL, remembers things about each port's window menus, etc,..
   struct   PortData   *PortZ[100];    // pointers to each port's PortData struct
   struct   Room       *CRoom[100];    // pointers to ROOM structures..

   struct   ULock *FirstULock;         // Account Locking ... list of dynamically locked accounts!
   short   LastTickHour;               // control is ticking ...
   short   LastTickMinute;

   short   HiPort; // highest port loaded
   short   nPorts;

   char   regto[32], regco[32];

   struct   SignalSemaphore ysem;      // the 3.1 Yank-Task
   struct   YankType *yfirst;

   struct   SignalSemaphore isem;      // WAS the 3.1 Item-Task = not used
   struct   ItemServerType *ifirst;    // not used!

   struct   SignalSemaphore csem;      // the 3.1 Charge-Server
   struct   ChargeType *cfirst;

   struct   SignalSemaphore sem;

   struct   PortData *z0;              // control's PortData structure - copied to other ports as default, when opened..

   /* new for v4.12 */
   UBYTE      cron_up;                 /* non-zero if events are being externally edited - Control WILL NOT exit as long as this is >0 */
   struct   JobType4 *Events;          /* configured events */
   struct   SignalSemaphore eventsem;  /* lock this using ObtainSemaphore() before altering myp->Events! */
                                       /* make sure to release using ReleaseSemaphore() after changing events list */

   struct   SignalSemaphore cesem;     /* control event semaphore locking  - internal use only! */
   struct   JobType4   *cEvents;       /* list of events to be executed by Control */

   struct   SignalSemaphore flsem;     /* semaphore for locking the LIST of file locks */

   struct   Library *NodelistBase;     /* traplist.library - nodelist base */

   UBYTE check_latch;      // 3rd party coders - SET this (1=SET) if ANY of the PortConfig "check" bits are altered via doors
                           //                    changes will not take effect unless this is latched!!

   // task pointers Added in v4.21/v4.22 - used to send signals to CNet servers
   // ie. use Signal(myp->mailtask, SIGBREAKF_CTRL_C) to tell mail-task to quit
   //
   //     note: server will set it's myp->#?task pointer to ZERO when it exits
   //           You cannot be sure the task has exited until it's Task pointer
   //           is 0.  Also, if a server is processing a packet (ie. mail item,
   //           test/transform, NNTPGet etc, the task/server will not exit
   //           until the current operation is complete.
   //
   struct Task   *mailtask;               // pointer to mail-task task structure
   struct Task   *filetask;               // pointer to file-task task structure
   struct Task   *nntptask;               // pointer to nntpget task structure
   struct Task   *yanktask;               // pointer to yank-task task structure

   struct MainPortExtension *MPE;         // MainPort data extensions - see struct MainPortExtension above.
} __attribute__((packed));

#define HIPORT_MAX 99 // highest port that can be loaded..

#define TASK_UP_NOTRUNNING 0
#define TASK_UP_GUIOPEN    1
#define TASK_UP_HIDDEN     2 // totally hidden - must use exchange to show iface
#define TASK_UP_MENUFIED   3
#define TASK_UP_ICONIFIED  4
#define TASK_UP_RUNNING    5 // temporary state - only valid when task started


#define NUMS_CURRENT_ACCOUNTS 0 // # of physical user accounts
#define NUMS_INUSE_ACCOUNTS   1 // # of in-use user accounts
#define NUMS_HIGH_ID          2 // last assigned unique ID Number
#define NUMS_CALLS_TOTAL      3 // # of calls to system TOTAL
#define NUMS_CALLS_LOGGED     4 // # of calls now logged          

#endif
