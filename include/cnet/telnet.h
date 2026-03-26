/* telnet.h - structures/variables used by CNet's telnet/telnetd clients */

#ifndef CNET_TELNET_H
#define CNET_TELNET_H

// netincludes containing typedefs & "struct sockaddr_in"
#include <sys/types.h>
#include <netinet/in.h>

// each host entry in the sysdata:bbs.tnhost file is parsed and entered into
// list of TNHostEntry structures.
struct TNHostEntry
	{
	char he_system[40];			// 0   - system name (VDE)
	char he_ip[40];				// 40  - pre-defined hostname or IP (VDE)
	LONG he_port;					// 80  - telnet port number (VDE)
	char he_data[40];				// 84  - additional/optional text (VDE)
	LONG he_Access;				// 124 - access groups allowed to see/use host (VDE)
	LONG he_Flags;					// 128 - user must have these Host flags (VDE)
	struct sockaddr_in he_sin;	// ??? - copy of the socket/address info after successful connect. (dynamic remote query)
	LONG he_status;				// ??? - status of host (ie. &128 = killed)
	BYTE he_expansion[100];			// future expansion
	struct TNHostEntry *next;	// 132 - pointer to next TNHostEntry (dynamic)
	} __attribute__((packed));


// The new GLOBAL host addressbook entry.
// A list of HostEntry structures will be attached to the EXEC list
// z->INetAddrBook
struct HostEntry5
	{
	char he_system[40];			// 0   - system name/description (VDE)
	char he_ip[40];				// 40  - pre-defined hostname or IP (VDE)
	LONG he_port;					// 80  - port number to connect to (VDE)
	LONG he_Access;				// 124 - access groups allowed to see/use host (VDE)
	LONG he_Flags;					// 128 - user must have these Host flags (VDE)
	struct sockaddr_in he_sin;	// ??? - copy of the peer socket/address info after successful connect.
	LONG he_status;				// ??? - status of host (ie. &128 = killed)

	UBYTE ContextData[256];		//     - context sensitive data. Context relations are:
                              //
                              //       FTP: the default remote DIR to start in (max size = 128)
                              //       TELNET: used for what was formerly he_data (max size = 40 bytes)

                              //       IRC: nick,aj_channel,realname,quitmsg
										//            (20)     (40)     (31)     (80)

	UBYTE addr_type;				// Service/client type this address applies to
                              // 0 = ANY
										// 1 = FTP
										// 2 = TELNET
										// 3 = IRC
										// 4 = Gopher/Lynx

	BYTE he_expansion[100];		// future expansion
	struct TNHostEntry *next;	// 132 - pointer to next TNHostEntry (dynamic)
	} __attribute__((packed));



// Socket port structure - will probably be moved to ports.h later..
//
// currently only used for telnetd port answering
//
// future uses: TCP/IP joinlinking (direct binary packet comm - no telnet states/options)
//              sysop telnet terminal (enhancement to built-in serial terminal)
//
// structure will probably grow slightly in size during the first few alpha/beta releases
// as future options and expansion space are added.. this growing should not affect
// anything except that a FULL install of all CNet executables will accompany changes.
//
// a pointer back to PortData will be added later so that status lookups(and stack
// exit notifications) can glean BBS port info and quit telnet ports if the
// stack is shutting down..
//
// there is no socket descriptor in this structure..  see dsock in struct portData
// for the socket used for internal telnetd comms.
//
struct TelnetdPort
   {
	struct Node tnd_node;		// Exec node for this entry - currently not used in any Exec lists

	struct sockaddr_in sin; 	// remote host (peer) name
	LONG sinsize;					// size of sin

	char portname[13];         // name of port.  tnd_node.ln_Name points here

	short portnum;					// CNet port number corresponding to this connection

	ULONG local_state;         // local telnet state options (aka WILLs and WONTs) if applicable

                              // states description:
										// - bits represent Telnet state options
                              // - bit set = WILL
                              // - bit clear = won't
                              //
                              // bitdefs (also see TSM_* defines below):
                              //
                              //          0 = linemode  (bit clear = char mode)
                              //          1 = echo
                              //          2 = binary
                              //          3 = environ
                              //          4 = lflow

	ULONG remote_state;		// remote telnet state options if applicable
	int fsm_state;				// current local telnet state options if applicable

	BYTE sigbit;				// signal bit for asynchronous socket event notification
   } __attribute__((packed));


// *_ts bits
#define TOPT_LINE  0
#define TOPT_ECHO  1
#define TOPT_BIN   2
#define TOPT_ENV   3
#define TOPT_LFLOW 4

// telnet receiver states
#define	TS_DATA		0
#define	TS_IAC		1
#define	TS_WILL		2
#define	TS_WONT		3
#define	TS_DO		   4
#define	TS_DONT		5
#define	TS_CR		   6
#define	TS_SB		   7		/* sub-options */
#define	TS_SE		   8		/* searching sub-option end */

#endif /* CNET_TELNET_H */
