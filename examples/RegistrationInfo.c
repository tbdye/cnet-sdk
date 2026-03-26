/* Ported from SAS/C to m68k-amigaos-gcc -noixemul */

#include <stdlib.h>
#include <stdio.h>

#include <exec/types.h>
#include <dos/dos.h>
#include <cnet/cnet.h>

#undef __asm
#include <proto/exec.h>

int main(int argc, char *argv[])
{
	struct MainPort *myp=NULL;

	if(myp=(struct MainPort *)FindPort("cnetport"))
		{
		printf("regto = %s\n", myp->regto);
		printf("regco = %s\n", myp->regco);
		}

	return 0;
}
