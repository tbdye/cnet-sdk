#ifndef _INLINE_CNETC_H
#define _INLINE_CNETC_H

#ifndef CLIB_CNETC_PROTOS_H
#define CLIB_CNETC_PROTOS_H
#endif

#ifndef __INLINE_MACROS_H
#include <inline/macros.h>
#endif

#include <exec/types.h>

#ifndef CNETC_BASE_NAME
#define CNETC_BASE_NAME CNetCBase
#endif

#define CNC_InitContext(cport_a1) \
	LP1(0x24, struct CNetCContext *, CNC_InitContext, char *, cport_a1, a1, \
	, CNETC_BASE_NAME)

#define CNC_DisposeContext() \
	LP0NR(0x2a, CNC_DisposeContext, \
	, CNETC_BASE_NAME)

#define CNC_CallHost(c) \
	LP1NR(0x30, CNC_CallHost, UBYTE, c, d1, \
	, CNETC_BASE_NAME)

#define CNC_PutText(text) \
	LP1NR(0x36, CNC_PutText, char *, text, a1, \
	, CNETC_BASE_NAME)

#define CNC_PutA() \
	LP0NR(0x3c, CNC_PutA, \
	, CNETC_BASE_NAME)

#define CNC_ShutDown(spawn) \
	LP1NR(0x42, CNC_ShutDown, char *, spawn, a1, \
	, CNETC_BASE_NAME)

#define CNC_EnterLine(len, flags, prompt) \
	LP3(0x48, int, CNC_EnterLine, UBYTE, len, d1, ULONG, flags, d2, char *, prompt, a1, \
	, CNETC_BASE_NAME)

#define CNC_OneKey() \
	LP0(0x4e, char, CNC_OneKey, \
	, CNETC_BASE_NAME)

#define CNC_EnterPassword(len) \
	LP1NR(0x54, CNC_EnterPassword, UBYTE, len, d1, \
	, CNETC_BASE_NAME)

#define CNC_CommonCommands() \
	LP0(0x5a, long, CNC_CommonCommands, \
	, CNETC_BASE_NAME)

#define CNC_ReadFile(path, flags) \
	LP2(0x60, UBYTE, CNC_ReadFile, char *, path, a1, UBYTE, flags, d1, \
	, CNETC_BASE_NAME)

#define CNC_SetDoing(what) \
	LP1NR(0x66, CNC_SetDoing, char *, what, a1, \
	, CNETC_BASE_NAME)

#define CNC_CallEditor(max, inlines) \
	LP2NR(0x6c, CNC_CallEditor, short, max, d1, short, inlines, d2, \
	, CNETC_BASE_NAME)

#define CNC_ReadGraphics(path, flags) \
	LP2(0x72, UBYTE, CNC_ReadGraphics, char *, path, a1, char, flags, d1, \
	, CNETC_BASE_NAME)

#define CNC_MakeDate(date, output) \
	LP2NR(0x78, CNC_MakeDate, struct IsDate *, date, a1, char *, output, a2, \
	, CNETC_BASE_NAME)

#define CNC_ReadAccount(id, udata) \
	LP2(0x7e, UBYTE, CNC_ReadAccount, short, id, d1, struct UserData *, udata, a1, \
	, CNETC_BASE_NAME)

#define CNC_SaveAccount(udata, id) \
	LP2(0x84, UBYTE, CNC_SaveAccount, struct UserData *, udata, a1, short, id, d1, \
	, CNETC_BASE_NAME)

#define CNC_AddCharge(n, a) \
	LP2(0x8a, UBYTE, CNC_AddCharge, short, n, d1, short, a, d2, \
	, CNETC_BASE_NAME)

#define CNC_CheckBalance(n, a) \
	LP2(0x90, UBYTE, CNC_CheckBalance, short, n, d1, short, a, d2, \
	, CNETC_BASE_NAME)

#define CNC_EnterText(firstchar, maxchars, perline, maxlines) \
	LP4(0x96, int, CNC_EnterText, char, firstchar, d1, short, maxchars, d2, short, perline, d3, short, maxlines, d4, \
	, CNETC_BASE_NAME)

#define CNC_ConferenceWait(a) \
	LP1(0x9c, long, CNC_ConferenceWait, short, a, d1, \
	, CNETC_BASE_NAME)

#define CNC_CheckChanges() \
	LP0NR(0xa2, CNC_CheckChanges, \
	, CNETC_BASE_NAME)

#define CNC_ConvertAccess(s) \
	LP1(0xa8, long, CNC_ConvertAccess, char *, s, a1, \
	, CNETC_BASE_NAME)

#define CNC_GetFree(name, quiet, deststring, bm) \
	LP4(0xae, long, CNC_GetFree, char *, name, a1, UBYTE, quiet, d1, char *, deststring, a2, char **, bm, a3, \
	, CNETC_BASE_NAME)

#define CNC_FindAccount(a, b, c) \
	LP3(0xb4, short, CNC_FindAccount, char *, a, a1, struct UserData *, b, a2, UBYTE, c, d1, \
	, CNETC_BASE_NAME)

#define CNC_CheckFlowControl() \
	LP0NR(0xba, CNC_CheckFlowControl, \
	, CNETC_BASE_NAME)

#define CNC_ListDir(a, b, c) \
	LP3(0xc0, long, CNC_ListDir, UBYTE, a, d1, UBYTE, b, d2, struct IsDate *, c, a1, \
	, CNETC_BASE_NAME)

#define CNC_Rnext() \
	LP0(0xc6, UBYTE, CNC_Rnext, \
	, CNETC_BASE_NAME)

#define CNC_ParseCommandLine(numargs) \
	LP1NR(0xcc, CNC_ParseCommandLine, UBYTE, numargs, d1, \
	, CNETC_BASE_NAME)

#define CNC_FindCommand(num) \
	LP1(0xd2, short, CNC_FindCommand, short, num, d1, \
	, CNETC_BASE_NAME)

#define CNC_ReadMessagePoint(a, b) \
	LP2NR(0xd8, CNC_ReadMessagePoint, char *, a, a1, long, b, d1, \
	, CNETC_BASE_NAME)

#define CNC_EditMessage(file) \
	LP1NR(0xde, CNC_EditMessage, char *, file, a1, \
	, CNETC_BASE_NAME)

#define CNC_LoadText(fh) \
	LP1NR(0xe4, CNC_LoadText, BPTR, fh, a1, \
	, CNETC_BASE_NAME)

#define CNC_WaitForInput(mics) \
	LP1(0xea, char, CNC_WaitForInput, long, mics, d1, \
	, CNETC_BASE_NAME)

#define CNC_SelectAndDownload(file, flags) \
	LP2(0xf0, UBYTE, CNC_SelectAndDownload, char *, file, a1, UBYTE, flags, d1, \
	, CNETC_BASE_NAME)

#define CNC_VisualDataEditor(file, data, size) \
	LP3(0xf6, short, CNC_VisualDataEditor, char *, file, a1, void *, data, a2, long, size, d1, \
	, CNETC_BASE_NAME)

#define CNC_ExtSetMinFree(free) \
	LP1NR(0xfc, CNC_ExtSetMinFree, long, free, d1, \
	, CNETC_BASE_NAME)

#define CNC_ExtSetProtocol(a) \
	LP1(0x102, UBYTE, CNC_ExtSetProtocol, char, a, d1, \
	, CNETC_BASE_NAME)

#define CNC_ExtDownload(args) \
	LP1(0x108, char       *, CNC_ExtDownload, char *, args, a1, \
	, CNETC_BASE_NAME)

#ifndef NO_INLINE_STDARG
#define CNC_ExtDownloadArgs(tags...) \
	({ULONG _tags[] = {tags}; CNC_ExtDownload((char *) _tags);})
#endif

#define CNC_ExtUpload(args) \
	LP1(0x10e, char       *, CNC_ExtUpload, char *, args, a1, \
	, CNETC_BASE_NAME)

#ifndef NO_INLINE_STDARG
#define CNC_ExtUploadArgs(tags...) \
	({ULONG _tags[] = {tags}; CNC_ExtUpload((char *) _tags);})
#endif

#define CNC_PutQ(a) \
	LP1(0x114, UBYTE, CNC_PutQ, char *, a, a1, \
	, CNETC_BASE_NAME)

#define CNC_DoReturn() \
	LP0NR(0x11a, CNC_DoReturn, \
	, CNETC_BASE_NAME)

#define CNC_WriteUKeys(getsem) \
	LP1(0x120, BYTE, CNC_WriteUKeys, BYTE, getsem, d1, \
	, CNETC_BASE_NAME)

#define CNC_DoANSI(n, a, b) \
	LP3NR(0x126, CNC_DoANSI, UBYTE, n, d1, USHORT, a, d2, USHORT, b, d3, \
	, CNETC_BASE_NAME)

#define CNC_DoANSIOut(n) \
	LP1NR(0x12c, CNC_DoANSIOut, UBYTE, n, d1, \
	, CNETC_BASE_NAME)

#define CNC_InputSignalInit(portname, timeout) \
	LP2(0x132, BYTE, CNC_InputSignalInit, char *, portname, a1, ULONG, timeout, d1, \
	, CNETC_BASE_NAME)

#define CNC_InputSignalDispose() \
	LP0NR(0x138, CNC_InputSignalDispose, \
	, CNETC_BASE_NAME)

#define CNC_OWriteLog(n, text1, text2) \
	LP3NR(0x13e, CNC_OWriteLog, short, n, d1, char *, text1, a1, char *, text2, a2, \
	, CNETC_BASE_NAME)

#define CNC_VDEapply(data0, data1) \
	LP2NR(0x144, CNC_VDEapply, void *, data0, a1, void *, data1, a2, \
	, CNETC_BASE_NAME)

#define CNC_VDEclose() \
	LP0NR(0x14a, CNC_VDEclose, \
	, CNETC_BASE_NAME)

#define CNC_EnterAccess(prompt, def) \
	LP2(0x150, long, CNC_EnterAccess, char *, prompt, a1, long, def, d1, \
	, CNETC_BASE_NAME)

#define CNC_CheckCarrier() \
	LP0(0x156, UBYTE, CNC_CheckCarrier, \
	, CNETC_BASE_NAME)

#define CNC_CheckOLM() \
	LP0(0x15c, char, CNC_CheckOLM, \
	, CNETC_BASE_NAME)

#define CNC_JMakeDate(date, output) \
	LP2NR(0x162, CNC_JMakeDate, struct IsDate *, date, a1, char *, output, a2, \
	, CNETC_BASE_NAME)

#define CNC_GrabFileSize(quiet) \
	LP1(0x168, long, CNC_GrabFileSize, UBYTE, quiet, d1, \
	, CNETC_BASE_NAME)

#define CNC_SaveUser(user, id) \
	LP2NR(0x16e, CNC_SaveUser, struct UserData *, user, a1, short, id, d1, \
	, CNETC_BASE_NAME)

#define CNC_PutQMult(prompt, poss) \
	LP2(0x174, UBYTE, CNC_PutQMult, char *, prompt, a1, char *, poss, a2, \
	, CNETC_BASE_NAME)

#define CNC_MailSend(account) \
	LP1NR(0x17a, CNC_MailSend, short, account, d1, \
	, CNETC_BASE_NAME)

#define CNC_ReadLog(logname) \
	LP1NR(0x180, CNC_ReadLog, char *, logname, a1, \
	, CNETC_BASE_NAME)

#define CNC_OpenDisplay(portnum) \
	LP1NR(0x186, CNC_OpenDisplay, short, portnum, d1, \
	, CNETC_BASE_NAME)

#define CNC_GetSetFileNote(pathfile) \
	LP1NR(0x18c, CNC_GetSetFileNote, char *, pathfile, a1, \
	, CNETC_BASE_NAME)

#define CNC_ExtractFile(pathfile) \
	LP1(0x192, UBYTE, CNC_ExtractFile, char *, pathfile, a1, \
	, CNETC_BASE_NAME)

#define CNC_DLoadSelect(optdel) \
	LP1NR(0x198, CNC_DLoadSelect, UBYTE, optdel, d1, \
	, CNETC_BASE_NAME)

#define CNC_FindForeignArea(gokey) \
	LP1(0x19e, short, CNC_FindForeignArea, char *, gokey, a1, \
	, CNETC_BASE_NAME)

#define CNC_OutScan() \
	LP0NR(0x1a4, CNC_OutScan, \
	, CNETC_BASE_NAME)

#endif /*  _INLINE_CNETC_H  */
