/*
 * CheckFlags.c - check if user has all required flags.
 *
 * Originally a CommonFuncs code snippet in the CNet SDK.
 * Converted to a proper compilable unit for m68k-amigaos-gcc.
 *
 * Parameters:
 *   MandFlags - bitmask of required flags (user must have ALL of these)
 *   Flags     - bitmask of flags the user actually has
 *
 * Returns TRUE if the user has all the mandatory flags.
 */

#include <exec/types.h>

BOOL CheckFlags(LONG MandFlags, LONG Flags)
{
    /* MandFlags - user must have ALL these flags! */
    /* Flags - Flags user has */

    if ((MandFlags & Flags) == MandFlags)
        return TRUE;
    return FALSE;
}
