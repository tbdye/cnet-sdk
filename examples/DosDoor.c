// CNet Amiga/4 Programming examples
// copyright (c) 1998 Ray A. Akey
// Exclusive license granted to ZenMetal Software and CNet door coders.
//
// DOSDOOR.C - A simple DOS DOOR to illustrate how to get input from users

/* Ported from SAS/C to m68k-amigaos-gcc -noixemul */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <exec/types.h>
#include <dos/dos.h>
#include <dos/dosextens.h>

#undef __asm
#include <proto/dos.h>

int main(int argc, char *argv[])
{
	BPTR inputfh=0;
	BPTR outputfh=0;
	char OutLine[100];
	char Buffer[5];
	BOOL done=FALSE;

	inputfh=Input();		/* get DOS input/output filehandles */
	outputfh=Output();

	SetMode(inputfh, 1);	/* set stdio to RAW: mode */
	SetMode(outputfh, 1);

	Write(outputfh, "Enter text: ", 6);
	while(!done)
		{
		if(WaitForChar( inputfh, 5000000))	/* wait at least 5 seconds for input */
			{
			if(Read(inputfh, Buffer, 1))	/* WaitForChar returned non-zero - get one character from stdio */
				{
				Buffer[1] = 0;
				switch(Buffer[0])
					{
					case 27:	/* ESCape pressed - exit! */
						done=TRUE;
					default:	/* just general baloney to verify keys pressed */
						sprintf(OutLine, "\nUser Entered '%c'\n", Buffer[0]);
						Write(outputfh, OutLine, strlen(OutLine));
					}
				}
			}
		else
			{
			Write(outputfh, "\n**timeout\n", 11);
			}
		}

	SetMode(inputfh, 0);	/* restore state of stdio to CON: mode */
	SetMode(outputfh, 0);
	exit(0);

	return 0;
}
