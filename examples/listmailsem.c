/*
 * Mail semaphore lister..  Unmolested output should be similar to that
 * below, with ss_Link, ss_NestCount, ss_MultipleLink and ss_Owner all 0.
 *
 * MailSem[21]
 * ss_Link        : 0
 * ss_NestCount   : 0
 * ss_WaitQueue   : 114A
 * ss_MultipleLink: 0
 * ss_Owner       : 0
 * ss_QueueCount  : FFFF
 *
 */

/* Ported from SAS/C to m68k-amigaos-gcc -noixemul */
/* Original used nostartup/nostackcheck; GCC libnix provides startup code,
 * so manual DOSBase open/close is retained for structural fidelity but
 * libnix handles the actual DOS setup. */

#include <exec/types.h>
#include <dos/dos.h>
#include <cnet/cnet.h>

#undef __asm
#include <proto/exec.h>
#include <proto/dos.h>

int main(int argc, char **argv)
{
	struct MainPort *myp=NULL;

	if(myp=(struct MainPort *)FindPort(CNETPORT))
		{
		int xxx;

		if(SetSignal(0L, SIGBREAKF_CTRL_C) & SIGBREAKF_CTRL_C)
			{
			return 0;
			}

		for(xxx=0;xxx<myp->Nums[0];xxx++)
			{
			Printf("\nMailSem[%ld]\n", 		(long)xxx);
			Printf("ss_Link        : %lx\n",	(unsigned long)&myp->MPE->MailSem[xxx].ss_Link);
			Printf("ss_NestCount   : %ld\n", (long)myp->MPE->MailSem[xxx].ss_NestCount);
			Printf("ss_WaitQueue   : 0x%lx\n",	(unsigned long)&myp->MPE->MailSem[xxx].ss_WaitQueue);
			Printf("ss_MultipleLink: %ld\n",	(unsigned long)&myp->MPE->MailSem[xxx].ss_MultipleLink);
			Printf("ss_Owner       : 0x%lx\n",	(unsigned long)myp->MPE->MailSem[xxx].ss_Owner);
			Printf("ss_QueueCount  : 0x%lx\n",	(unsigned long)myp->MPE->MailSem[xxx].ss_QueueCount);
			}
		}
	else
		Printf("\nNo Control!\n\n");

	return 0;
}
