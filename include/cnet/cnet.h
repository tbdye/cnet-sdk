/*
 * cnet.h - CNet BBS SDK master include
 *
 * Include this file as <cnet/cnet.h> with -Iinclude on the compiler
 * command line, or include individual headers as needed.
 */

#ifndef CNET_MASTER_H
#define CNET_MASTER_H

/*
 * AmigaOS system headers required by CNet data structures.
 * These provide struct definitions (SignalSemaphore, MsgPort, BitMap,
 * TmpRas, TextAttr, IBox, etc.) used by value in CNet structs.
 *
 * IMPORTANT: These must be included BEFORE the __asm redefinition
 * below, because the NDK's compiler-specific.h uses __asm as a
 * GCC built-in keyword in __REG__() macros.  Redefining __asm to
 * nothing before these headers would break function pointer
 * declarations in graphics/gfxnodes.h and graphics/monitor.h.
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

/*
 * GCC compatibility macros for SAS/C-specific qualifiers.
 * SAS/C uses __asm, __saveds, __regargs, and __stdargs as language
 * extensions. GCC does not recognize these; define them away or
 * map to GCC equivalents.
 *
 * These come AFTER the system includes so they only affect the
 * CNet SDK headers (which use bare __asm on SAS/C function decls).
 */
#ifdef __GNUC__
#ifndef __saveds
#define __saveds __attribute__((saveds))
#endif
#ifndef __asm
#define __asm
#endif
#ifndef __regargs
#define __regargs
#endif
#ifndef __stdargs
#define __stdargs
#endif
#endif /* __GNUC__ */

/* CNet data structure headers */
#include "bbslist.h"
#include "bbstext.h"   // 26-Mar-1998
#include "io.h"        //  3-Jun-1996
#include "subs.h"
#include "charges.h"
#include "config.h"
#include "control.h"
#include "dates.h"
#include "dcc.h"
#include "doors.h"
#include "editor.h"
#include "fido.h"
#include "files.h"
#include "ftp.h"
#include "getusers.h"
#include "internet.h"
#include "manager.h"
#include "mci.h"
#include "olms.h"
#include "other.h"
#include "ranges.h"
#include "rooms.h"
#include "users.h"
#include "vde.h"
#include "yank.h"
#include "ports.h"
#include "winscreen.h"
#include "telnet.h"
#include "news.h"                  		// 22-Feb-97
#include "convert.h"               		// 01-May-97
#include "sysfiles.h"              		// 02-May-97
#include "smtp.h"
#include "clipboard.h"
#include "menu.h"			           		// 03-Sep-97

/* Library proto/pragma headers - compiler-dependent */
#ifndef __GNUC__
/* SAS/C pragma includes */
#include "proto/cnetgui.h"
#include "pragma/cnetgui_pragmas.h"
#include "proto/cnetmail_protos.h"     // 08-Oct-97
#include "pragma/cnetmail_pragmas.h"   // 08-Oct-97
#include "proto/cnetlib_protos.h"  		// 11-Sep-97
#include "proto/cnetclib_protos.h" 		// 12-Sep-97
#include "proto/cnetclibrary_protos.h" // 12-Sep-97
#include "pragma/cnetclibrary_pragmas.h" 	// 12-Sep-97
#include "proto/cnet4_protos.h"
#include "pragma/cnet4_pragmas.h"
#include "proto/cnet_protos.h"
#include "pragma/cnet_pragmas.h"
#else
/* GCC inline stubs via proto headers */
#include <proto/cnet.h>
#include <proto/cnetc.h>
#include <proto/cnet4.h>
#include <proto/cnetmail.h>
#include <proto/cnetgui.h>
#endif

#endif /* CNET_MASTER_H */
