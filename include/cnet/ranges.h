
#ifndef CNET_RANGES
#define CNET_RANGES

#include <cnet/align.h>

CNET_PACK_BEGIN

struct RangeContext {
	long	nparts;			// number of sets in range
	long	parts[10][2];	// parts[x][0] = starting element of set x
								// parts[x][0] = final element in set x
	long	set;				// current set
	long	element;			// current value of element in set
};

CNET_PACK_END

#endif
