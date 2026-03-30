#ifndef CNET_NFS_H
#define CNET_NFS_H

#include <cnet/align.h>

#define INDEX_NONE 0
#define INDEX_ALPHA 1
#define INDEX_DATE 2
#define INDEX_DATE_REVERSE 3

#define FILEFLAG_UNISUB (1<<0)
#define FILEFLAG_FORCEALPHA  (1<<1)

CNET_PACK_BEGIN

struct	SubboardType {
	ULONG	key;		/* unique subboard ID */
	ULONG	parent;		/* to whom do I belong? */
	USHORT	number;		/* for QWK access */
	USHORT	priority;	/* for sorting on the list */

	char	origin[80];
	char	name[40], filler[8], path[80], handle[11];
	UBYTE	deleted;

	ULONG	parts;

	USHORT	FreeDays, TimeLock, MaxItems, InactiveDays;

	ULONG	SubOpIDs[6];

	ULONG	Arcs, Transforms;
	char	TransformTo[8];

	ULONG	ExportTo, hiwater;

	short	MRewardBytes;		/* % of byte  size to award */
	short	NRewardFiles;		/* # of file creds to award
					       uploader each time his file is
					       downloaded */
	UBYTE	Direct;			/* Disk-exchange type */
	UBYTE	Vote;			/* May non-sysops add topics? */
	UBYTE	Closed;
	UBYTE	Gender;			/* 0 or 'M', 'F' */

	long	UnionFlags;		/* Must have all of these flags */
	long	Access;			/* Groups which may ENTER */
	long	PostAccess;		/* Groups which may Post  */
	long	RespondAccess;		/* ... Respond */
	long	UploadAccess;		/* ... Upload */
	long	DownloadAccess;		/* ... Download */
	long	ComputerTypes;		/* */
	long	Hours;			/* restricted entry hours */
	long	HourUnionFlags;		/* */
	long	HourAccess;		/* groups which may Enter during Hours*/
	long	Baud;			/* minimum Baud rate to enter */
	long	BaudHours;		/* hours to enforce min baud rate */

	UBYTE	AllowAliases;
	UBYTE	PurgeStatus;		/* */
	UBYTE	Youngest;		/* */
	UBYTE	Oldest;			/* */

	short	MDownBytes;		/* % of download bytes to subtract */
	short	NDownFiles;		/* # of files to subtract per download */
	short	MTimeCredit;		/* % of time to give back for uploads */
	short	SubSort;		/* */
	long	MinFreeBytes;		/* to upload in this subboard */

	UBYTE	RealNames;		/* use real names instead of Handles? */
	UBYTE	Addressees;
	UBYTE	Anonymous;
	UBYTE	override;		/* how to set the use defaults flags in
					   newly uploaded items */

	UBYTE	Verification;		/* */
	UBYTE	DupCheck;
	UBYTE	ShowUnvalidated;
	UBYTE	NoMCI;

	UBYTE	PrivateArea;		/* 0/1 No/Yes ... 2==FORCE */
	BYTE	SDownFiles;		/* Which download charge schedules? */
	BYTE	SDownBytes;
	UBYTE	Test;			/* test at upload time */

	UBYTE	Transform;		/* run script at upload time */
	UBYTE	AutoAdopt;		/* Adopt Orphans at a-maint */
	UBYTE	WeedResponses;		/* Use inactive days against indiv responses */
	UBYTE	NoSignatures;

	short	UpRatioBytes;		/* which set of file/byte ratios to use? */
	short	UpChargeBytes;		/* which set of charges to use?          */
	short	UpRatioFiles;		/* */
	short	UpChargeFiles;		/* */

	UBYTE	Invitation;		/* */
	UBYTE	UserMustJoin;
	UBYTE	DeleteOwn;
	UBYTE	Marker;			/* 0 = Base, 1 = UDBASE, &128 = Killed */

	UBYTE	NetNumber;		/* */ 
	UBYTE	DLnotifyULer;		/* */
	UBYTE	NoReadCharges;		/* opt to short circuit accounting system */
	UBYTE	NoWriteCharges;		/* for reading/writing messages */

	UBYTE	QWKReplies;		/* */
	BYTE	SBaseUse;		/* which BBSCHARGE rate to use? */
	UBYTE	CarbonCopy;		/* */
	UBYTE	CDROM;			/* copy to hard drive before download? */
};

struct AreaManagerUserInfo {
	char	path[80];
	char	Access;
};

struct FileListType {
	char	filename[31];
	char	deleted;

	ULONG	area, item;
};

struct FileIndexNode {
	ULONG	fi[512];
};

struct FileListBase {
	struct	SignalSemaphore sem;
	char	portName[24];
	char	path[80];

	struct	FileIndexNode *fin[512];	/* enough for 256K files */
	ULONG	nf;
	USHORT	numlocks;
	UBYTE	savekeys;
	UBYTE	exclusive;
};

struct FileListHandle {
	char	root[80];
	char	pat[200];

	struct	FileListBase *flb;
	struct	FileListType file;
	struct	AreaManagerUserInfo mui;

	BPTR	fh;
	ULONG	pf, cf;
	long	iswild;

	UBYTE	beenthere[1024];	/* physical visited areas */

	ULONG	flags;
};

struct	ItemType {
	ULONG	key;
	ULONG	text, length;		/* short description */

	ULONG	date;
	char	subject[80];
	char	path[80];

	ULONG	size, downloads, bestcps;
	ULONG	used, DLableFrom, DLableTo, PurgeOn;

	long	VoteLink;		/* vote serial# */

	long	ByteRewards;		/* # of bytes awarded so far */
	long	BytePayBack;
	long	ByteCharges;
	long	ByteDownload;		/* Number of byte cred to download */

	long	FileRewards;		/* # of files awarded so far */
	short	FilePayBack;
	short	FileCharges;
	short	FileDownload;		/* Number of file cred to download */

	UBYTE	Private;
	UBYTE	DLnotifyULer;

	UBYTE	VirusChecked;
	UBYTE	override;
	UBYTE	Frozen;
	UBYTE	Free;

	UBYTE	Favorite;
	UBYTE	Validated;
	UBYTE	Finished;
	UBYTE	Described;

	UBYTE	Transformed;
	UBYTE	PurgeKill;
	UBYTE	MissingFile;
	BYTE	Integrity;		/* 	0 == not tested
						1 == passed
					       -1 == failed
						2 == not testable */

	UBYTE	AutoGrab;		/* ASCII-Grab when read */
	UBYTE	PurgeStatus;		/*      0 == Auto
						1 == @DL
						2 == Query@DL
						3 == @Amaint
						4 == Protected */

	char	deleted;
};

struct	MessageType {
	ULONG	ItemKey;
	ULONG	key;

	ULONG	date;
	char	subject[80],
		from[80],
		organ[32],
		to[80],
		cc[80],
		replyto[80];

	ULONG	toid, byid;
	ULONG	text, length;

	UBYTE	ByNotParanoid;		/* allow showing real name? */
	UBYTE	ByAnonymous;
	UBYTE	ToAnonymous;
	UBYTE	Private;

	UBYTE	Imported;
	UBYTE	ByAlias;		/* is BY an alais for ByUser? */
	UBYTE	ToNotParanoid;		/* allow showing real name? */
	UBYTE	ToAlias;		/* is TO an alias for ToUser? */

	UBYTE	Received;
	UBYTE	deleted;
	UBYTE	isfile;
};

struct	ItemKey {
	ULONG	key;
	ULONG	date;

	USHORT	AlphaIndex;
	USHORT	DateIndex;

	USHORT	start;
	USHORT	messages;

	ULONG	freshest;

	char	deleted;
};

struct	ItemKeyNode {
	struct	ItemKey ik[128];
};

struct	ItemIndexNode {
	USHORT	ii[128];
};

struct	MessageKeyNode {
	USHORT	mk[128];
};

struct	AreaPublic {
	struct	AreaPublic *next, *prev;

	char	path[83];
	char	exclusive;

	ULONG	users;

	struct	ItemKeyNode    *ikn[  8];
	struct	MessageKeyNode *mkn[512];

	USHORT	ni, nm;

	ULONG	tick;
	ULONG	lastkey;
	struct	SignalSemaphore sem;

	char	saveikeys, savemkeys;
};

struct	AreaHandle {
	struct	AreaPublic *ap;

	struct	ItemIndexNode *iin[  8];

	struct	ItemKey *ik;

	USHORT	ni, nm;
	USHORT	ci, cm;
	USHORT	pi, mi, cr;

	struct	ItemType    item;
	struct	MessageType message;

	BPTR	ih, mh, th, sh;

	ULONG	tick;

	char	indextype;
};

struct AreaKey {
	ULONG	key;
	ULONG	parent;
	USHORT	number;
	USHORT	priority;

	USHORT	index;
	USHORT	depth;
	USHORT	children;		/* may not now be used */
	USHORT	descendants;

	ULONG	date;			/* newest on this area */

	char	handle[11];
	char	deleted;
};

struct AreaUserItem {
	ULONG	key;
	ULONG	date;
};

struct AreaManagerArea {
	struct	AreaManagerArea *next, *prev;
	struct	AreaUserItem aui[1000];

	struct	AreaKey *ak;

	USHORT	numlocks;
	char	saveuitems;
};

struct AreaUser {
	ULONG	key;
	ULONG	date;
};

struct AreaUserNode {
	struct	AreaUser au[128];

	USHORT	index[128];
};

struct AreaManagerUser {
	struct	AreaManagerUser *next, *prev;
	USHORT	id;
	USHORT	numlocks;

	struct	AreaManagerArea *ama;
	struct	AreaUserNode *aun[64];

	ULONG	tick;
	struct	AreaManagerUserInfo mui;
	USHORT	ns;
	char	saveukeys;
};

struct AreaKeyNode {
	struct	AreaKey ak[128];
};

struct AreaManagerBase {
	struct	SignalSemaphore sem;
	char	portName[24];
	char	path[80];

	struct	AreaManagerUser *user;
	struct	AreaPublic *ap0;
	struct	AreaKeyNode *akn[64];		/* 8192 subs */

	ULONG	tick;
	USHORT	numlocks;
	USHORT	ns;
	char	exclusive;
	char	saveakeys;
};

// Need a call to 'revert' back to managed lock info?  If Open fails?

struct	AreaManagerLock {
	struct	AreaManagerBase *amb;
	struct	AreaManagerUser *amu;

// These two define the currently OpenedManagedArea ...
	struct	AreaManagerArea *ama;
	struct	AreaHandle *ah;

// These others define the current state of the lock ...
	struct	AreaKey  *ak;
	struct	AreaUser *au;
	struct	SubboardType sub;
	BPTR	sh;
	USHORT	ps;		/* phys pos. */
	USHORT	cs;		/* current logical pos.  0==None */
	USHORT	top;
};

CNET_PACK_END

long LockItem( struct AreaHandle *ah );
long UnLockItem( struct AreaHandle *ah );
long LockMessage( struct AreaHandle *ah );
long UnLockMessage( struct AreaHandle *ah );
long GotoItem( struct AreaHandle *ah, USHORT rec );
long GotoMessage( struct AreaHandle *ah, USHORT rec );
long SeekItem( struct AreaHandle *ah, ULONG key );
void CloseArea( struct AreaHandle *ah );
struct AreaHandle *OpenArea( char *path, char exclusive );
long AppendItem( struct AreaHandle *ah, struct ItemType *item );
long AppendMessage( struct AreaHandle *ah, struct MessageType *message, char *text );
void SecureArea( struct AreaHandle *ah );
void ReplaceBody( struct AreaHandle *ah, char *text );
void ReplaceShort( struct AreaHandle *ah, char *text );
long DeleteItem( struct AreaHandle *ah );
long RecallItem( struct AreaHandle *ah );
long DeleteMessage( struct AreaHandle *ah );
long RecallMessage( struct AreaHandle *ah );
long ReadBody( struct AreaHandle *ah, char *buffer, ULONG start, ULONG len );
long ReadShort( struct AreaHandle *ah, char *buffer, ULONG start, ULONG len );
long SeekNewResponse( struct AreaHandle *ah, ULONG refdate, char oneold );
long SeekResponse( struct AreaHandle *ah, USHORT n );
long SetAreaIndex( struct AreaHandle *ah, char index, char force );
long SeekFirstResponse( struct AreaHandle *ah );
long SeekLastResponse( struct AreaHandle *ah );
long SeekNextResponse( struct AreaHandle *ah );
long SeekPrevResponse( struct AreaHandle *ah );
long PackArea( struct AreaHandle *ah );
struct AreaManagerBase *CreateAreaManager( char *path );
void SecureAreaManager( void );
long DestroyAreaManager( void );
struct AreaManagerLock *OpenAreaManager( short id, struct AreaManagerUserInfo *mui );
struct AreaManagerLock *DupeAreaManager( struct AreaManagerLock *aml );
void CloseAreaManager( struct AreaManagerLock *aml );
long GotoArea( struct AreaManagerLock *aml, USHORT rec );
long SeekFirstArea( struct AreaManagerLock *aml );
long SeekNextArea( struct AreaManagerLock *aml );
long SeekPrevArea( struct AreaManagerLock *aml );
long SeekSibArea( struct AreaManagerLock *aml );
long SeekParentArea( struct AreaManagerLock *aml );
long SeekChildArea( struct AreaManagerLock *aml );
long LockManagedArea( struct AreaManagerLock *aml );
long UnLockManagedArea( struct AreaManagerLock *aml );
struct AreaHandle *OpenManagedArea( struct AreaManagerLock *aml );
void CloseManagedArea( struct AreaManagerLock *aml );

#endif /* CNET_NFS_H */
