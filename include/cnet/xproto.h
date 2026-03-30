/** xproto.h
*
*   Include file for External Protocol Handling
*
**/

#ifndef CNET_XPROTO_H
#define CNET_XPROTO_H

#include <cnet/align.h>

/*
 * SAS/C uses "__asm" and "register __aX/__dX" for register-parameter
 * function pointers. GCC m68k-amigaos uses a different syntax.
 * Provide a forward declaration of XPR_UPDATE so it can be referenced
 * in XPR_IO before the full definition.
 */
struct XPR_UPDATE;

/*
*   The structure
*/
CNET_PACK_BEGIN

#ifdef __GNUC__
struct XPR_IO {
		  /* File name(s) */
		  char	*xpr_filename;
		  /* Open file */
		  long (* xpr_fopen)(char *filename __asm__("a0"),
				     char *accessmode __asm__("a1"));
		  /* Close file */
		  long (* xpr_fclose)(long filepointer __asm__("a0"));
		  /* Get char from file */
		  long (* xpr_fread)(char *buffer __asm__("a0"),
				     long size __asm__("d0"),
				     long count __asm__("d1"),
				     long fileptr __asm__("a1"));
		  /* Put string to file */
		  long (* xpr_fwrite)(char *buffer __asm__("a0"),
				      long size __asm__("d0"),
				      long count __asm__("d1"),
				      long fileptr __asm__("a1"));
		  /* Get char from serial */
		  long (* xpr_sread)(char *buffer __asm__("a0"),
				     long size __asm__("d0"),
				     long timeout __asm__("d1"));
		  /* Put string to serial */
		  long (* xpr_swrite)(char *buffer __asm__("a0"),
				      long size __asm__("d0"));
		  /* Flush serial input buffer */
		  long (*xpr_sflush)(void);
		  /* Print stuff */
		  long (* xpr_update)(struct XPR_UPDATE *updatestruct __asm__("a0"));
		  /* Check for abort */
		  long (*xpr_chkabort)(void);
		  /* Check misc. stuff */
		  void (*xpr_chkmisc)(void);
		  /* Get string interactively */
		  long (* xpr_gets)(char *prompt __asm__("a0"),
				    char *buffer __asm__("a1"));
		  /* Set and Get serial info */
		  long (* xpr_setserial)(long newstatus __asm__("d0"));
		  /* Find first file name */
		  long (* xpr_ffirst)(char *buffer __asm__("a0"),
				      char *pattern __asm__("a1"));
		  /* Find next file name */
		  long (* xpr_fnext)(long oldstate __asm__("d0"),
				     char *buffer __asm__("a0"),
				     char *pattern __asm__("a1"));
		  /* Return file info */
		  long (* xpr_finfo)(char *filename __asm__("a0"),
				     long typeofinfo __asm__("d0"));
		  /* Seek in a file */
		  long (* xpr_fseek)(long fileptr __asm__("a0"),
				     long offset __asm__("d0"),
				     long origin __asm__("d1"));
		  /* Number of extensions */
		  long	 xpr_extension;
		  /* Initialized by Setup. */
		  long	*xpr_data;
		  /* Multiple XPR options. */
		  long (* xpr_options)(long n __asm__("d0"),
				       struct xpr_option **opt __asm__("a0"));
		  /* Delete a file. */
		  long (* xpr_unlink)(char *filename __asm__("a0"));
		  /* Query serial device */
		  long (*xpr_squery)(void);
		  /* Get various host ptrs */
		  long (* xpr_getptr)(long type __asm__("d0"));
	      };
#else /* SAS/C */
struct XPR_IO {
		  /* File name(s) */
		  char	*xpr_filename;
		  /* Open file */
		  long (* __asm xpr_fopen)(register __a0 char *filename,
					   register __a1 char *accessmode);
		  /* Close file */
		  long (* __asm xpr_fclose)(register __a0 long filepointer);
		  /* Get char from file */
		  long (* __asm xpr_fread)(register __a0 char *buffer,
					   register __d0 long size,
					   register __d1 long count,
					   register __a1 long fileptr);
		  /* Put string to file */
		  long (* __asm xpr_fwrite)(register __a0 char *buffer,
					    register __d0 long size,
					    register __d1 long count,
					    register __a1 long fileptr);
		  /* Get char from serial */
		  long (* __asm xpr_sread)(register __a0 char *buffer,
					   register __d0 long size,
					   register __d1 long timeout);
		  /* Put string to serial */
		  long (* __asm xpr_swrite)(register __a0 char *buffer,
					    register __d0 long size);
		  /* Flush serial input buffer */
		  long (*xpr_sflush)(void);
		  /* Print stuff */
		  long (* __asm xpr_update)(register __a0
					      struct XPR_UPDATE *updatestruct);
		  /* Check for abort */
		  long (*xpr_chkabort)(void);
		  /* Check misc. stuff */
		  void (*xpr_chkmisc)(void);
		  /* Get string interactively */
		  long (* __asm xpr_gets)(register __a0 char *prompt,
					  register __a1 char *buffer);
		  /* Set and Get serial info */
		  long (* __asm xpr_setserial)(register __d0 long newstatus);
		  /* Find first file name */
		  long (* __asm xpr_ffirst)(register __a0 char *buffer,
					    register __a1 char *pattern);
		  /* Find next file name */
		  long (* __asm xpr_fnext)(register __d0 long oldstate,
					   register __a0 char *buffer,
					   register __a1 char *pattern);
		  /* Return file info */
		  long (* __asm xpr_finfo)(register __a0 char *filename,
					   register __d0 long typeofinfo);
		  /* Seek in a file */
		  long (* __asm xpr_fseek)(register __a0 long fileptr,
					   register __d0 long offset,
					   register __d1 long origin);
		  /* Number of extensions */
		  long	 xpr_extension;
		  /* Initialized by Setup. */
		  long	*xpr_data;
		  /* Multiple XPR options. */
		  long (* __asm xpr_options)(register __d0 long n,
					     register __a0
						     struct xpr_option **opt);
		  /* Delete a file. */
		  long (* __asm xpr_unlink)(register __a0 char *filename);
		  /* Query serial device */
		  long (*xpr_squery)(void);
		  /* Get various host ptrs */
		  long (* __asm xpr_getptr)(register __d0 long type);
	      };
#endif /* __GNUC__ */
/*
*   Number of defined extensions
*/
#define XPR_EXTENSION 4L

/*
*   Flags returned by XProtocolSetup()
*/
#define XPRS_FAILURE	0x00000000L
#define XPRS_SUCCESS	0x00000001L
#define XPRS_NORECREQ	0x00000002L
#define XPRS_NOSNDREQ	0x00000004L
#define XPRS_HOSTMON	0x00000008L
#define XPRS_USERMON	0x00000010L
#define XPRS_HOSTNOWAIT 0x00000020L
/*
*   The update structure
*/
struct XPR_UPDATE {	long  xpru_updatemask;
			char *xpru_protocol;
			char *xpru_filename;
			long  xpru_filesize;
			char *xpru_msg;
			char *xpru_errormsg;
			long  xpru_blocks;
			long  xpru_blocksize;
			long  xpru_bytes;
			long  xpru_errors;
			long  xpru_timeouts;
			long  xpru_packettype;
			long  xpru_packetdelay;
			long  xpru_chardelay;
			char *xpru_blockcheck;
			char *xpru_expecttime;
			char *xpru_elapsedtime;
			long  xpru_datarate;
			long  xpru_reserved1;
			long  xpru_reserved2;
			long  xpru_reserved3;
			long  xpru_reserved4;
			long  xpru_reserved5;
		   };
/*
*   The possible bit values for the xpru_updatemask are:
*/
#define XPRU_PROTOCOL		0x00000001L
#define XPRU_FILENAME		0x00000002L
#define XPRU_FILESIZE		0x00000004L
#define XPRU_MSG		0x00000008L
#define XPRU_ERRORMSG		0x00000010L
#define XPRU_BLOCKS		0x00000020L
#define XPRU_BLOCKSIZE		0x00000040L
#define XPRU_BYTES		0x00000080L
#define XPRU_ERRORS		0x00000100L
#define XPRU_TIMEOUTS		0x00000200L
#define XPRU_PACKETTYPE 	0x00000400L
#define XPRU_PACKETDELAY	0x00000800L
#define XPRU_CHARDELAY		0x00001000L
#define XPRU_BLOCKCHECK 	0x00002000L
#define XPRU_EXPECTTIME 	0x00004000L
#define XPRU_ELAPSEDTIME	0x00008000L
#define XPRU_DATARATE		0x00010000L
/*
*   The xpro_option structure
*/
struct xpr_option {
   char *xpro_description;	/* description of the option		      */
   long  xpro_type;		/* type of option			      */
   char *xpro_value;		/* pointer to a buffer with the current value */
   long  xpro_length;		/* buffer size				      */
};

CNET_PACK_END

/*
*   Valid values for xpro_type are:
*/
#define XPRO_BOOLEAN 1L 	/* xpro_value is "yes", "no", "on" or "off"   */
#define XPRO_LONG    2L 	/* xpro_value is string representing a number */
#define XPRO_STRING  3L 	/* xpro_value is a string		      */
#define XPRO_HEADER  4L 	/* xpro_value is ignored		      */
#define XPRO_COMMAND 5L 	/* xpro_value is ignored		      */
#define XPRO_COMMPAR 6L 	/* xpro_value contains command parameters     */

extern struct Library *XProtocolBase;

long XProtocolSend   ( struct XPR_IO *io );
long XProtocolReceive( struct XPR_IO *io );
long XProtocolSetup  ( struct XPR_IO *io );
long XProtocolCleanup( struct XPR_IO *io );

#ifndef __GNUC__
#pragma libcall XProtocolBase XProtocolCleanup 1E 801
#pragma libcall XProtocolBase XProtocolSetup 24 801
#pragma libcall XProtocolBase XProtocolSend 2A 801
#pragma libcall XProtocolBase XProtocolReceive 30 801
#endif

#endif /* CNET_XPROTO_H */
