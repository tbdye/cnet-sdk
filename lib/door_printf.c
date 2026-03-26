/*
 * door_printf.c - CNCL_Printf implementation
 *
 * Provides a printf-style convenience function for CNet doors.
 * Formats the string into a static buffer and sends it to the
 * user's terminal via CNC_PutText().
 *
 * Compile: m68k-amigaos-gcc -noixemul -m68020 -Iinclude ...
 */

#include <stdarg.h>
#include <stdio.h>

#include <exec/types.h>
#include <exec/libraries.h>

/*
 * Include the cnetc.library inline stubs BEFORE any CNet SDK headers
 * that redefine __asm.  The LP* macros in inline/macros.h require
 * __asm as a GCC keyword for register binding; <cnet/cnet.h> redefines
 * __asm to nothing (for SAS/C compatibility) which would break them.
 */
#include <proto/cnetc.h>

/* CNetCBase is the library base used by the CNC_* inline stubs */
extern struct Library *CNetCBase;

/*
 * CNCL_Printf - formatted output to the BBS terminal.
 *
 * Works like printf() but sends the result through CNC_PutText()
 * to the user's terminal session.  Uses a static 4096-byte buffer,
 * so output is silently truncated if it exceeds that length.
 */
void CNCL_Printf(char *fmt, ...)
{
    static char buf[4096];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    CNC_PutText(buf);
}
