# CNet 5 Architecture Overview

This document describes the runtime architecture of CNet 5: how its processes are organized, how they share state, and how external programs (doors, utilities, scripts) interact with the system.

## CNet Process Model

CNet runs as multiple cooperating AmigaOS processes on a single Amiga system.

### Control (the supervisor)

The **Control** process is the master. It:

- Creates and owns the `MainPort` structure (the central shared data store).
- Publishes an Exec message port named `"cnetport"` so other programs can find MainPort via `FindPort("cnetport")`.
- Loads and unloads BBS ports (spawning/killing `bbs` processes).
- Manages the subboard tree, user account database, access groups, events, and system configuration.
- Launches and monitors server tasks.
- Provides the sysop GUI (Control panel) on the Amiga's Workbench screen.

Control's ARexx port is `CONTROLREXX.1`.

### bbs (per-port processes)

Each active BBS port runs a separate **`bbs`** process. CNet supports up to 100 ports (numbered 0-99). Each `bbs` process:

- Handles one user connection (modem, telnet, or local console).
- Owns its own `PortData` structure (`z`) containing all per-session state.
- Communicates with doors via the `CPort`/`CMessage` protocol.
- Has its own ARexx port: `CNETREXX0` through `CNETREXX99`.

The current highest loaded port number is `myp->HiPort`. The pointer `myp->PortZ[n]` gives the PortData for port `n`. Unloaded ports point to `myp->z0` (a null/default PortData), not NULL.

### Server tasks

CNet spawns several background server tasks, each with its own Exec message port:

| Task | Port name | Purpose |
|---|---|---|
| mail-task | `cnmail_serv` | Outbound mail spooling, UUCP processing |
| file-task | `cnfile_serv` | File test/transform processing |
| yank-task | `cnyank_serv` | Yank (batch download) processing |
| news-task | `cnnews_serv` | NNTP news import/export |
| telnetd | `cntlntd_port` | Telnet daemon, accepts incoming connections |
| smtpd | `cnsmtpd_port` | SMTP daemon for inbound email |
| ftpd | `cnftpd_port` | FTP daemon |
| cron | `cncron_port` | Timed event execution |
| usered | `cnusered_port` | User editor GUI |

Server status is tracked in MainPort fields (`myp->mail_up`, `myp->MPE->telnetd_up`, etc.) using the `TASK_UP_*` constants:

```c
#define TASK_UP_NOTRUNNING 0
#define TASK_UP_GUIOPEN    1
#define TASK_UP_HIDDEN     2
#define TASK_UP_MENUFIED   3
#define TASK_UP_ICONIFIED  4
#define TASK_UP_RUNNING    5
```

Task pointers (`myp->mailtask`, `myp->filetask`, etc.) allow signaling tasks directly with `Signal()`.

## MainPort

`MainPort` (`struct MainPort`, defined in `include/cnet/control.h`) is the global shared data structure that ties the entire BBS together. It is allocated by Control at startup and persists for the life of the BBS session.

### Finding MainPort

Any AmigaOS program can locate MainPort while CNet is running:

```c
#include <proto/exec.h>
struct MainPort *myp = (struct MainPort *)FindPort("cnetport");
```

The string `"cnetport"` is defined as the `CNETPORT` macro in `include/cnet/ports.h`.

MainPort embeds a standard `struct MsgPort` at offset 0, which is what `FindPort()` locates. The rest of the structure extends beyond it.

### Key fields

**System identity and state:**

| Field | Type | Description |
|---|---|---|
| `regto` | `char[32]` | Registered-to name |
| `regco` | `char[32]` | Registration code |
| `s1` | `long` | Control version |
| `s3` | `long` | Serial number (0 = demo) |
| `HiPort` | `short` | Highest loaded port number |
| `nPorts` | `short` | Number of configured ports |

**User database:**

| Field | Type | Description |
|---|---|---|
| `Nums[0]` | `long` | Number of user accounts (`NUMS_CURRENT_ACCOUNTS`) |
| `Nums[1]` | `long` | Currently in-use accounts (`NUMS_INUSE_ACCOUNTS`) |
| `Nums[2]` | `long` | Highest assigned unique ID (`NUMS_HIGH_ID`) |
| `Nums[3]` | `long` | Total calls to system (`NUMS_CALLS_TOTAL`) |
| `Nums[4]` | `long` | Currently logged-in calls (`NUMS_CALLS_LOGGED`) |
| `Key[]` | `KeyElement4*` | User key array (handles, IDs), alphabetical |
| `IName[]` | `short*` | Index into Key[] sorted by name |

**Per-port arrays:**

| Field | Type | Description |
|---|---|---|
| `PortZ[100]` | `PortData*` | Pointer to each port's PortData. Compare against `myp->z0` to check if loaded. |
| `pc[101]` | `PortConfig` | Port configuration (online, screen state, check bits) |
| `CRoom[100]` | `Room*` | Chat room pointers per port |

**Subboard tree:**

| Field | Type | Description |
|---|---|---|
| `Subboard` | `SubboardType4*` | Array of all subboard structures |
| `ns` | `long` | Number of allocated subboards |
| `root` | `short` | Physical number of the first root subboard |

**Configuration and access:**

| Field | Type | Description |
|---|---|---|
| `gc` | `NewConfig1` | Global configuration defaults |
| `AGC[32]` | `AccessGroup` | Access group definitions (up to 32) |
| `SysPassword[6][21]` | `char` | System passwords |

**Semaphores:**

| Field | Type | Description |
|---|---|---|
| `SEM[19]` | `SignalSemaphore` | System-wide semaphores (see below) |

**Extension:**

| Field | Type | Description |
|---|---|---|
| `MPE` | `MainPortExtension*` | Extended fields (identd, DCC, MIME, FTP, semaphores) |

**Programmer scratch space:**

| Field | Type | Description |
|---|---|---|
| `um[10]` | `long` | Ten long words reserved for programmer use. Typecast as needed. Initialized to 0 at boot. |

## PortData

`PortData` (`struct PortData`, defined in `include/cnet/ports.h`) contains all per-port, per-session state. It is a large structure (several KB) encompassing user data, I/O state, editor buffers, transfer state, screen/window handles, and more.

In door code, PortData is accessible as the global `z` pointer, set up by `CNCL_DoorInit()`. In standalone utilities, access it via `myp->PortZ[port_number]`.

### Key field groups

**Identity and timing:**

| Field | Offset | Description |
|---|---|---|
| `z->id` | 12 | Current user's account number |
| `z->InPort` | 14 | This port's number |
| `z->OnLine` | 1747 | Non-zero if a user is logged in |
| `z->loaded` | 1752 | Non-zero if port is loaded |
| `z->TimeLeft` | 28 | Tenths of minutes remaining |
| `z->TimeOnLine` | 32 | Tenths of minutes since logon |
| `z->TimeIdle` | varies | Tenths of minutes idle |

**User data:**

| Field | Description |
|---|---|
| `z->user1` | `UserData` struct for the current user (672 bytes) |
| `z->user2` | Scratch `UserData` for reading other accounts |
| `z->Caller` | Which caller number this is |

**I/O buffers:**

| Field | Description |
|---|---|
| `z->InBuffer[256]` | Last line input from user (filled by `CEnterLine`/`CNC_EnterLine`) |
| `z->ABuffer` | Dynamically allocated output buffer (use with `sprintf` then `CPutA`) |
| `z->KBuff[256]` | Keyboard buffer |
| `z->GBuffer[16][81]` | General-purpose 2D text buffer |

**Navigation state:**

| Field | Description |
|---|---|
| `z->bn` | Current subboard physical number |
| `z->sb` | Pointer to current `SubboardType4` |
| `z->dz` | Depth in subboard tree |
| `z->mygokeyword` | GO keyword for user's current location (NULL = main menu) |

**Door state:**

| Field | Description |
|---|---|
| `z->nPdepth` | How many doors deep (nested door count) |
| `z->CRunCount` | Depth of C PFile nesting |
| `z->CSpawn[84]` | Spawn command after current door exits |
| `z->uz[10]` | Ten long words reserved for programmer use (like `myp->um[]`) |
| `z->Doing[48]` | "What am I doing" string shown in WHO display |

**Semaphores (per-port):**

| Field | Description |
|---|---|
| `z->sem` | General-purpose port semaphore |
| `z->pesem` | Port events list semaphore |
| `z->mfsem` | Mail folders list semaphore |
| `z->masem` | Mail aliases list semaphore |
| `z->rmhsem` | Mail header list semaphore |

## PortDataExtension (PDE)

`PortDataExtension` (`struct PortDataExtension`, accessible as `z->PDE`) was added in CNet v4 to extend PortData without breaking binary compatibility. It contains:

| Field | Description |
|---|---|
| `PDE->TNHost0` | Telnet host entry being listed/edited |
| `PDE->CurrentTNHost` | Currently connected telnet host (NULL if none) |
| `PDE->sp` | `SerPort4` modem/port configuration (moved from PortData) |
| `PDE->telnetdata` | Telnetd connection data for this port |
| `PDE->dsock` | Socket descriptor for telnetd I/O |
| `PDE->TelnetDMode` | Non-zero if in telnetd I/O mode |
| `PDE->lastby[52]` | "Last caller" name (moved from PortData, expanded to 52 chars) |
| `PDE->MailArrived` | Count of new mails delivered while user is online |
| `PDE->mydloc[128]` | Current directory for directory maintenance |
| `PDE->DefItem`, `DefHeader`, `DefMessage`, `DefIHead` | Default structures for ARexx SETITEMFLAGS/ADDITEM |

## The Multi-Port Model

CNet's multi-port architecture allows up to 100 simultaneous users:

- Port numbers range from 0 to 99 (`HIPORT_MAX = 99`).
- Each port runs its own `bbs` process with its own `PortData`.
- All ports share a single `MainPort` structure in memory.
- Ports can be loaded/unloaded dynamically while CNet is running.

### Checking port status

```c
/* Iterate all loaded ports */
for (int i = 0; i <= myp->HiPort; i++) {
    struct PortData *zp = myp->PortZ[i];
    if (zp != myp->z0) {
        /* Port i is loaded */
        if (zp->OnLine) {
            /* A user is logged in on port i */
        }
    }
}
```

`myp->pc[i].online` also indicates whether port `i` is loaded.

### Semaphore locking

Because multiple `bbs` processes and server tasks run concurrently, shared data must be protected with Exec semaphores. MainPort provides `SEM[0..18]` for system-wide locking:

| Semaphore | Protects |
|---|---|
| `SEM[0]` | MainPort itself |
| `SEM[1]` | User key arrays (`Key[]`, `IName[]`) |
| `SEM[2]` | Upload/select lists |
| `SEM[3]` | Serial device |
| `SEM[4]` | `Nums[1]` (in-use account count) |
| `SEM[5]` | Subboard structures, net/echomail import/export |
| `SEM[7]` | Conference/join chat |
| `SEM[8]` | Conference room structures |
| `SEM[10]` | PFile locking |
| `SEM[11]` | User data files (`bbs.udata`, `bbs.cuser`, userlist) |
| `SEM[12]` | Log files |
| `SEM[13]` | LIST subboard items |
| `SEM[14]` | Vote items/results |
| `SEM[16]` | CD-ROM copy function |
| `SEM[18]` | SAM (System Activity Monitor) |

Usage pattern:

```c
#include <proto/exec.h>

/* Lock before reading/writing shared data */
ObtainSemaphore(&myp->SEM[1]);

/* ... access myp->Key[], myp->IName[] ... */

ReleaseSemaphore(&myp->SEM[1]);
```

**Do not hold semaphores for extended periods.** Other ports block while waiting, and deadlocks will occur if lock ordering is violated.

The MainPortExtension (`myp->MPE`) has its own semaphore array `MPE->sem[0..19]`:

| Semaphore | Protects |
|---|---|
| `MPE->sem[0]` | `reload_text` variable |
| `MPE->sem[1]` | Identd data list |
| `MPE->sem[2]` | Global MailKill list |
| `MPE->sem[3]` | IMP (Inter-Module Process) list |
| `MPE->sem[4]` | Hostname/spam filter config |
| `MPE->sem[5]` | uumail directory (pauses mail-task) |
| `MPE->sem[6]` | Global MIME list |
| `MPE->sem[7]` | FTPd process list |
| `MPE->sem[8]` | File test/transform process list |

## Subboard Tree

CNet organizes all content areas -- message bases, file transfer areas, doors, text files, ARexx scripts, macros -- into a hierarchical tree of **subboards**. Each subboard is a `SubboardType4` structure (defined in `include/cnet/subs.h`).

### Tree structure

The tree is built from three link fields in each `SubboardType4`:

| Field | Type | Meaning |
|---|---|---|
| `Parent` | `short` | Physical number of the first subboard on the parent list (-1 for none, -2 for main menu) |
| `Child` | `short` | Physical number of the first subboard in this subdirectory's child list (-1 if none) |
| `Next` | `short` | Physical number of the next subboard on the same list (-1 if last) |

```
Root list (Parent == MAIN_SUB_PHYSNUM == -2):
  Sub 0 (Subdirectory) --Child--> Sub 3 --> Sub 4 --> Sub 5 (-1)
  |
  Next
  v
  Sub 1 (Message base)
  |
  Next
  v
  Sub 2 (Door) (-1 = end of root list)
```

Subdirectories (subboards with `Subdirectory == 1`) contain child lists. Leaf nodes are message bases, file areas, doors, etc.

### Physical numbering vs logical ordering

- **Physical number** (`SerNum`): The subboard's index in the `myp->Subboard[]` array. This is a permanent identifier.
- **Logical order**: The display order within a list, determined by walking the `Next` linked list from the first child.
- **GO keywords**: Each subboard's `SubDirName` field doubles as its GO keyword. Users type `GO keyword` to jump directly to any subboard.

### Subboard types

The `Marker` field (masked with `MRK_SUBBOARD_BASE`) identifies the subboard type:

| Value | Constant | Type |
|---|---|---|
| 0 | `MRK_MSG_BASE` | Message base (also subdirectories, with `Subdirectory=1`) |
| 1 | `MRK_FILE_TXFER` | File transfer area |
| 3 | `MRK_TEXT_DOOR` | Text/file door |
| 4 | `MRK_TEXT_FILE` | Text file display |
| 5 | `MRK_CNETC_DOOR` | CNetC PFile (C door) |
| 6 | `MRK_AREXX_DOOR` | ARexx door |
| 7 | `MRK_ADOS_DOOR` | AmigaDOS door |
| 8 | `MRK_BBS_MACRO` | BBS macro |
| 9 | `MRK_DIRECT_COMMANDER` | Direct/Commander file exchange |

The `MRK_SUBBOARD_KILLED` flag (bit 7) marks deleted subboards. `MRK_SUBBOARD_ROOT` (bit 6) marks root-level subboards.

### Access control

Each subboard has independent access bitmasks checked against the user's access group:

| Field | Controls |
|---|---|
| `Access` | Who can enter/see the subboard |
| `PostAccess` | Who can post messages |
| `RespondAccess` | Who can respond to messages |
| `UploadAccess` | Who can upload files |
| `DownloadAccess` | Who can download files |
| `HourAccess` | Who can enter during restricted hours |
| `UnionFlags` | Required flag bits (must have ALL) |

## IPC Mechanisms

CNet uses several inter-process communication mechanisms.

### CMessage (door <-> bbs)

The primary mechanism for door-to-BBS communication. When a door needs the BBS to perform an action (display text, get input, read a file), it sends a `CMessage` to the BBS process via Exec message passing.

```c
struct CMessage {
    struct Message cn_Message;  /* Exec message header */
    ULONG  arg1;                /* Command-specific arguments */
    ULONG  arg2;
    ULONG  arg3;
    ULONG  arg4;
    ULONG  result;              /* Return value from BBS */
    UBYTE  command;             /* CMESS_CMD_* command code */
    UBYTE  align[3];
};
```

The `command` field specifies the operation. Key command codes (defined in `include/cnet/doors.h`):

| Code | Constant | Operation |
|---|---|---|
| 0 | `CMESS_CMD_SHUTDOWN` | Door is exiting |
| 1 | `CMESS_CMD_PUTTEXT` | Display text to user |
| 2 | `CMESS_CMD_ENTERLINE` | Prompt for line input |
| 3 | `CMESS_CMD_ONEKEY` | Wait for single keypress |
| 4 | `CMESS_CMD_ENTERPASS` | Password input (masked) |
| 6 | `CMESS_CMD_READFILE` | Display a file |
| 7 | `CMESS_CMD_SETDOING` | Set "Doing" status string |
| 8 | `CMESS_CMD_CALLEDITOR` | Invoke the text editor |
| 9 | `CMESS_CMD_READGRAPHICS` | Display ANSI art file |
| 15 | `CMESS_CMD_ENTERTEXT` | Enter multi-line text |
| 31 | `CMESS_CMD_WAITFORINPUT` | Wait for input with timeout |
| 34 | `CMESS_CMD_PUTITEM` | Write subboard item data |
| 35 | `CMESS_CMD_GETITEM` | Read subboard item data |
| 65 | `CMESS_CMD_CHECKCARRIER` | Check if user is still connected |
| 66 | `CMESS_CMD_MAILSEND` | Send mail |

The `CNetCContext` structure ties together the door's connection state:

```c
struct CNetCContext {
    struct MainPort *myp;    /* System-wide shared state */
    struct PortData *z;      /* This port's state */
    struct CPort    *cport;  /* Communication port (BBS side) */
    struct MsgPort  *replyp; /* Reply port (door side) */
    struct CMessage cmess;   /* Reusable message struct */
};
```

In practice, doors use `cnetc.library` functions (or `libcnet_door.a` wrappers) instead of constructing CMessages manually. The library fills in the CMessage fields and handles the `PutMsg`/`WaitPort` exchange.

### FIFO/PIPE (inter-port)

CNet uses AmigaDOS FIFO and PIPE handlers for inter-port text communication (e.g., the chat system, OLMs/online messages). These are AmigaOS-level named pipes, not CNet-specific constructs.

### ARexx (scripting)

Each loaded port exposes an ARexx port (`CNETREXX0` through `CNETREXX99`) for scripting. Control exposes `CONTROLREXX.1`. ARexx scripts can:

- Query user data via GETUSER codes
- Check port status
- Inject commands
- Trigger system operations

ARexx doors (subboard type `MRK_AREXX_DOOR`) run as ARexx scripts with full access to BBS state.

### Exec signals (task notification)

Server tasks can be signaled directly using AmigaOS `Signal()`:

```c
/* Tell mail-task to quit */
Signal(myp->mailtask, SIGBREAKF_CTRL_C);
```

Server task pointers are in MainPort (`myp->mailtask`, `myp->filetask`, `myp->yanktask`, `myp->nntptask`) and MainPortExtension (`myp->MPE->telnetdtask`, `myp->MPE->smtpdtask`).

A task sets its pointer to NULL when it exits. Check the pointer before signaling.

CNet also uses `SendTaskMessage()` with command codes (`TASK_QUIT_MSG`, `MT_RESCAN_MSG`, etc.) for structured commands to server tasks via their named message ports.
