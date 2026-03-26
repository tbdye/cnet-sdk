# CNet 5 SDK Examples Guide

Annotated walkthrough of the key example programs in `examples/`. Each
section explains what the example demonstrates, the patterns it uses, and
how to build and run it. Study these alongside the templates in
`templates/` and the API reference in `docs/library-reference.md`.

---

## Table of Contents

1. [empty.c](#emptyc) -- the PFile template
2. [CNetCTest.c](#cnetctestc) -- comprehensive door feature test
3. [FindHandle.c](#findhandlec) -- user lookup with manual library init
4. [GetMainPort.c](#getmainportc) -- simplest standalone utility
5. [mainporttest.c](#mainporttestc) -- semaphore inspection, MPE access
6. [ShowTelnetHosts.c](#showtelnethostsc) -- binary file reading
7. [identdtest.c](#identdtestc) -- linked list traversal
8. [PortStatus.c](#portstatusc) -- port iteration and user status
9. [ShowIdleTime.c](#showidletimec) -- PortData field access
10. [mailsend.c](#mailsendc) -- mail outbox protocol

---

## empty.c

**What it demonstrates:** The minimal CNetC door skeleton. This is the
starting point for every new PFile -- copy it, fill in `DoorMain()`, and
build.

**Program type:** CNetC door (runs within a BBS port session).

### Anatomy

The file has a rigid two-part structure separated by a comment banner:

```
DO NOT ALTER ANYTHING ABOVE THIS LINE OR FUTURE COMPATIBILITY MAY BE SACRIFICED
```

**Above the line** (the boilerplate):

1. **Standard includes** -- `<stdlib.h>`, `<stdio.h>`, `<exec/types.h>`,
   `<dos/dos.h>`, `<cnet/cnet.h>`.

2. **Global extern declarations** -- Five variables that `CNCL_DoorInit()`
   (in `libcnet_door.a`) populates:

   | Variable | Type | Purpose |
   |----------|------|---------|
   | `myrdargs` | `struct RDArgs *` | DOS argument parsing (if used) |
   | `CNetCBase` | `struct Library *` | cnetc.library base pointer |
   | `context` | `struct CNetCContext *` | Internal context (opaque) |
   | `z` | `struct PortData *` | Per-port session data |
   | `myp` | `struct MainPort *` | System-wide shared data |

3. **`main()` function** -- Calls `CNCL_DoorInit()`, dispatches to
   `DoorMain()`, and calls `exit(RETURN_OK)`. If `CNCL_DoorInit()` fails
   (binary was run from CLI, not from CNet), it prints an error and exits.

4. **`GetOut()` cleanup** -- Calls `DoorExit()` (your cleanup) then
   `CNCL_DoorCleanup()` (library cleanup). Registered via `atexit()` or
   called from the exit trap.

**Below the line** (your code):

5. **`DoorExit()`** -- Place resource cleanup here (close libraries you
   opened, free allocations). Do NOT close `cnetc.library` -- the
   framework handles that.

6. **`DoorMain(char *DosArgs)`** -- Your door logic goes here. The
   `DosArgs` parameter is NULL if no DOS arguments were passed.

### Key patterns

- **CNCL_DoorInit / CNCL_DoorCleanup lifecycle**: The init function opens
  `cnetc.library`, calls `CNC_InitContext()`, and populates the global
  pointers. Cleanup shuts everything down. You never call these directly
  beyond the initial `main()` boilerplate.

- **Graceful fallback**: When run from CLI instead of CNet, the door
  prints `errtext` and exits cleanly rather than crashing.

### Building

```sh
make examples/empty
```

Or manually:

```sh
m68k-amigaos-gcc -noixemul -m68020 -O2 -Wall -Wno-pointer-sign \
    -Iinclude -Iinclude/cnet -o examples/empty examples/empty.c \
    -Llib -lcnet_door
```

### Usage

Copy to `PFiles:empty` on the Amiga. Configure a PFile/C subboard entry
pointing to that path. When a user enters the subboard, the door runs
(and immediately exits, since `DoorMain()` is empty).

---

## CNetCTest.c

**What it demonstrates:** A comprehensive test of major CNetC door
features -- user data access, date formatting, port enumeration, the
text editor, line input, single-key input, and multi-choice prompts.

**Program type:** CNetC door.

### Key patterns

**Opening additional libraries:**

```c
if (!(CNet4Base = OpenLibrary("cnet4.library", 4L)))
    exit(RETURN_FAIL);
```

The `cnet4.library` provides `CNetPrintDate()` for formatting CNet's
internal `IsDate` structures into human-readable strings. Additional
libraries are opened in `DoorMain()` and closed in `DoorExit()`.

**Accessing user data via `z->user1`:**

```c
sprintf(z->ABuffer, "   Your Handle: %s\n", z->user1.Handle);
CPutA();
sprintf(z->ABuffer, "Your Real name: %s\n", z->user1.RealName);
CPutA();
sprintf(z->ABuffer, "      Your age: %d\n\n", CalculateAge(&z->user1));
CPutA();
```

The `z->user1` field is the `UserData` struct for the connected user.
`CalculateAge()` is a linker library utility function.

**Date formatting with `CNetPrintDate()`:**

```c
CNetPrintDate(&z->user1.LastCall, date, NULL, NULL);
```

Converts an `IsDate` struct to a printable string. The last two NULLs
are optional alternate format parameters.

**Port enumeration with `IsPortLoaded()` / `IsPortOccupied()`:**

```c
for (zzz = 0; zzz < 100; zzz++) {
    if (IsPortLoaded(zzz)) {
        sprintf(z->ABuffer, "%3d: %s\n", (int)zzz,
            IsPortOccupied(zzz) ? myp->PortZ[zzz]->user1.Handle : "[IDLE]");
        CPutA();
    }
}
```

This is the standard pattern for iterating all active ports and checking
whether a user is logged in on each.

**Editor integration:**

```c
CLoadText(fh);             /* Load file contents into editor buffer */
CCallEditor(255, TRUE);    /* Let user edit (255 line limit, use existing buffer) */
CMakeEd(edfile);           /* Build editor filename for this port */
CReadFile(edfile, TRUE);   /* Display the edited result */
```

The editor workflow is: load content, open editor, retrieve edited file.
`CMakeEd()` generates the port-specific editor buffer filename
(`_edbuffN`).

**Line input with `CEnterLine()`:**

```c
if (CEnterLine(10, ELINE_INPUTBOX, "\nEnter 'YES' or 'NO': ")) {
    if (!stricmp(z->InBuffer, "YES"))
        /* ... */
}
```

Returns non-zero if the user entered text. The result is in
`z->InBuffer`. `ELINE_INPUTBOX` draws an ANSI input box.

**Single key input with `COneKey()`:**

```c
z->NoCaseConvert = TRUE;
key = COneKey();
z->NoCaseConvert = FALSE;
```

By default `COneKey()` returns uppercase. Set `z->NoCaseConvert` to
preserve the original case.

**Multi-choice with `CPutQMult()`:**

```c
choices[0] = vowels[zzz2];
key = CPutQMult(z->ABuffer, choices);
```

Returns the 1-based index of the character the user pressed from the
`choices` string.

### Building

```sh
make examples/CNetCTest
```

### Usage

This is an interactive test door. It displays user info, shows loaded
ports, optionally opens the editor with the main menu text, then runs
input exercises. Useful for verifying that all I/O subsystems work
correctly on a new SDK installation.

---

## FindHandle.c

**What it demonstrates:** Looking up a user account by handle using
`FindHandle()` from `cnet.library`, with manual library initialization
(opening `cnet.library` directly rather than relying on wrapper
functions).

**Program type:** CNetC door.

### Key patterns

**Manual cnet.library init:**

```c
if (!(CNetBase = OpenLibrary("cnet.library", 4)))
    exit(RETURN_FAIL);
```

Unlike CNetCTest (which uses `cnet4.library`), this example opens
`cnet.library` directly. This is the core library containing user
database functions like `FindHandle()`.

**User lookup with `FindHandle()`:**

```c
short index;
if (FindHandle(&index, handlestr, atoi(z->InBuffer))) {
    sprintf(z->ABuffer, "\nindex: %d\n", index);
    CPutA();
    sprintf(z->ABuffer, "\nmyp->Key[ %d ].Handle: %s\n",
        (int)(myp->IName[index] - 1),
        myp->Key[myp->IName[index] - 1].Handle);
    CPutA();
}
```

`FindHandle()` takes a pointer to a short (receives the index), the
handle string to search for, and an optional serial ID. On success, it
returns non-zero and sets `index`. The index maps through `myp->IName[]`
to the user key array `myp->Key[]`, which contains the handle and other
search fields.

**User input for search parameters:**

```c
CEnterLine(21, 0, "Enter handle to find: ");
strcpy(handlestr, z->InBuffer);
CEnterLine(4, 0, "Enter optional serial id: ");
```

Note the pattern of copying `z->InBuffer` to a local buffer between
`CEnterLine()` calls, since each call overwrites `InBuffer`.

### Building

```sh
make examples/FindHandle
```

### Usage

Run from a CNet door. Prompts for a handle and optional serial ID, then
displays the index and key data if found. Demonstrates the user database
lookup path: `FindHandle()` -> `myp->IName[]` -> `myp->Key[]`.

---

## GetMainPort.c

**What it demonstrates:** The simplest possible standalone CNet utility.
Finds the CNet MainPort message port and confirms it exists.

**Program type:** Standalone CLI utility (no door framework, no
cnetc.library).

### Key patterns

**FindPort pattern:**

```c
struct MainPort *myp;
if (myp = (struct MainPort *)FindPort("cnetport")) {
    printf("MainPort found!\n");
}
```

This is the fundamental pattern for any standalone program that needs to
access CNet's shared data. The Exec `FindPort()` function looks up the
named message port that Control publishes at startup. The returned
pointer is cast to `struct MainPort *`, giving access to all system-wide
BBS state.

The port name `"cnetport"` is defined as the `CNETPORT` constant in the
SDK headers.

**No door boilerplate:** This program has no `CNCL_DoorInit()`, no
`CNCL_DoorCleanup()`, no `PortData *z`. It uses standard `printf()`
for output because it runs from an Amiga CLI shell, not within a BBS
session.

**Minimal includes:**

```c
#include <cnet/cnet.h>
#undef __asm
#include <proto/exec.h>
```

The `#undef __asm` after `<cnet/cnet.h>` restores GCC's `__asm` keyword
so that `<proto/exec.h>` inline stubs work correctly. This is the
standard pattern for standalone utilities that need both CNet struct
definitions and Exec library calls.

### Building

```sh
make examples/GetMainPort
```

### Usage

Run from an Amiga CLI while CNet is running:

```
1> GetMainPort
MainPort found!
```

If CNet is not running, the `FindPort()` call returns NULL and the
program prints an error. This is useful as a quick health check or as a
starting point for any standalone CNet utility.

---

## mainporttest.c

**What it demonstrates:** Accessing the MainPort Extension (MPE) and
working with Exec semaphores. Examines the mail semaphore array, tests
semaphore acquisition, and displays internal pointer addresses.

**Program type:** Standalone CLI utility.

### Key patterns

**MPE access:**

```c
Printf("MPE address     : 0x%lx\n", (unsigned long)myp->MPE);
Printf("MailSem Var     : 0x%lx\n", (unsigned long)&myp->MPE->MailSem);
```

The MainPort Extension (`myp->MPE`) holds data structures added after
v3, including the mail semaphore array, server task state, and network
configuration. It is always valid when `FindPort("cnetport")` succeeds.

**Semaphore inspection:**

```c
if (AttemptSemaphore(&myp->MPE->MailSem[account - 1])) {
    Printf("   (Semaphore attempable)\n");
    Printf("      Owner task (temp): 0x%lx\n",
        (unsigned long)myp->MPE->MailSem[account - 1].ss_Owner);
    ReleaseSemaphore(&myp->MPE->MailSem[account - 1]);
}
```

Demonstrates the Exec semaphore API:
- `AttemptSemaphore()` -- non-blocking try. Returns TRUE if obtained.
- `ObtainSemaphore()` -- blocking acquire.
- `ReleaseSemaphore()` -- release.

The `ss_Owner` field shows which task currently holds the semaphore,
useful for debugging lock contention between BBS ports.

**Command-line arguments:**

```c
if (argc > 1) {
    account = atoi(argv[1]);
    /* inspect semaphore for that account */
}
```

Without arguments, just displays addresses. With an account number
argument, performs the semaphore test for that account's mail semaphore.

### Building

```sh
make examples/mainporttest
```

### Usage

```
1> mainporttest
MainPort address: 0x12345678
MPE address     : 0x12345abc
...

1> mainporttest 2
   Examining mail semaphore for account 2..
```

Useful for debugging mail delivery issues or semaphore contention.

---

## ShowTelnetHosts.c

**What it demonstrates:** Reading binary data structures from a CNet data
file. Reads the `bbs.tnhost` file and displays telnet host entries.

**Program type:** Standalone CLI utility.

### Key patterns

**Binary struct I/O:**

```c
struct TNHostEntry he;
BPTR fh;

if (fh = Open(tnhostfile, MODE_OLDFILE)) {
    while (Read(fh, &he, sizeof(struct TNHostEntry))) {
        sprintf(hoststring, "%s:%d", he.he_ip, he.he_port);
        printf("\n%2d. %-24.24s %-20.20s %s",
            i, he.he_system, hoststring, he.he_data);
    }
    Close(fh);
}
```

This is the standard pattern for reading CNet binary data files:
1. Open the file with `dos.library` `Open()`.
2. Read fixed-size struct records with `Read()` in a loop.
3. `Read()` returns the number of bytes read; 0 at EOF terminates the loop.
4. Close with `Close()`.

The `TNHostEntry` struct (defined in `include/cnet/ports.h`) contains the
host name, IP address, port number, and description for each configured
telnet destination.

**No MainPort needed:** Unlike most CNet utilities, this program reads a
data file directly from disk rather than accessing shared memory. It
works whether or not CNet is running.

### Building

```sh
make examples/ShowTelnetHosts
```

### Usage

```
1> ShowTelnetHosts
 1. Some BBS              192.168.1.100:6900   A cool BBS
 2. Another System        10.0.0.5:23          Another system
```

Reads from `sysdata:bbs.tnhost`. Useful as a template for any program
that needs to read CNet's binary configuration files.

---

## identdtest.c

**What it demonstrates:** Traversing a linked list in CNet's shared
memory. Walks the identd (RFC 1413) data chain from the MainPort
Extension.

**Program type:** CNetC door.

### Key patterns

**Linked list traversal:**

```c
if (myp->MPE->idd) {
    struct IdentdData *iddptr = myp->MPE->idd;
    while (iddptr) {
        sprintf(z->ABuffer, "Socket number: %u, CNet Owner Port: %d\n",
            iddptr->socket, iddptr->port);
        CPutA();
        iddptr = iddptr->next;
    }
}
```

CNet uses singly-linked lists (NULL-terminated) for several runtime data
structures. The pattern is always:
1. Check for NULL head pointer.
2. Walk via `->next` until NULL.

The `IdentdData` struct tracks which TCP socket belongs to which CNet
port, used for RFC 1413 ident protocol responses.

**MPE access from a door:** This example accesses `myp->MPE->idd` from
within a door context. The `myp` pointer is set up by `CNCL_DoorInit()`,
so all MainPort data (including the extension) is available.

### Building

```sh
make examples/identdtest
```

### Usage

Run from a CNet door. Displays all active identd entries showing socket
numbers and their owning BBS ports, or "No identd entries" if the list
is empty. Useful for debugging network identity issues.

---

## PortStatus.c

**What it demonstrates:** Iterating all loaded BBS ports, checking
whether each is occupied, and displaying user information for active
sessions.

**Program type:** CNetC door.

### Key patterns

**Port iteration with status checking:**

```c
for (zzz = 0; zzz < 100; zzz++) {
    sprintf(z->ABuffer, "%4d: %s\n", (int)zzz,
        IsPortLoaded(zzz) ? "LOADED" : "Not loaded");
    CPutA();
    if (IsPortLoaded(zzz)) {
        sprintf(z->ABuffer, "user: %s\n",
            IsPortOccupied(zzz) ? myp->PortZ[zzz]->user1.Handle : "[idle]");
        CPutA();
    }
}
```

Two utility functions from `libcnet_door.a`:
- `IsPortLoaded(short portnum)` -- Returns TRUE if the port process is
  running. A port's PortData exists only when loaded.
- `IsPortOccupied(short portnum)` -- Returns TRUE if a user is logged in
  on that port.

When a port is loaded and occupied, `myp->PortZ[zzz]->user1.Handle`
gives the logged-in user's handle.

**Cross-port data access:** This demonstrates reading another port's
`PortData` through the `myp->PortZ[]` array. Each port has its own
`PortData`, but all are accessible from any context through MainPort.
This is the basis for WHO lists, inter-port messaging, and sysop
monitoring tools.

**Additional libraries:** Like CNetCTest, this example opens
`cnet4.library` for the `CalculateAge()` function and displays user age
alongside port status.

### Building

```sh
make examples/PortStatus
```

### Usage

Run from a CNet door. Displays all 100 possible port slots, showing
which are loaded and which have active users. More verbose than a
typical WHO command -- useful for debugging port configuration.

---

## ShowIdleTime.c

**What it demonstrates:** Accessing per-port `PortData` fields from a
standalone utility. Reads the idle timer for a specific port.

**Program type:** Standalone CLI utility.

### Key patterns

**Command-line port selection:**

```c
if (argc != 2) {
    printf("\nUsage: %s <port number>\n\n", argv[0]);
    exit(0);
}
portnum = atoi(argv[1]);
```

**Checking port validity:**

```c
if (myp->PortZ[portnum] == myp->z0) {
    printf("\n%s: port not loaded.\n\n", argv[0]);
    exit(0);
}
```

This is the standalone equivalent of `IsPortLoaded()`. Unloaded ports
have their `PortZ[]` entry pointing to `myp->z0` (a default/null
PortData), not NULL. Comparing against `z0` is the correct check.

**PortData field access:**

```c
if (myp->PortZ[portnum]->TimeIdle > 600)
    printf("Port %d idle %d.%d hours.\n", portnum,
        myp->PortZ[portnum]->TimeIdle / 600,
        (myp->PortZ[portnum]->TimeIdle % 600) / 60);
else
    printf("Port %d idle %d.%d minutes.\n", portnum,
        myp->PortZ[portnum]->TimeIdle / 10,
        myp->PortZ[portnum]->TimeIdle % 10);
```

`TimeIdle` is in tenths of minutes. The code converts to hours or
minutes depending on the value. This pattern of accessing another port's
fields through `myp->PortZ[n]->fieldname` works for any PortData field.

### Building

```sh
make examples/ShowIdleTime
```

### Usage

```
1> ShowIdleTime 0
Port 0 idle 2.3 minutes.

1> ShowIdleTime 1
ShowIdleTime: port not loaded.
```

Simple diagnostic utility. Demonstrates the pattern for any standalone
tool that needs to inspect per-port BBS state.

---

## mailsend.c

**What it demonstrates:** Constructing and saving a mail message directly
to CNet's mail outbox, bypassing the interactive mail composition
interface. Shows the outbox file protocol (`.cmh` header files and
`.mtl` recipient list files).

**Program type:** Code fragment (uncompiled reference, not a complete
program).

**Note:** This example is marked as "uncompiled and untested" in the
original SDK and contains syntax errors (`strcpy(mtl.UUCP, "SYSOP)` is
missing a closing quote; `OMail0.Subject` should be `OMail.Subject`;
`tempmailto = mailto;` references an undefined variable; missing
semicolons on `while` statements). It serves as a reference for the mail
outbox protocol, not as working code.

### Mail outbox protocol

Mail delivery in CNet uses a file-based spooling system in the
`CNETOUTBOX` directory (typically `Mail:outbox/`):

1. **Create the outbox directory** if it does not exist.

2. **Write the `.cmh` (CNet Mail Header) file:**
   - Fill in a `MailHeader4` struct with subject, date, sender/recipient
     IDs, message length, and internal date.
   - Generate a unique filename using the timestamp (e.g.,
     `Mail:outbox/XXXXXXXX.cmh`).
   - Write the `MailHeader4` struct followed by the message text body.

3. **Write the `.mtl` (Mail To List) file:**
   - First write to a `.mtt` (temporary) file -- the mail task only
     watches for `.mtl` files, so writing to a temp extension prevents
     a race condition.
   - The file contains: the `.cmh` header filename (as a text line),
     followed by one or more `MailToList` structs specifying recipients.
   - Rename the `.mtt` file to `.mtl`. The rename is atomic on AmigaOS,
     so the mail task only sees complete files.

4. **The mail task picks up the `.mtl` file**, reads the referenced
   `.cmh` file, and delivers the message to each recipient's mailbox.

### Key data structures

```c
struct MailHeader4 OMail;  /* Message metadata */
struct MailToList  mtl;    /* Recipient entry */
```

The `MailToList.mailtype` field controls delivery routing:
- `MAIL_TO_SYSOP` -- deliver to sysop
- Other values for regular users, UUCP addresses, etc.

### Usage notes

To use this pattern in a real door, fix the syntax errors and ensure:
- `FromAccount`, `fromid`, `toid` are set to valid account/ID values.
- `CNetTime()` returns the CNet internal timestamp.
- `FileExists()` is available from `cnet.library`.
- Proper error handling for `Open()` failures.

---

## Common Patterns Summary

### Door program structure

```
main() -> CNCL_DoorInit() -> DoorMain() -> exit() -> GetOut()
```

### Output to user

```c
/* Simple text */
CPutText("Hello\n");

/* Formatted text */
sprintf(z->ABuffer, "Value: %d\n", value);
CPutA();
```

### Input from user

```c
/* Line input */
CEnterLine(maxlen, flags, "Prompt: ");
/* result in z->InBuffer */

/* Single key */
char key = COneKey();

/* Yes/No */
if (CPutQ("Continue? [Y/n] ")) { /* yes */ }
```

### Port inspection (from standalone utility)

```c
struct MainPort *myp = (struct MainPort *)FindPort(CNETPORT);
if (myp && myp->PortZ[n] != myp->z0) {
    /* port n is loaded -- access myp->PortZ[n]->fieldname */
}
```

### Port inspection (from door)

```c
if (IsPortLoaded(n)) {
    if (IsPortOccupied(n)) {
        /* myp->PortZ[n]->user1.Handle has the user's name */
    }
}
```

### Additional library access

```c
/* Open in DoorMain() */
struct Library *CNet4Base = NULL;
if (!(CNet4Base = OpenLibrary("cnet4.library", 4)))
    exit(RETURN_FAIL);

/* Close in DoorExit() */
if (CNet4Base) {
    CloseLibrary(CNet4Base);
    CNet4Base = NULL;
}
```

### Binary file reading

```c
BPTR fh;
struct SomeStruct record;
if (fh = Open("path", MODE_OLDFILE)) {
    while (Read(fh, &record, sizeof(struct SomeStruct)))
        /* process record */;
    Close(fh);
}
```
