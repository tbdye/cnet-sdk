#ifndef CNET_EVENTDEFS_H
#define CNET_EVENTDEFS_H

#include <cnet/align.h>

// event types
#define EVENT_RUNCNETC	0
#define EVENT_RUNAREXX	1
#define EVENT_RUNDOS		2
#define EVENT_READFILE	3
#define EVENT_DOSCMD		4
#define EVENT_CLOSEPORT	5
#define EVENT_CHARGES	6
#define EVENT_LOGONBPS	7
#define EVENT_DLOADBPS	8
#define EVENT_ULOADBPS	9
#define EVENT_LOGONACC	10
#define EVENT_XFERSACC	11
#define EVENT_PFILEACC	12
#define EVENT_MODEMNUM	13
#define EVENT_CALLBACK	14
#define EVENT_AVALIDNUM	15
#define EVENT_PFILES		16
#define EVENT_UDBASE		17
#define EVENT_BASE		18
#define EVENT_NEWUSERS	19
#define EVENT_SYSOPIN	20
#define EVENT_JOINLINK	21
#define EVENT_ONLINE		22

// event status
#define EVENTSTATUS_READY     0x0000
#define EVENTSTATUS_ACTIVE    0x0001
#define EVENTSTATUS_HOLD      0x0002
#define EVENTSTATUS_CANCELLED 0x0003

// when/how to invoke event
#define INVOKETYPE_IMMEDIATE	0x0001
#define INVOKETYPE_FORCEIDLE	0x0002
#define INVOKETYPE_OFFLINE		0x0004
#define INVOKETYPE_ONLINE		0x0008

/*
 * JobType4 - Event/job configuration record.
 * Stored in cnet:configs/events.cfg and pointed to by MainPort->Events.
 * Field sizes inferred from original SDK examples and the events.cfg
 * binary format; the original struct definition was not shipped in
 * public headers.
 */
CNET_PACK_BEGIN

struct JobType4 {
   char   Name[40];         // event name
   char   args[80];         // command string
   char   ports[40];        // port range string
   LONG   invoke;           // invocation flags (bitmask, use BitPosition())
   UBYTE  type;             // event type (EVENT_* defines)
   UBYTE  status;           // event status (0=Ready, 1=Suspended, 2=Once/Delete)
   ULONG  StartTime;        // start time (seconds since Amiga epoch)
   ULONG  DateExecuted;     // last execution time
   LONG   valid;            // validity period in seconds
   UBYTE  Days;             // day-of-week bitmask (bit 0=Sun..bit 6=Sat)
   UBYTE  deleted;          // non-zero if deleted
   LONG   repeat;           // repeat interval in seconds
   UBYTE  runport;          // port action (0=Ignore, 1=Run, 2=Run/Close)
};

CNET_PACK_END

#endif /* CNET_EVENTDEFS_H */
