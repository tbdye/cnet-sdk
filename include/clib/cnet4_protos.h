
// dates.c
void		CNetPrintTime			(ULONG t, char *out, char **strings, struct UserData *user);
ULONG		CNetTime					(void);
void		CNetExplodeTime		(ULONG t, struct IsDate *date);
ULONG		CNetImplodeTime		(struct IsDate *date);
USHORT	CNetDowDate				(struct IsDate *d);
void		CNetPrintDate			(struct IsDate *date, char *out, char **strings, char *format);
LONG		CNetTimeNowDiff		(ULONG prevtime);
char		*CNetUpTime          (char *diffstr, LONG timediff);

// ranges.c
long		CNetFindRange			(char *buffer, long min, long max, struct RangeContext *rc);
long		CNetNextRange			(struct RangeContext *rc);
void		CNetLongToRange		(long def, char *where);
long		CNetRangeToLong		(char *s);
UBYTE		CNetEndOfRange			(struct RangeContext *rc); // returns TRUE if the current element is the last in the complete range
long		CNetPrevRange			(struct RangeContext *rc);
UBYTE		CNetStartOfRange		(struct RangeContext *rc); // returns TRUE if the current element is the first in the complete range
