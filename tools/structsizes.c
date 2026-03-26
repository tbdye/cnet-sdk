/* Build: m68k-amigaos-gcc -noixemul -m68020 -O2 -Iinclude -Iinclude/cnet -o tools/structsizes tools/structsizes.c */

#include <exec/types.h>
#include <stdio.h>

/* Include system headers first, then compat macros, then CNet headers */
#include <exec/lists.h>
#include <exec/ports.h>
#include <exec/semaphores.h>
#include <exec/io.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include <graphics/gfx.h>
#include <graphics/rastport.h>
#include <graphics/text.h>
#include <intuition/intuition.h>
#include <libraries/iffparse.h>

/* GCC compat macros */
#ifdef __GNUC__
#ifndef __saveds
#define __saveds __attribute__((saveds))
#endif
#ifndef __asm
#define __asm
#endif
#ifndef __regargs
#define __regargs
#endif
#ifndef __stdargs
#define __stdargs
#endif
#endif

#include <cnet/dates.h>
#include <cnet/users.h>
#include <cnet/subs.h>
#include <cnet/doors.h>
#include <cnet/ranges.h>
#include <cnet/mail.h>
#include <cnet/ports.h>
#include <cnet/control.h>

unsigned long __stack = 65536;

int main(void) {
    printf("=== CNet SDK Struct Sizes ===\n");

    /* dates.h */
    printf("IsDate: %lu\n", (unsigned long)sizeof(struct IsDate));

    /* users.h */
    printf("Privs: %lu\n", (unsigned long)sizeof(struct Privs));
    printf("UserData: %lu\n", (unsigned long)sizeof(struct UserData));
    printf("ULock: %lu\n", (unsigned long)sizeof(struct ULock));
    printf("AccessGroup: %lu\n", (unsigned long)sizeof(struct AccessGroup));
    printf("KeyElement3: %lu\n", (unsigned long)sizeof(struct KeyElement3));
    printf("KeyElement4: %lu\n", (unsigned long)sizeof(struct KeyElement4));
    printf("CNUserCache: %lu\n", (unsigned long)sizeof(struct CNUserCache));

    /* subs.h */
    printf("SelectType: %lu\n", (unsigned long)sizeof(struct SelectType));
    printf("BaseUser: %lu\n", (unsigned long)sizeof(struct BaseUser));
    printf("HeaderType: %lu\n", (unsigned long)sizeof(struct HeaderType));
    printf("MessageType3: %lu\n", (unsigned long)sizeof(struct MessageType3));
    printf("ItemType3: %lu\n", (unsigned long)sizeof(struct ItemType3));
    printf("ItemHeader: %lu\n", (unsigned long)sizeof(struct ItemHeader));
    printf("FreeType: %lu\n", (unsigned long)sizeof(struct FreeType));
    printf("SubboardType4: %lu\n", (unsigned long)sizeof(struct SubboardType4));

    /* doors.h */
    printf("CPort: %lu\n", (unsigned long)sizeof(struct CPort));
    printf("CMessage: %lu\n", (unsigned long)sizeof(struct CMessage));
    printf("CNetCContext: %lu\n", (unsigned long)sizeof(struct CNetCContext));

    /* mail.h */
    printf("OldMailHeader: %lu\n", (unsigned long)sizeof(struct OldMailHeader));
    printf("MailHeader4: %lu\n", (unsigned long)sizeof(struct MailHeader4));
    printf("NewMailFolder: %lu\n", (unsigned long)sizeof(struct NewMailFolder));

    /* ports.h */
    printf("CNetPort: %lu\n", (unsigned long)sizeof(struct CNetPort));
    printf("SerPort: %lu\n", (unsigned long)sizeof(struct SerPort));
    printf("SerPort4: %lu\n", (unsigned long)sizeof(struct SerPort4));
    printf("PortData: %lu\n", (unsigned long)sizeof(struct PortData));
    printf("PortDataExtension: %lu\n", (unsigned long)sizeof(struct PortDataExtension));

    /* control.h */
    printf("PortConfig: %lu\n", (unsigned long)sizeof(struct PortConfig));
    printf("NewConfig1: %lu\n", (unsigned long)sizeof(struct NewConfig1));
    printf("MainPort: %lu\n", (unsigned long)sizeof(struct MainPort));
    printf("MainPortExtension: %lu\n", (unsigned long)sizeof(struct MainPortExtension));

    /* Key field offsets in PortData */
    {
        struct PortData *p = (struct PortData *)0;
        printf("\n=== PortData Key Offsets ===\n");
        printf("user1 offset: %lu\n", (unsigned long)((char*)&p->user1 - (char*)p));
        printf("user2 offset: %lu\n", (unsigned long)((char*)&p->user2 - (char*)p));
        printf("bn offset: %lu\n", (unsigned long)((char*)&p->bn - (char*)p));
        printf("sb offset: %lu\n", (unsigned long)((char*)&p->sb - (char*)p));
    }

    /* Key field offsets in UserData */
    {
        struct UserData *u = (struct UserData *)0;
        printf("\n=== UserData Key Offsets ===\n");
        printf("Handle offset: %lu\n", (unsigned long)((char*)&u->Handle - (char*)u));
        printf("RealName offset: %lu\n", (unsigned long)((char*)&u->RealName - (char*)u));
        printf("UUCP offset: %lu\n", (unsigned long)((char*)&u->UUCP - (char*)u));
        printf("MyPrivs offset: %lu\n", (unsigned long)((char*)&u->MyPrivs - (char*)u));
    }

    return 0;
}
