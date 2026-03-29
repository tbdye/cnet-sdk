// CNet Amiga/4 Programming examples
// copyright (c) 1998 Ray A. Akey
// Exclusive license granted to ZenMetal Software and CNet door coders.

/* Ported from SAS/C to m68k-amigaos-gcc -noixemul */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <exec/types.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include <cnet/cnet.h>
#include <cnet/version.h>

#undef __asm
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/cnet.h>

struct Library        *CNetBase=NULL;
struct CNetFileEntry  *filelist;
struct FileInfoBlock  *fib;


void DoDisplay(struct CNetFileEntry *list, UBYTE depth)
{
   char buff[20];
   struct CNetFileEntry *fptr=list;
   UBYTE i;

   depth++;
   while((fptr=NextDirEntry(HeaderEntry(list), fptr)))
      {
      if(fptr->size)
         sprintf(buff, "(%ld bytes)", (long)fptr->size);

      for(i=0;i<depth;i++)
         Printf("\t");

      Printf("%3ld. %s%s %s\n", (long)EntryOrdinal(fptr, HeaderEntry(list)),
                                (long)(fptr->ftype ? "[DIR]":""),
                                (long)fptr->filename,
                                (long)(fptr->size ? buff:""));
      if(fptr->ftype)
         {
         DoDisplay(fptr->child, depth);
         }
      }
}

int main( int argc, char *argv[])
{
   struct CNetFileEntry *pointer=NULL;

   if((CNetBase=OpenLibrary(CNETLIBNAME, CNETLIBVERSION)))
      {
      if((fib=AllocVec(sizeof(struct FileInfoBlock), MEMF_CLEAR)))
         {

         // create the header entry and set it as the list TOP entry.

         strcpy(fib->fib_FileName, "DH0:somedirectory");
         filelist=CreateDirEntry(fib);

         // create a bunch of FAKE file entries in non-alpha order

         strcpy(fib->fib_FileName, "Entry 1");
         fib->fib_DirEntryType = -1; //make it look like a regular file.
         fib->fib_Size = 99999; // they will all appear to be the same size..
         pointer=CreateDirEntry(fib);
         InsertDirEntry(&filelist, pointer);

         strcpy(fib->fib_FileName, "Entry 3");
         pointer=CreateDirEntry(fib);
         InsertDirEntry(&filelist, pointer);

         strcpy(fib->fib_FileName, "Entry 2");
         pointer=CreateDirEntry(fib);
         InsertDirEntry(&filelist, pointer);

         strcpy(fib->fib_FileName, "Alpha TOP Entry");
         pointer=CreateDirEntry(fib);
         InsertDirEntry(&filelist, pointer);

         strcpy(fib->fib_FileName, "a Directory Entry");
         fib->fib_DirEntryType = 1; //make it look like a directory.
         pointer=CreateDirEntry(fib);
         InsertDirEntry(&filelist, pointer);

         strcpy(fib->fib_FileName, "Z Alpha BOTTOM Entry");
         fib->fib_DirEntryType = -1; //make it look like a regular file.
         pointer=CreateDirEntry(fib);
         InsertDirEntry(&filelist, pointer);

         // Display the sorted directory contents

         if((pointer=HeaderEntry(filelist))) // the header entry contains info
                                           // about the directory of files
                                           // attached to the linked list
            {
            Printf("Directory of: %s\n", (long)pointer->filename);
            DoDisplay(pointer, 0);
            }

         // free memory used by fib and file entry list
         // note that CNet frees ALL memory used by your entries when you call
         // CNetDisposeDir()

         CNetDisposeDir(&filelist);
         FreeVec(fib);
         }
      CloseLibrary(CNetBase);
      }
   exit(RETURN_OK);

   return 0;
}
