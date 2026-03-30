#ifndef CNET_ALIGN_H
#define CNET_ALIGN_H

/*
 * CNet SDK struct alignment compatibility.
 *
 * CNet was compiled with SAS/C v6.2, which uses 2-byte (word) alignment --
 * the natural m68k alignment. On m68k-amigaos-gcc, the default alignment
 * is already 2-byte, so no pragma is needed. On other architectures
 * (x86, ARM, etc.), we force 2-byte maximum alignment to match.
 */

#if defined(__mc68000__)
  #define CNET_PACK_BEGIN
  #define CNET_PACK_END
#else
  #define CNET_PACK_BEGIN _Pragma("pack(push, 2)")
  #define CNET_PACK_END   _Pragma("pack(pop)")
#endif

#endif /* CNET_ALIGN_H */
