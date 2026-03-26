/**************************************************************************
 *
 **************************************************************************/

#ifndef NNTP_RC_CODES
	#define NNTP_RC_CODES
	#define NNTP_NO_SOCKET        -1
#endif // SMTP_RC_CODES


#include <ctype.h>
#ifndef __GNUC__
#include <dos.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include <sys/types.h>
#ifndef __GNUC__
#include <sys/dir.h>
#endif
#include <exec/memory.h>
#include <exec/types.h>
#include <exec/ports.h>
#include <clib/alib_protos.h>
#include <clib/console_protos.h>
#include <clib/dos_protos.h>
#include <clib/exec_protos.h>
#include <clib/utility_protos.h>
#define AMITCP_NEW_NAMES
#ifndef __GNUC__
#include <tcp/sys/types.h>
#include <TCP/errno.h>
#include <tcp/netdb.h>
#else
#include <sys/types.h>
#include <errno.h>
#include <netdb.h>
#endif

#define CMDBUF_SIZE 512
#define DATBUF_SIZE 1024

/**************************************************************************
 * structure used to keep track of NNTP Dupetables during an
 * NNTPGet session.
 **************************************************************************/
struct NNTPDupeTab
   {
   char *ArticleID;           // pointer to malloc'd article ID - allows
                              // variable length article ID's. Current max
                              // article ID length is 512 bytes.

   struct NNTPDupeTab *next;  // pointer to next instance of struct NNTPDupeTab
   } __attribute__((packed));


/**************************************************************************
 * See also:
 * SubboardType4 in subs.h for
 *
 *        NNTPMaxDupes
 *        NNTPDupes
 **************************************************************************/
