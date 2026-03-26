#ifndef _INLINE_CNETMAIL_H
#define _INLINE_CNETMAIL_H

#ifndef CLIB_CNETMAIL_PROTOS_H
#define CLIB_CNETMAIL_PROTOS_H
#endif

#ifndef __INLINE_MACROS_H
#include <inline/macros.h>
#endif

#include <exec/types.h>

#ifndef CNETMAIL_BASE_NAME
#define CNETMAIL_BASE_NAME CNetMailBase
#endif

#define MIMEParseBoundary(contentline, boundary) \
	LP2(0x1e, UBYTE, MIMEParseBoundary, char *, contentline, a0, char *, boundary, a1, \
	, CNETMAIL_BASE_NAME)

#define MIMEIsBoundary(line, boundary) \
	LP2(0x24, UBYTE, MIMEIsBoundary, char *, line, a0, char *, boundary, a1, \
	, CNETMAIL_BASE_NAME)

#define MIMEParseContentType(input, contentres) \
	LP2(0x2a, UBYTE, MIMEParseContentType, char *, input, a0, char *, contentres, a1, \
	, CNETMAIL_BASE_NAME)

#define MIMEFindType(typestring, encodingstring) \
	LP2(0x30, struct MIMEContentType *, MIMEFindType, char *, typestring, a0, char *, encodingstring, a1, \
	, CNETMAIL_BASE_NAME)

#define MIMEParseName(line, name, path) \
	LP3(0x36, UBYTE, MIMEParseName, char *, line, a0, char *, name, a1, char *, path, a2, \
	, CNETMAIL_BASE_NAME)

#define MIMEFromExt(ext) \
	LP1(0x3c, struct MIMEContentType *, MIMEFromExt, char *, ext, a0, \
	, CNETMAIL_BASE_NAME)

#define DisposeMailTo(mailtolist) \
	LP1NR(0x42, DisposeMailTo, struct MailToList **, mailtolist, a0, \
	, CNETMAIL_BASE_NAME)

#define NameToList(name, uucp, mailtolist, mailtype) \
	LP4(0x48, struct MailToList *, NameToList, char *, name, a0, char *, uucp, a1, struct MailToList *, mailtolist, a2, UBYTE, mailtype, d0, \
	, CNETMAIL_BASE_NAME)

#define IsUUCP(net, mailtolist) \
	LP2(0x4e, struct MailToList *, IsUUCP, char *, net, a0, struct MailToList *, mailtolist, a1, \
	, CNETMAIL_BASE_NAME)

#define IsFido(net, faddr, nd, mailtolist) \
	LP4(0x54, struct MailToList *, IsFido, char *, net, a0, USHORT *, faddr, a1, struct CNetNodeDesc *, nd, a2, struct MailToList *, mailtolist, a3, \
	, CNETMAIL_BASE_NAME)

#define IsLocal(name, mailtolist) \
	LP2(0x5a, struct MailToList *, IsLocal, char *, name, a0, struct MailToList *, mailtolist, a1, \
	, CNETMAIL_BASE_NAME)

#define SendMail(from, to, subject, text, smflags) \
	LP5(0x60, UBYTE, SendMail, char *, from, a0, char *, to, a1, char *, subject, a2, char *, text, a3, LONG, smflags, d0, \
	, CNETMAIL_BASE_NAME)

#define GetMailSems() \
	LP0(0x66, struct SignalSemaphore *, GetMailSems, \
	, CNETMAIL_BASE_NAME)

#define GetMPSems() \
	LP0(0x6c, struct SignalSemaphore *, GetMPSems, \
	, CNETMAIL_BASE_NAME)

#endif /*  _INLINE_CNETMAIL_H  */
