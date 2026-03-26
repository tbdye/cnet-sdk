#ifndef CNET_OTHER_H
#define CNET_OTHER_H

#define TOP_STAT 51
#define TOP_NOSTAT 11

#define C_BLACK  0
#define C_RED    4
#define C_GREEN  2
#define C_YELLOW 6
#define C_BLUE   1
#define C_PURPLE 5
#define C_CYAN   3
#define C_WHITE  7

#define GRN_BLK 0x20
#define YEL_BLK 0x60
#define BLU_BLU 0x11
#define CYN_BLK 0x30
#define CYN_BLU 0x31
#define WHT_BLK 0x70
#define WHT_RED 0x74
#define WHT_BLU 0x71
#define WHT_PUR 0x75

#define EVER ;;

struct MarkType
	{
	short	sub;				// The new system can uniquely 
	ULONG	ItemNumber;		// identify a message with this
	long	Seek;				// information.  Used for Z/ZG.
	ULONG	Number;
	} __attribute__((packed));

struct TransType
	{
	struct	TransType *next;	// linked list

	long	lines;
	long	bytes;

	UBYTE	**mm;						// pointers to BBSMENU lines
	UBYTE	**bm;						// pointers to BBSTEXT lines

	short	menustart[50];			// RayNote: THIS is the reason for the 50 menu limit!!!
	short	menulen  [50];			//          MUST be broken for CNet/5!!
	} __attribute__((packed));

#endif /* CNET_OTHER_H */
