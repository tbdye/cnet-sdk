// CNet Amiga/4 Programming examples
// copyright (c) 1998 Ray A. Akey
// Exclusive license granted to ZenMetal Software and CNet door coders.
//
// ListEvents: Lists all configured events and their attributes.

/* Ported from SAS/C to m68k-amigaos-gcc -noixemul */

#include <stdlib.h>
#include <stdio.h>

#include <exec/types.h>
#include <dos/dos.h>
#include <cnet/cnet.h>
#include <cnet/eventdefs.h>

#undef __asm
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/cnet.h>

/* LongToDate is provided by libcnet_door.a (utility.c) */
extern void LongToDate(char *date, char *t, ULONG was);

char *InvokeLabels[] =
   {
   "Immediate-NoDump",
   "Immediate-UDump",
   "if port idle",
   "if user online",
   NULL
   };

char *StatusLabels[] =
   {
   "Ready",
   "Suspended",
   "Once/Delete",
   NULL };

char *PortLabels[] =
   {
   "Ignore",
   "Run",
   "Run/Close",
   NULL };

char *TypeLabels[] =
   {
   "RunCNetC",
   "RunARexx",
   "RunDOS",
   "ReadFile",
   "DOS-CMD",
   "ClosePort",
   "Charges#",
   "LogonBPS",
   "DloadBPS",
   "ULoadBPS",
   "LogonAccess",
   "XfersAccess",
   "DoorsAccess",
   "Modem#",
   "CallBack",
   "Avalid#",
   "SysopIn",
   "NewUsers",
   "Files",
   "Doors",
   "MsgArea",
   "JoinLink",
   "On-Line",
   NULL
   };

char *Days[] =
   {
   "Sun",
   "Mon",
   "Tue",
   "Wed",
   "Thu",
   "Fri",
   "Sat"
   };

int main(int argc, char **argv)
{
   char EventFile[] = "cnet:configs/events.cfg";
   BPTR fh;
   struct JobType4 job0;
   char datestr[40];
   char timestr[40];
   struct Library *CNetBase;

   // note: If you access this file in an ACTIVE (eg. CNET/CONTROL/BBS RUNNING)
   //       you MUST ObtainSemaphore(&myp->eventsem) before reading the file
   //       and ReleaseSemaphore(&myp->eventsem) thereafter.
   //       These functions are disabled in this source but can be enabled
   //       if you need to do this in an active BBS environment.  Don't forget
   //       to get a pointer to MainPort first.
   //       eg. myp=(struct MainPort *)FindPort(CNETPORT);

   if((CNetBase = OpenLibrary("cnet.library", 4L)))
      {
//    ObtainSemaphore(&myp->eventsem); // This line requires a MainPort pointer..

      if((fh=Open(EventFile, MODE_OLDFILE)))
         {
         BYTE xxx=0;
         while(Read(fh, &job0, sizeof(struct JobType4)))
            {
            printf("\n    Name: %s\n", job0.Name);
            printf(" Command: %s\n", job0.args);
            printf("   Ports: %s\n", job0.ports);
            printf("  Invoke: %s\n", InvokeLabels[BitPosition(job0.invoke)]); // BitPosition() is a cnet.library function
            printf("    Type: %s\n", TypeLabels[job0.type]);
            printf("  Status: %s\n", StatusLabels[job0.status]);

            LongToDate( datestr, timestr, job0.StartTime);
            timestr[5]=0;
            printf("   Start: %s %s\n", datestr, timestr);

            LongToDate( datestr, timestr, job0.DateExecuted);
            timestr[5]=0;
            printf("LastDate: %s %s\n", datestr, timestr);

            printf("   Valid: %ld seconds\n", (long)job0.valid);
            printf("    Days: ");
            for(xxx=0;xxx<7;xxx++)
               if(job0.Days & xxx)
                  printf("%s ", Days[xxx]);
            printf("\n Deleted: %s\n", (job0.deleted) ? "Yes":"No");
            printf("  Repeat: %d days, %d hours, %d minutes\n", job0.repeat/SECONDS_IN_DAY, (job0.repeat%SECONDS_IN_DAY)/SECONDS_IN_HOUR, (job0.repeat%SECONDS_IN_HOUR)/SECONDS_IN_MINUTE);
            printf("    Port: %s\n", PortLabels[job0.runport]);
            }
         Close(fh);
//         ReleaseSemaphore(&myp->eventsem);
         }
      CloseLibrary(CNetBase);
      }

   return 0;
}
