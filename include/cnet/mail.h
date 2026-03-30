/*
 * mail.h - CNet mail system structures
 *
 * This header was not included in the original SDK distribution.
 * Struct definitions here are reconstructed from usage in ports.h
 * and control.h where these types appear by value (not just as
 * pointers).  Pointer-only types are forward-declared.
 *
 * WARNING: Field offsets in OldMailHeader and MailHeader4 are
 * approximate reconstructions.  Do not rely on sizeof() or
 * offsetof() matching the actual CNet binary layout.  Use the
 * CNet library API functions (GetMailHeader, UpdateMailHeader,
 * etc.) for all mail operations.
 */

#ifndef CNET_MAIL_H
#define CNET_MAIL_H

#include "dates.h"
#include <cnet/align.h>

/*
 * OldMailHeader - legacy mail header (pre-v4.12)
 * Used by value as OMail0_NOT in struct PortData.
 * Exact size unknown; padded to 256 bytes as a safe upper bound
 * based on typical BBS mail header sizes.
 */
CNET_PACK_BEGIN

struct OldMailHeader {
    char From[36];
    char To[36];
    char Subject[72];
    struct IsDate Date;
    long Seek;
    long Length;
    ULONG Flags;
    ULONG Number;
    short FromAccount;
    short ToAccount;
    UBYTE expansion[84];
};

/*
 * MailHeader4 - v4.12+ mail header
 * Used by value as OMail0 in struct PortData and
 * PortDataExtension (DefItem context).
 * Padded to 512 bytes as a safe upper bound.
 */
struct MailHeader4 {
    char From[40];
    char To[40];
    char Subject[80];
    struct IsDate Date;
    struct IsDate ReadDate;
    long Seek;
    long Length;
    ULONG Flags;
    ULONG Number;
    short FromAccount;
    short ToAccount;
    char FolderName[32];
    char UUCP_From[32];
    char UUCP_To[32];
    UBYTE expansion[240];
};

/*
 * NewMailFolder - mail folder structure
 * Used only as pointer in PortData; forward declaration suffices
 * for compilation, but we provide a minimal struct for completeness.
 */
struct NewMailFolder {
    char FolderName[64];
    long NumMessages;
    UBYTE temporary;
    UBYTE expansion[63];
    struct NewMailFolder *next;
};

CNET_PACK_END

/* Forward declarations for pointer-only types */
struct MailAlias;
struct MailLock;
struct MailToList;
struct MIMEContentType;

#endif /* CNET_MAIL_H */
