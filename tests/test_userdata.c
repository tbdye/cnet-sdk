/*
 * test_userdata.c - UserData struct layout verification
 *
 * Reads z->user1 fields and prints them to verify that the compiled
 * struct layout matches the live CNet system's memory layout.
 * If fields display correctly, the struct offsets are verified.
 *
 * Build:
 *   m68k-amigaos-gcc -noixemul -m68020 -O2 -Wall -Wno-pointer-sign \
 *       -Iinclude -Iinclude/cnet -o tests/test_userdata \
 *       tests/test_userdata.c -Llib -lcnet_door
 */

#include <stdlib.h>
#include <stdio.h>

#include <exec/types.h>
#include <cnet/cnet.h>

/* Globals provided by libcnet_door.a (door_init.c) */
extern struct PortData *z;
extern struct MainPort *myp;

/* Door lifecycle functions from libcnet_door.a */
extern BOOL CNCL_DoorInit(int argc, char **argv);
extern void CNCL_DoorCleanup(void);

/* CNetC library wrappers from libcnet_door.a */
extern void CPutText(char *text);

int main(int argc, char **argv)
{
    char buf[256];
    struct UserData *u;

    if (!CNCL_DoorInit(argc, argv)) {
        return 20;
    }

    u = &z->user1;

    CPutText("\r\n*** CNet SDK Test: UserData Struct Layout ***\r\n\r\n");

    sprintf(buf, "Handle: %.21s\r\n", u->Handle);
    CPutText(buf);

    sprintf(buf, "RealName: %.26s\r\n", u->RealName);
    CPutText(buf);

    sprintf(buf, "Access Group: %d\r\n", (int)u->Access);
    CPutText(buf);

    sprintf(buf, "TotalCalls: %ld\r\n", u->TotalCalls);
    CPutText(buf);

    sprintf(buf, "UUCP: %.11s\r\n", u->UUCP);
    CPutText(buf);

    sprintf(buf, "sizeof(UserData): %lu (expected 672)\r\n",
            (unsigned long)sizeof(struct UserData));
    CPutText(buf);

    sprintf(buf, "sizeof(Privs): %lu (expected 92)\r\n",
            (unsigned long)sizeof(struct Privs));
    CPutText(buf);

    if (sizeof(struct UserData) == 672 && sizeof(struct Privs) == 92) {
        CPutText("\r\nStruct sizes MATCH expected values.\r\n");
        CPutText("If fields show correctly, struct layout is verified!\r\n");
        CPutText("\r\nTest PASSED.\r\n");
    } else {
        CPutText("\r\nWARNING: Struct sizes do NOT match expected values!\r\n");
        CPutText("\r\nTest FAILED.\r\n");
    }

    return 0;
}
