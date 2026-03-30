
#ifndef CNET_FILES
#define CNET_FILES

#include <cnet/align.h>

CNET_PACK_BEGIN

struct CNetFileEntry
	{
	struct CNetFileEntry *DirHeader; // pointer to entry containing the root dir info
												// also contains FULL PATH to the directory
	char filename[256];
	LONG size;
	char filenote[80];
	struct IsDate ChangedDate;			// date file last changed
	UBYTE ftype;							// file type - see CNFE_TYPE_* below

	LONG flag;								// general purpose user flag.
	                                 // used in CNet applications as "deleted"
	                                 // marker..  You may use it in your code
	                                 // for whatever purpose desired. CNet does
	                                 // not use it internally.

	APTR userdata;							// can be cast to any type.. use for whatever you wish
	struct CNetFileEntry *parent;		// parent directory - NULL if none
	struct CNetFileEntry *child;		// subdirectory - NULL if none
	struct CNetFileEntry *prevfile;	// previous file - NULL if none
	struct CNetFileEntry *nextfile;	// next file - NULL if none
	};

CNET_PACK_END

#define CNFE_TYPE_FILE 0 // regular file
#define CNFE_TYPE_DIR  1 // subdirectory

#define CNFE_FLAG_DELETED 1L // For CNet apps: (flag==CNFE_FLAG_DELETED) means file deleted.

#endif
