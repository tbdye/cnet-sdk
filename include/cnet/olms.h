
#ifndef CNET_OLMS
#define CNET_OLMS

#include <cnet/align.h>

#define	OLM_BROADCAST	0x00000001

CNET_PACK_BEGIN

struct OLMHeader {
	ULONG	ByID;
	char	ByUser[26];		// actual handle only
	short	ByAccount;
	short	Port;

	UBYTE	broadcast;

	LONG date; // 23-Apr-97: date/time OLM filed

	UBYTE	other[23];
};

CNET_PACK_END

#endif
