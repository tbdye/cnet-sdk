/*
 * standalone.c - Standalone CNet utility template for GCC
 *
 * This template shows how to write a standalone AmigaDOS program that
 * reads CNet's shared data structures without being a door.
 *
 * How it works:
 *   - CNet's main process creates an Exec message port named "cnetport".
 *   - The port's address IS the MainPort structure, which contains
 *     all system-wide BBS state: port data, user keys, subboards,
 *     configuration, etc.
 *   - A standalone utility calls FindPort("cnetport") to get a pointer
 *     to the MainPort, then reads or modifies fields as needed.
 *
 * When to use this pattern:
 *   - CLI tools that inspect BBS state (port status, user info, etc.)
 *   - Utilities run from AmigaDOS shell while CNet is active
 *   - ARexx-triggered tools, cron jobs, event scripts
 *   - Any program that does NOT need the door I/O framework
 *
 * Important:
 *   - These utilities do NOT link with libcnet_door.a
 *   - Access to MainPort fields is NOT protected by CNet's I/O layer
 *   - Use ObtainSemaphore/ReleaseSemaphore when modifying shared data
 *   - CNet must be running for FindPort to succeed
 *
 * GCC note on include order:
 *   <proto/exec.h> must be included BEFORE any CNet SDK header that
 *   redefines __asm.  See pfile_raw.c header comment for details.
 *   We include system struct/proto headers first, then apply the SAS/C
 *   compat macros (excluding __asm), then include CNet struct headers.
 *
 * Build:
 *   m68k-amigaos-gcc -noixemul -m68020 -O2 -Wall -Wno-pointer-sign \
 *       -Iinclude -Iinclude/cnet -o templates/standalone \
 *       templates/standalone.c
 *
 * Runtime requirements:
 *   CNet 5 running (cnetport must exist)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * System struct definitions and proto headers FIRST.
 * The proto headers contain inline stubs that need __asm as a
 * GCC keyword -- they must be processed before any CNet header
 * that redefines __asm.
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

/*
 * GCC compat macros for SAS/C qualifiers in CNet struct headers.
 * Do NOT redefine __asm -- that would break the Exec inline stubs.
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
 * CNet struct headers (individual, not the master cnet.h).
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
#include <cnet/control.h>

/* libnix stack size */
unsigned long __stack = 65536;


int main(int argc, char **argv)
{
    struct MainPort *myp;

    /*
     * FindPort() locates the named Exec message port.  The returned
     * pointer is the MainPort structure itself (CNet uses the MsgPort
     * at the beginning of MainPort as the Exec port).
     *
     * CNETPORT is defined as "cnetport" in <cnet/ports.h>.
     *
     * Wrap in Forbid/Permit if you plan to use the port pointer
     * across potential task switches.  For a simple read-and-exit
     * utility, it's safe without.
     */
    myp = (struct MainPort *)FindPort(CNETPORT);
    if (!myp) {
        printf("CNet is not running.\n");
        return 1;
    }

    /*
     * Example: print basic system info from MainPort.
     *
     * MainPort contains:
     *   myp->HiPort         - highest configured port number
     *   myp->Nums[0]        - number of user accounts
     *   myp->PortZ[n]       - pointer to PortData for port n
     *   myp->Subboard[n]    - subboard configuration array
     *   myp->Key[n]         - user key (handle, ID) array
     *   myp->MPE            - MainPort extension (identd, DCC, etc.)
     *   myp->regto          - registered-to name
     *   myp->regco          - registration code
     *   myp->gc             - global configuration
     */

    printf("CNet MainPort found at 0x%lx\n", (unsigned long)myp);
    printf("Registered to: %s\n", myp->regto);
    printf("Highest port:  %d\n", myp->HiPort);
    printf("User accounts: %ld\n", myp->Nums[0]);

    /*
     * Example: iterate loaded ports.
     *
     * myp->z0 is the "null" PortData -- ports that aren't loaded
     * point to z0 instead of NULL.  Compare against z0 to check
     * if a port is actually active.
     */
    {
        int i;
        for (i = 0; i <= myp->HiPort; i++) {
            struct PortData *zp = myp->PortZ[i];
            if (zp != myp->z0) {
                printf("Port %2d: loaded, online=%d\n",
                       i, myp->pc[i].online);
            }
        }
    }

    return 0;
}
