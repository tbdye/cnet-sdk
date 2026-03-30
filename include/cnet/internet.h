#ifndef CNET_INTERNET_H
#define CNET_INTERNET_H

#include <cnet/align.h>

// ---------------- internet structures used by CNet/4+ Pro-------------------
// **     FIRST APPEARED in CNet V4.2bR2 (post final-beta version)



CNET_PACK_BEGIN

// config structure saved as a **header** in mail:users/xxx/prefs/cnirc
// following this structure - on disk - are server, info and auto-join channel
// instances each on a separate line.
struct IrcUserConfig
	{
	UBYTE ANSI; // 0=ANSI scrolling, 1=ANSI locking, 2=simple ANSI
	ULONG flags; // currently unused - expansion
	struct List *serverlist;	// Exec list of servers to autojoin - currently internal use only
                              // may be read directly from user cnirc config
                              // file.
	};



// server structure used internally by CNetIRC
struct ServerNode
	{
	struct Node snode;	// IP/hostname in snode.ln_Name
	char sinfo[36];		// textual description of server
	char channel[40];		// autojoin channel
	};


// NEW 4.26c+ server structure used internally by CNetIRC
struct ServerNode1
	{
	struct Node snode;	// IP/hostname in snode.ln_Name
	char sinfo[36];		// textual description of server
	char channel[40];	// autojoin channel
	char RealName[31];	// max length = 30
	char NickName[21];	// max length = 10
	char QuitMsg[80];	// msg to print when quitting - maxlength = 79
	};


// identd structure saved in linked list myp->MPE->idd
// always ObtainSemaphore(myp->MPE->sem[1]) before accessing myp->MPE->idd
// as IdentData may be removed at any time when a client is done with it's
// TCP socket(s).
//
// Also see cnetfuncs.h for CNet 3rd party-friendly identd
// insert/remove/lookup functions present in cnet.library.
//
// an example of identd insertion/removal/lookup is present as
// sdk/examples/identdtest.c
//
struct IdentdData
	{
	unsigned int socket;	// local port owned by user
	short port;	   // local user's CNet port number
	struct IdentdData *next;
	};


// structure currently used by Telnetd's IP blocking functions
// SMTPd will use this structure in the future and will
// then obsolete the SpamStruct structure in mail.h..
struct InternetBlock
{
	struct Node ib_node;   // node for Exec list manipulation
	                       // ib_node.ln_Name may point to either HostName or UserName
	                       // depending on the context..

	char HostName[128];    // actually, IP (future resolver will allow hostnames also)
	                       // patterns will also be added in future enhancements.

   char UserName[128];  // Username and hostname may contain AmigaDOS patterns
                        //
                        // Hostname may be IP address with ranges of IP bits or may be host/domain names.
                        // eg. ipbits1.ipbits2.ipbits3.ipbits4
                        //     aol.com
                        //     192.0-50.#?.#?
};

CNET_PACK_END

#endif /* CNET_INTERNET_H */
