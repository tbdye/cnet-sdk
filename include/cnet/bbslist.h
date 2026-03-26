#ifndef CNET_BBSLIST
#define CNET_BBSLIST

#include "dates.h"

struct   BBSItem {
   long   ID;            // serial ID of user who added entry
   char   Phone[16];
   char   Title[26];
   char   Location[21];
   char   Baud[6];
   char   Comments[62];
   char   Country[4];
   char   Flags[10];
   UBYTE  Immortal;
   struct IsDate Date;
   UBYTE  killed;
} __attribute__((packed));



#endif
