// introduced as of CNet v4.21 - 06-Aug-96 to perform background
// test and transform of file items.

#ifndef CNET_NEWSMANAGER
	#define CNET_NEWSMANAGER

#include <cnet/align.h>

#define CNETNEWSBOX "cnet:NEWSMAN"

// defines for ProcessType of struct NRQFile
#define NRQ_POST 0x01

CNET_PACK_BEGIN

struct NRQFile
	{
	LONG id;					// serial id of the user who posted or replied
                     	// account number not used to avoid using the wrong user name
								// if set to 0, the "From: " line must be filled out in
								// the accompanying data file.

	char UniqueBase[75];	// physical location of post/response -- may be set to NULL (eg. UniqueBase[0]=0) if PhysNum is set to the physical subboard number
								// may also be set to a NULL string if the "Newsgroups:" field of the message is already filled in
								// this is used ONLY to get the Newsgroup name from CNet subboards

	short PhysNum;			// (2-Feb-1997) -  Physical subboard NUMBER of the base where the file exists.
								// may be set to -1 if the "Newsgroups:" field of the message is already filled in
								// this is used ONLY to get the Newsgroup name from CNet subboards

	char FileName[72];	// name of temp file that is in CNETNEWSBOX - filename only, no path.
								// the temp file is the actual message header and body text in
								// RFC-822 format

	UBYTE Flags;			// See NRQ_* bits above
	};

CNET_PACK_END

// to have cnet's newsmanager perform a post, file a single FILLED in
// NRQFile structure to a file (with any UNIQUE filename with a .NRQ
// extension) to CNETNEWSBOX (defined above).
//
// The data file MUST be written first. Make sure the DATA filename without
// path is placed in the NRQ structure's FileName field. The data file can be
// any UNIQUE filename as long as it does not have a .nrq extension
//
// The data file MUST be a properly structured news item following RFC822
// guidelines. An example follows:
//
// Most of the header is not required but the SUBJECT should exist in the header!
// Also, the blank line (0x0A) between header and body is MANDATORY.
//
//
//--- SAMPLE RFC 822 header - Notice only part of the RFC-822 header is completed.  news-task will fill in the rest.
// Newsgroups: comp.sys.amiga.introduction
// Subject: A test message
// Followup-To: comp.sys.amiga.introduction
// Expires: Thursday, 25-Dec-97 00:00:00 EST
// 
// This is the body text of this message, written after a blank line which
// separates body from header
//--- END OF SAMPLE
// 
//
// When the NRQ file is closed, news-task will be notified that a new news
// item is to be posted and news-task will then process and post the
// news item to the news server.

#endif
