
#ifndef CNET_ROOMS
#define CNET_ROOMS

struct RoomConfig {
	long	Access;		/* which groups may enter */

	short	Creator;	/* who created this room (id#) */
	short	Youngest;
	short	Oldest;

	UBYTE	Doors;
	UBYTE	NameType;
	UBYTE	Gender;
	UBYTE	Quiet;
	UBYTE	Public;
	UBYTE	PermaRoom;

	char	Name[31];
	char	Topic[31];
	char	Entry[81];	/* Messages for entry, exit */
	char	Exit[81];

	UBYTE	Scribe;			/* are we writing a scribe? */
	UBYTE	MCIScribe;		/* remove MCI's ?? */
	char	SName[78];		/* name of current scribe file */

	UBYTE	Channel;
	UBYTE	LinkID;

	UBYTE	Invited[300];		/* 1 bit each for 2400 users */

	short	MaxUsers;
	UBYTE	Chaos;
	UBYTE	LurkTimeout;

	short	NoiseSet;
} __attribute__((packed));

struct Room {
	struct	RoomConfig rc;

	long	Pointer;
	long	Users;			/* how many users are in this room */

	short	pSBuff;			/* pointer into Scribe Buffer */

	UBYTE	Buffer[15000];		/* Actual I/O buffer (Q) */

	UBYTE	SBuff[1024];		/* buffer for Scribe */
} __attribute__((packed));

/* LONG aligned.  Total length == 304 */

struct RoomUser {
	long	MyPointer;		/* 0 */
	long	UID;			/* 4 */
	short	Room;			/* 8 */

	char	Entry[71];		/* 10 */
	char	Exit[71];
	char	Topic[31];
	char	Alias[21];
	char	Noise[71];
	UBYTE	Action;
	UBYTE	DefRoom;
	UBYTE	Quit;
	UBYTE	AllowReal;

	UBYTE	expansion[25];		/* 278 */
} __attribute__((packed));

struct NoiseType {
	struct	NoiseType *next;

	long	lines;
	long	bytes;

	char	**nn;
	char	*args;
} __attribute__((packed));


#endif
