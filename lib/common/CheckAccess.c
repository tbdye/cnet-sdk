/*
 * CheckAccess.c - check user access level against allowed access mask.
 *
 * Originally a CommonFuncs code snippet in the CNet SDK.
 * Converted to a proper compilable unit for m68k-amigaos-gcc.
 *
 * Parameters:
 *   MandAccess - bitmask of allowed access levels
 *   Access     - the user's current access level (0-31)
 *
 * Returns TRUE if the user's access level is set in MandAccess.
 */

#include <exec/types.h>

BOOL CheckAccess(LONG MandAccess, BYTE Access)
{
    /* MandAccess is the accesses allowed, Access is the user's access */

    if (MandAccess & (1L << Access))
        return TRUE;
    return FALSE;
}
