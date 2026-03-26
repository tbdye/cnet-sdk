/*
 * door_init.c - CNet door (PFile) lifecycle management
 *
 * Provides CNCL_DoorInit() and CNCL_DoorCleanup(), which replace the
 * original SAS/C cnet.lib implementations for use with m68k-amigaos-gcc.
 *
 * CNCL_DoorInit() opens cnetc.library, initializes the door context via
 * CNC_InitContext(), and populates the global pointers that door code
 * accesses directly (CNetCBase, context, z, myp, cport).
 *
 * CNCL_DoorCleanup() tears down the context and closes the library.
 * It is registered as an atexit() handler so that exit() from anywhere
 * in door code cleanly shuts down the CNet connection.
 *
 * Usage:
 *   CNet launches doors with argv[1] set to the CPort name.
 *   Call CNCL_DoorInit(argc, argv) at the top of main().
 *   On success it returns non-zero (TRUE); on failure, zero.
 *   The door calls exit() when finished; atexit runs cleanup.
 *
 * NOTE: This file calls CNC_* library functions AND needs CNet struct
 * definitions.  The inline stubs (LP* macros) require __asm as a GCC
 * keyword for register binding, but <cnet/cnet.h> redefines __asm to
 * nothing for SAS/C compatibility.  To resolve this:
 *   1. Include <proto/cnetc.h> FIRST (gets inline stubs with __asm intact)
 *   2. Do NOT include <cnet/cnet.h>
 *   3. Duplicate the minimal CNet struct definitions needed here
 *
 * Compile: m68k-amigaos-gcc -noixemul -Iinclude ...
 */

#include <stdlib.h>

#include <exec/types.h>
#include <exec/ports.h>
#include <exec/libraries.h>
#include <proto/exec.h>
#include <dos/rdargs.h>

/*
 * Include cnetc.library inline stubs BEFORE anything that might
 * redefine __asm.  The LP* macros require __asm as a GCC keyword.
 */
#include <proto/cnetc.h>

/*
 * Forward declarations for CNet struct types used as opaque pointers.
 */
struct PortData;
struct MainPort;
struct CPort;

/*
 * Minimal struct definitions needed to access CNetCContext members.
 * These are duplicated from include/cnet/doors.h to avoid including
 * the full cnet.h header chain (which would redefine __asm and break
 * the inline stubs above).
 *
 * These must match the definitions in include/cnet/doors.h exactly.
 */
struct CMessage
{
    struct Message cn_Message;
    ULONG          arg1;
    ULONG          arg2;
    ULONG          arg3;
    ULONG          arg4;
    ULONG          result;
    UBYTE          command;
    UBYTE          align[3];
} __attribute__((packed));

struct CNetCContext
{
    struct MainPort *myp;
    struct PortData *z;
    struct CPort    *cport;
    struct MsgPort  *replyp;
    struct CMessage cmess;
} __attribute__((packed));

/* libnix stack size -- doors need headroom beyond the default 4KB */
unsigned long __stack = 65536;

/*
 * Global door state.  These are accessed directly by door code and by
 * the CNC_* inline stubs (CNetCBase is the library base name used by
 * the inline macros in <inline/cnetc.h>).
 *
 * myrdargs: SAS/C doors declared this for ReadArgs() parsing.  GCC
 * doors don't use ReadArgs(), but we provide the symbol so that code
 * ported from SAS/C examples (which may reference it) still links.
 */
struct Library      *CNetCBase = NULL;
struct CNetCContext *context   = NULL;
struct PortData     *z         = NULL;
struct MainPort     *myp       = NULL;
struct CPort        *cport     = NULL;
struct RDArgs       *myrdargs  = NULL;

/*
 * CNCL_DoorCleanup - shut down the door's CNet connection.
 *
 * Safe to call at any point, including partial initialization.
 * Registered as an atexit() handler by CNCL_DoorInit().
 */
void CNCL_DoorCleanup(void)
{
    if (context) {
        CNC_ShutDown(NULL);
        CNC_DisposeContext();
        context = NULL;
    }

    if (CNetCBase) {
        CloseLibrary(CNetCBase);
        CNetCBase = NULL;
    }

    z     = NULL;
    myp   = NULL;
    cport = NULL;
}

/*
 * CNCL_DoorInit - initialize the door's CNet connection.
 *
 * argc/argv come from main().  CNet passes the CPort name as argv[1].
 *
 * API change from SAS/C: The original SAS/C cnet.lib used ReadArgs()
 * to parse the CNet command-line template into a LONG myargs[] array,
 * so the signature was CNCL_DoorInit(LONG *myargs).  GCC/libnix
 * provides standard argc/argv, so the signature is now (int, char**).
 * Door code should use: int main(int argc, char **argv) and pass
 * argc/argv to CNCL_DoorInit().  argv[1] is the CPort name (equivalent
 * to the old myargs[0]).  Additional CNet arguments, if any, follow
 * in argv[2..].
 *
 * Returns non-zero (TRUE) on success, 0 on failure.  On failure, all
 * partially-acquired resources are released before returning.
 */
BOOL CNCL_DoorInit(int argc, char **argv)
{
    if (argc < 2) {
        return FALSE;
    }

    CNetCBase = OpenLibrary("cnetc.library", 0);
    if (!CNetCBase) {
        return FALSE;
    }

    context = CNC_InitContext(argv[1]);
    if (!context) {
        CloseLibrary(CNetCBase);
        CNetCBase = NULL;
        return FALSE;
    }

    z     = context->z;
    myp   = context->myp;
    cport = context->cport;

    atexit(CNCL_DoorCleanup);

    return TRUE;
}
