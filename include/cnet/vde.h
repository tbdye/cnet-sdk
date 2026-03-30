
#ifndef CNET_VDE
#define CNET_VDE

#include <cnet/align.h>

CNET_PACK_BEGIN

struct VDEentry {
   char  text[48];

   short xpos,      /* where to print these things */
         ypos;

   long  min,       /* for numerix */
         max;       /* for numerix */

   short length,    /* for string entry, field size */
                    /* for bit boolean, which bit? */

         type;      // see VDE_TYPE_* below

   long  offset;    /* position in structure */

   short screen0,   /* belongs-to screen */
         screen2;   /* goes-to screen, -1 for exit */

   short goup,      /* VDEentry for movement */
         godown,
         goleft,
         goright;

   UWORD ghost;
};

CNET_PACK_END

#define VDE_TYPE_NULL         -1
#define VDE_TYPE_UBYTE         0 // UBYTE 0/1
#define VDE_TYPE_ULONG_BIT     1 // ULONG BIT 0/1
#define VDE_TYPE_TEXT          2 // Text (char *)
#define VDE_TYPE_BYTE_NUM      3 // BYTE numeric
#define VDE_TYPE_SHORT_NUM     4 // short numeric
#define VDE_TYPE_USHORT_NUM    5 // USHORT numeric
#define VDE_TYPE_LONG_NUM      6 // long numeric
#define VDE_TYPE_ISDATE        7 // date
#define VDE_TYPE_FLAGS         8 // flags
#define VDE_TYPE_BBSTEXT_LIST  9 // ordered BBSTEXT list (UBYTE)
#define VDE_TYPE_BBSMENU_LIST 10 // ordered BBSMENU list (UBYTE)
#define VDE_TYPE_CHAR_PTRPTR  11 // char **
#define VDE_TYPE_UBYTE_BIT    12 // UBYTE BIT 0/1

#define VDE_TYPE_USE_PORTDATA    (1<<4) // if this bit set, offset will be relative to current port's PortData

#define TOSCREEN_EXIT -1

#define SCREEN_ZERO 0

// defines for easy VDE source legibility
#define TOSCREEN_ZERO 0    // commonly used screen2 value
#define OFFSET_NULL 0      // for unused offset value
#define FROMSCREEN_ZERO 0  // commonly used screen0 value
#define VDE_NULL 0         // other zero values
#define MIN_0 0            // for unused/zero MIN value
#define MAX_0 0            // for unused/zero MAX value

#endif
