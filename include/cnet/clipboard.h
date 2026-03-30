#ifndef CNET_CLIPBOARD_H
#define CNET_CLIPBOARD_H

#include <cnet/align.h>

CNET_PACK_BEGIN

struct cbbuf
   {
   ULONG size;     /* size of memory allocation            */
   ULONG count;    /* number of characters after stripping */
   UBYTE *mem;     /* pointer to memory containing data    */
   };

CNET_PACK_END

#define ID_FTXT MAKE_ID('F','T','X','T')
#define ID_CHRS MAKE_ID('C','H','R','S')

#endif
