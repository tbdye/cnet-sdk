#ifndef _PROTO_CNET_H
#define _PROTO_CNET_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#if !defined(CLIB_CNET_PROTOS_H) && !defined(__GNUC__)
#include <clib/cnet_protos.h>
#endif

#ifndef __NOLIBBASE__
extern struct Library *CNetBase;
#endif

#ifdef __GNUC__
#ifdef __AROS__
#include <defines/cnet.h>
#else
#include <inline/cnet.h>
#endif
#elif defined(__VBCC__)
#include <inline/cnet_protos.h>
#else
#include <pragma/cnet_lib.h>
#endif

#endif	/*  _PROTO_CNET_H  */
