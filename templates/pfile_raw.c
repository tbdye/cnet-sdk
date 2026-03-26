/*
 * pfile_raw.c - Raw CMessage door template (v3-style) for GCC
 *
 * This template shows the low-level door pattern used since CNet v3.
 * Instead of the cnetc.library convenience functions (CNC_PutText, etc.),
 * the door communicates with CNet by sending CMessage commands directly
 * via an Exec message port.
 *
 * How it works:
 *   - CNet launches the door with argv[1] set to the CPort name.
 *   - The door calls FindPort(argv[1]) to locate the CPort.
 *   - It creates a reply port, sets up a CMessage, then uses
 *     CallHost() to send commands to CNet and receive results.
 *   - Command codes are defined in <cnet/doors.h> (CMESS_CMD_*).
 *
 * When to use this pattern:
 *   - Porting existing v3 doors that use the CMessage interface.
 *   - When you need raw CMessage access without cnetc.library.
 *   - For lightweight doors that want minimal library dependencies.
 *
 * For new doors, prefer the CNetC pattern (pfile_cnetc.c) -- it's
 * simpler and provides a richer API via cnetc.library.
 *
 * GCC note on include order:
 *   <proto/exec.h> must be included BEFORE any CNet SDK header that
 *   redefines __asm.  The GCC inline stubs for Exec functions (FindPort,
 *   PutMsg, WaitPort, GetMsg, etc.) use __asm as a GCC keyword for
 *   register binding.  <cnet/cnet.h> redefines __asm to nothing (for
 *   SAS/C compat), which breaks the Exec stubs.  We avoid this by
 *   including <proto/exec.h> first and then individual CNet struct
 *   headers instead of the master <cnet/cnet.h>.
 *
 * Build:
 *   m68k-amigaos-gcc -noixemul -m68020 -O2 -Wall -Wno-pointer-sign \
 *       -Iinclude -Iinclude/cnet -o templates/pfile_raw \
 *       templates/pfile_raw.c -Llib -lcnet_door
 *
 * Runtime requirements:
 *   CNet 5 running with the door configured as a C-language PFile
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * System proto headers FIRST -- these need __asm as a GCC keyword.
 * Must come before any CNet headers that redefine __asm.
 */
#include <exec/types.h>
#include <exec/lists.h>
#include <exec/ports.h>
#include <exec/semaphores.h>
#include <exec/io.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include <graphics/gfx.h>
#include <graphics/rastport.h>
#include <graphics/text.h>
#include <intuition/intuition.h>
#include <libraries/iffparse.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <clib/alib_protos.h>

/*
 * GCC compat macros for SAS/C qualifiers used in CNet struct headers.
 *
 * We define __saveds, __regargs, __stdargs but NOT __asm.
 * The GCC inline stubs for Exec/DOS functions use __asm("reg") for
 * register binding.  Redefining __asm to nothing (as <cnet/cnet.h>
 * does for SAS/C compat) breaks those stubs.  The CNet struct headers
 * (doors.h, ports.h) don't use __asm in struct definitions, so
 * omitting the redefinition is safe here.
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

/*
 * Include CNet struct headers individually.  We avoid the master
 * <cnet/cnet.h> because it redefines __asm to nothing, which breaks
 * the GCC inline stubs for Exec/DOS functions.
 *
 * ports.h requires struct definitions from several other headers
 * (subs.h needs io.h, dates.h, users.h, etc.), so we include them
 * in dependency order.
 */
#include <cnet/io.h>
#include <cnet/dates.h>
#include <cnet/bbstext.h>
#include <cnet/users.h>
#include <cnet/subs.h>
#include <cnet/charges.h>
#include <cnet/config.h>
#include <cnet/doors.h>
#include <cnet/mail.h>
#include <cnet/ranges.h>
#include <cnet/lists.h>
#include <cnet/links.h>
#include <cnet/telnet.h>
#include <cnet/ports.h>

/* libnix stack size */
unsigned long __stack = 65536;

/*
 * Global door state.
 *
 * cport  - the CPort message port that CNet created for this door
 * replyp - our reply port for receiving CMessage responses
 * cmess  - the CMessage structure sent back and forth
 * myp    - pointer to CNet's MainPort (system-wide state)
 * z      - pointer to this port's PortData (per-user state)
 */
static struct MsgPort  *replyp = NULL;
static struct CPort    *cport  = NULL;
static struct CMessage cmess;
struct MainPort        *myp    = NULL;
struct PortData        *z      = NULL;


/* -----------------------------------------------------------------------
 * CMessage interface functions
 * ----------------------------------------------------------------------- */

/*
 * CallHost - send a command to CNet and wait for the reply.
 *
 * This is the fundamental communication primitive.  Set cmess fields
 * (arg1..arg4) before calling, then check cmess.result after return.
 * Command codes are in <cnet/doors.h>.
 */
static void RawCallHost(UBYTE c)
{
    cmess.command = c;
    PutMsg((struct MsgPort *)cport, (struct Message *)&cmess);
    WaitPort(replyp);
    GetMsg(replyp);
}

/*
 * PutText - display text to the user.
 * Supports CNet MCI codes (color, cursor, etc.).
 */
static void RawPutText(char *text)
{
    cmess.arg1 = (ULONG)text;
    RawCallHost(CMESS_CMD_PUTTEXT);
}

/*
 * OneKey - wait for a single keypress and return it.
 */
static char RawOneKey(void)
{
    RawCallHost(CMESS_CMD_ONEKEY);
    return (char)cmess.result;
}

/*
 * EnterLine - prompt the user for text input.
 *
 * len   - maximum input length
 * flags - ELINE_* flags (see <cnet/doors.h>)
 * prompt - text to display before input
 *
 * Result is placed in z->InBuffer.
 * Returns the length of the input string.
 */
static int RawEnterLine(UBYTE len, ULONG flags, char *prompt)
{
    cmess.arg1 = (ULONG)len;
    cmess.arg2 = (ULONG)flags;
    cmess.arg3 = (ULONG)prompt;
    RawCallHost(CMESS_CMD_ENTERLINE);
    return (int)strlen(z->InBuffer);
}

/*
 * ShutDown - tell CNet this door is finished.
 *
 * spawn: if non-NULL, full path of another program to chain-run.
 *        CNet will execute it after this door exits.
 */
static void RawShutDown(char *spawn)
{
    if (spawn)
        strcpy(z->CSpawn, spawn);
    RawCallHost(CMESS_CMD_SHUTDOWN);
}


/* -----------------------------------------------------------------------
 * Lifecycle
 * ----------------------------------------------------------------------- */

static void GetOut(void)
{
    RawShutDown(NULL);

    if (replyp) {
        /* Drain any pending messages before deleting the port */
        while (GetMsg(replyp))
            ;
        DeletePort(replyp);
        replyp = NULL;
    }
}


/* -----------------------------------------------------------------------
 * Door logic
 * ----------------------------------------------------------------------- */

static void DoorMain(void)
{
    /* Example: display text with MCI color codes */
    RawPutText("\nc4Hello from raw CMessage door!q1\n");

    /* Example: get a keypress */
    RawPutText("Press any key to continue...");
    RawOneKey();
    RawPutText("\n");

    /* Example: text input */
    if (RawEnterLine(40, 0, "Enter something: ")) {
        char buf[128];
        sprintf(buf, "You entered: %s\n", z->InBuffer);
        RawPutText(buf);
    }
}


int main(int argc, char **argv)
{
    /* Must be launched by CNet with CPort name as argv[1] */
    if (argc < 2) {
        printf("This is a CNet C PFile -- run from CNet BBS.\n");
        return 0;
    }

    /* Find the CPort that CNet created for this door session */
    Forbid();
    cport = (struct CPort *)FindPort(argv[1]);
    Permit();

    if (!cport) {
        printf("Could not find CNet port '%s'.\n", argv[1]);
        return 1;
    }

    /* Create our reply port for CMessage communication */
    replyp = CreatePort(0, 0);
    if (!replyp)
        return 1;

    /* Set up the CMessage structure */
    cmess.cn_Message.mn_ReplyPort    = replyp;
    cmess.cn_Message.mn_Length       = sizeof(struct CMessage);
    cmess.cn_Message.mn_Node.ln_Name = "cstuff";

    /* CNet version handshake: cport->ack == 30 means v4+ */
    if (cport->ack != 30) {
        cport->ack = 1;
        DeletePort(replyp);
        return 1;
    }
    cport->ack = 0;

    /* Get pointers to the per-port and system-wide data */
    z   = cport->zp;
    myp = cport->myp;

    /* Register cleanup handler */
    atexit(GetOut);

    /* Run the door */
    DoorMain();

    /* exit() triggers GetOut() via atexit */
    return 0;
}
