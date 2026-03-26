#ifndef _INLINE_CNET_H
#define _INLINE_CNET_H

#ifndef CLIB_CNET_PROTOS_H
#define CLIB_CNET_PROTOS_H
#endif

#ifndef __INLINE_MACROS_H
#include <inline/macros.h>
#endif

#include <exec/types.h>

#ifndef CNET_BASE_NAME
#define CNET_BASE_NAME CNetBase
#endif

#define ReplaceText(line, from, to) \
	LP3(0x1e, short, ReplaceText, char *, line, a0, char *, from, a1, char *, to, a2, \
	, CNET_BASE_NAME)

#define GiveAmigaDays(date) \
	LP1(0x24, long, GiveAmigaDays, struct IsDate *, date, a0, \
	, CNET_BASE_NAME)

#define FormDate(date, parts) \
	LP2NR(0x2a, FormDate, struct IsDate *, date, a0, long *, parts, a1, \
	, CNET_BASE_NAME)

#define AlterDate(date, i) \
	LP2NR(0x30, AlterDate, struct IsDate *, date, a0, long, i, d0, \
	, CNET_BASE_NAME)

#define UltimateFindParts(buffer, min, maz, np, parts) \
	LP5NR(0x36, UltimateFindParts, char *, buffer, a0, long, min, d0, long, maz, d1, long *, np, a1, long *, parts, a2, \
	, CNET_BASE_NAME)

#define ExpandFlags(def, where) \
	LP2NR(0x3c, ExpandFlags, long, def, d0, char *, where, a0, \
	, CNET_BASE_NAME)

#define ConvertAccess(s) \
	LP1(0x42, long, ConvertAccess, char *, s, a0, \
	, CNET_BASE_NAME)

#define SaveFree(s) \
	LP1(0x48, long, SaveFree, struct SubboardType4 *, s, a0, \
	, CNET_BASE_NAME)

#define OneLessUser(r) \
	LP1NR(0x4e, OneLessUser, struct SubboardType4 *, r, a0, \
	, CNET_BASE_NAME)

#define OneMoreUser(r, lock) \
	LP2(0x54, UBYTE, OneMoreUser, struct SubboardType4 *, r, a0, UBYTE, lock, d0, \
	, CNET_BASE_NAME)

#define FreeText(s, pos, length) \
	LP3(0x5a, long, FreeText, struct SubboardType4 *, s, a0, long, pos, d0, long, length, d1, \
	, CNET_BASE_NAME)

#define AllocText(s, length) \
	LP2(0x60, long, AllocText, struct SubboardType4 *, s, a0, long, length, d0, \
	, CNET_BASE_NAME)

#define LockAccount(acc) \
	LP1(0x66, struct UserData *, LockAccount, short, acc, d0, \
	, CNET_BASE_NAME)

#define UnLockAccount(acc, save) \
	LP2NR(0x6c, UnLockAccount, short, acc, d0, UBYTE, save, d1, \
	, CNET_BASE_NAME)

#define FindPhone(n, phone, id) \
	LP3(0x72, long, FindPhone, short *, n, a0, char *, phone, a1, short, id, d0, \
	, CNET_BASE_NAME)

#define FindHandle(n, name, id) \
	LP3(0x78, long, FindHandle, short *, n, a0, char *, name, a1, short, id, d0, \
	, CNET_BASE_NAME)

#define FormatVanilla(in, out) \
	LP2NR(0x7e, FormatVanilla, BPTR, in, a0, BPTR, out, a1, \
	, CNET_BASE_NAME)

#define MakeDateZone(date, whereto, text, tz, form) \
	LP5NR(0x84, MakeDateZone, struct IsDate *, date, a0, char *, whereto, a1, char **, text, a2, BYTE, tz, d0, BYTE, form, d1, \
	, CNET_BASE_NAME)

#define ZPutItem(item, head, sub, n) \
	LP4NR(0x8a, ZPutItem, struct ItemType3 *, item, a0, struct ItemHeader *, head, a1, struct SubboardType4 *, sub, a2, short, n, d0, \
	, CNET_BASE_NAME)

#define ZGetItem(item, head, sub, n) \
	LP4NR(0x90, ZGetItem, struct ItemType3 *, item, a0, struct ItemHeader *, head, a1, struct SubboardType4 *, sub, a2, short, n, d0, \
	, CNET_BASE_NAME)

#define ZAddItem(item, head, sub) \
	LP3(0x96, UBYTE, ZAddItem, struct ItemType3 *, item, a0, struct ItemHeader *, head, a1, struct SubboardType4 *, sub, a2, \
	, CNET_BASE_NAME)

#define ZAddMessage(message, sub) \
	LP2NR(0x9c, ZAddMessage, struct MessageType3 *, message, a0, struct SubboardType4 *, sub, a1, \
	, CNET_BASE_NAME)

#define DelayAddItems(sub) \
	LP1NR(0xa2, DelayAddItems, struct SubboardType4 *, sub, a0, \
	, CNET_BASE_NAME)

#define FlushAddItems(sub) \
	LP1NR(0xa8, FlushAddItems, struct SubboardType4 *, sub, a0, \
	, CNET_BASE_NAME)

#define FileOLM(z, text, id, flags) \
	LP4(0xae, UBYTE, FileOLM, struct PortData *, z, a0, char *, text, a1, short, id, d0, long, flags, d1, \
	, CNET_BASE_NAME)

#define XAddCharge(set, id, n, a, info) \
	LP5(0xb4, UBYTE, XAddCharge, struct ChargeSet *, set, a0, short, id, d0, short, n, d1, short, a, d2, char *, info, a1, \
	, CNET_BASE_NAME)

#define AddCredits(item, m) \
	LP2NR(0xba, AddCredits, struct ItemType3 *, item, a0, short, m, d0, \
	, CNET_BASE_NAME)

#define WriteLog(z, n, text1, text2) \
	LP4NR(0xc0, WriteLog, struct PortData *, z, a0, short, n, d0, char *, text1, a1, char *, text2, a2, \
	, CNET_BASE_NAME)

#define DirectorySize(dir, lock) \
	LP2(0xc6, ULONG, DirectorySize, char *, dir, a0, BPTR, lock, a1, \
	, CNET_BASE_NAME)

#define FileExists(filename) \
	LP1(0xcc, short, FileExists, char *, filename, a0, \
	, CNET_BASE_NAME)

#define CreateMailDir(uucpid) \
	LP1NR(0xd2, CreateMailDir, char *, uucpid, a0, \
	, CNET_BASE_NAME)

#define CreateFolderName(destpath, UUCP, foldername) \
	LP3(0xd8, UBYTE, CreateFolderName, char *, destpath, a0, char *, UUCP, a1, char *, foldername, a2, \
	, CNET_BASE_NAME)

#define BuildDir(path) \
	LP1(0xde, short, BuildDir, char *, path, a0, \
	, CNET_BASE_NAME)

#define BitPosition(bitpattern) \
	LP1(0xe4, short, BitPosition, LONG, bitpattern, d0, \
	, CNET_BASE_NAME)

#define MCIFilter(text, length, flags) \
	LP3NR(0xea, MCIFilter, char *, text, a0, LONG, length, d0, short, flags, d1, \
	, CNET_BASE_NAME)

#define AccountToID(account) \
	LP1(0xf0, LONG, AccountToID, LONG, account, d0, \
	, CNET_BASE_NAME)

#define IDToAccount(userid) \
	LP1(0xf6, short, IDToAccount, LONG, userid, d0, \
	, CNET_BASE_NAME)

#define StrToUpper(string) \
	LP1NR(0xfc, StrToUpper, char *, string, a0, \
	, CNET_BASE_NAME)

#define FileSize(Name) \
	LP1(0x102, LONG, FileSize, char *, Name, a0, \
	, CNET_BASE_NAME)

#define AddIdentdUser(Port, Socket) \
	LP2(0x108, BYTE, AddIdentdUser, short, Port, d0, LONG, Socket, d1, \
	, CNET_BASE_NAME)

#define RemoveIdentdUser(Socket) \
	LP1(0x10e, BYTE, RemoveIdentdUser, ULONG, Socket, d0, \
	, CNET_BASE_NAME)

#define LookupIdentd(Socket) \
	LP1(0x114, short, LookupIdentd, ULONG, Socket, d0, \
	, CNET_BASE_NAME)

#define CNetAddressType(user) \
	LP1(0x11a, UBYTE, CNetAddressType, char *, user, a0, \
	, CNET_BASE_NAME)

#define CNetAddressToAccount(uucpid) \
	LP1(0x120, short, CNetAddressToAccount, char *, uucpid, a0, \
	, CNET_BASE_NAME)

#define HNameToUUCP(name) \
	LP1(0x126, char   *, HNameToUUCP, char *, name, a0, \
	, CNET_BASE_NAME)

#define CNetIDToUUCP(userid) \
	LP1(0x12c, char   *, CNetIDToUUCP, LONG, userid, d0, \
	, CNET_BASE_NAME)

#define NumFromUnique(gokeyword) \
	LP1(0x132, short, NumFromUnique, char *, gokeyword, a0, \
	, CNET_BASE_NAME)

#define FindOrdinal(list, n) \
	LP2(0x138, struct Node *, FindOrdinal, struct List *, list, a0, short, n, d0, \
	, CNET_BASE_NAME)

#define InStr(string, isin) \
	LP2(0x13e, short, InStr, char *, string, a0, char *, isin, a1, \
	, CNET_BASE_NAME)

#define AllDigits(string) \
	LP1(0x144, BOOL, AllDigits, char *, string, a0, \
	, CNET_BASE_NAME)

#define CopyFilterText(dest, source) \
	LP2NR(0x14a, CopyFilterText, char *, dest, a0, char *, source, a1, \
	, CNET_BASE_NAME)

#define IsNowOnLine(myp, account) \
	LP2(0x150, struct PortData *, IsNowOnLine, struct MainPort *, myp, a0, short, account, d0, \
	, CNET_BASE_NAME)

#define GetRFCDate(datedest) \
	LP1NR(0x156, GetRFCDate, char *, datedest, a0, \
	, CNET_BASE_NAME)

#define AppendToFile(filename, text, AddCR) \
	LP3(0x15c, BOOL, AppendToFile, char *, filename, a0, char *, text, a1, BOOL, AddCR, d0, \
	, CNET_BASE_NAME)

#define PItemToArgV(portnum, argv) \
	LP2NR(0x162, PItemToArgV, short, portnum, d0, char *, argv, a0, \
	, CNET_BASE_NAME)

#define CreateDirEntry(fib) \
	LP1(0x168, struct CNetFileEntry   *, CreateDirEntry, struct FileInfoBlock *, fib, a0, \
	, CNET_BASE_NAME)

#define InsertDirEntry(listhead, newentry) \
	LP2NR(0x16e, InsertDirEntry, struct CNetFileEntry **, listhead, a0, struct CNetFileEntry *, newentry, a1, \
	, CNET_BASE_NAME)

#define CNetDisposeDir(flist) \
	LP1NR(0x174, CNetDisposeDir, struct CNetFileEntry **, flist, a0, \
	, CNET_BASE_NAME)

#define CNetReadDir(path, recurse) \
	LP2(0x17a, struct CNetFileEntry   *, CNetReadDir, char *, path, a0, UBYTE, recurse, d0, \
	, CNET_BASE_NAME)

#define GetDirEntry(ordinal, list) \
	LP2(0x180, struct CNetFileEntry   *, GetDirEntry, LONG, ordinal, d0, struct CNetFileEntry *, list, a0, \
	, CNET_BASE_NAME)

#define CountDirEntries(listhead) \
	LP1(0x186, LONG, CountDirEntries, struct CNetFileEntry *, listhead, a0, \
	, CNET_BASE_NAME)

#define FindParent(list) \
	LP1(0x18c, struct CNetFileEntry   *, FindParent, struct CNetFileEntry *, list, a0, \
	, CNET_BASE_NAME)

#define NextDirEntry(list, nowentry) \
	LP2(0x192, struct CNetFileEntry   *, NextDirEntry, struct CNetFileEntry *, list, a0, struct CNetFileEntry *, nowentry, a1, \
	, CNET_BASE_NAME)

#define PrevDirEntry(list, nowentry) \
	LP2(0x198, struct CNetFileEntry   *, PrevDirEntry, struct CNetFileEntry *, list, a0, struct CNetFileEntry *, nowentry, a1, \
	, CNET_BASE_NAME)

#define HeaderEntry(anyentry) \
	LP1(0x19e, struct CNetFileEntry   *, HeaderEntry, struct CNetFileEntry *, anyentry, a0, \
	, CNET_BASE_NAME)

#define LastEntry(anyentry) \
	LP1(0x1a4, struct CNetFileEntry   *, LastEntry, struct CNetFileEntry *, anyentry, a0, \
	, CNET_BASE_NAME)

#define EntryOrdinal(entry, list) \
	LP2(0x1aa, LONG, EntryOrdinal, struct CNetFileEntry *, entry, a0, struct CNetFileEntry *, list, a1, \
	, CNET_BASE_NAME)

#define GetVolAssList() \
	LP0(0x1b0, struct CNetFileEntry   *, GetVolAssList, \
	, CNET_BASE_NAME)

#define CNetSearchEntry(list, destlist, pat, recurse) \
	LP4(0x1b6, struct CNetFileEntry   *, CNetSearchEntry, struct CNetFileEntry *, list, a0, struct CNetFileEntry **, destlist, a1, char *, pat, a2, UBYTE, recurse, d0, \
	, CNET_BASE_NAME)

#define MCIRemove(text) \
	LP1(0x1bc, char                   *, MCIRemove, char *, text, a0, \
	, CNET_BASE_NAME)

#define UCacheDispose() \
	LP0NR(0x1c2, UCacheDispose, \
	, CNET_BASE_NAME)

#define UCacheInitUser(account) \
	LP1(0x1c8, struct CNUserCache     *, UCacheInitUser, short, account, d0, \
	, CNET_BASE_NAME)

#define UCacheLookup(account) \
	LP1(0x1ce, struct CNUserCache     *, UCacheLookup, short, account, d0, \
	, CNET_BASE_NAME)

#define UCacheRead(account, userdata, numaccounts) \
	LP3(0x1d4, short, UCacheRead, short, account, d0, struct UserData *, userdata, a0, short, numaccounts, d1, \
	, CNET_BASE_NAME)

#define UCacheWrite(account, userdata, numaccounts) \
	LP3(0x1da, short, UCacheWrite, short, account, d0, struct UserData *, userdata, a0, short, numaccounts, d1, \
	, CNET_BASE_NAME)

#define StartupInitCache() \
	LP0NR(0x1e0, StartupInitCache, \
	, CNET_BASE_NAME)

#define ZGetItemPtr(subptr, itemnum) \
	LP2(0x1e6, struct ItemType3       *, ZGetItemPtr, struct SubboardType4 *, subptr, a0, short, itemnum, d0, \
	, CNET_BASE_NAME)

#define ZGetIHeadPtr(subptr, itemnum) \
	LP2(0x1ec, struct ItemHeader      *, ZGetIHeadPtr, struct SubboardType4 *, subptr, a0, short, itemnum, d0, \
	, CNET_BASE_NAME)

#define CNetStripChars(s, strip) \
	LP2NR(0x1f2, CNetStripChars, char *, s, a0, char *, strip, a1, \
	, CNET_BASE_NAME)

#endif /*  _INLINE_CNET_H  */
