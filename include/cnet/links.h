/*
 * links.h - CNet inter-port linking structures
 *
 * This header was not included in the original SDK distribution.
 * Struct definitions here are reconstructed from usage in ports.h
 * where TermLink appears by value in struct PortData.
 *
 * WARNING: The TermLink struct size is a reconstruction.  Do not
 * rely on sizeof() matching the actual CNet binary layout.
 */

#ifndef CNET_LINKS_H
#define CNET_LINKS_H

/*
 * TermLink - terminal link state for inter-port communication
 * Used by value as tl in struct PortData (ports.h line 868).
 * Padded to 128 bytes as a safe upper bound.
 */
struct TermLink {
    short RemotePort;
    short LocalPort;
    UBYTE Active;
    UBYTE Mode;
    char RemoteSystem[40];
    char RemoteHandle[32];
    UBYTE expansion[52];
} __attribute__((packed));

/* Forward declaration for pointer-only type */
struct LinkPort;

#endif /* CNET_LINKS_H */
