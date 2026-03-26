#ifndef _PROTO_CNETGUI_H
#define _PROTO_CNETGUI_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#if !defined(CLIB_CNETGUI_PROTOS_H) && !defined(__GNUC__)
#include <clib/cnetgui_protos.h>
#endif

#ifndef __NOLIBBASE__
extern struct Library *CNetGuiBase;
#endif

#ifdef __GNUC__
#ifdef __AROS__
#include <defines/cnetgui.h>
#else
#include <inline/cnetgui.h>
#endif
#elif defined(__VBCC__)
#include <inline/cnetgui_protos.h>
#else
#include <pragma/cnetgui_lib.h>
#endif

#endif	/*  _PROTO_CNETGUI_H  */
