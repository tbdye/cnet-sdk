# PFile Development Guide

This guide covers everything you need to write CNet 5 PFiles (doors) using the modernized SDK. It assumes you have read `docs/getting-started.md` and can build and deploy a basic door.

## What is a PFile

A PFile ("Program File") is an external program that CNet launches when a user accesses a door subboard. PFiles extend the BBS with custom functionality -- games, utilities, database lookups, chat systems, file managers, or anything else that interacts with the connected user.

CNet supports several door types (identified by the subboard Marker field):

| Marker | Constant | Type |
|---|---|---|
| 5 | `MRK_CNETC_DOOR` | C-language PFile (this guide) |
| 6 | `MRK_AREXX_DOOR` | ARexx script door |
| 7 | `MRK_ADOS_DOOR` | AmigaDOS script door |

C-language PFiles are native AmigaOS executables. They communicate with the BBS process through either the cnetc.library shared library (the CNetC pattern) or raw CMessage IPC (the v3 pattern). This guide covers both approaches.

## Door Lifecycle

Every C-language PFile follows the same lifecycle:

1. **CNet launches the door.** When a user enters a PFile subboard, CNet creates a `CPort` message port and runs the door binary, passing the CPort name as `argv[1]`.

2. **The door locates the CPort.** It calls `FindPort(argv[1])` (wrapped by `CNCL_DoorInit()` in the CNetC pattern) to find the message port that CNet created.

3. **The door opens its communication channel.**
   - CNetC pattern: Opens `cnetc.library`, calls `CNC_InitContext()`, which sets up internal CMessage handling and returns pointers to the per-port data (`z`) and system-wide data (`myp`).
   - Raw pattern: Creates a reply port, initializes a `CMessage` structure, and accesses `z` and `myp` directly from the CPort.

4. **The door does its work.** It interacts with the user (text output, input prompts, file operations) and accesses BBS data structures as needed.

5. **The door shuts down.** It calls `CNC_ShutDown()` (or sends `CMESS_CMD_SHUTDOWN`) to tell CNet the door is finished, cleans up its resources, and exits. The `atexit()` mechanism handles this automatically in the CNetC pattern.

6. **CNet returns the user to the BBS.** The user is back at the menu they came from, or CNet chain-runs another program if `CSpawn` was set.

## The CMessage Protocol

All door-to-BBS communication uses the `CMessage` structure (defined in `include/cnet/doors.h`):

```c
struct CMessage {
    struct Message cn_Message;  // Exec message header
    ULONG  arg1;                // Command argument 1
    ULONG  arg2;                // Command argument 2
    ULONG  arg3;                // Command argument 3
    ULONG  arg4;                // Command argument 4
    ULONG  result;              // Result from CNet
    UBYTE  command;             // Command code (CMESS_CMD_*)
    UBYTE  align[3];
};
```

The protocol works like this:

1. The door sets `cmess.command` to a `CMESS_CMD_*` value and fills in the `arg1`-`arg4` fields as needed by that command.
2. The door sends the CMessage to the CPort via `PutMsg()`.
3. The door waits for the reply via `WaitPort()` on its reply port.
4. CNet processes the command (displaying text, reading input, etc.) and replies.
5. The door reads `cmess.result` for the return value.

This is a synchronous request/reply protocol. The door blocks while CNet processes each command.

### Key command codes

| Constant | Value | Purpose | arg fields |
|---|---|---|---|
| `CMESS_CMD_SHUTDOWN` | 0 | Door is finished | (none) |
| `CMESS_CMD_PUTTEXT` | 1 | Display text to user | arg1 = text pointer |
| `CMESS_CMD_ENTERLINE` | 2 | Line input | arg1 = max len, arg2 = ELINE_* flags, arg3 = prompt |
| `CMESS_CMD_ONEKEY` | 3 | Single keypress | result = key char |
| `CMESS_CMD_ENTERPASS` | 4 | Password input (masked) | arg1 = max len |
| `CMESS_CMD_CCOMMANDS` | 5 | Process common commands | result = command code |
| `CMESS_CMD_READFILE` | 6 | Display file to user | arg1 = path, arg2 = flags |
| `CMESS_CMD_SETDOING` | 7 | Set "Doing" status | arg1 = text pointer |
| `CMESS_CMD_CALLEDITOR` | 8 | Open text editor | arg1 = max chars, arg2 = initial lines |
| `CMESS_CMD_READGRAPHICS` | 9 | Display ANSI art file | arg1 = path, arg2 = flags |
| `CMESS_CMD_READACCOUNT` | 11 | Read user account | arg1 = account #, arg2 = UserData pointer |
| `CMESS_CMD_SAVEACCOUNT` | 12 | Save user account | arg1 = UserData pointer, arg2 = account # |
| `CMESS_CMD_WAITFORINPUT` | 31 | Wait for input with timeout | arg1 = microseconds |
| `CMESS_CMD_CHECKOLMS` | 63 | Check for OLMs | result = OLM status |
| `CMESS_CMD_PUTQMULT` | 64 | Multi-choice prompt | arg1 = prompt, arg2 = choices string |
| `CMESS_CMD_CHECKCARRIER` | 65 | Check connection alive | result = carrier status |
| `CMESS_CMD_MAILSEND` | 66 | Send mail to user | arg1 = account # |

The full list of command codes is in `include/cnet/doors.h`.

## cnetc.library vs Raw CMessage

There are two patterns for writing doors. Choose based on your needs:

### CNetC pattern (recommended for new doors)

Uses `cnetc.library` and `libcnet_door.a`. The library wraps the CMessage protocol behind clean C function calls.

**Advantages:**
- Simpler code -- no manual message port or CMessage setup
- `CNCL_DoorInit()` / `CNCL_DoorCleanup()` handle all lifecycle management
- `atexit()` ensures clean shutdown even on unexpected exit
- Richer API (66 functions covering I/O, files, mail, VDE, etc.)

**Disadvantages:**
- Requires `cnetc.library` (v4+) in `LIBS:` on the Amiga
- Slightly larger binary (links against `libcnet_door.a`)

Template: `templates/pfile_cnetc.c`

### Raw CMessage pattern (v3-style)

Communicates directly with CNet via `PutMsg()`/`WaitPort()` on the CPort. No shared library dependency beyond Exec.

**Advantages:**
- No runtime library dependency beyond AmigaOS itself
- Full control over the message-passing protocol
- Suitable for porting existing v3 doors

**Disadvantages:**
- More boilerplate (port creation, message setup, version handshake)
- Fewer convenience functions -- you implement what you need
- Must manually handle cleanup and shutdown

Template: `templates/pfile_raw.c`

### Which to choose

Use the CNetC pattern unless you have a specific reason not to. The raw pattern is primarily useful when porting existing v3 doors or when you want minimal dependencies.

## The `__asm` Conflict

This is the most important GCC-specific issue to understand when writing doors.

### The problem

CNet's master header `<cnet/cnet.h>` redefines `__asm` to nothing:

```c
#ifdef __GNUC__
#ifndef __asm
#define __asm
#endif
#endif
```

This is needed because the original SAS/C CNet headers use `__asm` as a function qualifier (meaning "use register parameters"), which is not valid in GCC.

However, GCC's AmigaOS NDK headers use `__asm` as the GCC keyword `__asm__` for register binding in inline library stubs. The `LP*` macros in `<inline/macros.h>` generate code like:

```c
register type __res __asm("d0");
register type arg __asm("a1") = value;
```

If `__asm` is redefined to nothing, these inline stubs break silently -- register bindings are lost, and function calls pass arguments in the wrong registers, causing crashes.

### The conflict

You cannot include both `<cnet/cnet.h>` (which kills `__asm`) and `<proto/cnetc.h>` (which needs `__asm`) in the same translation unit. This means you cannot call `CNC_*` functions directly in files that also access CNet struct definitions via `<cnet/cnet.h>`.

### Solution 1: C* wrapper functions (recommended)

`libcnet_door.a` provides wrapper functions with a `C` prefix. Each wrapper is compiled in a separate file that includes `<proto/cnetc.h>` before any CNet headers, so `__asm` is intact for the inline stubs.

Your door code includes `<cnet/cnet.h>` for struct access and calls the `C*` wrappers:

```c
#include <cnet/cnet.h>   /* struct access -- kills __asm */

/* Call wrappers, not CNC_* directly */
CPutText("Hello\n");
CEnterLine(40, 0, "Prompt: ");
```

Complete wrapper mapping (from `lib/cdoor_wrappers.c`):

| Wrapper | Library function |
|---|---|
| `CPutText(text)` | `CNC_PutText(text)` |
| `CPutA()` | `CNC_PutA()` |
| `COneKey()` | `CNC_OneKey()` |
| `CEnterLine(len, flags, prompt)` | `CNC_EnterLine(len, flags, prompt)` |
| `CEnterPassword(len)` | `CNC_EnterPassword(len)` |
| `CCommonCommands()` | `CNC_CommonCommands()` |
| `CReadFile(path, flags)` | `CNC_ReadFile(path, flags)` |
| `CSetDoing(what)` | `CNC_SetDoing(what)` |
| `CCallEditor(max, inlines)` | `CNC_CallEditor(max, inlines)` |
| `CReadGraphics(path, flags)` | `CNC_ReadGraphics(path, flags)` |
| `CMakeDate(date, output)` | `CNC_MakeDate(date, output)` |
| `CReadAccount(id, user)` | `CNC_ReadAccount(id, user)` |
| `CSaveAccount(user, id)` | `CNC_SaveAccount(user, id)` |
| `CAddCharge(n, a)` | `CNC_AddCharge(n, a)` |
| `CCheckBalance(n, a)` | `CNC_CheckBalance(n, a)` |
| `CEnterText(firstchar, maxchars, perline, maxlines)` | `CNC_EnterText(...)` |
| `CConferenceWait(a)` | `CNC_ConferenceWait(a)` |
| `CCheckChanges()` | `CNC_CheckChanges()` |
| `CConvertAccess(s)` | `CNC_ConvertAccess(s)` |
| `CGetFree(s, q)` | `CNC_GetFree(s, q, NULL, NULL)` |
| `CFindAccount(a, b, quiet)` | `CNC_FindAccount(a, b, quiet)` |
| `CCheckFlowControl()` | `CNC_CheckFlowControl()` |
| `CListDir(a, b, c)` | `CNC_ListDir(a, b, c)` |
| `CRnext()` | `CNC_Rnext()` |
| `CParseCommandLine(numargs)` | `CNC_ParseCommandLine(numargs)` |
| `CFindCommand(num)` | `CNC_FindCommand(num)` |
| `CReadMessagePoint(a, b)` | `CNC_ReadMessagePoint(a, b)` |
| `CEditMessage(file)` | `CNC_EditMessage(file)` |
| `CLoadText(fh)` | `CNC_LoadText(fh)` |
| `CWaitForInput(mics)` | `CNC_WaitForInput(mics)` |
| `CSelectAndDownload(file, flags)` | `CNC_SelectAndDownload(file, flags)` |
| `CVisualDataEditor(file, data, size)` | `CNC_VisualDataEditor(file, data, size)` |
| `CExtSetMinFree(free)` | `CNC_ExtSetMinFree(free)` |
| `CExtSetProtocol(a)` | `CNC_ExtSetProtocol(a)` |
| `CExtDownload(args)` | `CNC_ExtDownload(args)` |
| `CExtUpload(args)` | `CNC_ExtUpload(args)` |
| `CPutQ(a)` | `CNC_PutQ(a)` |
| `CDoReturn()` | `CNC_DoReturn()` |
| `CWriteUKeys(getsem)` | `CNC_WriteUKeys(getsem)` |
| `CDoANSI(n, a, b)` | `CNC_DoANSI(n, a, b)` |
| `CDoANSIOut(n)` | `CNC_DoANSIOut(n)` |
| `CCheckOLM()` | `CNC_CheckOLM()` |
| `CGrabFileSize(quiet)` | `CNC_GrabFileSize(quiet)` |
| `CSaveUser(user, id)` | `CNC_SaveUser(user, id)` |
| `CPutQMult(prompt, poss)` | `CNC_PutQMult(prompt, poss)` |
| `CCheckCarrier()` | `CNC_CheckCarrier()` |
| `COWriteLog(n, text1, text2)` | `CNC_OWriteLog(n, text1, text2)` |
| `CVDEapply(data0, data1)` | `CNC_VDEapply(data0, data1)` |
| `CVDEclose()` | `CNC_VDEclose()` |
| `CEnterAccess(prompt, def)` | `CNC_EnterAccess(prompt, def)` |
| `CShutDown(spawn)` | `CNC_ShutDown(spawn)` |
| `CallHost(c)` | `CNC_CallHost(c)` |

Note: `CGetFree()` has a simplified signature (2 args) compared to `CNC_GetFree()` (4 args). The wrapper passes NULL for the extra `deststring` and `bm` parameters.

### Solution 2: `#undef __asm` and manual include ordering

If you need to call `CNC_*` functions directly (bypassing the wrappers), you can manage the include ordering yourself:

```c
/* 1. Include system protos FIRST -- __asm is a GCC keyword here */
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/cnetc.h>   /* CNC_* inline stubs need __asm */

/* 2. Define SAS/C compat macros WITHOUT __asm */
#ifdef __GNUC__
#ifndef __saveds
#define __saveds __attribute__((saveds))
#endif
#ifndef __regargs
#define __regargs
#endif
#ifndef __stdargs
#define __stdargs
#endif
#endif

/* 3. Include CNet struct headers individually (NOT <cnet/cnet.h>) */
#include <cnet/io.h>
#include <cnet/dates.h>
#include <cnet/users.h>
#include <cnet/subs.h>
#include <cnet/doors.h>
#include <cnet/ports.h>
/* ... add others as needed in dependency order */
```

This pattern is used by `templates/pfile_raw.c` and `lib/door_init.c`. It works but is more fragile -- you must track header dependencies manually and avoid ever including `<cnet/cnet.h>`.

## User I/O

### Text output

**`CPutText(char *text)`** -- Send text to the connected user. Supports CNet MCI codes for colors and formatting:

```c
CPutText("Hello, world!\n");
CPutText("c1This is red.c0 This is default.\n");
```

**`CPutA()`** -- Send the contents of `z->ABuffer` to the user. Use `sprintf()` to format into ABuffer first:

```c
sprintf(z->ABuffer, "You have %ld credits remaining.\n", credits);
CPutA();
```

The ABuffer is dynamically allocated at port startup with a configurable size (set in CNet's Config/Limits). Do not assume a fixed size. For safety, use `snprintf()` or check your string lengths.

### Line input

**`CEnterLine(UBYTE len, ULONG flags, char *prompt)`** -- Prompt the user for a line of text. Returns the length of the input. The entered text is placed in `z->InBuffer` (256 bytes).

```c
if (CEnterLine(40, 0, "Enter your name: ")) {
    sprintf(z->ABuffer, "Hello, %s!\n", z->InBuffer);
    CPutA();
}
```

The `flags` parameter controls input behavior using `ELINE_*` constants from `include/cnet/io.h`:

| Flag | Value | Effect |
|---|---|---|
| `ELINE_NONE` | 0x0 | Default behavior |
| `ELINE_ALLCAPS` | 0x00001 | Convert input to uppercase |
| `ELINE_FILENAME` | 0x00002 | Filename-safe characters only |
| `ELINE_USEINBUFF` | 0x00004 | Pre-fill with current InBuffer contents |
| `ELINE_NOLEADSPACE` | 0x00008 | Strip leading spaces |
| `ELINE_CAPWORDSTART` | 0x00010 | Capitalize first letter of each word |
| `ELINE_RESTWORDLOWER` | 0x00020 | Lowercase non-initial letters |
| `ELINE_NUMBERSONLY` | 0x00040 | Accept digits only |
| `ELINE_INPUTBOX` | 0x00080 | Use ANSI input box |
| `ELINE_ALLOWMCI` | 0x00100 | Allow MCI codes in input |
| `ELINE_NOOLMS` | 0x01000 | Suppress OLM delivery during input |
| `ELINE_ALLOWCHAT` | 0x02000 | Allow chat request during input |
| `ELINE_NOSPACES` | 0x04000 | Reject spaces |
| `ELINE_ALPHAONLY` | 0x40000 | Alphabetic characters only |

Combine flags with bitwise OR: `CEnterLine(20, ELINE_ALLCAPS | ELINE_NOSPACES, "Code: ")`.

### Single keypress

**`COneKey()`** -- Wait for a single keypress and return it.

```c
CPutText("Press any key to continue...");
char key = COneKey();
```

By default, the returned character is converted to uppercase. Set `z->NoCaseConvert = 1` before calling `COneKey()` to preserve case, and reset it after.

### Yes/No prompts

**`CPutQ(char *prompt)`** -- Display a yes/no prompt. Returns non-zero for "yes", zero for "no".

```c
if (CPutQ("Delete this item? [Y/n] ")) {
    /* user said yes */
}
```

### Multi-choice prompts

**`CPutQMult(char *prompt, char *possibilities)`** -- Display a prompt with multiple choices. The `possibilities` string lists the valid key characters. Returns the 1-based index of the selected character.

```c
/* choices: A=1, B=2, C=3 */
UBYTE choice = CPutQMult("Select [A/B/C]: ", "ABC");
switch (choice) {
    case 1: /* A selected */ break;
    case 2: /* B selected */ break;
    case 3: /* C selected */ break;
}
```

### Password input

**`CEnterPassword(UBYTE len)`** -- Like `CEnterLine()` but input is masked. Result goes into `z->InBuffer`.

```c
CEnterPassword(20);
if (strcmp(z->InBuffer, expected_password) == 0) {
    /* password correct */
}
```

### Timed input

**`CWaitForInput(long mics)`** -- Wait for user input with a timeout in microseconds. Returns the key character, or 0 if the timeout expired.

```c
/* Wait up to 5 seconds */
char key = CWaitForInput(5000000);
if (key) {
    /* user pressed a key */
} else {
    /* timeout */
}
```

## File I/O

### Display a text file

**`CReadFile(char *path, UBYTE flags)`** -- Display a text file to the user, processing MCI codes. Returns non-zero on success.

```c
CReadFile("cnet:text/welcome.txt", 0);
```

### Display ANSI art

**`CReadGraphics(char *path, char flags)`** -- Display an ANSI art file. Unlike `CReadFile`, this sends raw bytes suitable for ANSI terminal rendering.

```c
CReadGraphics("cnet:ansi/logo.ans", 0);
```

### Text editor

**`CCallEditor(short maxchars, short inlines)`** -- Launch the CNet text editor. The user can write and edit multi-line text. After the editor closes, the text is accessible via the editor buffer.

```c
CCallEditor(4000, 0);  /* 4000 char limit, no initial text */
```

**`CEnterText(char firstchar, short maxchars, short perline, short maxlines)`** -- A simpler text entry mode. Returns the result status.

### File size

**`CGrabFileSize(UBYTE quiet)`** -- Get the size of a file. The file path is read from `z->InBuffer`. Returns the file size in bytes.

## User Data Access

### The current user

The `z->user1` field contains the `UserData` structure for the currently connected user. This is a copy of the user's account data loaded at login.

```c
sprintf(z->ABuffer, "Welcome, %s!\n", z->user1.Handle);
CPutA();

/* Check user's access level */
if (z->user1.Access >= 5) {
    CPutText("You have sysop access.\n");
}

/* Check user privileges */
if (z->user1.MyPrivs.ABits & SYSOP_FLAG) {
    CPutText("You are a sysop.\n");
}
```

Key `UserData` fields (from `include/cnet/users.h`):

| Field | Type | Description |
|---|---|---|
| `Handle[21]` | char[] | User's handle/nickname |
| `RealName[26]` | char[] | Real name |
| `Address[31]` | char[] | Street address |
| `CityState[31]` | char[] | City and state |
| `PhoneNo[17]` | char[] | Data phone number |
| `VoiceNo[17]` | char[] | Voice phone number |
| `Comments[34]` | char[] | User comments |
| `IDNumber` | long | Unique ID (serial, never reused) |
| `Access` | BYTE | Current access group (0-31) |
| `TotalCalls` | long | Lifetime call count |
| `PubMessages` | long | Public messages posted |
| `UpBytes` | long | Kilobytes uploaded |
| `DownBytes` | long | Kilobytes downloaded |
| `Balance` | long | Account balance (credits) |
| `DoorPoints` | long | Door game points |
| `MyPrivs` | struct Privs | User's privilege flags |

### Reading and saving other accounts

**`CReadAccount(short id, struct UserData *user)`** -- Read an account by number into a UserData buffer. Returns non-zero on success.

**`CSaveAccount(struct UserData *user, short id)`** -- Save a UserData buffer back to the account file. Returns non-zero on success.

```c
struct UserData other;
if (CReadAccount(42, &other)) {
    sprintf(z->ABuffer, "Account 42 is %s\n", other.Handle);
    CPutA();
    other.DoorPoints += 100;
    CSaveAccount(&other, 42);
}
```

### Account locking

For concurrent-safe access to user accounts (critical when multiple ports are active), use the locking functions from `cnet.library`:

**`LockAccount(short acc)`** -- Lock an account and return a pointer to its UserData. Returns NULL if the lock could not be obtained. The returned pointer is valid until `UnLockAccount()` is called.

**`UnLockAccount(short acc, UBYTE save)`** -- Unlock a previously locked account. If `save` is non-zero, changes made through the locked pointer are saved to disk.

```c
struct UserData *locked = LockAccount(42);
if (locked) {
    locked->DoorPoints += 100;
    UnLockAccount(42, 1);  /* save=1: write changes */
}
```

Always unlock accounts promptly. Holding a lock blocks other ports from accessing that account.

### Finding accounts

**`CFindAccount(char *name, struct UserData *buf, UBYTE quiet)`** -- Search for a user by handle or name. Returns the account number, or -1 if not found.

```c
struct UserData found;
short acct = CFindAccount("SomeUser", &found, 1);
if (acct >= 0) {
    sprintf(z->ABuffer, "Found: %s (account %d)\n", found.Handle, acct);
    CPutA();
}
```

### Input and output buffers

- **`z->InBuffer`** (256 bytes) -- Contains the result of the last `CEnterLine()` or `CEnterPassword()` call. This is where user input goes.

- **`z->ABuffer`** (dynamically allocated) -- General-purpose output buffer. Format your text here with `sprintf()`, then call `CPutA()` to send it to the user. The buffer size is configurable in CNet's Config/Limits. Do not hardcode a size assumption.

- **`z->GBuffer[16][81]`** -- 16 general-purpose 80-character text buffers. Available for door use.

- **`z->uz[10]`** -- 10 general-purpose `long` values for door use. Persist for the duration of the call (reset at logon). Useful for passing state between chain-spawned doors.

## System Queries

### Port status

- **`myp->PortZ[n]`** -- Pointer to the `PortData` for port `n`. Unloaded ports point to `myp->z0` (a default/null PortData), not NULL. Check `myp->PortZ[n]->loaded` to determine if a port is actually in use.

- **`myp->HiPort`** -- The highest currently loaded port number.

- **`myp->nPorts`** -- Number of loaded ports.

### System counters

`myp->Nums[]` provides system-wide counters:

| Index | Constant | Meaning |
|---|---|---|
| 0 | `NUMS_CURRENT_ACCOUNTS` | Total physical user accounts |
| 1 | `NUMS_INUSE_ACCOUNTS` | Currently active (in-use) accounts |
| 2 | `NUMS_HIGH_ID` | Last assigned unique ID number |
| 3 | `NUMS_CALLS_TOTAL` | Total calls to the system |
| 4 | `NUMS_CALLS_LOGGED` | Users currently logged in |

### Access groups

`myp->AGC[n]` (n = 0-31) contains `AccessGroup` structures defining each access group's name and default privileges:

```c
sprintf(z->ABuffer, "Your access group: %s\n",
        myp->AGC[z->user1.Access].Name);
CPutA();
```

### Port identification

- **`z->InPort`** -- This port's number (0-99).
- **`z->id`** -- Current user's account number.
- **`z->OnLine`** -- Non-zero if a user is logged in.
- **`z->Carrier`** -- Non-zero if there is an active connection (carrier detect or local mode).

## Subboard Access

Doors can read and manipulate subboard data through `cnet.library` functions. The subboard array is `myp->Subboard`, and `myp->ns` is the number of subboards allocated.

### Reading and writing items

**`ZGetItem(item, head, sub, n)`** -- Read item `n` from subboard `sub` into `item` and `head` buffers.

**`ZPutItem(item, head, sub, n)`** -- Write item `n` back to subboard `sub` from `item` and `head` buffers.

**`ZAddItem(item, head, sub)`** -- Add a new item to subboard `sub`. Returns non-zero on success.

```c
struct SubboardType4 *sub = &myp->Subboard[z->bn];
struct ItemType3 item;
struct ItemHeader ihead;

/* Read item 0 from the current subboard */
ZGetItem(&item, &ihead, sub, 0);
sprintf(z->ABuffer, "Item: %s (%ld downloads)\n", item.Title, item.Downloads);
CPutA();
```

### Subboard user count

**`OneMoreUser(struct SubboardType4 *r, UBYTE lock)`** -- Increment the subboard's `Users` count. The `lock` parameter controls semaphore behavior. Returns non-zero on success.

**`OneLessUser(struct SubboardType4 *r)`** -- Decrement the subboard's `Users` count.

These must be called in matched pairs. Always call `OneLessUser()` when leaving a subboard, even if exiting due to an error.

### Semaphores

Subboard data is shared across all ports. Before modifying subboard structures, obtain the appropriate semaphore:

```c
/* Lock the subboard's semaphore before modification */
ObtainSemaphore(sub->sem);

/* ... modify subboard data ... */

ReleaseSemaphore(sub->sem);
```

For system-wide locks, CNet provides `myp->SEM[0..18]`:

| SEM index | Protects |
|---|---|
| 0 | MainPort itself |
| 1 | User key arrays (myp->Key[], IName, IPhone) |
| 5 | Subboard structures and data files |
| 10 | PFile counter |
| 11 | User data files (bbs.udata, bbs.cuser) |
| 13 | List-type subboard items |
| 14 | Vote items |

Always hold semaphores for the shortest possible duration.

## Mail Operations

**`CNC_MailSend(short account)`** -- Open the mail composition interface to send mail to the specified account number.

The mail system also provides folder-level access through `z->Folders` (linked list of `NewMailFolder`) and message access through `z->rmhead` (linked list of `MailHeader4`). These are loaded on demand and protected by `z->mfsem` and `z->rmhsem` respectively.

## Logging

**`COWriteLog(short lognum, char *text1, char *text2)`** -- Write an entry to a CNet log file. The `lognum` parameter selects which log.

```c
COWriteLog(0, "Door accessed", z->user1.Handle);
```

## Best Practices

### Cleanup on exit

Always register a cleanup function via `atexit()` before calling `CNCL_DoorInit()`. Register yours first so it runs before the CNet cleanup (atexit uses LIFO order):

```c
int main(int argc, char **argv)
{
    atexit(DoorCleanup);   /* runs first */
    /* CNCL_DoorInit registers CNCL_DoorCleanup -- runs second */
    if (!CNCL_DoorInit(argc, argv))
        return 0;
    /* ... */
    return 0;  /* exit triggers atexit chain */
}
```

Your cleanup should free any memory you allocated, close any libraries you opened, and release any semaphores you hold. Do not close `cnetc.library` or call `CNC_ShutDown()` -- `CNCL_DoorCleanup()` handles that.

### Stack size

The default stack for Amiga CLI programs is 4-8KB, which is far too small for most doors. `libcnet_door.a` sets `__stack = 65536` (64KB). If your door uses large local buffers, increase it:

```c
unsigned long __stack = 131072;  /* 128KB */
```

Stack overflow on 68k corrupts memory silently, then crashes as an Address Error or with garbled alert codes. If your door crashes mysteriously, increasing the stack is the first thing to try.

### Avoid large stack-allocated buffers

Due to the limited stack, do not declare large arrays as local variables. Use `static` or `malloc()`:

```c
/* BAD -- 8KB on the stack */
void bad_function(void) {
    char buffer[8192];
}

/* GOOD -- static allocation */
void good_function(void) {
    static char buffer[8192];
}

/* GOOD -- heap allocation */
void also_good(void) {
    char *buffer = malloc(8192);
    if (buffer) {
        /* ... */
        free(buffer);
    }
}
```

### ABuffer size

The `z->ABuffer` pointer is dynamically allocated by CNet with a configurable size (set in Config/Limits). Do not assume a specific size. If you need to format long strings, build them in your own buffer first, then copy a safe amount to ABuffer, or use `CPutText()` directly with your own buffer.

### Check carrier

Long-running doors should periodically check if the user is still connected:

```c
if (!CCheckCarrier()) {
    /* user disconnected -- clean up and exit */
    return;
}
```

### Semaphore discipline

- Always pair `ObtainSemaphore()` with `ReleaseSemaphore()`.
- Never hold a semaphore while waiting for user input (this blocks other ports).
- Hold semaphores for the shortest possible duration.
- Be aware of the lock ordering to avoid deadlocks: if you need multiple semaphores, always acquire them in ascending index order.

### Chain-spawning doors

A door can launch another door when it exits by setting `z->CSpawn` to the path of the next program:

```c
strcpy(z->CSpawn, "PFiles:next_door");
/* exit normally -- CNet will run next_door */
```

Alternatively, pass the spawn path to `CShutDown()`:

```c
CShutDown("PFiles:next_door");
```

### The uz[] scratch area

`z->uz[0]` through `z->uz[9]` are 10 `long` values reserved for door use. They persist across chain-spawned doors within a single call but are reset at logon. Use them to pass state between related doors.

### Floating point

The m68k-amigaos-gcc cross-compiler with `-noixemul` (libnix) has no soft-float library linked by default. Using `double` or `float` causes undefined reference errors at link time (`__divdf3`, `__muldf3`, etc.). Use integer arithmetic, or add `-lm` to LDFLAGS if you need floating point.

## Debugging Tips

- If the door fails to launch, verify the binary path in the subboard config and that the file has execute permissions.
- If you see "This is a CNet door -- run from CNet BBS", the binary was run from a CLI shell rather than being launched by CNet.
- If the door crashes immediately, check that `cnetc.library` is present in `LIBS:`.
- Address Error (vector 3) or alert `0x80000003` usually indicates stack overflow or memory corruption. Increase `__stack` and check for buffer overruns.
- Alert `0x01000000` (`AG_OpenLib`) indicates a corrupted library base pointer, often caused by stack overflow corrupting nearby memory.
- Use `amigactl` for remote inspection of the running Amiga system during development.
