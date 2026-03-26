/*
 * test_hello.c - Minimal CNetC door test
 *
 * Validates the basic door lifecycle: CNCL_DoorInit, CPutText,
 * and clean exit via the atexit chain (CNCL_DoorCleanup).
 *
 * Build:
 *   m68k-amigaos-gcc -noixemul -m68020 -O2 -Wall -Wno-pointer-sign \
 *       -Iinclude -Iinclude/cnet -o tests/test_hello \
 *       tests/test_hello.c -Llib -lcnet_door
 */

#include <stdlib.h>

#include <exec/types.h>
#include <cnet/cnet.h>

/* Globals provided by libcnet_door.a (door_init.c) */
extern struct PortData *z;
extern struct MainPort *myp;

/* Door lifecycle functions from libcnet_door.a */
extern BOOL CNCL_DoorInit(int argc, char **argv);
extern void CNCL_DoorCleanup(void);

/* CNetC library wrapper from libcnet_door.a */
extern void CPutText(char *text);

int main(int argc, char **argv)
{
    if (!CNCL_DoorInit(argc, argv)) {
        return 20;
    }

    CPutText("\r\n*** CNet SDK Test: Hello World ***\r\n");
    CPutText("If you can read this, the SDK door lifecycle works!\r\n");
    CPutText("\r\nTest PASSED.\r\n");

    return 0;
}
