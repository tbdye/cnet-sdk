#ifndef _INLINE_CNETGUI_H
#define _INLINE_CNETGUI_H

#ifndef CLIB_CNETGUI_PROTOS_H
#define CLIB_CNETGUI_PROTOS_H
#endif

#ifndef __INLINE_MACROS_H
#include <inline/macros.h>
#endif

#include <exec/types.h>

#ifndef CNETGUI_BASE_NAME
#define CNETGUI_BASE_NAME CNetGuiBase
#endif

#define CNGTextWidth(str, Attr) \
	LP2(0x1e, UWORD, CNGTextWidth, UBYTE *, str, a0, struct TextAttr *, Attr, a1, \
	, CNETGUI_BASE_NAME)

#define InitWindowStruct() \
	LP0(0x24, APTR, InitWindowStruct, \
	, CNETGUI_BASE_NAME)

#endif /*  _INLINE_CNETGUI_H  */
