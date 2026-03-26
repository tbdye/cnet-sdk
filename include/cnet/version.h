#ifndef CNET_VERSION_H
#define CNET_VERSION_H

/*
 * CNet SDK version identifiers
 * Original SDK: CNet PRO 5.01
 * Modernized for compatibility through v5.36c
 */

#define CNET_VERSION      "$VER: CNet PRO 5.01"
#define CNET_VERLETTER    "\0" // letter pertaining to the current version - eg. the 'c' in "4.28c"

#define MAILTASK_VERSION    "$VER: Mail-Task 2.10"
#define FILETASK_VERSION    "$VER: File-Task 3.6"
#define NEWSTASK_VERSION    "$VER: News-Task 1.7"
#define YANKTASK_VERSION    "$VER: Yank-Task 1.05"
#define CNETIRC_VERSION     "$VER: CNetIRC 3.7"
#define CNETTR_VERSION      "$VER: CNet TraceRoute 2.2"
#define CNETSMTPD_VERSION   "$VER: CNetESMTPd 2.07"
#define USEREDIT_VERSION    "$VER: UserEd 1.06"
#define MIMEEDIT_VERSION    "$VER: MimeEd 1.05"
#define CNFING_VERSION      "$VER: CNet FingerD v1.4"
#define CNFTPD_VERSION      "$VER: cnFTPd 1.00"
#define CNFTP_VERSION       "$VER: cnFTP 1.04"
#define CNETTELNETD_VERSION "$VER: cnTelnetd 1.06"

// defines for CNet library names/versions/revisions
#define CNETLIBNAME      "cnet.library"
#define CNETLIBVERSION   5L
#define CNETLIBREVISION  0L

#define CNETCLIBNAME     "cnetc.library"
#define CNETCLIBVERSION  4L
#define CNETCLIBREVISION 1L

#define CNET4LIBNAME     "cnet4.library"
#define CNET4LIBVERSION  5L
#define CNET4LIBREVISION 0L

#define CMAILLIBNAME     "cnetmail.library"
#define CMAILLIBVERSION  4L
#define CMAILLIBREVISION 1L

#define CNETGUILIBNAME     "cnetgui.library"
#define CNETGUILIBVERSION  4L
#define CNETGUILIBREVISION 4L

#define DOSLIBNAME       "dos.library"
#define MINDOSVERSION    37L

#endif /* CNET_VERSION_H */
