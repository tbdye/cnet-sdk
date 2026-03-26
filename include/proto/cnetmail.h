#ifndef _PROTO_CNETMAIL_H
#define _PROTO_CNETMAIL_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#if !defined(CLIB_CNETMAIL_PROTOS_H) && !defined(__GNUC__)
#include <clib/cnetmail_protos.h>
#endif

#ifndef __NOLIBBASE__
extern struct Library *CNetMailBase;
#endif

#ifdef __GNUC__
#ifdef __AROS__
#include <defines/cnetmail.h>
#else
#include <inline/cnetmail.h>
#endif
#elif defined(__VBCC__)
#include <inline/cnetmail_protos.h>
#else
#include <pragma/cnetmail_lib.h>
#endif

#endif	/*  _PROTO_CNETMAIL_H  */
