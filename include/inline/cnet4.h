#ifndef _INLINE_CNET4_H
#define _INLINE_CNET4_H

#ifndef CLIB_CNET4_PROTOS_H
#define CLIB_CNET4_PROTOS_H
#endif

#ifndef __INLINE_MACROS_H
#include <inline/macros.h>
#endif

#include <exec/types.h>

#ifndef CNET4_BASE_NAME
#define CNET4_BASE_NAME CNet4Base
#endif

/*
 * LP4NRA4 - 4-arg no-return macro (SAS/C a4-relative variant).
 * Not in the NDK's inline/macros.h; alias to the standard LP4NR.
 * The "A4" suffix referred to SAS/C's small data model (a4 base
 * register); GCC does not use a4-relative addressing, so LP4NR
 * is functionally identical.
 */
#ifndef LP4NRA4
#define LP4NRA4 LP4NR
#endif

#define CNetPrintTime(t, out, strings, format) \
	LP4NR(0x1e, CNetPrintTime, ULONG, t, d0, char *, out, a0, char **, strings, a1, struct UserData *, format, a2, \
	, CNET4_BASE_NAME)

#define CNetTime() \
	LP0(0x24, ULONG, CNetTime, \
	, CNET4_BASE_NAME)

#define CNetExplodeTime(t, date) \
	LP2NR(0x2a, CNetExplodeTime, ULONG, t, d0, struct IsDate *, date, a0, \
	, CNET4_BASE_NAME)

#define CNetImplodeTime(date) \
	LP1(0x30, ULONG, CNetImplodeTime, struct IsDate *, date, a0, \
	, CNET4_BASE_NAME)

#define CNetDowDate(date) \
	LP1(0x36, USHORT, CNetDowDate, struct IsDate *, date, a0, \
	, CNET4_BASE_NAME)

#define CNetPrintDate(date, out, strings, format) \
	LP4NRA4(0x3c, CNetPrintDate, struct IsDate *, date, a1, char *, out, a2, char **, strings, a3, char *, format, d7, \
	, CNET4_BASE_NAME)

#define CNetFindRange(buffer, min, max, rc) \
	LP4(0x42, long, CNetFindRange, char *, buffer, a0, long, min, d0, long, max, d1, struct RangeContext *, rc, a1, \
	, CNET4_BASE_NAME)

#define CNetNextRange(rc) \
	LP1(0x48, long, CNetNextRange, struct RangeContext *, rc, a0, \
	, CNET4_BASE_NAME)

#define CNetLongToRange(def, where) \
	LP2NR(0x4e, CNetLongToRange, long, def, d0, char *, where, a0, \
	, CNET4_BASE_NAME)

#define CNetRangeToLong(s) \
	LP1(0x54, long, CNetRangeToLong, char *, s, a0, \
	, CNET4_BASE_NAME)

#define CNetTimeNowDiff(prevtime) \
	LP1(0x5a, LONG, CNetTimeNowDiff, ULONG, prevtime, d0, \
	, CNET4_BASE_NAME)

#define CNetEndOfRange(rc) \
	LP1(0x60, UBYTE, CNetEndOfRange, struct RangeContext *, rc, a0, \
	, CNET4_BASE_NAME)

#define CNetPrevRange(rc) \
	LP1(0x66, long, CNetPrevRange, struct RangeContext *, rc, a0, \
	, CNET4_BASE_NAME)

#define CNetStartOfRange(rc) \
	LP1(0x6c, UBYTE, CNetStartOfRange, struct RangeContext *, rc, a0, \
	, CNET4_BASE_NAME)

#define CNetUpTime(out, timediff) \
	LP2(0x72, char  *, CNetUpTime, char *, out, a1, LONG, timediff, d1, \
	, CNET4_BASE_NAME)

#endif /*  _INLINE_CNET4_H  */
