/*
 * ClonePortPri.c - match this task's priority to the CNet port's task.
 *
 * Originally a CommonFuncs code snippet in the CNet SDK.
 * Converted to a proper compilable unit for m68k-amigaos-gcc.
 *
 * Sets the current task's priority to match the CNet port's task
 * priority (z->ThisTask->tc_Node.ln_Pri).  This ensures the door
 * runs at the same scheduling priority as the BBS port that launched it.
 *
 * NOTE: This file uses Exec inline stubs (FindTask, SetTaskPri) which
 * require __asm as a GCC keyword.  Do NOT include <cnet/cnet.h> here
 * because it redefines __asm to nothing.  Include individual CNet
 * struct headers instead (they do not use __asm).
 */

#include <exec/types.h>
#include <exec/tasks.h>
#include <exec/lists.h>
#include <exec/nodes.h>
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
 * GCC compatibility macros for SAS/C qualifiers in CNet struct headers.
 * Do NOT redefine __asm -- it is needed by the Exec inline stubs above.
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

/* CNet SDK struct headers (no __asm usage) */
#include <cnet/dates.h>
#include <cnet/users.h>
#include <cnet/subs.h>
#include <cnet/doors.h>
#include <cnet/control.h>

/* Global from door_init.c */
extern struct PortData *z;

void ClonePortPri(void)
{
    struct Process *myproc;

    /* adjust own priority to that of CNet port's */
    if ((myproc = (struct Process *)FindTask(NULL)) != NULL) {
        SetTaskPri((struct Task *)myproc, z->ThisTask->tc_Node.ln_Pri);
    }
}
