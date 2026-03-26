#ifndef CNET_CHARGES
#define CNET_CHARGES

struct   ChargeType
   {
   struct   ChargeType  *next;

   short                id;

   short                schedule;
   short                type;
   short                units;

   ULONG                fcredits;
   ULONG                bcredits;
   } __attribute__((packed));

struct   ChargeSet
   {
   struct   ChargeSet   *next;
   short                schedule;
   short                ACC[32][80];
   } __attribute__((packed));

#endif


#define CHARGETYPE_MINBALANCE        0
#define CHARGETYPE_NEWAG_LOWBAL      1
#define CHARGETYPE_LOGONSUCCESS      2
#define CHARGETYPE_FIRSTDAYCALL      3
#define CHARGETYPE_FIRSTWEEKCALL     4
#define CHARGETYPE_FIRSTMONTHCALL    5
#define CHARGETYPE_RELOGON           6
#define CHARGETYPE_CONNECTMINUTE     7
#define CHARGETYPE_OLM               8
#define CHARGETYPE_FINGER            9
#define CHARGETYPE_CONFERENCEMINUTE 10
#define CHARGETYPE_IUCMINUTE			11
#define CHARGETYPE_AREAMINUTE1      12
#define CHARGETYPE_AREAMINUTE2      13
#define CHARGETYPE_AREAMINUTE3      14
#define CHARGETYPE_AREAMINUTE4      15
#define CHARGETYPE_AREAMINUTE5      16
#define CHARGETYPE_AREAMINUTE6      17
#define CHARGETYPE_AREAMINUTE7      18
#define CHARGETYPE_AREAMINUTE8      19
#define CHARGETYPE_AREAMINUTE9      20
#define CHARGETYPE_DOORRATE1        21
#define CHARGETYPE_DOORRATE2        22
#define CHARGETYPE_DOORRATE3        23
#define CHARGETYPE_FEEDBACK         24
#define CHARGETYPE_EMAILSENT        25
#define CHARGETYPE_EMAILREAD        26
#define CHARGETYPE_MV_EDIT          27
#define CHARGETYPE_MV_KILL          28
#define CHARGETYPE_FTNSEND          29
#define CHARGETYPE_UUCPSEND         30
#define CHARGETYPE_VOTEMINS         31
#define CHARGETYPE_VOTE             32
#define CHARGETYPE_TEXTREAD1        33
#define CHARGETYPE_TEXTREAD2        34
#define CHARGETYPE_TEXTREAD3        35
#define CHARGETYPE_SUBBDITEMREAD    36
#define CHARGETYPE_SUBBDITEMPOST    37
#define CHARGETYPE_SUBBDITEMRESP    38
#define CHARGETYPE_SUBBDITEMEDIT    39
#define CHARGETYPE_SUBBDITEMDEL     40
#define CHARGETYPE_YANK             41
#define CHARGETYPE_LOCALZSEARCH     42
#define CHARGETYPE_GLOBALZSEARCH    43
#define CHARGETYPE_KBDLOAD1         44
#define CHARGETYPE_KBDLOAD2         45
#define CHARGETYPE_KBDLOAD3         46
#define CHARGETYPE_FILEDLOAD1       47
#define CHARGETYPE_FILEDLOAD2       48
#define CHARGETYPE_FILEDLOAD3       49
#define CHARGETYPE_KBUPLOAD1        50
#define CHARGETYPE_KBUPLOAD2        51
#define CHARGETYPE_KBUPLOAD3        52
#define CHARGETYPE_FILEUPLOAD1      53
#define CHARGETYPE_FILEUPLOAD2      54
#define CHARGETYPE_FILEUPLOAD3      55
#define CHARGETYPE_USERLISTMIN      56
#define CHARGETYPE_BBSLISTMIN       57
#define CHARGETYPE_BBSLISTADD       58
#define CHARGETYPE_TERMLINK         59
#define CHARGETYPE_TERMLINKMIN1     60
#define CHARGETYPE_TERMLINKMIN2     61
#define CHARGETYPE_TERMLINKMIN3     62
