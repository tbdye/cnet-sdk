#ifndef _PROTO_CNETC_H
#define _PROTO_CNETC_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#if !defined(CLIB_CNETC_PROTOS_H) && !defined(__GNUC__)
#include <clib/cnetc_protos.h>
#endif

#ifndef __NOLIBBASE__
extern struct Library *CNetCBase;
#endif

#ifdef __GNUC__
#ifdef __AROS__
#include <defines/cnetc.h>
#else
#include <inline/cnetc.h>
#endif
#elif defined(__VBCC__)
#include <inline/cnetc_protos.h>
#else
#include <pragma/cnetc_lib.h>
#endif

#endif	/*  _PROTO_CNETC_H  */
