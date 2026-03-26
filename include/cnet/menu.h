#ifndef CNET_MENU_H
#define CNET_MENU_H

#define MENUNUM_ANY_UNKNOWN       0

// definition of section numbers in BBSMENU
#define MENUNUM_MAINTENANCE       1
#define MENUNUM_EVERYWHERE        2
#define MENUNUM_NEWBASE           3
#define MENUNUM_NEWMSGFILE        4
#define MENUNUM_RESPOND           5
#define MENUNUM_TEXTDOORS         6
#define MENUNUM_MAILREAD          7
#define MENUNUM_NEWSCAN           8
#define MENUNUM_READALL           9
#define MENUNUM_EDITORALL        10
#define MENUNUM_EDITOR           11
#define MENUNUM_EDITORJUST       12
#define MENUNUM_NSAL             13
#define MENUNUM_BBSLIST          14
#define MENUNUM_CONFERENCE       16
#define MENUNUM_MAIL             17
#define MENUNUM_VOTE             18
#define MENUNUM_NETWORKS			19
#define MENUNUM_BROWSE           20
#define MENUNUM_MAILALIAS        21
#define MENUNUM_MSGQUOTE         22
#define MENUNUM_ALTTEXTMENU      23
#define MENUNUM_BEFORETXFER      24
#define MENUNUM_JOINVISIT        25
#define MENUNUM_READOLM          26
#define MENUNUM_ENTRYSCAN        27
#define MENUNUM_COMPTYPES        30
#define MENUNUM_MSGREAD          31
#define MENUNUM_MAILOPTS         38
#define MENUNUM_MAINMENU         39
#define MENUNUM_TELNET           40
#define MENUNUM_TELNETMODES      41
#define MENUNUM_READLOG          42
#define MENUNUM_MAILFILTERS      43
#define MENUNUM_CNETIRC          44
#define MENUNUM_MAILATTACH       45
#define MENUNUM_FTP              46
#define MENUNUM_DMAINT           47

// sub-menu defs for MENUNUM_MAIL
#define MENUNUM_MAIL_MENU        1
#define MENUNUM_MAIL_QUIT        2
#define MENUNUM_MAIL_READ        3
#define MENUNUM_MAIL_SEND        4
#define MENUNUM_MAIL_ALIAS       5
#define MENUNUM_MAIL_VERIFY      6
#define MENUNUM_MAIL_PREFS       7
#define MENUNUM_MAIL_NODELIST    8
#define MENUNUM_MAIL_FILTER      9

// sub-menu defs for MENUNUM_MAILOPTS
#define MENUNUM_MO_PRI           1
#define MENUNUM_MO_COPYSELF      2
#define MENUNUM_MO_RECEIPT       3
#define MENUNUM_MO_DELIVERY      4
#define MENUNUM_MO_FILE          5
#define MENUNUM_MO_SIGNATURE     6
#define MENUNUM_MO_NAME          7
#define MENUNUM_MO_EXPIRE        8
#define MENUNUM_MO_QUOTE         9

// sub-menu defs for MENUNUM_MAILATTACH
#define MENUNUM_MA_MENU          1
#define MENUNUM_MA_ADOPT         2
#define MENUNUM_MA_UPLOAD        3

// sub-menu defs for MENUNUM_MAILREAD
#define MENUNUM_MR_MENU          1
#define MENUNUM_MR_GROUP         2
#define MENUNUM_MR_REPLY         3
#define MENUNUM_MR_REPLYALL      4
#define MENUNUM_MR_FWD           5
#define MENUNUM_MR_CHANGEFOLDER  6
#define MENUNUM_MR_LISTFOLDERS   7
#define MENUNUM_MR_CREATEFOLDER  8
#define MENUNUM_MR_KILLFOLDER    9
#define MENUNUM_MR_MOVE         10
#define MENUNUM_MR_KILL         11
#define MENUNUM_MR_QUIT         12
#define MENUNUM_MR_SCAN         13
#define MENUNUM_MR_AGAIN        14
#define MENUNUM_MR_SELECT       15
#define MENUNUM_MR_DNLOAD       16
#define MENUNUM_MR_GRAB         17
#define MENUNUM_MR_EXAMINE      18
#define MENUNUM_MR_EDIT         19
#define MENUNUM_MR_YANK         20
#define MENUNUM_MR_NETINFO      21
#define MENUNUM_MR_RRM          22
#define MENUNUM_MR_RESCAN       23
#define MENUNUM_MR_RFCHEAD      24

// sub-menu defs for MENUNUM_MAILALIAS
#define MENUNUM_MALIAS_MENU      1
#define MENUNUM_MALIAS_QUIT      2
#define MENUNUM_MALIAS_SCAN      3
#define MENUNUM_MALIAS_ADD       4
#define MENUNUM_MALIAS_KILL      5
#define MENUNUM_MALIAS_EDIT      6

// sub-menu defs for MENUNUM_BBSLIST
#define MENUNUM_BBL_ADD				1
#define MENUNUM_BBL_KILL			2
#define MENUNUM_BBL_SCAN			3
#define MENUNUM_BBL_QUIT			4
#define MENUNUM_BBL_MENU			5
#define MENUNUM_BBL_SELECT			6
#define MENUNUM_BBL_DLOAD			7
#define MENUNUM_BBL_EDIT			8

// submenu defs for MENUNUM_EP 
#define MENUNUM_EP_HELPLEVEL   1
#define MENUNUM_EP_MOREMODE    2
#define MENUNUM_EP_MSGPAUSE    3
#define MENUNUM_EP_DEFPROTO    4
#define MENUNUM_EP_TIMEZONE    5
#define MENUNUM_EP_TIMEFORM    6
#define MENUNUM_EP_TEXTSET     7
#define MENUNUM_EP_AUTOHIMU    8
#define MENUNUM_EP_DEFEDITOR   9
#define MENUNUM_EP_YANKPREF   10
#define MENUNUM_EP_MAILID     11
#define MENUNUM_EP_INTERFACE  12
#define MENUNUM_EP_MAILSORT   13
#define MENUNUM_EP_EDSIG      14
#define MENUNUM_EP_EDBANNER   15
#define MENUNUM_EP_MACROS     16
#define MENUNUM_EP_EDDICT     17

// submenu defs for MENUNUM_MP
#define MENUNUM_MP_REPLYTO  1
#define MENUNUM_MP_AUTOMIME 2



// to be continued..

#endif /* CNET_MENU_H */
