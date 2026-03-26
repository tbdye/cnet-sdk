#ifndef _PROTO_CNET4_H
#define _PROTO_CNET4_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#if !defined(CLIB_CNET4_PROTOS_H) && !defined(__GNUC__)
#include <clib/cnet4_protos.h>
#endif

#ifndef __NOLIBBASE__
extern struct Library *CNet4Base;
#endif

#ifdef __GNUC__
#ifdef __AROS__
#include <defines/cnet4.h>
#else
#include <inline/cnet4.h>
#endif
#elif defined(__VBCC__)
#include <inline/cnet4_protos.h>
#else
#include <pragma/cnet4_lib.h>
#endif

#endif	/*  _PROTO_CNET4_H  */
