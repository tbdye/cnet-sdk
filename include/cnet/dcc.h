#ifndef CNET_DCC_H
#define CNET_DCC_H

// irc-ii likes this size so we might as well use it
#define BIG_BUFFER_SIZE 1024

// Maximum path+filename size (CNet internal constant)
#ifndef FMSIZE
#define FMSIZE 256
#endif

#include <cnet/align.h>

/*
 * DCCStruct - DCC request node stored in MainPortExtension->DCCList.
 * Protected by MPE->dccsem.  Field sizes inferred from original SDK
 * examples; the original struct definition was not shipped in public
 * headers.
 */
CNET_PACK_BEGIN

struct DCCStruct {
   struct Node node;            // Exec list linkage
   char   nickname[32];         // IRC nickname of sender
   char   senderaddr[20];       // sender address (numeric string)
   char   Type[8];              // DCC type (SEND, CHAT, etc.)
   char   arg[FMSIZE];          // filename or argument
   LONG   size;                 // file size in bytes (0 if not applicable)
   UBYTE  status;               // DCC status index (see StatTypes[])
   char   ID[20];               // DCC identifier
   LONG   UserID;               // CNet user ID
};

// information about DCC file in progress
struct DCCFile
	{
	char pathfile[FMSIZE];	// full path/filename of sent/received file
	BPTR fh;						// file handle of open file or 0 if none open
	ULONG txfer_bytes;		// bytes transferred so far
	ULONG start_time;			// local start time - reference for calculating CPS
	ULONG ETA;					// time to completion
	USHORT last_CPS;			// last calculated CPS
	};

CNET_PACK_END

#endif
