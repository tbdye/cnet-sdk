/*
 * test_library.c - cnet.library function verification
 *
 * Opens cnet.library at runtime and calls functions through the
 * inline stubs to verify they work correctly.  This validates that
 * the GCC inline stubs generate correct library calls.
 *
 * Note on __asm: <cnet/cnet.h> redefines __asm to nothing (for SAS/C
 * compat).  The inline stubs are defined as macros containing __asm
 * in their bodies; since macro bodies are expanded at the call site,
 * #undef __asm before calling them restores the GCC built-in keyword.
 * See ZGettest.c for the original pattern.
 *
 * Build:
 *   m68k-amigaos-gcc -noixemul -m68020 -O2 -Wall -Wno-pointer-sign \
 *       -Iinclude -Iinclude/cnet -o tests/test_library \
 *       tests/test_library.c -Llib -lcnet_door
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <exec/types.h>
#include <cnet/cnet.h>

/* Restore __asm as a GCC keyword so the LP* macros in inline/cnet.h
 * expand correctly at call sites. Must come AFTER cnet.h. */
#undef __asm
#include <proto/exec.h>

/* cnet.library base -- must be a global (used by inline stubs) */
struct Library *CNetBase = NULL;

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
    char teststr[64];
    int pass = 1;

    if (!CNCL_DoorInit(argc, argv)) {
        return 20;
    }

    /* Open cnet.library for direct function calls via inline stubs.
     * CNetBase is declared by proto/cnet.h (included via cnet.h). */
    CNetBase = OpenLibrary("cnet.library", 0);
    if (!CNetBase) {
        CPutText("FAILED: Could not open cnet.library!\r\n");
        return 20;
    }

    CPutText("\r\n*** CNet SDK Test: Library Functions ***\r\n\r\n");

    /* Test StrToUpper */
    strcpy(teststr, "hello world");
    StrToUpper(teststr);
    sprintf(buf, "StrToUpper(\"hello world\"): \"%s\"", teststr);
    CPutText(buf);
    if (strcmp(teststr, "HELLO WORLD") == 0) {
        CPutText(" [OK]\r\n");
    } else {
        CPutText(" [FAIL]\r\n");
        pass = 0;
    }

    /* Test FileExists on a known file */
    {
        short exists = FileExists("SYS:C/Dir");
        sprintf(buf, "FileExists(\"SYS:C/Dir\"): %d", (int)exists);
        CPutText(buf);
        if (exists) {
            CPutText(" [OK]\r\n");
        } else {
            CPutText(" [FAIL - expected nonzero]\r\n");
            pass = 0;
        }
    }

    /* Test FileExists on a nonexistent file */
    {
        short exists = FileExists("SYS:nonexistent_file_xyz");
        sprintf(buf, "FileExists(\"SYS:nonexistent_file_xyz\"): %d", (int)exists);
        CPutText(buf);
        if (!exists) {
            CPutText(" [OK]\r\n");
        } else {
            CPutText(" [FAIL - expected 0]\r\n");
            pass = 0;
        }
    }

    /* Test BitPosition */
    {
        short bp = BitPosition(8);
        sprintf(buf, "BitPosition(8): %d (expected 3)", (int)bp);
        CPutText(buf);
        if (bp == 3) {
            CPutText(" [OK]\r\n");
        } else {
            CPutText(" [FAIL]\r\n");
            pass = 0;
        }
    }

    {
        short bp = BitPosition(1);
        sprintf(buf, "BitPosition(1): %d (expected 0)", (int)bp);
        CPutText(buf);
        if (bp == 0) {
            CPutText(" [OK]\r\n");
        } else {
            CPutText(" [FAIL]\r\n");
            pass = 0;
        }
    }

    /* Test AllDigits */
    {
        BOOL ad = AllDigits("12345");
        sprintf(buf, "AllDigits(\"12345\"): %d", (int)ad);
        CPutText(buf);
        if (ad) {
            CPutText(" [OK]\r\n");
        } else {
            CPutText(" [FAIL - expected TRUE]\r\n");
            pass = 0;
        }
    }

    {
        BOOL ad = AllDigits("123x5");
        sprintf(buf, "AllDigits(\"123x5\"): %d", (int)ad);
        CPutText(buf);
        if (!ad) {
            CPutText(" [OK]\r\n");
        } else {
            CPutText(" [FAIL - expected FALSE]\r\n");
            pass = 0;
        }
    }

    CPutText("\r\n");
    if (pass) {
        CPutText("All library function tests PASSED.\r\n");
        CPutText("\r\nTest PASSED.\r\n");
    } else {
        CPutText("Some library function tests FAILED.\r\n");
        CPutText("\r\nTest FAILED.\r\n");
    }

    CloseLibrary(CNetBase);
    CNetBase = NULL;

    return 0;
}
