/*
 * test_structsizes.c - Comprehensive struct size check (door version)
 *
 * Reports struct sizes from a door context and compares against
 * expected values.  The struct sizes were already verified by
 * the standalone structsizes tool (tools/structsizes), but this
 * verifies them from within a running door where shared library
 * stubs are loaded and real PortData/MainPort are in use.
 *
 * Build:
 *   m68k-amigaos-gcc -noixemul -m68020 -O2 -Wall -Wno-pointer-sign \
 *       -Iinclude -Iinclude/cnet -o tests/test_structsizes \
 *       tests/test_structsizes.c -Llib -lcnet_door
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

/*
 * Check a struct size and report pass/fail.
 * Returns 1 if match, 0 if mismatch.
 */
static int check_size(const char *name, unsigned long actual, unsigned long expected)
{
    char buf[128];

    if (actual == expected) {
        sprintf(buf, "  %-22s %5lu  (expected %5lu) [OK]\r\n",
                name, actual, expected);
    } else {
        sprintf(buf, "  %-22s %5lu  (expected %5lu) [FAIL]\r\n",
                name, actual, expected);
    }
    CPutText(buf);

    return (actual == expected) ? 1 : 0;
}

int main(int argc, char **argv)
{
    char buf[256];
    int pass = 0;
    int total = 0;

    if (!CNCL_DoorInit(argc, argv)) {
        return 20;
    }

    CPutText("\r\n*** CNet SDK Test: Struct Sizes (Door Context) ***\r\n\r\n");
    CPutText("  Struct                  Size   Expected\r\n");
    CPutText("  ---------------------- -----  ----------\r\n");

    /* dates.h */
    total++; pass += check_size("IsDate",
        (unsigned long)sizeof(struct IsDate), 6);

    /* users.h */
    total++; pass += check_size("Privs",
        (unsigned long)sizeof(struct Privs), 92);
    total++; pass += check_size("UserData",
        (unsigned long)sizeof(struct UserData), 672);
    total++; pass += check_size("AccessGroup",
        (unsigned long)sizeof(struct AccessGroup), 156);
    total++; pass += check_size("ULock",
        (unsigned long)sizeof(struct ULock), 62);
    total++; pass += check_size("KeyElement3",
        (unsigned long)sizeof(struct KeyElement3), 68);
    total++; pass += check_size("KeyElement4",
        (unsigned long)sizeof(struct KeyElement4), 72);
    total++; pass += check_size("CNUserCache",
        (unsigned long)sizeof(struct CNUserCache), 14);

    /* doors.h */
    total++; pass += check_size("CPort",
        (unsigned long)sizeof(struct CPort), 48);
    total++; pass += check_size("CMessage",
        (unsigned long)sizeof(struct CMessage), 44);
    total++; pass += check_size("CNetCContext",
        (unsigned long)sizeof(struct CNetCContext), 60);

    /* subs.h */
    total++; pass += check_size("SubboardType4",
        (unsigned long)sizeof(struct SubboardType4), 693);

    /* ports.h */
    total++; pass += check_size("CNetPort",
        (unsigned long)sizeof(struct CNetPort), 140);
    total++; pass += check_size("SerPort4",
        (unsigned long)sizeof(struct SerPort4), 490);

    CPutText("\r\n");
    sprintf(buf, "Results: %d/%d struct sizes verified.\r\n", pass, total);
    CPutText(buf);

    if (pass == total) {
        CPutText("\r\nTest PASSED.\r\n");
    } else {
        CPutText("\r\nTest FAILED: some struct sizes do not match.\r\n");
    }

    return 0;
}
