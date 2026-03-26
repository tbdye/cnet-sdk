BOOL CheckOnline( int account ) 	/* check if user is online */
{
	int portcnt;

	for(portcnt=0;portcnt < 100;portcnt++)
		{
		if( (myp->pc[portcnt].online) && (myp->PortZ[portcnt]->loaded) && (z->OnLine) && (myp->PortZ[portcnt]->id == account) )
			{
			OnPort=portcnt;
			return TRUE;
			}
		else
			OnPort=0;
		}

	return FALSE;
}
