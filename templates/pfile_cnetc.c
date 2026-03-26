/*
 * pfile_cnetc.c - CNetC door template for GCC cross-compiler
 *
 * This is a clean starting point for CNet 5 doors that use the
 * cnetc.library shared library interface (the "CNetC" pattern).
 *
 * How it works:
 *   - CNet launches the door with argv[1] set to the CPort name.
 *   - CNCL_DoorInit() (from libcnet_door.a) opens cnetc.library,
 *     calls CNC_InitContext(), and populates the global pointers:
 *     CNetCBase, context, z, myp.
 *   - Door code uses wrapper functions (CPutText, CEnterLine,
 *     COneKey, etc.) from libcnet_door.a to interact with the user.
 *   - On exit(), the atexit handler calls CNCL_DoorCleanup() to
 *     tear down the context and close the library.
 *
 * GCC note on CNC_* vs C* function names:
 *   The original SAS/C examples call CNC_PutText(), CNC_EnterLine(),
 *   etc. directly.  These are inline macros in <proto/cnetc.h> that
 *   use GCC's __asm keyword for register binding.  However,
 *   <cnet/cnet.h> redefines __asm to nothing (for SAS/C compat),
 *   making it impossible to use both in the same file.
 *
 *   libcnet_door.a provides wrapper functions with a "C" prefix that
 *   call through to the CNC_* shared library functions internally:
 *     CPutText()     -> CNC_PutText()
 *     CEnterLine()   -> CNC_EnterLine()
 *     COneKey()      -> CNC_OneKey()
 *     CPutA()        -> CNC_PutA()
 *     CPutQ()        -> CNC_PutQ()
 *     CWaitForInput() -> CNC_WaitForInput()
 *     ... etc.
 *
 *   Use the C* wrapper names in GCC door code.
 *
 * Build:
 *   m68k-amigaos-gcc -noixemul -m68020 -O2 -Wall -Wno-pointer-sign \
 *       -Iinclude -Iinclude/cnet -o templates/pfile_cnetc \
 *       templates/pfile_cnetc.c -Llib -lcnet_door
 *
 * Runtime requirements:
 *   cnetc.library (v4+) in LIBS:
 *   CNet 5 running with the door configured in BBSMENU
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <exec/types.h>
#include <cnet/cnet.h>

/*
 * CNCL_DoorInit() and CNCL_DoorCleanup() are provided by libcnet_door.a.
 * They manage cnetc.library open/close, context init/dispose, and the
 * global pointer setup.  The library also defines these globals:
 *
 *   struct Library      *CNetCBase;
 *   struct CNetCContext *context;
 *   struct PortData     *z;
 *   struct MainPort     *myp;
 *
 * Do NOT redeclare them here -- just use extern.
 */
extern struct PortData *z;
extern struct MainPort *myp;

extern BOOL CNCL_DoorInit(int argc, char **argv);
extern void CNCL_DoorCleanup(void);

/*
 * CNetC library wrapper function prototypes (from libcnet_door.a).
 * These call through to the CNC_* shared library functions.
 */
extern void  CPutText(char *text);
extern void  CPutA(void);
extern char  COneKey(void);
extern int   CEnterLine(UBYTE len, ULONG flags, char *prompt);
extern void  CEnterPassword(UBYTE len);
extern long  CCommonCommands(void);
extern UBYTE CReadFile(char *path, UBYTE flags);
extern void  CSetDoing(char *what);
extern void  CCallEditor(short max, short inlines);
extern UBYTE CReadGraphics(char *path, char flags);
extern UBYTE CReadAccount(short id, struct UserData *user);
extern UBYTE CSaveAccount(struct UserData *user, short id);
extern UBYTE CAddCharge(short n, short a);
extern UBYTE CCheckBalance(short n, short a);
extern int   CEnterText(char firstchar, short maxchars, short perline, short maxlines);
extern void  CCheckChanges(void);
extern long  CGetFree(char *s, UBYTE q);
extern void  CCheckFlowControl(void);
extern char  CWaitForInput(long mics);
extern UBYTE CPutQ(char *a);
extern void  CDoReturn(void);
extern UBYTE CCheckCarrier(void);
extern char  CCheckOLM(void);
extern void  CShutDown(char *spawn);
extern void  CallHost(UBYTE c);

/*
 * Stack size: libcnet_door.a already defines __stack = 65536.
 * If you need a larger stack, uncomment and increase this:
 *   unsigned long __stack = 131072;
 */


/*
 * DoorCleanup - release any resources YOUR code allocated.
 *
 * This is called from the atexit chain before CNCL_DoorCleanup()
 * tears down the CNet connection.  Close any libraries you opened,
 * free any memory you allocated, etc.
 *
 * Do NOT close cnetc.library here -- CNCL_DoorCleanup handles that.
 */
static void DoorCleanup(void)
{
    /* Your cleanup code here */
}


/*
 * DoorMain - your door logic goes here.
 *
 * At this point, CNCL_DoorInit() has succeeded and the globals
 * (z, myp) are valid.  Use C* wrapper functions to interact
 * with the user.
 */
static void DoorMain(int argc, char **argv)
{
    /* Example: greet the user */
    CPutText("\nHello from the CNetC door template!\n");

    /* Example: prompt for input */
    if (CEnterLine(40, 0, "Enter something: ")) {
        sprintf(z->ABuffer, "You entered: %s\n", z->InBuffer);
        CPutA();
    }

    /* Example: yes/no question */
    if (CPutQ("Continue? [Y/n] ")) {
        CPutText("You chose yes.\n");
    } else {
        CPutText("You chose no.\n");
    }
}


int main(int argc, char **argv)
{
    /* Register our cleanup before CNCL_DoorInit registers its own,
     * so ours runs first in the atexit chain (LIFO order). */
    atexit(DoorCleanup);

    /* Initialize the CNetC door connection.
     * Returns FALSE if not launched as a CNet door (argc < 2)
     * or if cnetc.library / CNC_InitContext() fails. */
    if (!CNCL_DoorInit(argc, argv)) {
        printf("This is a CNetC door -- run from CNet BBS.\n");
        return 0;
    }

    /* Run the door */
    DoorMain(argc, argv);

    /* exit() triggers atexit chain: DoorCleanup, then CNCL_DoorCleanup */
    return 0;
}
