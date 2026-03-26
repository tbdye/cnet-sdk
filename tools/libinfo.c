/* Build: m68k-amigaos-gcc -noixemul -m68020 -O2 -Iinclude -Iinclude/cnet -o tools/libinfo tools/libinfo.c */

#include <exec/types.h>
#include <exec/libraries.h>
#include <proto/exec.h>
#include <stdio.h>

unsigned long __stack = 65536;

int main(void) {
    struct Library *lib;

    static const char *names[] = {
        "cnet.library",
        "cnetc.library",
        "cnet4.library",
        "cnetmail.library",
        "cnetgui.library",
        NULL
    };

    int i;
    for (i = 0; names[i]; i++) {
        lib = OpenLibrary((CONST_STRPTR)names[i], 0);
        if (lib) {
            printf("%s: v%d.%d  NegSize=%d  LVOs=%d  PosSize=%d\n",
                   names[i],
                   lib->lib_Version, lib->lib_Revision,
                   lib->lib_NegSize, lib->lib_NegSize / 6,
                   lib->lib_PosSize);
            CloseLibrary(lib);
        } else {
            printf("%s: FAILED to open\n", names[i]);
        }
    }

    return 0;
}
