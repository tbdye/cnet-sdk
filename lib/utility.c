/*
 * utility.c - CNet door utility functions (Tier 1 reimplementation)
 *
 * These functions were originally in cnet.lib (SAS/C linker library).
 * They are standalone utility functions that do not use the CNC_*
 * shared library calls, so there is no __asm conflict to worry about.
 *
 * Extern globals (from door_init.c):
 *   z   - pointer to current port's PortData
 *   myp - pointer to CNet MainPort
 *
 * Compile: m68k-amigaos-gcc -noixemul -m68020 -Iinclude ...
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
 * AmigaOS system headers.  These must be included before any CNet
 * SDK headers because the CNet struct definitions embed AmigaOS
 * types by value (TmpRas, BitMap, SignalSemaphore, etc.).
 */
#include <exec/types.h>
#include <exec/memory.h>
#include <exec/lists.h>
#include <exec/nodes.h>
#include <exec/ports.h>
#include <exec/semaphores.h>
#include <exec/io.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include <proto/dos.h>
#include <graphics/gfx.h>
#include <graphics/rastport.h>
#include <graphics/text.h>
#include <intuition/intuition.h>
#include <libraries/iffparse.h>

/*
 * GCC compatibility macros for SAS/C-specific qualifiers used in
 * some CNet SDK headers (function pointer declarations).
 *
 * NOTE: Do NOT redefine __asm here.  The AmigaOS inline stubs
 * (from proto/exec.h, proto/dos.h) use __asm as a GCC keyword
 * for register variable binding.  Redefining it to nothing breaks
 * Open(), Close(), DateStamp(), Lock(), etc.  The individual CNet
 * struct headers (dates.h, users.h, ports.h, etc.) do not use
 * __asm -- only the master cnet.h does, and we don't include that.
 */
#ifdef __GNUC__
#ifndef __saveds
#define __saveds __attribute__((saveds))
#endif
#ifndef __regargs
#define __regargs
#endif
#ifndef __stdargs
#define __stdargs
#endif
#endif

/* CNet SDK headers -- include subs.h before control.h because ports.h
 * (included by control.h) uses types defined in subs.h (SORDER_LIMIT,
 * BaseUser, ItemType3, HeaderType, etc.) */
#include <cnet/dates.h>
#include <cnet/users.h>
#include <cnet/sysfiles.h>
#include <cnet/doors.h>
#include <cnet/subs.h>
#include <cnet/control.h>
#include <cnet/align.h>

/* Globals from door_init.c */
extern struct PortData *z;
extern struct MainPort *myp;

/* ================================================================== */
/*  String utilities                                                    */
/* ================================================================== */

/*
 * compstra - case-insensitive string comparison.
 *
 * Returns 0 if equal, <0 if s < t, >0 if s > t (like stricmp).
 */
short compstra(char *s, char *t)
{
    if (!s || !t)
        return s ? 1 : (t ? -1 : 0);

    while (*s && *t) {
        int c1 = toupper((unsigned char)*s);
        int c2 = toupper((unsigned char)*t);
        if (c1 != c2)
            return (short)(c1 - c2);
        s++;
        t++;
    }
    return (short)(toupper((unsigned char)*s) - toupper((unsigned char)*t));
}

/*
 * SToUpper - convert string to uppercase in-place.
 */
void SToUpper(char *s)
{
    if (!s)
        return;
    while (*s) {
        *s = toupper((unsigned char)*s);
        s++;
    }
}

/*
 * SToLower - convert string to lowercase in-place.
 */
void SToLower(char *s)
{
    if (!s)
        return;
    while (*s) {
        *s = tolower((unsigned char)*s);
        s++;
    }
}

/*
 * stristr - case-insensitive substring search.
 *
 * Returns pointer to first occurrence of needle in haystack,
 * or NULL if not found.
 */
char *stristr(char *haystack, char *needle)
{
    size_t nlen;

    if (!haystack || !needle)
        return NULL;

    nlen = strlen(needle);
    if (nlen == 0)
        return haystack;

    while (*haystack) {
        if (toupper((unsigned char)*haystack) == toupper((unsigned char)*needle)) {
            size_t i;
            for (i = 1; i < nlen; i++) {
                if (toupper((unsigned char)haystack[i]) !=
                    toupper((unsigned char)needle[i]))
                    break;
            }
            if (i == nlen)
                return haystack;
        }
        haystack++;
    }
    return NULL;
}

/*
 * RemoveLeadingSpace - remove leading whitespace from string in-place.
 *
 * Returns pointer to (modified) string.
 */
UBYTE *RemoveLeadingSpace(UBYTE *s)
{
    UBYTE *src;

    if (!s)
        return NULL;

    src = s;
    while (*src && (*src == ' ' || *src == '\t'))
        src++;

    if (src != s)
        memmove(s, src, strlen((char *)src) + 1);

    return s;
}

/*
 * RemoveNonAlNum - remove all non-alphanumeric characters in-place.
 */
void RemoveNonAlNum(char *a)
{
    char *src, *dst;

    if (!a)
        return;

    src = dst = a;
    while (*src) {
        if (isalnum((unsigned char)*src))
            *dst++ = *src;
        src++;
    }
    *dst = '\0';
}

/*
 * checkspaces - trim trailing spaces from a string.
 *
 * The original prototype is: BOOL checkspaces(UBYTE *str, LONG checklen)
 * It trims trailing spaces and returns TRUE if the string is non-empty
 * after trimming.
 */
BOOL checkspaces(UBYTE *str, LONG checklen)
{
    LONG len;

    if (!str)
        return FALSE;

    len = strlen((char *)str);
    if (checklen > 0 && checklen < len)
        len = checklen;

    while (len > 0 && str[len - 1] == ' ')
        len--;

    str[len] = '\0';

    return (BOOL)(len > 0);
}

/* ================================================================== */
/*  Port queries                                                        */
/* ================================================================== */

/*
 * IsPortLoaded - check if a BBS port has an active process.
 *
 * A port is considered loaded if its PortZ entry is non-NULL and
 * does not point to z0 (the Control default/idle PortData).
 */
BOOL IsPortLoaded(short portnum)
{
    if (!myp)
        return FALSE;
    if (portnum < 0 || portnum > HIPORT_MAX)
        return FALSE;
    if (!myp->PortZ[portnum])
        return FALSE;
    if (myp->PortZ[portnum] == myp->z0)
        return FALSE;

    return TRUE;
}

/*
 * IsPortOccupied - check if a port has a connected user.
 *
 * A port is occupied if it is loaded and the user is online
 * (PortData->OnLine is set).
 */
BOOL IsPortOccupied(short portnum)
{
    if (!IsPortLoaded(portnum))
        return FALSE;

    return (BOOL)(myp->PortZ[portnum]->OnLine != 0);
}

/*
 * IsOnLine - check if a given account number is currently online.
 *
 * Scans all ports looking for a logged-in user with the given account.
 * Returns pointer to the PortData of the online user, or NULL.
 *
 * The original prototype is: struct PortData *IsOnLine(short acc)
 * The cnet.library has IsNowOnLine(myp, acc) which does the same thing.
 */
struct PortData *IsOnLine(short acc)
{
    short i;

    if (!myp)
        return NULL;

    for (i = 0; i <= myp->HiPort; i++) {
        if (!IsPortLoaded(i))
            continue;
        if (!myp->PortZ[i]->OnLine)
            continue;
        if (myp->PortZ[i]->id == acc)
            return myp->PortZ[i];
    }

    return NULL;
}

/* ================================================================== */
/*  User helpers                                                        */
/* ================================================================== */

/*
 * Helper: days in a given month (1-based), accounting for leap years.
 */
static short days_in_month(short month, short year)
{
    static const short dtab[] = { 0, 31, 28, 31, 30, 31, 30,
                                     31, 31, 30, 31, 30, 31 };
    short d;

    if (month < 1 || month > 12)
        return 30;

    d = dtab[month];
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
            d = 29;
    }
    return d;
}

/*
 * CalculateAge - calculate a user's age from their birthday.
 *
 * The original takes a UserData pointer and uses the Birthdate field.
 * Uses dos.library DateStamp() to get the current date.
 *
 * IsDate.Year stores year minus ISDATE_BASE_YEAR (1900).
 */
short CalculateAge(struct UserData *user)
{
    struct DateStamp ds;
    LONG days;
    short cur_year, cur_month, cur_day;
    short birth_year, birth_month, birth_day;
    short age;

    if (!user)
        return 0;

    birth_year  = user->Birthdate.Year + ISDATE_BASE_YEAR;
    birth_month = user->Birthdate.Month;
    birth_day   = user->Birthdate.Date;

    if (birth_month == 0 || birth_day == 0)
        return 0;

    /* Get current date from AmigaOS DateStamp.
     * ds_Days is days since Jan 1, 1978.
     * Convert to year/month/day. */
    DateStamp(&ds);
    days = ds.ds_Days;

    /* Walk through years from 1978 to find current date */
    cur_year = 1978;
    for (;;) {
        LONG yd = 365;
        if ((cur_year % 4 == 0 && cur_year % 100 != 0) || cur_year % 400 == 0)
            yd = 366;
        if (days < yd)
            break;
        days -= yd;
        cur_year++;
    }

    cur_month = 1;
    while (cur_month < 12) {
        short md = days_in_month(cur_month, cur_year);
        if (days < md)
            break;
        days -= md;
        cur_month++;
    }
    cur_day = (short)days + 1;

    /* Calculate age */
    age = cur_year - birth_year;
    if (cur_month < birth_month ||
        (cur_month == birth_month && cur_day < birth_day))
        age--;

    if (age < 0)
        age = 0;

    return age;
}

/*
 * ReadAccount - read a user account record from disk.
 *
 * Opens USERDATAFILE (sysdata:bbs.udata4), seeks to the record at
 * accnum * sizeof(UserData), and reads one UserData record.
 *
 * Returns TRUE on success, FALSE on failure.
 *
 * Note: For production use within the BBS, the SEM[11] semaphore
 * should be obtained before accessing the user data file.  Door code
 * should prefer CNC_ReadAccount() (via cnetc.library) which handles
 * locking internally.  This function is provided for standalone tools
 * or cases where the shared library is unavailable.
 */
UBYTE ReadAccount(short id, struct UserData *user)
{
    BPTR fh;
    LONG seekpos;
    LONG bytes;

    if (!user || id < 0)
        return FALSE;

    fh = Open(USERDATAFILE, MODE_OLDFILE);
    if (!fh)
        return FALSE;

    seekpos = (LONG)id * sizeof(struct UserData);
    if (Seek(fh, seekpos, OFFSET_BEGINNING) == -1) {
        Close(fh);
        return FALSE;
    }

    bytes = Read(fh, user, sizeof(struct UserData));
    Close(fh);

    return (UBYTE)(bytes == sizeof(struct UserData));
}

/*
 * SaveAccount - write a user account record to disk.
 *
 * Opens USERDATAFILE, seeks to the record position, writes one record.
 * Returns TRUE on success, FALSE on failure.
 *
 * Same locking caveat as ReadAccount.
 */
UBYTE SaveAccount(struct UserData *user, short id)
{
    BPTR fh;
    LONG seekpos;
    LONG bytes;

    if (!user || id < 0)
        return FALSE;

    fh = Open(USERDATAFILE, MODE_READWRITE);
    if (!fh)
        return FALSE;

    seekpos = (LONG)id * sizeof(struct UserData);
    if (Seek(fh, seekpos, OFFSET_BEGINNING) == -1) {
        Close(fh);
        return FALSE;
    }

    bytes = Write(fh, user, sizeof(struct UserData));
    Close(fh);

    return (UBYTE)(bytes == sizeof(struct UserData));
}

/*
 * FindPosition - find a node's ordinal position in an Exec list.
 *
 * The original prototype is: short FindPosition(struct List *list, struct Node *node)
 * Returns the 0-based position, or -1 if not found.
 */
short FindPosition(struct List *list, struct Node *node)
{
    struct Node *n;
    short pos = 0;

    if (!list || !node)
        return -1;

    for (n = list->lh_Head; n->ln_Succ; n = n->ln_Succ) {
        if (n == node)
            return pos;
        pos++;
    }

    return -1;
}

/* ================================================================== */
/*  I/O helpers                                                         */
/* ================================================================== */

/*
 * IsAbort - check if the user has pressed space (skip) or ^C (abort).
 *
 * Checks z->PrintFlags.  The original calls CheckFlowControl() first
 * (which is a CNC_* function), but since this is a linker library
 * function, we check the flags directly.  Doors using the CNetC
 * architecture should call CCheckFlowControl() before this.
 *
 * Returns TRUE if abort/skip was detected.
 */
BOOL IsAbort(void)
{
    if (!z)
        return FALSE;

    if (z->PrintFlags >= 1)
        return TRUE;

    return FALSE;
}

/*
 * AddKeyBuffer - inject characters into the port's keyboard buffer.
 *
 * The original prototype is: void AddKeyBuffer(char *c)
 * This appends a single character string to z->KBuff using the
 * KB1/KB2 circular buffer pointers.
 */
void AddKeyBuffer(char *c)
{
    LONG i;

    if (!z || !c)
        return;

    /* KBuff is a 256-byte circular buffer with KB2 as the write pointer */
    for (i = 0; c[i]; i++) {
        z->KBuff[z->KB2] = c[i];
        z->KB2 = (z->KB2 + 1) & 0xFF;  /* wrap at 256 */
        if (z->KB2 == z->KB1) {
            /* Buffer full -- advance read pointer (drop oldest) */
            z->KB1 = (z->KB1 + 1) & 0xFF;
        }
    }
}

/* ================================================================== */
/*  Date/time                                                           */
/* ================================================================== */

/*
 * GetDate - fill an IsDate structure with the current date and time.
 *
 * Uses dos.library DateStamp() and converts to CNet's IsDate format.
 * IsDate.Year is stored as (year - 1900).
 */
void GetDate(struct IsDate *dest)
{
    struct DateStamp ds;
    LONG days, secs;
    short year, month, day, hour, minute, second;

    if (!dest)
        return;

    DateStamp(&ds);

    /* Convert tick count to seconds within the day */
    secs = ds.ds_Minute * 60 + ds.ds_Tick / TICKS_PER_SECOND;
    hour   = (short)(secs / 3600);
    minute = (short)((secs % 3600) / 60);
    second = (short)(secs % 60);

    /* Convert days since 1978-01-01 to year/month/day */
    days = ds.ds_Days;
    year = 1978;
    for (;;) {
        LONG yd = 365;
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
            yd = 366;
        if (days < yd)
            break;
        days -= yd;
        year++;
    }

    month = 1;
    while (month < 12) {
        short md = days_in_month(month, year);
        if (days < md)
            break;
        days -= md;
        month++;
    }
    day = (short)days + 1;

    dest->Year   = (UBYTE)(year - ISDATE_BASE_YEAR);
    dest->Month  = (UBYTE)month;
    dest->Date   = (UBYTE)day;
    dest->Hour   = (UBYTE)hour;
    dest->Minute = (UBYTE)minute;
    dest->Second = (UBYTE)second;
}

/*
 * PutDateHere - format an IsDate into the given IsDate structure
 * (i.e., snapshot the current date into the destination).
 *
 * The original prototype is: void PutDateHere(struct IsDate *dest)
 * This is functionally identical to GetDate() -- it fills dest
 * with the current date/time.
 */
void PutDateHere(struct IsDate *dest)
{
    GetDate(dest);
}

/*
 * LongToDate - convert a seconds-since-epoch value to date/time strings.
 *
 * The original prototype is:
 *   void LongToDate(char *date, char *t, ULONG was)
 *
 * 'was' is a seconds value (Amiga epoch: seconds from 1978-01-01).
 * 'date' receives the date string (e.g., "01-15-04" for Jan 15, 2004).
 * 't' receives the time string (e.g., "14:30:00").
 *
 * Both buffers must be at least 12 bytes.
 */
void LongToDate(char *date, char *t, ULONG was)
{
    LONG days, secs;
    short year, month, day, hour, minute, second;

    if (!date || !t)
        return;

    /* Split into days and seconds-within-day */
    days = (LONG)(was / SECONDS_IN_DAY);
    secs = (LONG)(was % SECONDS_IN_DAY);

    hour   = (short)(secs / SECONDS_IN_HOUR);
    secs  %= SECONDS_IN_HOUR;
    minute = (short)(secs / SECONDS_IN_MINUTE);
    second = (short)(secs % SECONDS_IN_MINUTE);

    /* Convert days since 1978-01-01 */
    year = 1978;
    for (;;) {
        LONG yd = 365;
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
            yd = 366;
        if (days < yd)
            break;
        days -= yd;
        year++;
    }

    month = 1;
    while (month < 12) {
        short md = days_in_month(month, year);
        if (days < md)
            break;
        days -= md;
        month++;
    }
    day = (short)days + 1;

    /* CNet date format: MM-DD-YY */
    sprintf(date, "%02d-%02d-%02d", month, day, year % 100);

    /* Time format: HH:MM:SS */
    sprintf(t, "%02d:%02d:%02d", hour, minute, second);
}

/* ================================================================== */
/*  Memory                                                              */
/* ================================================================== */

/*
 * SafeFreeMem - NULL-safe wrapper for exec.library FreeMem().
 */
void SafeFreeMem(void *b, ULONG s)
{
    if (b && s > 0)
        FreeMem(b, s);
}

/*
 * SafeFreeVec - NULL-safe wrapper for exec.library FreeVec().
 */
void SafeFreeVec(void *b)
{
    if (b)
        FreeVec(b);
}

/* ================================================================== */
/*  File helpers                                                        */
/* ================================================================== */

/*
 * OpenAppend - open a file for appending.
 *
 * If the file exists, opens it and seeks to the end.
 * If it doesn't exist, creates it.
 * Returns BPTR file handle, or ZERO on failure.
 */
BPTR OpenAppend(char *name)
{
    BPTR fh;

    if (!name)
        return (BPTR)0;

    fh = Open(name, MODE_READWRITE);
    if (fh) {
        Seek(fh, 0, OFFSET_END);
        return fh;
    }

    /* File doesn't exist -- create it */
    fh = Open(name, MODE_NEWFILE);
    return fh;
}

/*
 * GetFileDate - get file modification date as an IsDate.
 *
 * Returns TRUE on success, FALSE on failure.
 * The IsDate uses CNet's (year - 1900) format.
 */
UBYTE GetFileDate(char *file, struct IsDate *ModDate)
{
    struct FileInfoBlock *fib;
    BPTR lock;

    if (!file || !ModDate)
        return FALSE;

    lock = Lock(file, ACCESS_READ);
    if (!lock)
        return FALSE;

    fib = (struct FileInfoBlock *)AllocVec(sizeof(struct FileInfoBlock),
                                           MEMF_PUBLIC | MEMF_CLEAR);
    if (!fib) {
        UnLock(lock);
        return FALSE;
    }

    if (!Examine(lock, fib)) {
        FreeVec(fib);
        UnLock(lock);
        return FALSE;
    }

    /* Convert DateStamp to IsDate.
     * fib->fib_Date is a struct DateStamp. */
    {
        struct DateStamp *ds = &fib->fib_Date;
        LONG days = ds->ds_Days;
        LONG secs = ds->ds_Minute * 60 + ds->ds_Tick / TICKS_PER_SECOND;
        short year, month, day;

        year = 1978;
        for (;;) {
            LONG yd = 365;
            if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
                yd = 366;
            if (days < yd)
                break;
            days -= yd;
            year++;
        }

        month = 1;
        while (month < 12) {
            short md = days_in_month(month, year);
            if (days < md)
                break;
            days -= md;
            month++;
        }
        day = (short)days + 1;

        ModDate->Year   = (UBYTE)(year - ISDATE_BASE_YEAR);
        ModDate->Month  = (UBYTE)month;
        ModDate->Date   = (UBYTE)day;
        ModDate->Hour   = (UBYTE)(secs / 3600);
        ModDate->Minute = (UBYTE)((secs % 3600) / 60);
        ModDate->Second = (UBYTE)(secs % 60);
    }

    FreeVec(fib);
    UnLock(lock);
    return TRUE;
}

/*
 * GetFileDateY2K - Y2K-aware version of GetFileDate.
 *
 * The original uses IsDate5 which likely stores the full year as a
 * USHORT instead of a UBYTE offset from 1900.  Since we don't have
 * the IsDate5 definition, we define a minimal compatible version.
 *
 * TODO: Replace with actual IsDate5 struct when its definition is
 * located.  For now, this stores the full year in the first two bytes.
 */
CNET_PACK_BEGIN

struct IsDate5 {
    USHORT Year;    /* full year, e.g. 2026 */
    UBYTE  Month;
    UBYTE  Date;
    UBYTE  Hour;
    UBYTE  Minute;
    UBYTE  Second;
    UBYTE  pad;
};

CNET_PACK_END

UBYTE GetFileDateY2K(char *file, struct IsDate5 *ModDate)
{
    struct IsDate tmp;

    if (!file || !ModDate)
        return FALSE;

    if (!GetFileDate(file, &tmp))
        return FALSE;

    ModDate->Year   = (USHORT)(tmp.Year + ISDATE_BASE_YEAR);
    ModDate->Month  = tmp.Month;
    ModDate->Date   = tmp.Date;
    ModDate->Hour   = tmp.Hour;
    ModDate->Minute = tmp.Minute;
    ModDate->Second = tmp.Second;

    return TRUE;
}

/*
 * CopyFile - copy a file from src to dst.
 *
 * Returns non-zero byte count on success, 0 on failure.
 */
ULONG CopyFile(char *fromfile, char *tofile)
{
    BPTR fh_in, fh_out;
    char buf[4096];
    LONG n;
    ULONG total = 0;

    if (!fromfile || !tofile)
        return 0;

    fh_in = Open(fromfile, MODE_OLDFILE);
    if (!fh_in)
        return 0;

    fh_out = Open(tofile, MODE_NEWFILE);
    if (!fh_out) {
        Close(fh_in);
        return 0;
    }

    while ((n = Read(fh_in, buf, sizeof(buf))) > 0) {
        if (Write(fh_out, buf, n) != n) {
            Close(fh_in);
            Close(fh_out);
            return 0;
        }
        total += n;
    }

    Close(fh_in);
    Close(fh_out);
    return total;
}

/*
 * AppendFile - append contents of source file to destination file.
 */
void AppendFile(char *dest, char *source)
{
    BPTR fh_in, fh_out;
    char buf[4096];
    LONG n;

    if (!dest || !source)
        return;

    fh_in = Open(source, MODE_OLDFILE);
    if (!fh_in)
        return;

    fh_out = OpenAppend(dest);
    if (!fh_out) {
        Close(fh_in);
        return;
    }

    while ((n = Read(fh_in, buf, sizeof(buf))) > 0) {
        if (Write(fh_out, buf, n) != n)
            break;
    }

    Close(fh_in);
    Close(fh_out);
}

/*
 * DeleteAFile - delete a file.  Silently ignores errors.
 */
void DeleteAFile(char *name)
{
    if (name)
        DeleteFile(name);
}

/*
 * MakePath - ensure a directory path exists (like mkdir -p).
 *
 * Creates each component of the path that doesn't already exist.
 */
void MakePath(char *where)
{
    char pathbuf[256];
    char *p;
    BPTR lock;

    if (!where || !*where)
        return;

    /* Try to lock the full path first */
    lock = Lock(where, ACCESS_READ);
    if (lock) {
        UnLock(lock);
        return;
    }

    strncpy(pathbuf, where, sizeof(pathbuf) - 1);
    pathbuf[sizeof(pathbuf) - 1] = '\0';

    /* Walk through path components and create directories.
     * Handle AmigaOS paths: volume:dir/dir/dir
     * Skip past the volume/device name (up to first ':') */
    p = strchr(pathbuf, ':');
    if (p)
        p++;
    else
        p = pathbuf;

    while (*p) {
        /* Find next separator */
        while (*p && *p != '/')
            p++;

        if (*p == '/') {
            *p = '\0';

            lock = Lock(pathbuf, ACCESS_READ);
            if (!lock) {
                lock = CreateDir(pathbuf);
                if (lock)
                    UnLock(lock);
            } else {
                UnLock(lock);
            }

            *p = '/';
            p++;
        }
    }

    /* Create the final directory component */
    lock = Lock(pathbuf, ACCESS_READ);
    if (!lock) {
        lock = CreateDir(pathbuf);
        if (lock)
            UnLock(lock);
    } else {
        UnLock(lock);
    }
}

/* ================================================================== */
/*  Exec list utilities                                                 */
/* ================================================================== */

/*
 * init_list - allocate and initialize a new Exec List.
 *
 * If 'is_public' is TRUE, allocates from MEMF_PUBLIC.
 * Returns pointer to the new list, or NULL on failure.
 */
struct List *init_list(UBYTE is_public)
{
    struct List *list;
    ULONG memtype = MEMF_CLEAR;

    if (is_public)
        memtype |= MEMF_PUBLIC;

    list = (struct List *)AllocVec(sizeof(struct List), memtype);
    if (list) {
        list->lh_Head     = (struct Node *)&list->lh_Tail;
        list->lh_Tail     = NULL;
        list->lh_TailPred = (struct Node *)&list->lh_Head;
    }
    return list;
}

/*
 * free_list - free all nodes in an Exec list.
 *
 * Removes and frees each node using FreeVec().  The list head itself
 * is also freed (since init_list allocates it).
 */
void free_list(struct List *list)
{
    struct Node *node, *next;

    if (!list)
        return;

    node = list->lh_Head;
    while (node->ln_Succ) {
        next = node->ln_Succ;
        Remove(node);
        FreeVec(node);
        node = next;
    }

    FreeVec(list);
}

/* ================================================================== */
/*  Editor                                                              */
/* ================================================================== */

/*
 * TextToEditor - parse text into the port's editor buffer.
 *
 * The CNet editor uses z->EdBuffer (array of char pointers) and
 * z->txline (line count).  This function splits text at newlines
 * and copies each line into the editor buffer.
 *
 * UNIMPLEMENTED: The original accesses z->EdBuffer, z->EdBase,
 * z->MaxLines, and z->txline.  Full implementation requires the
 * editor buffer allocation scheme which is not documented in the
 * surviving SDK headers.  Doors that need editor functionality
 * should use CNC_CallEditor() or CNC_EditMessage() instead.
 */
void TextToEditor(char *text)
{
    (void)text;
}

/* ================================================================== */
/*  Socket helpers                                                      */
/* ================================================================== */

/*
 * Socket functions require bsdsocket.library, which on AmigaOS must
 * be opened per-task.  CNet stores the socket base in z->SocketBase.
 * These functions use the port's SocketBase for all socket operations.
 *
 * The original cnet.lib socket functions accessed SocketBase as a
 * global (per-task) variable.  In the GCC reimplementation, we
 * pull it from z->SocketBase.
 *
 * Note: For these to compile, the bsdsocket.library headers must be
 * available in the include path.  If not present, this section can
 * be conditionally compiled.
 */

#ifdef CNET_SOCKET_SUPPORT

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <proto/socket.h>

/*
 * SockPrintf - formatted write to a socket.
 *
 * Like printf() but sends to a socket via send().
 */
void SockPrintf(int sock, char *fmt, ...)
{
    static char buf[4096];
    va_list ap;
    int len;

    va_start(ap, fmt);
    len = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    if (len > 0)
        send(sock, buf, len, 0);
}

/*
 * TNSockWrite - telnet-aware socket write.
 *
 * Writes data to a socket, doubling any IAC (0xFF) bytes as required
 * by the Telnet protocol (RFC 854).
 */
void TNSockWrite(int sock, char *buff, LONG length)
{
    char outbuf[512];
    LONG i, o;

    if (!buff || length <= 0)
        return;

    o = 0;
    for (i = 0; i < length; i++) {
        if ((UBYTE)buff[i] == 0xFF) {
            /* Escape IAC byte by sending it twice */
            outbuf[o++] = (char)0xFF;
        }
        outbuf[o++] = buff[i];

        if (o >= (LONG)(sizeof(outbuf) - 2)) {
            send(sock, outbuf, o, 0);
            o = 0;
        }
    }

    if (o > 0)
        send(sock, outbuf, o, 0);
}

/*
 * SockGetC - read a single character from a socket.
 *
 * Returns the character value (0-255), or -1 on error/disconnect.
 */
int SockGetC(int sock)
{
    char c;
    int n;

    n = recv(sock, &c, 1, 0);
    if (n <= 0)
        return -1;

    return (int)(unsigned char)c;
}

/*
 * SockStatus - check socket status.
 *
 * Uses select() with zero timeout to check readability.
 * Returns non-zero if data is available, 0 otherwise.
 */
int SockStatus(int sock)
{
    fd_set readfds;
    struct timeval tv;

    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);
    tv.tv_sec  = 0;
    tv.tv_usec = 0;

    return select(sock + 1, &readfds, NULL, NULL, &tv);
}

/*
 * SockQueue - check number of bytes queued on socket.
 *
 * The original prototype is: int SockQueue(int sock, int number)
 * Uses ioctl(FIONREAD) to check pending bytes.
 * Returns number of bytes available, or -1 on error.
 */
int SockQueue(int sock, int number)
{
    long avail = 0;

    if (IoctlSocket(sock, FIONREAD, (char *)&avail) < 0)
        return -1;

    return (int)avail;
}

/*
 * SockCarrier - check if socket is still connected.
 *
 * Attempts a zero-length recv with MSG_PEEK to check connection state.
 * Returns TRUE if connected, FALSE if disconnected.
 */
BOOL SockCarrier(int sock)
{
    fd_set readfds;
    struct timeval tv;
    char c;
    int n;

    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);
    tv.tv_sec  = 0;
    tv.tv_usec = 0;

    n = select(sock + 1, &readfds, NULL, NULL, &tv);
    if (n <= 0)
        return TRUE;  /* No event -- still connected (or error) */

    /* Socket is readable -- peek at data.  If recv returns 0,
     * the remote end has closed the connection. */
    n = recv(sock, &c, 1, MSG_PEEK);
    if (n == 0)
        return FALSE;  /* Disconnected */

    return TRUE;
}

/*
 * CNL_SockWait - wait for socket data with timeout.
 *
 * The original prototype is:
 *   ULONG CNL_SockWait(ULONG waitmask, int sock, ULONG secs, ULONG mics)
 *
 * Waits for data on the socket OR for Amiga signals in waitmask.
 * Uses WaitSelect() which is the bsdsocket.library equivalent of
 * select() that also waits on Amiga signal masks.
 *
 * Returns the signal mask that was triggered, or 0 if the socket
 * became readable.
 */
ULONG CNL_SockWait(ULONG waitmask, int sock, ULONG secs, ULONG mics)
{
    fd_set readfds;
    struct timeval tv;
    ULONG signals = waitmask;
    int n;

    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);
    tv.tv_sec  = secs;
    tv.tv_usec = mics;

    n = WaitSelect(sock + 1, &readfds, NULL, NULL, &tv, &signals);

    if (signals)
        return signals;  /* An Amiga signal was received */

    return 0;  /* Socket became readable or timeout */
}

#else /* !CNET_SOCKET_SUPPORT */

/*
 * Socket function stubs when bsdsocket.library headers are not
 * available at compile time.  These provide the correct symbols
 * for linking but will fail gracefully at runtime.
 */

void SockPrintf(int sock, char *fmt, ...)
{
    (void)sock;
    (void)fmt;
}

void TNSockWrite(int sock, char *buff, LONG length)
{
    (void)sock;
    (void)buff;
    (void)length;
}

int SockGetC(int sock)
{
    (void)sock;
    return -1;
}

int SockStatus(int sock)
{
    (void)sock;
    return 0;
}

int SockQueue(int sock, int number)
{
    (void)sock;
    (void)number;
    return -1;
}

BOOL SockCarrier(int sock)
{
    (void)sock;
    return FALSE;
}

ULONG CNL_SockWait(ULONG waitmask, int sock, ULONG secs, ULONG mics)
{
    (void)waitmask;
    (void)sock;
    (void)secs;
    (void)mics;
    return 0;
}

#endif /* CNET_SOCKET_SUPPORT */
