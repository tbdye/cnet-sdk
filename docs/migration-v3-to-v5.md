# Migration Guide: CNet v3 to CNet v5

This guide documents the differences between CNet v3 and CNet v5 that affect door (PFile) development. It covers architectural changes, struct layout changes, function signature changes, and LVO reordering that breaks binary compatibility.

## Architecture Change

### v3: CallHost IPC only

In CNet v3, all door-to-BBS communication used a single mechanism: the `CallHost()` function, which sends a `CMessage` to CNet's `CPort` and waits for the reply. Every operation -- text output, user input, file display, account access -- was performed by setting `cmess.command` to a `CMESS_CMD_*` value, filling in arg fields, and calling `CallHost()`.

Doors linked against `cnet.lib` (for system-level functions like `OneLessUser`, `ZGetItem`, `LockAccount`) and used `CallHost()` for all user-facing operations.

### v5: cnetc.library added

CNet v5 introduced `cnetc.library`, an AmigaOS shared library that wraps the CMessage protocol behind clean function calls (`CNC_PutText()`, `CNC_EnterLine()`, etc.). It also introduced `cnetcdoor.lib`, a link library providing `C*`-prefixed wrapper functions (`CPutText()`, `CEnterLine()`, etc.) and the `CNCL_DoorInit()`/`CNCL_DoorCleanup()` lifecycle management.

The raw CMessage approach still works in v5 -- the protocol is unchanged. The cnetc.library is an additional convenience layer, not a replacement.

### What this means for porting

- **v3 doors using raw CMessage**: These work in v5 with the CPort handshake adjustment (`cport->ack == 30` for v4+, vs the older handshake values for v3). The door logic itself does not need to change, but the binary must be recompiled against v5 libraries (see LVO Reordering below).

- **New doors**: Should use the CNetC pattern (`CNCL_DoorInit()`, `C*` wrappers). The raw CMessage pattern is available for cases where cnetc.library is unavailable or where maximum control is needed.

## Struct Changes

### SubboardType4 vs NewSubboardType

The subboard structure grew substantially between v3 and v5.

| | v3 `NewSubboardType` | v5 `SubboardType4` |
|---|---|---|
| Size | 488 bytes | ~696 bytes |
| Header file | `cnet3_obs.h` | `subs.h` |
| Title field | 31 chars | 60 chars |
| SubDirName | 21 chars | 75 chars |
| DataPath | 40 chars | 95 chars |
| ZeroPath | 48 chars | 90 chars |
| SubSort | short (2 bytes) | UBYTE (1 byte) |
| MaxItems | short (2 bytes) | USHORT (2 bytes) |

Major additions in v5:
- `NNTPMaxDupes` -- NNTP article dedup tracking
- `subvalid` -- data integrity sentinel (should be `1234567890L`)
- `obits` / OFB_* flags -- feature bitflags (DIZ processing, etc.)
- `NNTPMarker` -- newsgroup type classification
- `DizTypes`, `DizSave`, `DizEdit` -- DIZ file support
- `NNTPLastImport` -- NNTP import timestamp
- `SubDoing[17]` -- per-subboard "Doing" status string
- `SerNum` -- physical subboard number
- `SuperUser_Only` -- superuser-restricted entry

Field-level comparison for shared fields (v3 offset -> v5 offset):

| Field | v3 offset | v5 offset | Notes |
|---|---|---|---|
| Title | 0 | 0 | Expanded from 31 to 60 chars |
| SubDirName | 31 | 60 | Expanded from 21 to 75 chars |
| DataPath | 52 | 135 | Expanded from 40 to 95 chars |
| Parts | 92 | 230 | Same type (long) |
| SubOpIDs | 96 | 234 | Same type (long[6]) |
| SubOpAccs | 136 | 274 | Same type (short[6]) |
| Direct | 156 | 294 | Same type (UBYTE) |
| Vote | 157 | 295 | Same type (UBYTE) |
| FreeDays | 158 | 296 | Same type (short) |
| Access | 166 | 304 | Same type (long) |
| ZeroPath | 332 | 470 | Expanded from 48 to 90 chars |
| Origin | 380 | 560 | Same size (60 chars) |
| MaxItems | 440 | 620 | v3: short, v5: USHORT |
| Item (pointer) | ~452 | 624 | Same type |
| sem (pointer) | ~468 | ~636 | Same type |

All v3 subboard fields exist in v5 (no fields were removed), but the offsets are completely different due to field expansions and insertions. Code that accesses subboard fields by struct member name recompiles correctly against v5 headers. Code that uses hardcoded offsets or casts will break.

### MessageType3

The `MessageType3` structure is shared between v3 and v5 with minor changes:

```c
struct MessageType3 {
    ULONG  ItemNumber;   /* v3: long */
    long   Seek;         /* v5 added this field */
    long   ByID;
    long   ToID;
    ULONG  Number;       /* v5 added this field */
    struct IsDate PostDate;
    UBYTE  Imported;
    UBYTE  IsFile;
};
```

The v3 `OldMessageType` in `cnet3_obs.h` has `ItemNumber`, `ResponseNumber`, `ByID`, `ToID`, `PostDate`, `Imported`, `IsFile` -- notably missing `Seek` and `Number`, and having `ResponseNumber` which was removed in v5.

### UserData

The `UserData` structure is largely unchanged between v3 and v5. Field offsets and types are the same. Some fields were added in spare/expansion areas.

### PortData

The `PortData` structure grew in v5 with additions for:
- Mail folder support (`Folders`, `Folder0`, `rmhead`, `Aliases`)
- Event system (`pEvents`, `pesem`, `eventtick`, `nextevent`)
- Telnet support (`TELNETD_MASK`, `TelnetDPort`, `SocketBase`, `SOCKET_MASK`)
- `PortDataExtension` (`z->PDE`) for extended data (telnetd, SerPort4, dmaint)

### PortDataExtension (PDE)

Entirely new in v5. Accessed via `z->PDE`. Contains:
- Telnet session data (`TNHost0`, `CurrentTNHost`, `telnetdata`, `dsock`)
- `SerPort4` configuration (expanded serial port config, moved from PortData)
- Directory maintenance state (`mydloc`, `dflag`, `dmaintup`)
- Default item/header/message structures for ARexx
- GO keyword matching data

## cnet3_obs.h: Deprecated Structures

The file `include/cnet/cnet3_obs.h` preserves the v3 structure definitions for reference:

- **`NewSubboardType`** -- The v3 subboard structure (488 bytes). Replaced by `SubboardType4`.
- **`OldMessageType`** -- The v3 message structure. Replaced by `MessageType3`.

These are provided so that developers porting v3 code can see the exact original layouts and understand how fields moved. They should not be used in new v5 code.

## New in v5

### Shared libraries

- **`cnetc.library`** (v4.1) -- 66 exported functions for door I/O, account access, editor, file transfer, VDE, and more. Provides the `CNC_*` API.
- **`cnetmail.library`** (v4.1) -- 14 exported functions for mail system operations (MIME, encoding, folder management).
- **`cnet4.library`** (v5.0) -- 15 exported functions for time/date operations, range parsing, and uptime formatting.

### Internet support

- Telnet daemon (`telnetd`) for remote access over TCP/IP
- SMTP daemon (`smtpd`) for inbound email
- FTP daemon (`ftpd`) for file access
- NNTP/Usenet support (NNTPGet, mailing list pseudo-newsgroups)
- Identd integration (`AddIdentdUser`, `RemoveIdentdUser`, `LookupIdentd`)

### User cache

v5 introduced a user data cache (`CNUserCache`) for efficient repeated access to user accounts. Functions:

- `UCacheInitUser(account)` -- Initialize a cache entry
- `UCacheLookup(account)` -- Find a cached entry
- `UCacheRead(account, userdata, numaccounts)` -- Read through cache
- `UCacheWrite(account, userdata, numaccounts)` -- Write through cache
- `UCacheDispose()` -- Free all cache entries
- `StartupInitCache()` -- Initialize the cache subsystem

### Directory browsing API

v5 added a full directory browsing API through `cnet.library`:

- `CNetReadDir(path, recurse)` -- Read a directory into a linked list
- `CreateDirEntry(fib)` / `InsertDirEntry(listhead, entry)` -- Build directory listings
- `GetDirEntry(ordinal, list)` / `NextDirEntry()` / `PrevDirEntry()` -- Navigate entries
- `CountDirEntries(listhead)` / `FindParent()` / `HeaderEntry()` / `LastEntry()` -- Query entries
- `CNetDisposeDir(flist)` -- Free a directory listing
- `CNetSearchEntry(list, destlist, pat, recurse)` -- Search with patterns
- `GetVolAssList()` -- List volumes and assigns

### Other additions

- MIME content type handling via `cnetmail.library`
- Extended mail headers (`MailHeader4` with UUCP fields, folder names)
- Mail folder system (`NewMailFolder` linked list)
- Extended archiver support (`NewArcType` with test/transform paths)
- `MainPortExtension` for additional system-wide state
- `ConfigExtension` for additional configuration variables
- VDE (Visual Data Editor) improvements
- MCI code extensions (filtering, removal)
- RFC date formatting (`GetRFCDate`)

## Function Signature Changes

Several functions changed their register assignments between v3 and v5:

### AlterDate

```
v3: AlterDate(date, i)(A0, A1)
v5: AlterDate(date, i)(A0, D0)
```

The second parameter changed from address register A1 to data register D0. v3 code that passes a value in A1 will put garbage in D0 under v5.

### OneLessUser

```
v3: OneLessUser(r)(D0)
v5: OneLessUser(r)(A0)
```

The subboard pointer moved from data register D0 to address register A0. This is a critical change -- passing a pointer in D0 under v5 would dereference an arbitrary address.

### OneMoreUser

```
v3: OneMoreUser(r)(D0)
v5: OneMoreUser(r, lock)(A0, D0)
```

Two changes: the subboard pointer moved from D0 to A0, and a new `lock` parameter was added in D0. v3 code calling the v5 function would pass the subboard pointer in D0 (interpreted as the `lock` parameter) and have garbage in A0.

### UnLockAccount

```
v3: UnLockAccount(acc)(D0)
v5: UnLockAccount(acc, save)(D0, D1)
```

A new `save` parameter was added. In v3, `UnLockAccount` always saved changes. In v5, the caller controls whether to save via the `save` parameter. v3 code calling the v5 function would have an undefined value in D1, causing unpredictable save/no-save behavior.

## Known v3 FD Errors

The original v3 `cnet.fd` file contained register assignment errors:

- **`OneLessUser`**: v3 FD specified `(d0)` but the actual v3 implementation used `(a0)` (or `(d0)` -- the discrepancy between the FD file and the actual library is the error). The v5 FD correctly specifies `(A0)`.

- **`OneMoreUser`**: v3 FD specified `(d0)` but the actual implementation expected the subboard pointer in an address register. The v5 FD correctly specifies `(A0/D0)`.

Doors compiled against the erroneous v3 FD would have been passing arguments in the wrong registers, causing crashes or data corruption. The v5 FD files correct these errors.

## LVO Reordering Between v3 and v5

This is the most critical compatibility issue. The order of functions in the `cnet.library` jump table changed between v3 and v5, which means **v3-compiled door binaries are NOT binary-compatible with v5 libraries**.

### What is an LVO?

AmigaOS shared libraries use a jump table (Library Vector Offset table) to dispatch function calls. Each function has a fixed negative offset from the library base pointer. The inline stubs generated from FD files encode these offsets at compile time. If the offsets change, a recompile is required.

### Specific LVO changes

Comparing the first 24 function slots (the v3 function set) between v3 and v5:

#### Slot reordering

v3 had `OneLessUser`, `OneMoreUser`, `SaveFree` at LVOs -72, -78, -84.
v5 has `SaveFree`, `OneLessUser`, `OneMoreUser` at those same LVOs.

A v3 door calling what it thinks is `OneLessUser` at LVO -72 would actually call `SaveFree` under v5.

v3 had `ZGetItem` at -138 and `ZPutItem` at -144.
v5 has `ZPutItem` at -138 and `ZGetItem` at -144 (swapped).

A v3 door calling `ZGetItem` would actually call `ZPutItem`, and vice versa. Since `ZPutItem` writes data, this could corrupt subboard content.

#### v5 cnet.library LVO table (first 24 functions)

| LVO | v5 function | v3 function at same LVO |
|---|---|---|
| -30 (0x1e) | `ReplaceText` | `ReplaceText` |
| -36 (0x24) | `GiveAmigaDays` | `GiveAmigaDays` |
| -42 (0x2a) | `FormDate` | `FormDate` |
| -48 (0x30) | `AlterDate` | `AlterDate` (different regs) |
| -54 (0x36) | `UltimateFindParts` | `UltimateFindParts` |
| -60 (0x3c) | `ExpandFlags` | `ExpandFlags` |
| -66 (0x42) | `ConvertAccess` | `ConvertAccess` |
| -72 (0x48) | `SaveFree` | `OneLessUser` |
| -78 (0x4e) | `OneLessUser` | `OneMoreUser` |
| -84 (0x54) | `OneMoreUser` | `SaveFree` |
| -90 (0x5a) | `FreeText` | `FreeText` |
| -96 (0x60) | `AllocText` | `AllocText` |
| -102 (0x66) | `LockAccount` | `LockAccount` |
| -108 (0x6c) | `UnLockAccount` | `UnLockAccount` (different regs) |
| -114 (0x72) | `FindPhone` | `FindPhone` |
| -120 (0x78) | `FindHandle` | `FindHandle` |
| -126 (0x7e) | `FormatVanilla` | `FormatVanilla` |
| -132 (0x84) | `MakeDateZone` | `MakeDateZone` |
| -138 (0x8a) | `ZPutItem` | `ZGetItem` |
| -144 (0x90) | `ZGetItem` | `ZPutItem` |
| -150 (0x96) | `ZAddItem` | `ZAddItem` |
| -156 (0x9c) | `ZAddMessage` | `ZAddMessage` |
| -162 (0xa2) | `DelayAddItems` | `DelayAddItems` |
| -168 (0xa8) | `FlushAddItems` | `FlushAddItems` |

### Consequences

A v3-compiled door binary running against v5 libraries will:

1. **Call the wrong function** at swapped LVO positions (OneLessUser/OneMoreUser/SaveFree, ZGetItem/ZPutItem).
2. **Pass arguments in the wrong registers** for functions with changed signatures (AlterDate, OneLessUser, OneMoreUser, UnLockAccount).
3. **Potentially corrupt data** -- calling `ZPutItem` when `ZGetItem` was intended writes data instead of reading it.

### How to migrate

There is no binary compatibility path. All v3 doors must be **recompiled** against the v5 SDK headers and FD files. This is not optional -- running a v3 binary against v5 libraries will crash or corrupt data.

The recompilation process:

1. Replace v3 headers with v5 headers.
2. Update struct references (`NewSubboardType` -> `SubboardType4`).
3. Update function calls with changed signatures:
   - `OneLessUser(r)` -- now takes subboard pointer in A0 (same source syntax, different register binding handled by inline stubs).
   - `OneMoreUser(r, lock)` -- add the `lock` parameter.
   - `UnLockAccount(acc, save)` -- add the `save` parameter.
   - `AlterDate(date, i)` -- no source change needed (register binding handled by inline stubs).
4. Rebuild against `libcnet_door.a` (replaces `cnet.lib` and `cnetcdoor.lib`).

If the v3 source code uses struct member names (not hardcoded offsets) and does not use inline assembly for register binding, the port is typically straightforward -- update the struct type names, add the new parameters, and recompile.

## cnet.h Offset Comments

The offset comments in v3 `cnet.h` (and carried forward into some v5 headers) are **not authoritative**. Verified discrepancies exist between the comments and the actual binary layouts. Always rely on `sizeof()` and `offsetof()` measurements, or on the `__attribute__((packed))` struct definitions in the SDK headers, rather than the inline comments.

## Summary Checklist for Porting v3 Doors

1. [ ] Replace all v3 SDK headers with v5 equivalents.
2. [ ] Change `NewSubboardType` references to `SubboardType4`.
3. [ ] Change `OldMessageType` references to `MessageType3`.
4. [ ] Add `lock` parameter to `OneMoreUser()` calls.
5. [ ] Add `save` parameter to `UnLockAccount()` calls.
6. [ ] Review any hardcoded struct offsets -- all subboard offsets changed.
7. [ ] Replace `cnet.lib` / `cnetcdoor.lib` linkage with `-lcnet_door`.
8. [ ] Consider switching from raw CMessage to CNetC pattern (`CNCL_DoorInit`, `C*` wrappers).
9. [ ] If using GCC, handle the `__asm` conflict (see `docs/pfile-guide.md`).
10. [ ] Recompile and test. There is no binary compatibility -- old v3 binaries **will not work** with v5 libraries.
