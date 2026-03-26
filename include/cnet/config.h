
#ifndef CNET_CONFIG
#define CNET_CONFIG

#define CONFIG_MSGPORT      "cnetconfig"

#define CONFIG_MAILTASK 	 "cnet:configs/Mail-Task.cfg\0"
#define CONFIG_NEWSTASK 	 "cnet:configs/News-Task.cfg\0"
#define CONFIG_FILETASK 	 "cnet:configs/File-Task.cfg\0"
#define CONFIG_YANKTASK 	 "cnet:configs/Yank-Task.cfg\0"
#define CONFIG_CRON			 "cnet:configs/events.cfg\0"
#define CONFIG_USEREDIT 	 "cnet:configs/usered.cfg\0"
#define CONFIG_IMP			 "cnet:configs/imp\0"
#define CONFIG_SMTPHOSTS	 "cnet:configs/smtpdhosts.cfg\0"
#define CONFIG_SMTPSPAM 	 "cnet:configs/smtpspam.cfg\0"
#define CONFIG_SMTPD 		 "cnet:configs/smtpd.cfg\0"
#define CONFIG_FTPD 			 "cnet:configs/ftpd.cfg\0"
#define CONFIG_MIME 			 "cnet:configs/mimewin.cfg\0"
#define CONFIG_MIMETYPES 	 "cnet:configs/mime.cfg\0"
#define CONFIG_TELNETD 	    "cnet:configs/telnetd.cfg\0"
#define CONFIG_TELNETDWIN   "cnet:configs/telnetdwin.cfg\0"
#define CONFIG_TELNETDBLOCK "cnet:configs/telnetdblock.cfg\0"
#define CONFIG_CONFIG       "cnet:configs/Config.cfg" 
#define CONFIG_ARCHIVERS    "cnet:configs/archivers"


struct LogType
	{
	char  name  [10];
	short id;

	char  custom[10];

	UBYTE flag;
	UBYTE other;
	} __attribute__((packed));

struct   FidoType
	{
	char  Name[25];

	USHORT   ZoneB,  Zone,
		NetB,   Net,
		NodeB,  Node,
		PointB, Point;

	USHORT   ZoneC, NetC, NodeC, PointC;

	char  EchoOrigin[78];
	USHORT   LoZone;

	char  NetOrigin[76];
	USHORT   Pointnet;
	USHORT   HiZone;
	} __attribute__((packed));

struct   ProtoType
	{
	char  Name[16];
	char  Env[36];
	short Flags;
	char  One[2];
	char  Path[22];
	} __attribute__((packed));

struct   ArcType
	{
	char  Suffix[8];
	char  Keyword[16];
	char  View[64];
	char  Test[64];
	char  Pack[64];
	char  Extract[64];

	long  Row;
	long  Column;
	} __attribute__((packed));

// new for 4.12
struct   NewArcType
	{
	char  Suffix[8];
	char  Keyword[16];
	char  View[64];
	char  Test[64];
	char  Pack[64];
	char  Extract[64];

	long  Row;
	long  Column;

	char  DizExtract[64];   /* CNet does a "CD <Config1.RAMpath>" before extracting */
	char  DizAdd[64];       /* CNet does a "CD <Config1.RAMpath>" before adding */
	} __attribute__((packed));

struct   ArcType4
	{
	char  Suffix[8];
	char  Keyword[16];
	char  View[64];
	char  Test[64];
	char  Pack[64];
	char  Extract[64];

	long  Row;
	long  Column;

	char  DizExtract[64];   // CNet does a "CD <Config1.RAMpath>" before extracting
	char  DizAdd[64];       // CNet does a "CD <Config1.RAMpath>" before adding

	char  LocalView[64];    // local viewer

	UBYTE ViewType;			// type of online viewer.  See ARCVT_* defs below
	BYTE	expansion[63];
	} __attribute__((packed));


// defines for Archiver ViewTypes
#define ARCVT_PIPE   0
#define ARCVT_SHELL  1
#define ARCVT_CNETC  2
#define ARCVT_AREXX  3



struct   EditorType
	{
	char  name[20];
	char  path[32];
	long  flags;
	} __attribute__((packed));

struct   Config1
	{
	char  MySystemName[26];
	char  MySysopName[26];
	char  MyLocation[26];
	char  MyPhoneNumber[20];
	char  MyBBSID[10];

	char  FontName[40];
	long  DefBalance;
	long  DefaultColor;
	long  BlankTicks;
	long  BlankBright;

	long  nUDBASEx;         // highest UDBASE partition
	long  MaxShortLines;    // Short description lines - 16 MAX
	long  MaxOpenPfiles;    // stuff from BBSCONFIG
	long  maxUserAccounts;
	long  NumRooms;
	long  nLinkPorts;       // max link ID
	long  nsub;
	long  BListPurgeDays;
	long  nselect;
	long  nupload;
	long  nlist;
	long  maxYankTasks;
	long  maxYankSize;
	long  maxYankDays;
	long  maxLogonAttempts;
	long  maxLogonTime;
	long  maxYanksPerUser;

	long  DefNetCredits;
	long  DefByteCredits;
	long  DefFileCredits;
	char  MyCountry[4];
	char  MyAreaCode[12];
	char  DefDefProtocol[4];
	char  Yprefix[4];       // Yank identifier
	long  DefDoorPoints;
	long  DefTimeForm;

	UBYTE MyLinkID;

	UBYTE LogonFeedback;
	UBYTE LogonSearch;
	UBYTE GuestUsers;
	UBYTE HideStatus;
	UBYTE ConfProfile;
	UBYTE MailFeedback;
	UBYTE SeparateTexts;
	UBYTE IndentSpaces;
	UBYTE CIDvalidate;
	UBYTE CIDhandleskip;
	UBYTE SkipIdlePorts;

	char  MyLinkPass[8];

	char  OLMpath[40],
		ZIPpath[40],
		EXTRACTpath[40],
		YANKwork[40],
		RAMpath[40],
		TERMpath[40],
		LocalEditor[40],
		CDROMpath[40],
		DictPath[40];

	char  FKey [10][80];
	char  TFKey[10][80];

	USHORT   DColors2[2];
	USHORT   DColors4[4];
	USHORT   DColors8[8];
	USHORT   DColors16[16];

	long  narc;
	long  ned;
	long  nproto;
	long  nfido;
	long  nlog;

	struct   ArcType     arc   [20];
	struct   EditorType ed     [8];
	struct   ProtoType  proto [16];
	struct   FidoType   fido  [12];
	struct   LogType     log   [88];

	char  MyUUCPName  [44];
	char  OutboundPath[40];
	char  InboundPath [40];

	ULONG TossInputSize;
	ULONG TossOutputSize;
	USHORT   DupeTableSize;

	UBYTE BadUnknown;
	UBYTE Trapdoor174;
	UBYTE BadDupes;
	UBYTE ZoneGateAdjust;
	UBYTE ShowKludges;
	UBYTE ThreeD;
	UBYTE DeleteUUCP;
	UBYTE CIDsilence;
	UBYTE TwoDotMsg;

	char  expansion;

	USHORT   RamUploadSize;

	char  RamUpload [30];
	char  DiskUpload[40];

	char  Nodelist  [40];
	} __attribute__((packed));


// used for CNet/4
struct   NewConfig1
	{
	char  MySystemName[26];
	char  MySysopName[26];
	char  MyLocation[26];
	char  MyPhoneNumber[20];
	char  MyBBSID[10];

	char  FontName[40];
	long  DefBalance;
	long  DefaultColor;
	long  BlankTicks;
	long  BlankBright;

	long  nUDBASEx;         // highest UDBASE partition
	long  MaxShortLines;    // Short description lines
	long  MaxOpenPfiles;    // stuff from BBSCONFIG
	long  maxUserAccounts;
	long  NumRooms;
	long  nLinkPorts;       // max link ID
	long  nsub;
	long  BListPurgeDays;
	long  nselect;
	long  nupload;
	long  nlist;
	long  maxYankTasks;
	long  maxYankSize;
	long  maxYankDays;
	long  maxLogonAttempts;
	long  maxLogonTime;
	long  maxYanksPerUser;

	long  DefNetCredits;
	long  DefByteCredits;
	long  DefFileCredits;
	char  MyCountry[4];
	char  MyAreaCode[12];
	char  DefDefProtocol[4];
	char  Yprefix[4];       // Yank identifier
	long  DefDoorPoints;
	long  DefTimeForm;

	UBYTE MyLinkID;

	UBYTE LogonFeedback;
	UBYTE LogonSearch;
	UBYTE GuestUsers;
	UBYTE HideStatus;
	UBYTE ConfProfile;
	UBYTE MailFeedback;
	UBYTE SeparateTexts;
	UBYTE IndentSpaces;
	UBYTE CIDvalidate;
	UBYTE CIDhandleskip;
	UBYTE SkipIdlePorts;

	char  MyLinkPass[8];

	char  OLMpath[40],
		ZIPpath[40],
		EXTRACTpath[40],
		YANKwork[40],
		RAMpath[40],
		TERMpath[40],
		LocalEditor[40],
		CDROMpath[40],
		DictPath[40];

	char  FKey [10][80];
	char  TFKey[10][80];

	USHORT   DColors2[2];
	USHORT   DColors4[4];
	USHORT   DColors8[8];
	USHORT   DColors16[16];

	long  narc;
	long  ned;
	long  nproto;
	long  nfido;
	long  nlog;

	struct ArcType4 *arc; // allocated array of arctype4's

	short MaxFileProcess;	// maximum number of simultaneous test/tx.

	UBYTE ForceEmptyTrash;	// force trashcan empty at post-office/mailread exit
									// global setting here, over-rides user privs setting
									// for quick application of FORCE EMPTY globally..

	BYTE expansion2[8313]; // was struct NewArcType arc[20];

	struct   EditorType ed     [8];
	struct   ProtoType  proto [16];
	struct   FidoType   fido  [12];
	struct   LogType     log   [88];

	char  MyUUCPName  [44];
	char  OutboundPath[40];
	char  InboundPath [40];

	ULONG TossInputSize;
	ULONG TossOutputSize;
	USHORT   DupeTableSize;

	UBYTE BadUnknown;
	UBYTE Trapdoor174;
	UBYTE BadDupes;
	UBYTE ZoneGateAdjust;
	UBYTE ShowKludges;
	UBYTE ThreeD;
	UBYTE DeleteUUCP;
	UBYTE CIDsilence;
	UBYTE TwoDotMsg;

	UBYTE expansion;

	USHORT   RamUploadSize;

	char  RamUpload [30];
	char  DiskUpload[40];

	char  Nodelist  [40];

	UBYTE MIDFromHandle;    // if TRUE, use user's Handle to create UUCP/Mail ID, else use Handle.

	UBYTE FileTaskNotify;   // 0=mail/OLM users only on file test/tx failure
									// 1=mail/OLM users with ALL results

	BYTE DefMailSort;       // "System default" mail sorting method

	// 19-Feb-1997
	UBYTE MonitorUUMail;    // True if mail-task should monitor and import msgs from IUUMAIL:
	char UUMailPath[40];    // Path to where internet mail is placed (UUMAIL: is usual but configurable for future expansion etc,..)

	// 27-Feb-1997
	UBYTE CreateWebDir;     // CNet will create the user Web dir "mail:users/<MailID>/WEB" if this is TRUE (1)
									// This is for support of the new AWS cnetdir.library and will be used
									// by CNet itself in the future..

	// 01-Mar-97
	UBYTE NewsTaskPost;     // Enable news task posting.  If this is enabled (1)
									// all CNet posts/replies in a "Usenet news" subboard
									// are posted to the Internet service provider
									// using news-task.  Otherwise, CNet will use
									// "PostNews" or whatever program is specified on BBSTEXT
									// line 2151

	// 03-Mar-97
	char NewsServer[80];    // NewsServer to connect to for posting and retrieving news.
	// 08-Mar 97
	char NNTPPort;          // default=119

	UBYTE dynamicIP;        // if true, "NEWS" is placed in the path: line of outbound usenet messages processed by news-task.
									// else configured INet Hostname is used in the path: line


	char RootName[20];      // should match the username specified as "root" in Miami
									// and/or AmiTCP

	short ABufferSize;      // Sysops can now set their ABuffer size - can be made as
									// as the longest BBSTEXT line!  (See Config/Limits)

	char NewsPath[80];		// 23-Apr-97: Path to NEWS directory.
	} __attribute__((packed));


struct ConfigExtension
	{
	char MailServer[80];	// 30-May-97 - SMTP mail host

	UBYTE SMTPMail;		// 01-Jun-97 - TRUE (1) if CNet should use internal
                        //             SMTP routines to send outgoing mail to
                        //             mail host myp->gc2.MailServer
                        //
								//             FALSE (0) if CNet should post using the
                        //             "sendmail" command configured on BBSTEXT line
                        //             2133.

	ULONG	mailtimeout;	// mail-task timeout to check for spooled mail send
								// in seconds

	ULONG MailTaskBufMax;		// number of LINES to trim mail-task buffer to (0=no trim)
	ULONG NewsTaskBufMax;		// number of LINES to trim news-task buffer to (0=no trim)
	ULONG FileTaskBufMax;		// number of LINES to trim file-task buffer to (0=no trim)
	ULONG YankTaskBufMax;		// number of LINES to trim yank-task buffer to (0=no trim)
	ULONG SMTPBufMax;				// number of LINES to trim SMTPd buffer to (0=no trim)
	char	TimeZone[4];			// 3 characters representing the local timezone (eg. EST, CST, etc.)
	ULONG smtpdtimeout;			// number of seconds to wait before timing out on a cnsmtpd connection.
                              // SMTPD_MINTIMEOUT seconds is minimum.
	char PortLogDir[40];			// location to store user logs (named DDMMYY_USERID.cap)
                              // non-user caps (eg. term caps) are stored as portX.cap
                              // as before.
	char smtpdtemp[128];			// path to use for cnsmtpd tempfiles.
	short cache;               // number of accounts to cache.
	                           //       -1 = ALL - init at control boot
	                           //        0 = grow cache
	                           //        x = max cached accounts

	UBYTE telnetd_aload;       // autorun unused port(s) for telnetd?
	                           // if FALSE, only use LOADED telnetd ports..

	short maxtelnetd;				// maximum number of ports to allow/load for telnetd
	UBYTE ShowIPWhere;			// show IP instead of City/State for telnetd connections
	ULONG TelnetdBufMax;			// number of LINES to trim telnetd buffer to (0=no trim)
	
	ULONG FTPdBufMax;				// number of LINES to trim ftpd buffer to (0=no trim)

	ULONG mintelnetdmem;			// minimum amount of memory, in TRUE kilobytes
                              // (bytes/1024) that must be available before
                              // an autoload telnetd port can be spawned.

	ULONG nextsubser;			   // next subboard serial number to be used
	                           // when a subboard is created

	ULONG	newstimeout;			// news-task timeout to check for spooled news send
										// in seconds
	} __attribute__((packed));

#define SMTPD_MINTIMEOUT 60
#define UDATA_CACHE_ALL  -1



// this is the structure used (saved to disk also) for *-task configs
struct ServTaskConfig
	{
	char Version[30];
	char FontName[128];
	short FontSize;
	short WinTop;
	short WinLeft;
	short WinHeight;
	short WinWidth;

	// 24-Jan-97
	short hidestate;		// should never be 0 - see control.h TASK_UP_* defines
	} __attribute__((packed));


// added 11-Jan-97
// CNetGui.library's GetScreenInfo() fills this structure.
// The Screen must first be locked before calling GetScreenInfo()
struct CNetScreenInfo
	{
	char PubName[40];          // public screen name
	WORD LeftEdge;             // Display clip dimensions.  NOT the full screen size,
	WORD TopEdge;
	WORD Width;
	WORD Height;
	struct ColorMap *colormap; // screen's colormap
	struct TextAttr Font;      // font used for Title/drag-bar rendering
	} __attribute__((packed));


struct OtherStuff
	{
	LONG	lvtop;
	short	ileft;
	short	itop;

	struct IBox ibox; // window sizes

	// struct IBox
	// {
	//     WORD Left;
	//     WORD Top;
	//     WORD Width;
	//     WORD Height;
	// };

	long	click;

	USHORT	wzoom[4];

	UBYTE itype;	// hide type
	               // TASK_UP_GUIOPEN   = NO HIDE/GUI open
	               // TASK_UP_ICONIFIED = iconify
	               // TASK_UP_HIDDEN    = cx_hide only

	char	expansion[63];
} __attribute__((packed));

#endif
