
#ifndef CNET_DATES
#define CNET_DATES

struct IsDate {
	UBYTE Year;
	UBYTE Month;
	UBYTE Date;
	UBYTE Hour;
	UBYTE Minute;
	UBYTE Second;
} __attribute__((packed));


#define SECONDS_IN_DAY    86400
#define SECONDS_IN_HOUR    3600
#define SECONDS_IN_MINUTE    60

#define ISDATE_BASE_YEAR   1900

#endif
