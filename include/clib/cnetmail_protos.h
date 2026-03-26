UBYTE MIMEParseBoundary  ( char *contentline, char *boundary);	// get the MIME boundary delimiter from a "Content-Type: ..." line
UBYTE MIMEIsBoundary  ( char *line, char *boundary);	// check if this line is a MIME boundary line matching a boundary filled by MIMEParseBoundary()
UBYTE MIMEParseContentType( char *input, char *contentres); // get the content-type if one exists in the input line and place it in contentres
struct MIMEContentType *MIMEFindType( char *typestring, char *encodingstring); // Find the CNet MIMEContentType structure for the content and encoding types passed.  encodingstring may be NULL if one needs to find a base content-type match
UBYTE MIMEParseName(char *line, char *name, char *base); // get the filename for this mime attachment.  base = the path to the dir where the attachment is to be stored
struct MIMEContentType *MIMEFromExt(char *ext);	// find the MIMEContentType structure associated with the specified file extension
void DisposeMailTo(struct MailToList **mailtolist);
struct MailToList *NameToList(char *name, char *uucp, struct MailToList *mailtolist, UBYTE mailtype );
struct MailToList *IsUUCP(char *net, struct MailToList *mailtolist);
struct MailToList *IsFido(char *net, USHORT *faddr, struct CNetNodeDesc *nd, struct MailToList *mailtolist );
struct MailToList *IsLocal(char *name, struct MailToList *mailtolist);
UBYTE SendMail(char *from, char *to, char *subject, char *text, LONG smflags);
struct SignalSemaphore *GetMailSems( void );
struct SignalSemaphore *GetMPSems( void );
