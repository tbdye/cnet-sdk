
#ifndef CNET_FIDO
#define CNET_FIDO

#include <cnet/align.h>

#define PRIVATE		0x0001
#define CRASH		0x0002
#define HOLD		0x0200
#define FILEREQUEST	0x0800
#define FILESEND	0x0010

CNET_PACK_BEGIN

struct FidoNet {
	char from[36];
	char to  [36];
	char subject [72];
	char dateline[20];
	USHORT time;
	USHORT destnode;
	USHORT orignode;
	USHORT cost;
	USHORT orignet;
	USHORT destnet;
	USHORT destzone;
	USHORT orignzone;
	USHORT destpoint;
	USHORT orignpoint;
	USHORT replyto;
	USHORT attribute;
	USHORT nextrply;
};

struct PacketHeader {
	USHORT	orignode;
	USHORT	destnode;
	USHORT	year;
	USHORT	month;
	USHORT	day;
	USHORT	hour;
	USHORT	minute;
	USHORT	second;
	USHORT	baud;
	USHORT	two_oh;
	USHORT	orignet;
	USHORT	destnet;
	char	fill[34];
};

struct PackedMessage {
	USHORT	two_oh;
	USHORT	orignode;
	USHORT	destnode;
	USHORT	orignet;
	USHORT	destnet;
	USHORT	attribute;
	USHORT	cost;
	char	date[20];
};

struct NodeType {			// a node that we export-to
	char	*buffer;
	ULONG	buffpos;
	ULONG	buffsize;

	USHORT	zone;
	USHORT	net;
	USHORT	node;
	USHORT	point;

	USHORT	packer;
	USHORT	format;		// 4d or 2d
	USHORT	type;			// normal, crash, hold

	USHORT	flag;			// 0-31 ... which subboards?
	USHORT	domain;		// which fido-net domain?

	UBYTE	Access;
	UBYTE	Flags;

	USHORT	more[4];

	ULONG	incount;
	ULONG	outcount;

	char	Password[12];
};

struct RouteType
	{
	USHORT	zone;		// a netmail routing pattern from (-1's mean wildcard)
	USHORT	net;		// zone, net and node to route (again, -1 in any one means wildcard for that address node)
	USHORT	node;
	USHORT	point;

	USHORT	zone2;	// route to zone:net/node.point
	USHORT	net2;
	USHORT	node2;
	USHORT	point2;

	USHORT	domain;		// which fido-net domain (eg. which net)?
	USHORT	type;			// normal, crash, hold

	USHORT	more[4];

	ULONG	outcount;
	};


struct PacketType5
	{
	struct	PacketType *next;

	char	filename[32];

	ULONG date;
	BYTE expansion[2];
	};

struct PacketType
	{
	struct	PacketType *next;

	char	filename[32];
	struct	IsDate date;
	};



struct ImportType5
	{
	struct	ImportType *next;

	USHORT	zone0, net0, node0, point0;
	ULONG	data;

	ULONG	msgstart;
	long	seek;
	long	end;
	long	seen;				// index
	long	xoff;				// index

	ULONG date;
	BYTE expansion[2];

	UBYTE	isdupe, isfull;
	};

struct ImportType
	{
	struct	ImportType *next;

	USHORT	zone0, net0, node0, point0;
	ULONG	data;

	ULONG	msgstart;
	long	seek;
	long	end;
	long	seen;				// index
	long	xoff;				// index

	struct	IsDate date;

	UBYTE	isdupe, isfull;
	};

struct	DupeType
	{
	USHORT	zone;
	USHORT	net;
	USHORT	node;
	USHORT	point;
	ULONG	data;
	};

struct	EchoType
	{
	char	Title[24];
	ULONG	ExportTo[4];				// export to which node types

	struct	ImportType *first;	// during toss, list to import
	struct	ImportType *last;

	USHORT	listn;					// during toss, which local sub?
	USHORT	domain;

	struct	DupeType *duptab;
	ULONG	incount;
	ULONG	outcount;

	USHORT	dupsize;

	UBYTE	dupdelta;
	UBYTE	Access;
	UBYTE	Flags;

	UBYTE	other[3];

	char	Info[48];
	};

struct	EchoType4
	{
	char	Title[80];					// same as unique dirname
	ULONG	ExportTo[4];				// export to which node types

	struct	ImportType *first;	// during toss, list to import
	struct	ImportType *last;

	USHORT	listn;					// during toss, which local sub?
	USHORT	domain;

	struct	DupeType *duptab;
	ULONG	incount;
	ULONG	outcount;

	USHORT	dupsize;

	UBYTE	dupdelta;
	UBYTE	Access;
	UBYTE	Flags;

	UBYTE	other[3];

	char	Info[48];
	};

CNET_PACK_END


#endif
