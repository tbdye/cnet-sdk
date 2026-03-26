
#ifndef CNET_OLMS
#define CNET_OLMS

#define	OLM_BROADCAST	0x00000001

struct OLMHeader {
	ULONG	ByID;
	char	ByUser[26];		// actual handle only
	short	ByAccount;
	short	Port;

	UBYTE	broadcast;

	LONG date; // 23-Apr-97: date/time OLM filed

	UBYTE	other[23];
} __attribute__((packed));

#endif
