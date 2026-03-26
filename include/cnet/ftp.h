#ifndef CNET_FTP_H
#define CNET_FTP_H

// defs for cnFTP anf cnFTPd

#define   FTP_BLKSIZE   2048         // packet size for file I/O

#define   ASCII_TYPE      0
#define   IMAGE_TYPE      1
#define   LOGICAL_TYPE   2

#define   CTLZ            26      // EOF for CP/M
#define   IPPORT_FTPD      20      // data port

// permission flags
#define   PERMIT_READ   1         // Read file(s)
#define   PERMIT_CREATE   2      // Create file(s)
#define   PERMIT_WRITE   4      // R/W/D existing file(s)

// FTP commands
#define   CMD_USER   0
#define   CMD_ACCT     1
#define   CMD_PASS   2
#define   CMD_TYPE     3
#define   CMD_LIST     4
#define   CMD_CWD     5
#define   CMD_DELE     6
#define   CMD_NAME     7
#define   CMD_QUIT     8
#define   CMD_RETR     9
#define   CMD_STOR     10
#define   CMD_PORT     11
#define   CMD_NLST     12
#define   CMD_PWD     13
#define   CMD_XPWD     14
#define   CMD_MKD     15
#define   CMD_XMKD     16
#define   CMD_XRMD     17
#define   CMD_RMD     18
#define   CMD_STRU     19
#define   CMD_MODE     20
#define   CMD_PASV   21
#define   CMD_FMAINT 22


struct ftpserv
	{
	struct Window *MonitorWin;	// pointer to sysop monitor window..
	                           // NULL if session not being monitored.
	                           // (not yet implemented)

	LONG psock;    // PASV socket
	char ttype;    // txfer type - see defines for TTYPE_XXX below
	int logbsize;  // Logical byte size for logical type

	BPTR *txfh;           // AmigaDOS file-handle of file being txfer'd

	char txfilename[128]; // FilePart() of file being transferred
	                      // only valid filename if txfh is non-NULL

	struct sockaddr_in port; // remote port for data connection
	struct sockaddr_in pasv; // for PASV mode

	short sblist;			// index of current LIST in myp->Subboard[] .. -1 if NONE
	short sbnum;			// index of current subboard in myp->Subboard[] .. -1 if NONE

	char home[512];		// path to user's FILES dir..
	char VSysLoc[1024];	// current (virtual file system) directory
								// eg. "/" or "/BASE/..."
								// "/BASE" = symlink to CNet file base
								// "/" = user's HOME dir
								
								// in the near future code will be added to allow
                        // sysop-defined symlinks similar to the internal
                        // symbolic link (mapping) of /BASE/ to the CNet
                        // file base..
								//
								// eg: in a ftpd config window (NOT in CNet:Config!),
                        // allow mapping of symlinks.
                        //
								//     eg. symlink "/LIBS/" as "dh0:libs"
								//     or  symlink "/CNETSUPPORT/" as "/base/cnet4_beta/"
								//
								// note that /BASE/ includes ALL partitions
								// for the CNet file base..

	LONG txloc;				         // # of bytes txferred..
	struct IsDate start_time;		// time file txfer began

	char PctStr[4];		// ASCII coded file progress (in %)
	char CPS[20];			// ASCII coded CPS (in bytes)
	} __attribute__((packed));

// mutually exclusive settings for ftpserv.ttype
#define TTYPE_PUT 0x01 // incoming txfer
#define TTYPE_GET 0x02 // outgoing txfer


// structure used to track FTPd processes.
// all FTPd sessions processes are maintained in the Exec List MainPort->MPE->FTPList
struct FTPProcess
   {
   struct Node fp_node;      // obligatory node for Exec list handling.
   struct Process *fp_proc;  // pointer to the process handling the FTP session.
   char fp_procname[20];     // process name - ftp_node.ln_Name points here.
   LONG fp_sockkey;          // socket ID used for Obtain/ReleaseSocket().

   int fp_ctrlsock;          // control socket / private - read only
   int fp_datasock;          // data socket - for file txfer / private - read only

   struct sockaddr_in sin;   // remote host (peer) name
   LONG sinsize;             // size of sin

   char status[80];          // current status of process, for display in GUI
   
	struct ftpserv *ftpdata; // info relating to user currently logged in

	short Account;            // user account or -1 if anon..
	char	CurUserName[40];   // current login name (ahhh.. lots of expansion space..)

	char AcctStr[12];        // ASCII coded account #..
	int LastCmdIdx;          // index into command table
   } __attribute__((packed));


// FTP error strings
#define FTPERR_EINTR "system call interrupted"
#define FTPERR_EIO   "I/O error"
#define FTPERR_EBADF "bad file descriptor"


// FTPd response strings
#define FTPDRESP_LOGINOK   "230 %s\r\n"
#define FTPDRESP_BANNER    "220-%s\r\n"
#define FTPDRESP_BANNEREND "220 %s\r\n"
#define FTPDRESP_BYE       "221 %s\r\n"

#define FTPDRESP_ENTERPW   "331 %s\r\n"

#define FTPDRESP_DUMP      "421 %s\r\n"

#define FTPDRESP_UNKNOWN   "500 %s\r\n"
#define FTPDRESP_USERFIRST "503 %s\r\n"
#define FTPDRESP_BADLOGIN  "530 %s\r\n"


// FTPd error strings
#define FTPDERR_CONNECTION_LOST "connection lost\n"

// standard CNet FTPD files
#define FILE_FTPBANNER "systext:sys.ftpstart"

#endif /* CNET_FTP_H */
