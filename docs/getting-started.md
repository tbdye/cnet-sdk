# Getting Started with the CNet 5 SDK

This guide walks you through building and deploying your first CNet 5 PFile (door) using the modernized SDK and the m68k-amigaos-gcc cross-compiler.

## Prerequisites

- **m68k-amigaos-gcc cross-compiler** installed at `/opt/amiga/` (or adjust paths in the Makefile). This is the Bebbo GCC toolchain for AmigaOS 68k.
- **A running CNet 5 BBS** (v5.x) on a real or emulated Amiga, accessible for file transfer (e.g., via network share, FTP, or amigactl).
- Basic familiarity with C programming and the AmigaOS environment.

## Installing the SDK

Clone the repository and verify the cross-compiler is available:

```sh
git clone <repo-url> cnet-sdk
cd cnet-sdk
/opt/amiga/bin/m68k-amigaos-gcc --version
```

Build the support library (`libcnet_door.a`) that all door programs link against:

```sh
make lib
```

This compiles the door lifecycle code, CNC_* wrapper functions, and utility routines into `lib/libcnet_door.a`.

## Writing Your First PFile

Create a file called `hello.c`:

```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <exec/types.h>
#include <cnet/cnet.h>

extern struct PortData *z;
extern struct MainPort *myp;

extern BOOL CNCL_DoorInit(int argc, char **argv);
extern void CNCL_DoorCleanup(void);

/* Wrapper function prototypes (from libcnet_door.a) */
extern void CPutText(char *text);
extern int  CEnterLine(UBYTE len, ULONG flags, char *prompt);
extern void CPutA(void);

static void DoorCleanup(void)
{
    /* Free any resources your code allocated */
}

int main(int argc, char **argv)
{
    atexit(DoorCleanup);

    if (!CNCL_DoorInit(argc, argv)) {
        printf("This is a CNet door -- run from CNet BBS.\n");
        return 0;
    }

    CPutText("\r\nHello from my first PFile!\r\n\r\n");

    if (CEnterLine(40, 0, "What is your name? ")) {
        sprintf(z->ABuffer, "Nice to meet you, %s!\r\n", z->InBuffer);
        CPutA();
    }

    return 0;
}
```

### Key concepts in this code

- **`CNCL_DoorInit(argc, argv)`** -- Opens cnetc.library, establishes the door context, and populates the global pointers `z` (PortData) and `myp` (MainPort). CNet passes the communication port name as `argv[1]`.
- **`CPutText()`** -- Sends text to the connected user. Use `\r\n` for line breaks.
- **`CEnterLine()`** -- Prompts the user for a line of text. The result goes into `z->InBuffer`.
- **`CPutA()`** -- Sends the contents of `z->ABuffer` to the user. Format your output into `z->ABuffer` with `sprintf()`, then call `CPutA()` to display it.
- **`atexit(DoorCleanup)`** -- Registers your cleanup *before* `CNCL_DoorInit` registers its own, so yours runs first (LIFO order). When `main()` returns or `exit()` is called, `DoorCleanup` runs first, then `CNCL_DoorCleanup` shuts down the CNet connection.

### The `__asm` conflict

CNet's struct headers (included via `<cnet/cnet.h>`) redefine `__asm` to nothing for SAS/C compatibility. This conflicts with GCC's `__asm` keyword, which the AmigaOS NDK proto headers use for register binding in inline library stubs.

`<cnet/cnet.h>` handles the include ordering: it pulls in all necessary system headers (exec, dos, graphics, intuition) *before* redefining `__asm`, so the NDK inline stubs are compiled correctly. However, this means you **cannot** call `CNC_*` functions (which use `<proto/cnetc.h>` inline stubs) directly in the same file that includes `<cnet/cnet.h>`.

The SDK solves this with wrapper functions in `libcnet_door.a`:

| Wrapper (use in your code) | Underlying library call |
|---|---|
| `CPutText()` | `CNC_PutText()` |
| `CEnterLine()` | `CNC_EnterLine()` |
| `COneKey()` | `CNC_OneKey()` |
| `CPutA()` | `CNC_PutA()` |
| `CPutQ()` | `CNC_PutQ()` |
| `CWaitForInput()` | `CNC_WaitForInput()` |
| `CReadFile()` | `CNC_ReadFile()` |
| `CReadGraphics()` | `CNC_ReadGraphics()` |
| `CSetDoing()` | `CNC_SetDoing()` |
| `CCallEditor()` | `CNC_CallEditor()` |
| `CShutDown()` | `CNC_ShutDown()` |
| `CCheckCarrier()` | `CNC_CheckCarrier()` |
| ... | ... |

Use the `C*` wrapper names in your door code. See `templates/pfile_cnetc.c` for the full list.

**Alternative approach for standalone utilities**: If you need to call Exec/DOS library functions AND access CNet structs in the same file without the door framework, include the system proto headers first, then define only the SAS/C compat macros you need (excluding `__asm`), then include CNet struct headers individually. See `templates/standalone.c` for this pattern.

## Compiling

```sh
m68k-amigaos-gcc -noixemul -m68020 -O2 -Wall -Wno-pointer-sign \
    -Iinclude -Iinclude/cnet -o hello hello.c -Llib -lcnet_door
```

Flags explained:

| Flag | Purpose |
|---|---|
| `-noixemul` | Use libnix (lightweight AmigaOS C runtime), not ixemul.library |
| `-m68020` | Generate 68020+ code (CNet 5 requires 68020 or better) |
| `-O2` | Optimization level |
| `-Wall -Wno-pointer-sign` | Warnings on, but suppress char/UBYTE pointer sign mismatches common in CNet headers |
| `-Iinclude -Iinclude/cnet` | SDK header search paths |
| `-Llib -lcnet_door` | Link against `lib/libcnet_door.a` |

The output is a native AmigaOS 68k executable.

### Using the Makefile

For anything beyond a quick test, add your program to the top-level Makefile. Door programs go in the `CNETC_DOORS` list:

```makefile
CNETC_DOORS = \
    examples/empty \
    examples/CNetCTest \
    hello \
    ...
```

Then `make examples` builds everything.

## Deploying

Copy the compiled binary to the Amiga's `PFiles:` directory (or any path CNet can reach). Common methods:

- **Network share**: Copy to a Samba/NFS share mounted on the Amiga.
- **amigactl**: `amigactl put hello PFiles:hello`
- **FTP**: Upload to the Amiga's FTP server.

The binary must be accessible from the Amiga side as a path like `PFiles:hello` or `cnet:doors/hello`.

## Configuring CNet to Run It

1. From the CNet sysop console or a sysop-level telnet session, enter **BBSMENU** (the subboard/menu editor).
2. Navigate to the subboard list where you want the door to appear.
3. **Add** a new subboard entry:
   - **Type**: PFile/C (marker type 5 / `MRK_CNETC_DOOR`)
   - **Title**: The name users will see (e.g., "Hello Door")
   - **Path**: The AmigaDOS path to your binary (e.g., `PFiles:hello`)
   - **GO keyword**: Optional shortcut (e.g., `HELLO`) so users can type `GO HELLO`
4. Save the entry.

The subboard tree organizes all areas -- message bases, file areas, doors, text files -- into a hierarchical structure. Your door appears as a leaf node that users reach by navigating the menu or using the GO keyword.

## Testing

1. **Connect** to the BBS via telnet (or local console).
2. **Navigate** to the subboard where you configured the door, or type `GO HELLO`.
3. CNet launches your binary, passing the CPort name as `argv[1]`.
4. You should see your greeting text and input prompt.
5. When you enter text and press Return, the door displays the response and exits.
6. CNet returns the user to the BBS menu.

### Debugging tips

- If the door fails to launch, check that the binary path in the subboard config is correct and the file has execute permissions.
- If you see "This is a CNet door -- run from CNet BBS", the binary was run from a CLI shell instead of being launched by CNet (no `argv[1]` CPort name).
- CNet's `cnetc.library` (v4+) must be present in `LIBS:` on the Amiga.
- The default stack size set by `libcnet_door.a` is 64KB (`__stack = 65536`). If your door uses large buffers, increase it by defining `unsigned long __stack = 131072;` (or higher) in your source file.

## Next Steps

- Study `templates/pfile_cnetc.c` for the complete door template with all wrapper prototypes.
- Study `templates/standalone.c` for writing CLI utilities that read CNet state without the door framework.
- Read `docs/architecture.md` to understand the CNet process model, MainPort, and PortData.
- Browse `examples/` for working programs demonstrating various SDK features.
