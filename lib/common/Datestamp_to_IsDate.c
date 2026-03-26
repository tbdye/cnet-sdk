/*
 * Datestamp_to_IsDate.c - convert AmigaOS DateStamp to CNet IsDate.
 *
 * Originally a CommonFuncs code snippet in the CNet SDK.
 * Converted to a proper compilable unit for m68k-amigaos-gcc.
 *
 * The original used utunpk(), a SAS/C runtime function that converts
 * seconds since the Unix epoch to a broken-down date.  This version
 * uses manual date arithmetic against the Amiga epoch (1 Jan 1978).
 *
 * IsDate stores Year as offset from 1900 (per ISDATE_BASE_YEAR),
 * so 1978 = 78, 2000 = 100, etc.
 */

#include <exec/types.h>
#include <dos/dos.h>

#include <cnet/dates.h>

/*
 * is_leap_year - test whether a year is a leap year.
 */
static int is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

/*
 * days_in_month - return the number of days in a given month (1-12).
 */
static int days_in_month(int month, int year)
{
    static const int mdays[12] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    if (month == 2 && is_leap_year(year))
        return 29;
    return mdays[month - 1];
}

/*
 * FileDate - convert an AmigaOS DateStamp to a CNet IsDate.
 *
 * The DateStamp contains:
 *   ds_Days   - days since 1 January 1978
 *   ds_Minute - minutes past midnight
 *   ds_Tick   - ticks past the current minute (50 ticks/second)
 *
 * The IsDate Year field is stored as offset from 1900.
 */
void FileDate(struct DateStamp *dstamp, struct IsDate *CNDate)
{
    LONG days;
    int year, month;

    /* Convert days since Amiga epoch to year/month/day */
    days = dstamp->ds_Days;
    year = 1978;

    /* Advance year by year until remaining days < days in current year */
    for (;;) {
        int yd = is_leap_year(year) ? 366 : 365;
        if (days < yd)
            break;
        days -= yd;
        year++;
    }

    /* Advance month by month within the year */
    month = 1;
    while (month < 12) {
        int md = days_in_month(month, year);
        if (days < md)
            break;
        days -= md;
        month++;
    }

    /* Store as CNet IsDate (Year is offset from 1900) */
    CNDate->Year   = (UBYTE)(year - ISDATE_BASE_YEAR);
    CNDate->Month  = (UBYTE)month;
    CNDate->Date   = (UBYTE)(days + 1);   /* 1-based day of month */

    /* Time of day from minutes and ticks */
    CNDate->Hour   = (UBYTE)(dstamp->ds_Minute / 60);
    CNDate->Minute = (UBYTE)(dstamp->ds_Minute % 60);
    CNDate->Second = (UBYTE)(dstamp->ds_Tick / TICKS_PER_SECOND);
}
