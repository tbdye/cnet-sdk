/*
 * IsAbort.c - verbose abort check with user feedback.
 *
 * Originally a CommonFuncs code snippet in the CNet SDK.
 * Converted to a proper compilable unit for m68k-amigaos-gcc.
 *
 * This is the "verbose" version that calls CNC_CheckFlowControl()
 * and sends "[SKIP]" or "^C" to the user's terminal.  It is named
 * IsAbortVerbose() to avoid a duplicate symbol with the simpler
 * IsAbort() in utility.c (which only checks flags without I/O).
 *
 * Doors that want the original CommonFuncs behavior (with output)
 * should call IsAbortVerbose().  Doors that just want a silent
 * flag check should call IsAbort().
 *
 * Because this file calls CNC_CheckFlowControl() and CNC_PutText()
 * via inline stubs, the cnetc.library proto header must be included
 * BEFORE any CNet headers (to avoid the __asm conflict).
 */

#include <exec/types.h>

/*
 * Include cnetc.library inline stubs BEFORE any CNet SDK headers
 * that redefine __asm.
 */
#include <proto/cnetc.h>

/*
 * Forward declarations for CNet struct types.  We avoid including
 * <cnet/cnet.h> because its __asm redefinition would break the
 * inline stubs already included above.
 */
struct PortData;
struct MainPort;
struct CNetCContext;

/* Globals from door_init.c */
extern struct Library      *CNetCBase;

/*
 * IsAbortVerbose - check for user abort with terminal feedback.
 *
 * API change from the original CommonFuncs IsAbort():
 *   Original: BOOL IsAbort(void)  -- accessed z->PrintFlags directly.
 *   This:     BOOL IsAbortVerbose(UBYTE *printFlags) -- takes a pointer
 *             to the flags, because we cannot include cnet.h (which
 *             defines PortData) without breaking the __asm inline stubs.
 *
 * Renamed to IsAbortVerbose() to avoid symbol collision with the
 * simpler IsAbort() in utility.c (which checks flags silently).
 * Doors wanting the original CommonFuncs behavior (with terminal
 * output) should call IsAbortVerbose(&z->PrintFlags).
 *
 * Calls CNC_CheckFlowControl() to poll for space/^C, then checks
 * the printFlags value.  If abort was detected, sends feedback text
 * ("[SKIP]" for space, "^C" for control-C) to the user's terminal.
 *
 * Returns TRUE if abort/skip was detected.
 */
BOOL IsAbortVerbose(UBYTE *printFlags)
{
    CNC_CheckFlowControl();
    if (*printFlags >= 1) {
        if (*printFlags == 1)
            CNC_PutText("[SKIP]");
        if (*printFlags > 1)
            CNC_PutText("^C");
        return TRUE;
    }
    return FALSE;
}
