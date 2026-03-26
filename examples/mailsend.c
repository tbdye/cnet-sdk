// An example of raw mail save to mail:outbox.
// Note that this code is uncompiled and untested.
// use of this example without testing first is at your own risk.

void MailSendExample( void )
{
   struct MailHeader4 OMail;
   struct MailToList  mtl;
	LONG t=0;
	char outheadername[128];
	char tempmtoname[128];
	char outmtoname[128];
	BPTR cmh=0;
	BPTR mth=0;
	char out[200];

   // zero structure memory..
	memset(&OMail, 0, sizeof(struct MailHeader4));

	// create outgoing mailbox if non-existant 
	if(!FileExists(CNETOUTBOX))
		CreateDir(CNETOUTBOX);

   // Fill in REQUIRED structure variables
   strcpy(mtl.Name, "SYSOP");
   strcpy(mtl.UUCP, "SYSOP);
   mtl.mailtype = MAIL_TO_SYSOP;
	strcpy( OMail0.Subject, "Test Sysop Mail");
	
	time(&t);
	OMail.Date				= t;
	OMail.Number			= t;
	OMail.ByAccount		= FromAccount;
	OMail.ByID				= fromid;
	OMail.ToID				= toid;
	OMail.Length	 		= strlen(text);
	OMail.IDate				= CNetTime();

	// then save mailheader and it's associated text.
	// FileExists() check assures unique name.
	do
	   {
   	sprintf( outheadername, "%s/%8x.cmh", CNETOUTBOX, t++);
   	} while(FileExists(outheadername))

	if(cmh=Open(outheadername, MODE_NEWFILE) )
		{
		Write(cmh, &OMail, sizeof(struct MailHeader4));

      // write message text of length 13 characters..
		Write(cmh, "test message!\n", 13);
		Close(cmh);
		}
	else
		return; // exception that should never be reached!

   do
		{
   	// temp mailto file! 
		sprintf( tempmtoname, "%s/%8x.mtt", CNETOUTBOX, t );

   	// only after renamed will mail-task "see" the file 
		sprintf( outmtoname, "%s/%8x.mtl", CNETOUTBOX, t++ );
		} while(FileExists(outmtoname) || FileExists(tempmtoname));

	// open mtt file and write the mail header filename to it
   // and then any mailto structures representing recipients.
	if( mth=Open(tempmtoname, MODE_NEWFILE) )
		{
		USHORT nameoffs=0;

		// write header name to mailto file 
		sprintf(out, "%s\n", outheadername);
		Write(mth, &out, strlen(out));

		tempmailto = mailto;
		Write(mth, &mtl, sizeof(struct MailToList));
	
		Close(mth);

      // attempt rename until we hit success..
		while(!Rename(tempmtoname, outmtoname))
			sprintf( outmtoname, "%s/%8x%d.mtl", CNETOUTBOX, t, nameoffs++);
		}
	}

}
