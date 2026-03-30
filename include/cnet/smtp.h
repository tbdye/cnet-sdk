#ifndef CNET_SMTP_H
#define CNET_SMTP_H

#include <cnet/align.h>

#define SMTP_CMD_HELO               "HELO"
#define SMTP_CMD_EHLO               "EHLO"
#define SMTP_CMD_QUIT               "QUIT"
#define SMTP_CMD_RSET               "RSET"
#define SMTP_CMD_NOOP               "NOOP"
#define SMTP_CMD_DATA               "DATA"
#define SMTP_CMD_MAIL               "MAIL"
#define SMTP_CMD_RCPT               "RCPT"
#define SMTP_CMD_VRFY               "VRFY"

#define SMTP_RESP_READY             "220 %s %s ready\r\n"
#define SMTP_RESP_CLOSE             "221 %s closing transmission channel\r\n"
#define SMTP_RESP_OK                "250 OK\r\n"
#define SMTP_RESP_OK_VRFY           "250 <%s@%s>\r\n"
#define SMTP_RESP_OK_SUB            "250 %s\r\n"

#define SMTP_RESP_RECEIVING         "354 Receiving...\r\n"

#define SMTP_RESP_BUSY              "421 Service busy, closing transmission channel\r\n"
#define SMTP_RESP_NO_DELIVER        "450 Delivery failure\r\n"

#define SMTP_RESP_WHAT              "502 Unknown command.\r\n"
#define SMTP_RESP_WHO               "550 Who?????\r\n"
#define SMTP_RESP_NO_RECIP          "550 No recipients\r\n"
#define SMTP_RESP_RECIP_FAIL        "550 Couldn't write recipient\r\n"
#define SMTP_RESP_VRFY_FAIL         "550 %s... User unknown\r\n"
#define SMTP_RESP_NO_MAILBOX        "553 Access denied; name %s unknown\r\n"

#define SMTPERR_UNKNOWN_ERR         "??Unknown\n"
#define SMTPERR_BAD_RESP            "Bad response: %s\n"
#define SMTPERR_NOTEMP               "could not open temp file: %s\n"
#define SMTPERR_INVALID_CMD         "Invalid SMTP command: %s\n"
#define SMTPERR_SEND_ERROR          "Error sending remote data\n"
#define SMTPERR_NO_MEMORY           "out of memory\n"
#define SMTPERR_EXPECTING_HELO      "Expecting HELO; received %s\n"
#define SMTPERR_TEMPWRITE           "Error writing temp file %s\n"
#define SMTPERR_CONNECTION_LOST     "Connection lost\n"
#define SMTPERR_SELECT              "Select error: %s\n"
#define SMTPERR_TIMEOUT             "Timeout: %s\n"
#define SMTPERR_LOCALHOST           "Local host name not found\n"
#define SMTPERR_NO_DELIVERY         "Couldn't deliver mail to %s\n"
#define SMTPERR_NOT_DELIVERED       "Couldn't deliver mail item %s\n"
#define SMTPERR_CMD_REJECT          "Command rejected: %s\n"


CNET_PACK_BEGIN

struct SMTPProcess
   {
   struct Process *process; // SMTP process pointer - could be a pointer to any one of 32
                            // active or inactive simultaneous SMTP connections.

   UBYTE added;         // TRUE if processing
   UBYTE complete;      // TRUE if complete

	struct sockaddr_in sin; // remote host (peer) name
	LONG sinsize;				// size of sin

	char status[80];		// current status of process, for display in GUI
	APTR entry;				// pointer to BGUI LV entry
   };


struct SMTPHostStruct
   {
   char HostName[128];  // 64 is max domain length expected according to RFC-822
                        // 128 used in case of expansion. May be either Hostname
                        // or IP.
   };

struct SpamStruct
   {
   char HostName[128];  // 64 is max domain length expected according to RFC-822
                        // 128 used in case of expansion. May be either Hostname
                        // or IP.
   char UserName[128];  // Username and hostname may contain AmigaDOS patterns
                        //
                        // Hostname may be IP address with ranges of IP bits or may be host/domain names.
                        // eg. ipbits1.ipbits2.ipbits3.ipbits4
                        //     aol.com
                        //     192.0-50.#?.#?
   };

CNET_PACK_END

#define SMTPSESS_HELORECVD	(1L << 1)

#endif /* CNET_SMTP_H */
