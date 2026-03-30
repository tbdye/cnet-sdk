
#ifndef CNET_DOORS
#define CNET_DOORS

#include <cnet/align.h>

#define CN_USERINPUT 0xff      // Message/code sent to doors when user keyboard input
                               // is waiting. see InputSignalInit() in empty.c 


// some commonly used door paths
#define BMAINT_DOOR "cnet:doors/bmaint"

#define NUM_CNETC_ARGS 2
#define CNETC_MAINARG  1

CNET_PACK_BEGIN

// setup when a C pfile is ran
struct CPort
   {
   struct MsgPort  mport;      // 0
   short           align;
   struct MainPort *myp;       // 36 pointer to MainPort
   struct PortData *zp;        // 40 pointer to my PortData

   UBYTE           ack;        // 44 tell CNet we've arrived
   UBYTE           align2[3];
};


// passed between C pfile and C-Net
struct CMessage
   {
   struct Message cn_Message; // 0 communication stuff
   ULONG          arg1;       // 20
   ULONG          arg2;
   ULONG          arg3;
   ULONG          arg4;
   ULONG          result;
   UBYTE          command;    // 40 which command? see CNet Programmer's Reference Manual
                              //    and CMESS_CMD_xxxxx defines below. cnetc.library
                              //    usually takes care of filling this in for us..
   UBYTE          align[3];
};


// defines for door CMessage commands added 11-Oct-97
#define CMESS_CMD_SHUTDOWN          0
#define CMESS_CMD_PUTTEXT           1
#define CMESS_CMD_ENTERLINE         2
#define CMESS_CMD_ONEKEY            3
#define CMESS_CMD_ENTERPASS         4
#define CMESS_CMD_CCOMMANDS         5
#define CMESS_CMD_READFILE          6
#define CMESS_CMD_SETDOING          7
#define CMESS_CMD_CALLEDITOR        8
#define CMESS_CMD_READGRAPHICS      9
#define CMESS_CMD_MAKEDATE          10
#define CMESS_CMD_READACCOUNT       11
#define CMESS_CMD_SAVEACCOUNT       12
#define CMESS_CMD_ADDCHARGE         13
#define CMESS_CMD_CHECKBALANCE      14
#define CMESS_CMD_ENTERTEXT         15
#define CMESS_CMD_CONFWAIT          16
#define CMESS_CMD_CHECKCHANGES      17
#define CMESS_CMD_CONVERTACCESS     18
#define CMESS_CMD_GETFREE           19
#define CMESS_CMD_FINDACCOUNT       20
#define CMESS_CMD_CHECKFLOW         21
#define CMESS_CMD_LISTDIR           22
#define CMESS_CMD_RNEXT             24
#define CMESS_CMD_PARSECMDLINE      25
#define CMESS_CMD_FINDCOMMAND       26
#define CMESS_CMD_READMSGPOINT      27
#define CMESS_CMD_EDITMESSAGE       28
#define CMESS_CMD_PREPEDITOR        29
#define CMESS_CMD_FILESIZE          30
#define CMESS_CMD_WAITFORINPUT      31
#define CMESS_CMD_VERIFYOFF         32
#define CMESS_CMD_PUTITEM           34
#define CMESS_CMD_GETITEM           35
#define CMESS_CMD_FINDARC           38
#define CMESS_CMD_SELANDDLOAD       39
#define CMESS_CMD_VDE               40
#define CMESS_CMD_WRITELOG          41
#define CMESS_CMD_SETMINFREE        42
#define CMESS_CMD_SETPROTO          43
#define CMESS_CMD_DLOAD             44
#define CMESS_CMD_ULOAD             45
#define CMESS_CMD_VDECLOSE          56
#define CMESS_CMD_VDEAPPLY          57
#define CMESS_CMD_WRITEUKEYS        58
#define CMESS_CMD_DOANSI            59
#define CMESS_CMD_DOANSIOUT         60
#define CMESS_CMD_INPUTSIGINIT      61
#define CMESS_CMD_INPUTSIGDISP      62
#define CMESS_CMD_CHECKOLMS         63
#define CMESS_CMD_PUTQMULT          64
#define CMESS_CMD_CHECKCARRIER      65
#define CMESS_CMD_MAILSEND          66
#define CMESS_CMD_READLOG           67
#define CMESS_CMD_OPENDISPLAY       68
#define CMESS_CMD_GSETFILENOTE      69
#define CMESS_CMD_EXTRACTFILE       70
#define CMESS_CMD_DLOADSELECTED     71
#define CMESS_CMD_FINDFOREIGNAREA   72  // added 03-Sep-98
#define CMESS_CMD_OUTSCAN           73  // added 04-Sep-98


// Added 07-Oct-97 - thanks to Kelly Cochran who did the initial conversion
//                   of empty.c code to a shared lib, leaving me free to work
//                   on other CNet features/bugs.
struct CNetCContext
{
   struct MainPort *myp;        // It's MainPort, duh
   struct PortData *z;          // and PortData too!
   struct CPort    *cport;      // CNet-initialized communication port
   struct MsgPort  *replyp;     // Library-allocated reply port
   struct CMessage cmess;       // CMessage struct for communication
};

CNET_PACK_END

#endif

