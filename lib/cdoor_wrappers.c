/*
 * cdoor_wrappers.c - cnetcdoor.lib wrapper function reimplementation
 *
 * These are the C-wrapper convenience functions from cnetcdoor.lib.
 * Each wraps a CNC_* shared library function from cnetc.library,
 * providing the simpler API that most door code uses.
 *
 * Prototypes: reference/cnetclib_protos.h
 * CNC_* stubs: include/inline/cnetc.h (via include/proto/cnetc.h)
 *
 * Two functions are NOT simple CNC_* wrappers:
 *   CallHost()   - delegates to CNC_CallHost()
 *   CLoadError() - cleanup-and-exit error handler
 *
 * Compile: m68k-amigaos-gcc -noixemul -m68020 -Iinclude ...
 */

#include <stdlib.h>

#include <exec/types.h>
#include <exec/libraries.h>
#include <dos/dos.h>

/*
 * Include the cnetc.library inline stubs BEFORE any CNet SDK headers
 * that redefine __asm.  The LP* macros in inline/macros.h require
 * __asm as a GCC keyword for register binding; <cnet/cnet.h> redefines
 * __asm to nothing (for SAS/C compatibility) which would break them.
 */
#include <proto/cnetc.h>

/*
 * Forward declarations for CNet struct types used only as pointer
 * parameters in the wrapper functions.  We avoid including
 * <cnet/cnet.h> because its __asm redefinition would break the
 * inline stubs already included above.
 */
struct IsDate;
struct UserData;
struct CNetCContext;
struct PortData;
struct MainPort;

/* Globals from door_init.c -- library base and context pointers */
extern struct Library      *CNetCBase;
extern struct CNetCContext *context;
extern struct PortData     *z;
extern struct MainPort     *myp;

/* ------------------------------------------------------------------ */
/*  Simple CNC_* wrapper functions                                     */
/* ------------------------------------------------------------------ */

void CPutText(char *text)
{
    CNC_PutText(text);
}

void CPutA(void)
{
    CNC_PutA();
}

char COneKey(void)
{
    return CNC_OneKey();
}

int CEnterLine(UBYTE len, ULONG flags, char *prompt)
{
    return CNC_EnterLine(len, flags, prompt);
}

void CEnterPassword(UBYTE len)
{
    CNC_EnterPassword(len);
}

long CCommonCommands(void)
{
    return CNC_CommonCommands();
}

UBYTE CReadFile(char *path, UBYTE flags)
{
    return CNC_ReadFile(path, flags);
}

void CSetDoing(char *what)
{
    CNC_SetDoing(what);
}

void CCallEditor(short max, short inlines)
{
    CNC_CallEditor(max, inlines);
}

UBYTE CReadGraphics(char *path, char flags)
{
    return CNC_ReadGraphics(path, flags);
}

void CMakeDate(struct IsDate *date, char *output)
{
    CNC_MakeDate(date, output);
}

UBYTE CReadAccount(short id, struct UserData *user)
{
    return CNC_ReadAccount(id, user);
}

UBYTE CSaveAccount(struct UserData *user, short id)
{
    return CNC_SaveAccount(user, id);
}

UBYTE CAddCharge(short n, short a)
{
    return CNC_AddCharge(n, a);
}

UBYTE CCheckBalance(short n, short a)
{
    return CNC_CheckBalance(n, a);
}

int CEnterText(char firstchar, short maxchars, short perline, short maxlines)
{
    return CNC_EnterText(firstchar, maxchars, perline, maxlines);
}

long CConferenceWait(short a)
{
    return CNC_ConferenceWait(a);
}

void CCheckChanges(void)
{
    CNC_CheckChanges();
}

long CConvertAccess(char *s)
{
    return CNC_ConvertAccess(s);
}

/*
 * CGetFree - wrapper with simplified signature.
 *
 * The CNC_* version takes 4 args: (name, quiet, deststring, bm).
 * The cnetcdoor.lib wrapper takes only 2: (name, quiet).
 * The extra args (deststring, bm) are passed as NULL since the
 * simplified wrapper doesn't expose them to the caller.
 */
long CGetFree(char *s, UBYTE q)
{
    return CNC_GetFree(s, q, NULL, NULL);
}

short CFindAccount(char *a, struct UserData *b, UBYTE quiet)
{
    return CNC_FindAccount(a, b, quiet);
}

void CCheckFlowControl(void)
{
    CNC_CheckFlowControl();
}

long CListDir(UBYTE a, UBYTE b, struct IsDate *c)
{
    return CNC_ListDir(a, b, c);
}

UBYTE CRnext(void)
{
    return CNC_Rnext();
}

void CParseCommandLine(UBYTE numargs)
{
    CNC_ParseCommandLine(numargs);
}

short CFindCommand(short num)
{
    return CNC_FindCommand(num);
}

void CReadMessagePoint(char *a, long b)
{
    CNC_ReadMessagePoint(a, b);
}

void CEditMessage(char *file)
{
    CNC_EditMessage(file);
}

void CLoadText(BPTR fh)
{
    CNC_LoadText(fh);
}

char CWaitForInput(long mics)
{
    return CNC_WaitForInput(mics);
}

UBYTE CSelectAndDownload(char *file, UBYTE flags)
{
    return CNC_SelectAndDownload(file, flags);
}

short CVisualDataEditor(char *file, void *data, long size)
{
    return CNC_VisualDataEditor(file, data, size);
}

void CExtSetMinFree(long free)
{
    CNC_ExtSetMinFree(free);
}

UBYTE CExtSetProtocol(char a)
{
    return CNC_ExtSetProtocol(a);
}

char *CExtDownload(char *args)
{
    return CNC_ExtDownload(args);
}

char *CExtUpload(char *args)
{
    return CNC_ExtUpload(args);
}

UBYTE CPutQ(char *a)
{
    return CNC_PutQ(a);
}

void CDoReturn(void)
{
    CNC_DoReturn();
}

BYTE CWriteUKeys(BYTE getsem)
{
    return CNC_WriteUKeys(getsem);
}

void CDoANSI(UBYTE n, USHORT a, USHORT b)
{
    CNC_DoANSI(n, a, b);
}

void CDoANSIOut(UBYTE n)
{
    CNC_DoANSIOut(n);
}

char CCheckOLM(void)
{
    return CNC_CheckOLM();
}

long CGrabFileSize(UBYTE quiet)
{
    return CNC_GrabFileSize(quiet);
}

void CSaveUser(struct UserData *user, short id)
{
    CNC_SaveUser(user, id);
}

UBYTE CPutQMult(char *prompt, char *poss)
{
    return CNC_PutQMult(prompt, poss);
}

/*
 * PutQMult - cnet.lib wrapper for CNC_PutQMult().
 *
 * Presents a prompt and accepts one of the characters in 'possibilities'.
 * Returns the 1-based index of the selected character.
 */
UBYTE PutQMult(char *prompt, char *possibilities)
{
    return CNC_PutQMult(prompt, possibilities);
}

UBYTE CCheckCarrier(void)
{
    return CNC_CheckCarrier();
}

void COWriteLog(short n, char *text1, char *text2)
{
    CNC_OWriteLog(n, text1, text2);
}

void CVDEapply(void *data0, void *data1)
{
    CNC_VDEapply(data0, data1);
}

void CVDEclose(void)
{
    CNC_VDEclose();
}

long CEnterAccess(char *prompt, long def)
{
    return CNC_EnterAccess(prompt, def);
}

void CShutDown(char *spawn)
{
    CNC_ShutDown(spawn);
}

/*
 * CMakeEd() is NOT a CNC_* shared library wrapper -- it's a linker
 * library function that constructs the editor temp file path from
 * myp->gc.ZIPpath and z->InPort.  It will be implemented in
 * cdoor_extra.c along with the other non-shared-library functions
 * (CNC_MakeEd, CNC_DeleteEd, CNC_OpenEd, CNC_PrepEditor,
 * CNC_SaveEditor, CNC_compstra).
 */

/* ------------------------------------------------------------------ */
/*  Non-wrapper functions                                              */
/* ------------------------------------------------------------------ */

/*
 * CallHost - send a command to the BBS process.
 *
 * This is the v3-style raw IPC function.  Since cnetc.library provides
 * CNC_CallHost() which handles the CMessage/PutMsg/WaitPort cycle
 * internally, we simply delegate to it.
 */
void CallHost(UBYTE c)
{
    CNC_CallHost(c);
}

/*
 * CLoadError - error handler for door load failures.
 *
 * In the original SAS/C implementation, this drained pending messages
 * from the reply port and exited.  Since CNCL_DoorInit() registers
 * CNCL_DoorCleanup() as an atexit() handler, we can simply call
 * exit() with RETURN_FAIL (20) and let the cleanup handler do the
 * teardown.
 */
void CLoadError(void)
{
    exit(RETURN_FAIL);
}
