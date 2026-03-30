
#ifndef CNET_EDITOR
#define CNET_EDITOR

#include <cnet/align.h>

CNET_PACK_BEGIN

struct DrawElement {
	UBYTE	x;				// x screen position
	UBYTE	y;				// y screen position
	UBYTE	att;			// the character's attribute (colors)
	UBYTE	att2;			// the character's attribute (others)
	UBYTE	c;				// the character
	UBYTE	att0;			// Was the attribute (underneath)
	UBYTE	att20;		// Was the attribute (underneath)
	UBYTE	c0;			// Was the character (underneath)
};

CNET_PACK_END

#endif
