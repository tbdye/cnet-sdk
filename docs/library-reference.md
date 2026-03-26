# CNet 5 SDK Library Reference

Complete reference for all exported functions across the five CNet shared
libraries and the `libcnet_door.a` linker library.

---

## Table of Contents

1. [cnet.library](#cnetlibrary) -- 79 functions (system services)
2. [cnetc.library](#cnetclibrary) -- 66 entries (door I/O and BBS services)
3. [cnet4.library](#cnet4library) -- 15 functions (date/time and ranges)
4. [cnetmail.library](#cnetmaillibrary) -- 14 functions (MIME and mail routing)
5. [cnetgui.library](#cnetguilibrary) -- 2 functions (GUI support)
6. [libcnet_door.a](#libcnet_doora) -- linker library (door lifecycle, wrappers, utilities)

---

## Conventions

**LVO (Library Vector Offset):** Each shared library function is accessed at
a negative offset from the library base pointer. The first public function
is at offset -30 (LVO 30), and each subsequent entry adds -6. Private
(reserved) entries also consume a -6 slot.

**Registers:** Listed from the FD file as the register assignments for the
m68k calling convention. The GCC inline stubs in `include/inline/*.h`
handle register binding automatically.

**Categories:**

| Abbrev | Meaning |
|--------|---------|
| DATE   | Date/time manipulation |
| DIR    | Directory/file listing |
| FILE   | File system operations |
| IDENT  | User identity lookup |
| IO     | Terminal I/O |
| LOG    | Logging |
| MAIL   | Mail system |
| MCI    | MCI (color/control code) processing |
| PARSE  | String parsing/range |
| SUB    | Subboard/message base |
| SYS    | System/port operations |
| USER   | User account management |
| CACHE  | User data cache |
| XFER   | File transfer |
| ACCT   | Accounting/charges |
| CONF   | Conference/chat |
| MISC   | Miscellaneous |

---

## cnet.library

Base name: `_CNetBase` / `CNetBase`

79 functions providing system-level BBS services. These are called by
Control, port processes, and server tasks directly. Door programs
normally access them through `cnetc.library` wrappers, but they can
be called directly when the library is opened.

### Function Table

| # | LVO | Function | Category |
|---|-----|----------|----------|
| 1 | -30 | ReplaceText | PARSE |
| 2 | -36 | GiveAmigaDays | DATE |
| 3 | -42 | FormDate | DATE |
| 4 | -48 | AlterDate | DATE |
| 5 | -54 | UltimateFindParts | PARSE |
| 6 | -60 | ExpandFlags | PARSE |
| 7 | -66 | ConvertAccess | PARSE |
| 8 | -72 | SaveFree | SUB |
| 9 | -78 | OneLessUser | SUB |
| 10 | -84 | OneMoreUser | SUB |
| 11 | -90 | FreeText | SUB |
| 12 | -96 | AllocText | SUB |
| 13 | -102 | LockAccount | USER |
| 14 | -108 | UnLockAccount | USER |
| 15 | -114 | FindPhone | IDENT |
| 16 | -120 | FindHandle | IDENT |
| 17 | -126 | FormatVanilla | MCI |
| 18 | -132 | MakeDateZone | DATE |
| 19 | -138 | ZPutItem | SUB |
| 20 | -144 | ZGetItem | SUB |
| 21 | -150 | ZAddItem | SUB |
| 22 | -156 | ZAddMessage | SUB |
| 23 | -162 | DelayAddItems | SUB |
| 24 | -168 | FlushAddItems | SUB |
| 25 | -174 | FileOLM | IO |
| 26 | -180 | XAddCharge | ACCT |
| 27 | -186 | AddCredits | ACCT |
| 28 | -192 | WriteLog | LOG |
| 29 | -198 | DirectorySize | FILE |
| 30 | -204 | FileExists | FILE |
| 31 | -210 | CreateMailDir | MAIL |
| 32 | -216 | CreateFolderName | MAIL |
| 33 | -222 | BuildDir | FILE |
| 34 | -228 | BitPosition | MISC |
| 35 | -234 | MCIFilter | MCI |
| 36 | -240 | AccountToID | IDENT |
| 37 | -246 | IDToAccount | IDENT |
| 38 | -252 | StrToUpper | PARSE |
| 39 | -258 | FileSize | FILE |
| 40 | -264 | AddIdentdUser | SYS |
| 41 | -270 | RemoveIdentdUser | SYS |
| 42 | -276 | LookupIdentd | SYS |
| 43 | -282 | CNetAddressType | MAIL |
| 44 | -288 | CNetAddressToAccount | MAIL |
| 45 | -294 | HNameToUUCP | IDENT |
| 46 | -300 | CNetIDToUUCP | IDENT |
| 47 | -306 | NumFromUnique | SUB |
| 48 | -312 | FindOrdinal | MISC |
| 49 | -318 | InStr | PARSE |
| 50 | -324 | AllDigits | PARSE |
| 51 | -330 | CopyFilterText | MCI |
| 52 | -336 | IsNowOnLine | SYS |
| 53 | -342 | GetRFCDate | DATE |
| 54 | -348 | AppendToFile | FILE |
| 55 | -354 | PItemToArgV | PARSE |
| 56 | -360 | CreateDirEntry | DIR |
| 57 | -366 | InsertDirEntry | DIR |
| 58 | -372 | CNetDisposeDir | DIR |
| 59 | -378 | CNetReadDir | DIR |
| 60 | -384 | GetDirEntry | DIR |
| 61 | -390 | CountDirEntries | DIR |
| 62 | -396 | FindParent | DIR |
| 63 | -402 | NextDirEntry | DIR |
| 64 | -408 | PrevDirEntry | DIR |
| 65 | -414 | HeaderEntry | DIR |
| 66 | -420 | LastEntry | DIR |
| 67 | -426 | EntryOrdinal | DIR |
| 68 | -432 | GetVolAssList | DIR |
| 69 | -438 | CNetSearchEntry | DIR |
| 70 | -444 | MCIRemove | MCI |
| 71 | -450 | UCacheDispose | CACHE |
| 72 | -456 | UCacheInitUser | CACHE |
| 73 | -462 | UCacheLookup | CACHE |
| 74 | -468 | UCacheRead | CACHE |
| 75 | -474 | UCacheWrite | CACHE |
| 76 | -480 | StartupInitCache | CACHE |
| 77 | -486 | ZGetItemPtr | SUB |
| 78 | -492 | ZGetIHeadPtr | SUB |
| 79 | -498 | CNetStripChars | PARSE |

### Function Reference -- cnet.library

---

#### ReplaceText
```c
short ReplaceText(char *line, char *from, char *with);
```
**Registers:** A0/A1/A2
**Category:** PARSE
**Description:** Replace all occurrences of substring `from` with `with` in `line`.
**Returns:** Number of replacements made.

---

#### GiveAmigaDays
```c
long GiveAmigaDays(struct IsDate *date);
```
**Registers:** A0
**Category:** DATE
**Description:** Calculate the number of days between the given date and January 1, 1978 (the Amiga epoch).
**Returns:** Day count from Amiga epoch.

---

#### FormDate
```c
void FormDate(struct IsDate *date, long *parts);
```
**Registers:** A0/A1
**Category:** DATE
**Description:** Convert an Amiga DateStamp (array of 2 LONGs) into a CNet `IsDate` structure.

---

#### AlterDate
```c
void AlterDate(struct IsDate *date, long i);
```
**Registers:** A0/D0
**Category:** DATE
**Description:** Shift an `IsDate` by `i` days. Positive values advance the date; negative values move it backward.

---

#### UltimateFindParts
```c
void UltimateFindParts(char *buffer, long min, long max, long *nparts, long *parts);
```
**Registers:** A0/D0/D1/A1/A2
**Category:** PARSE
**Description:** Parse a CNet range string (e.g., `"1-5,8,10-20"`) into discrete min/max part pairs. `nparts` receives the count; `parts` is an array of at least 10x2 LONGs. Values are clamped to `[min, max]`. Maximum 10 parts.

---

#### ExpandFlags
```c
void ExpandFlags(long def, char *where);
```
**Registers:** D0/A0
**Category:** PARSE
**Description:** Convert a packed LONG bitfield to a human-readable range string (e.g., `"1-3,5,7"`). Inverse of `ConvertAccess()`.

---

#### ConvertAccess
```c
long ConvertAccess(char *s);
```
**Registers:** A0
**Category:** PARSE
**Description:** Parse a flag/bit range string (e.g., `"1-5,8"`) into a packed LONG where each named bit is set. Inverse of `ExpandFlags()`.
**Returns:** Packed LONG bitfield.

---

#### SaveFree
```c
long SaveFree(struct SubboardType4 *s);
```
**Registers:** A0
**Category:** SUB
**Description:** Write the subboard's free-text hole list to disk. Called after `FreeText()` operations to persist the available space map.
**Returns:** TRUE (1) always.

---

#### OneLessUser
```c
void OneLessUser(struct SubboardType4 *s);
```
**Registers:** A0
**Category:** SUB
**Description:** Decrement the subboard's `Users` count (active usage lock) by one. If the last lock is released, the subboard is fully unlocked.
**See also:** `OneMoreUser()`

---

#### OneMoreUser
```c
UBYTE OneMoreUser(struct SubboardType4 *s, UBYTE lock);
```
**Registers:** A0/D0
**Category:** SUB
**Description:** Increment the subboard's active usage count. If `lock` is TRUE, request an exclusive lock (preventing other exclusive lockers).
**Returns:** TRUE if the usage count was incremented successfully.
**See also:** `OneLessUser()`

---

#### FreeText
```c
long FreeText(struct SubboardType4 *s, long pos, long length);
```
**Registers:** A0/D0/D1
**Category:** SUB
**Description:** Mark a block of subboard `_text` data as available for reuse. `pos` is the seek offset, `length` is the byte count.
**Returns:** Start position of the freed block.
**See also:** `AllocText()`, `SaveFree()`

---

#### AllocText
```c
long AllocText(struct SubboardType4 *s, long length);
```
**Registers:** A0/D0
**Category:** SUB
**Description:** Allocate a block of `length` bytes from the subboard's `_text` file, reusing holes if possible.
**Returns:** Seek position of the allocated block.
**See also:** `FreeText()`

---

#### LockAccount
```c
struct UserData *LockAccount(short acc);
```
**Registers:** D0
**Category:** USER
**Description:** Lock a user account for reading or modification. Returns a pointer to the account's `UserData` (valid until `UnLockAccount()`). Each call must be paired with `UnLockAccount()`. Holding the lock for extended periods risks deadlocks.
**Returns:** Pointer to `UserData`, or NULL on failure.
**See also:** `UnLockAccount()`

---

#### UnLockAccount
```c
void UnLockAccount(short acc, UBYTE save);
```
**Registers:** D0/D1
**Category:** USER
**Description:** Release the lock on a user account obtained via `LockAccount()`. If `save` is TRUE, modifications to the returned `UserData` are committed to disk and cache. If FALSE, changes are discarded.
**See also:** `LockAccount()`

---

#### FindPhone
```c
long FindPhone(short *n, char *phone, short id);
```
**Registers:** A0/A1/D0
**Category:** IDENT
**Description:** Locate the account that owns a phone number. If `id` is non-zero, verifies whether the number belongs to that account. If `id` is zero, searches all accounts and stores the result in `*n`.
**Returns:** TRUE if found/verified, FALSE otherwise.

---

#### FindHandle
```c
long FindHandle(short *n, char *name, short id);
```
**Registers:** A0/A1/D0
**Category:** IDENT
**Description:** Locate the account that owns a handle. If `id` is non-zero, verifies whether the handle belongs to that account. If `id` is zero, searches all accounts and stores the result in `*n`.
**Returns:** TRUE if found/verified, FALSE otherwise.

---

#### FormatVanilla
```c
void FormatVanilla(BPTR in, BPTR out);
```
**Registers:** A0/A1
**Category:** MCI
**Description:** Read from file handle `in`, strip all MCI codes and ANSI escape sequences, word-wrap at 80 columns, and write the result to file handle `out`.

---

#### MakeDateZone
```c
void MakeDateZone(struct IsDate *date, char *whereto, char **text, BYTE tz, BYTE form);
```
**Registers:** A0/A1/A2/D0/D1
**Category:** DATE
**Description:** Format an `IsDate` into a human-readable ASCII string adjusted by timezone offset `tz`. `text` points to a format template (typically BBSTEXT line 1070). `form`: 0 = AM/PM, 1 = 24-hour.

---

#### ZPutItem
```c
void ZPutItem(struct ItemType3 *Item0, struct ItemHeader *ihead0, struct SubboardType4 *s, short c);
```
**Registers:** A0/A1/A2/D0
**Category:** SUB
**Description:** Save item `c` (post or file metadata) to the subboard's data files.
**See also:** `ZGetItem()`, `ZAddItem()`

---

#### ZGetItem
```c
void ZGetItem(struct ItemType3 *Item0, struct ItemHeader *ihead0, struct SubboardType4 *s, short c);
```
**Registers:** A0/A1/A2/D0
**Category:** SUB
**Description:** Load item `c` from the subboard's data files into the provided `ItemType3` and `ItemHeader` structures.
**See also:** `ZPutItem()`, `ZAddItem()`

---

#### ZAddItem
```c
UBYTE ZAddItem(struct ItemType3 *Item0, struct ItemHeader *ihead0, struct SubboardType4 *s);
```
**Registers:** A0/A1/A2
**Category:** SUB
**Description:** Post a new item (message post or file upload entry) to the subboard.
**See also:** `ZAddMessage()`, `DelayAddItems()`, `FlushAddItems()`

---

#### ZAddMessage
```c
void ZAddMessage(struct MessageType3 *Message0, struct SubboardType4 *s);
```
**Registers:** A0/A1
**Category:** SUB
**Description:** Post a new response/message to an existing item in the subboard.
**See also:** `ZAddItem()`, `DelayAddItems()`, `FlushAddItems()`

---

#### DelayAddItems
```c
void DelayAddItems(struct SubboardType4 *s);
```
**Registers:** A0
**Category:** SUB
**Description:** Begin batching item/message additions. Actual writes to the data files are deferred until `FlushAddItems()` is called. Useful for bulk import operations.
**See also:** `FlushAddItems()`

---

#### FlushAddItems
```c
void FlushAddItems(struct SubboardType4 *s);
```
**Registers:** A0
**Category:** SUB
**Description:** Write all deferred items and messages to the subboard's data files. Must be called after `DelayAddItems()` to commit the batch.
**See also:** `DelayAddItems()`

---

#### FileOLM
```c
UBYTE FileOLM(struct PortData *z, char *text, short id, long flags);
```
**Registers:** A0/A1/D0/D1
**Category:** IO
**Description:** Send an Online Message (OLM) to a user. `z` is the sender's PortData (may be NULL for system messages). `id` is the recipient's account number. `flags` can include `OLM_BROADCAST`.
**Returns:** TRUE if the OLM was saved successfully.

---

#### XAddCharge
```c
UBYTE XAddCharge(struct ChargeSet *set, short id, short n, short a, char *info);
```
**Registers:** A0/D0/D1/D2/A1
**Category:** ACCT
**Description:** Apply a charge from `set` to account `id`. `n` is the charge type index (see `CHARGETYPE_*` defines in `charges.h`). `a` is a multiplier. `info` is currently unused.
**Returns:** TRUE if charge was applied, FALSE otherwise.

---

#### AddCredits
```c
void AddCredits(struct ItemType3 *i, short m);
```
**Registers:** A0/D0
**Category:** ACCT
**Description:** Award file/byte credits to the uploader of item `i`, multiplied by `m`.

---

#### WriteLog
```c
void WriteLog(struct PortData *z, short n, char *text1, char *text2);
```
**Registers:** A0/D0/A1/A2
**Category:** LOG
**Description:** Append an entry to CNet log number `n`. `z` identifies the port/user context. `text1` and `text2` become the two text columns in the log entry.

---

#### DirectorySize
```c
ULONG DirectorySize(char *dir, BPTR lock);
```
**Registers:** A0/A1
**Category:** FILE
**Description:** Get the total size in bytes of all files in a directory (non-recursive). Supply either `dir` (a path string) or `lock` (an AmigaOS file lock). If `lock` is provided, `dir` is ignored.
**Returns:** Total bytes. Limited to 4 GB.

---

#### FileExists
```c
short FileExists(char *filename);
```
**Registers:** A0
**Category:** FILE
**Description:** Test whether a file exists.
**Returns:** TRUE if the file exists, FALSE otherwise.

---

#### CreateMailDir
```c
void CreateMailDir(char *uucpid);
```
**Registers:** A0
**Category:** MAIL
**Description:** Create the disk directory path where a user's mail is stored. `uucpid` is the user's UUCP/Mail ID.

---

#### CreateFolderName
```c
UBYTE CreateFolderName(char *destpath, char *UUCP, char *foldername);
```
**Registers:** A0/A1/A2
**Category:** MAIL
**Description:** Build the full filesystem path for a user's mail folder. `UUCP` is the user's Mail ID, `foldername` is the folder name. Result is placed in `destpath`.
**Returns:** 0 always (current implementation).

---

#### BuildDir
```c
short BuildDir(char *path);
```
**Registers:** A0
**Category:** FILE
**Description:** Create a directory path, including any intermediate directories that do not exist.
**Returns:** TRUE if the path was created or already exists, FALSE on failure.

---

#### BitPosition
```c
short BitPosition(LONG bitpattern);
```
**Registers:** D0
**Category:** MISC
**Description:** Find the bit number of the highest set bit in `bitpattern`. If `z = 1L << y`, then `BitPosition(z)` returns `y`.
**Returns:** Bit position (0-31).

---

#### MCIFilter
```c
void MCIFilter(char *text, LONG length, short flags);
```
**Registers:** A0/D0/D1
**Category:** MCI
**Description:** Neutralize MCI codes in a string by replacing control characters. `flags`: 1 = filter Ctrl-Y (replace with `\`), 2 = filter Ctrl-Q (replace with `{`), 3 = filter both.

---

#### AccountToID
```c
LONG AccountToID(LONG account);
```
**Registers:** D0
**Category:** IDENT
**Description:** Convert a user account number to the user's unique serial ID. Serial IDs are never reused, unlike account numbers.
**Returns:** The user's serial ID.
**Caveat:** Does no validity checking on the account number. Caller must ensure `account >= 0 && account < myp->Nums[0]`.
**See also:** `IDToAccount()`

---

#### IDToAccount
```c
short IDToAccount(LONG userid);
```
**Registers:** D0
**Category:** IDENT
**Description:** Find the account number for a given unique serial ID.
**Returns:** Account number, or 0 if not found.
**See also:** `AccountToID()`

---

#### StrToUpper
```c
void StrToUpper(char *string);
```
**Registers:** A0
**Category:** PARSE
**Description:** Convert a string to all uppercase in-place.

---

#### FileSize
```c
LONG FileSize(char *Name);
```
**Registers:** A0
**Category:** FILE
**Description:** Get the size of a file in bytes. Requires the full path.
**Returns:** File size in bytes. Limited to 4 GB.

---

#### AddIdentdUser
```c
BYTE AddIdentdUser(short Port, LONG Socket);
```
**Registers:** D0/D1
**Category:** SYS
**Description:** Register an identd association between a CNet port number and a TCP socket.
**Returns:** 0 on success, 1 if MainPort not found, 2 if allocation failed.

---

#### RemoveIdentdUser
```c
BYTE RemoveIdentdUser(ULONG Socket);
```
**Registers:** D0
**Category:** SYS
**Description:** Remove the identd entry for the specified socket.
**Returns:** 0 on success, 1 if MainPort not found, 2 if no matching entry.

---

#### LookupIdentd
```c
short LookupIdentd(ULONG Socket);
```
**Registers:** D0
**Category:** SYS
**Description:** Find which CNet port is associated with a given socket via identd.
**Returns:** CNet port number, or -1 if not found.

---

#### CNetAddressType
```c
UBYTE CNetAddressType(char *user);
```
**Registers:** A0
**Category:** MAIL
**Description:** Determine the type of a mail address (local, Internet, FidoNet, etc.).
**Returns:** `ADDRESSTYPE_*` constant (see `mail.h`).

---

#### CNetAddressToAccount
```c
short CNetAddressToAccount(char *a);
```
**Registers:** A0
**Category:** MAIL
**Description:** Look up the account number for a CNet UUCP/Mail ID. Special mappings: `AREAFIX` and `ROOT` map to account 1, `FEEDBACK` to -1, `NEWUSER` to -2, `SYSOP` to -3.
**Returns:** Account number, or 0 if not found.

---

#### HNameToUUCP
```c
char *HNameToUUCP(char *hname);
```
**Registers:** A0
**Category:** IDENT
**Description:** Look up a user's UUCP/Mail ID by handle or real name. Handle matches take priority over real name matches. Special names (`FEEDBACK`, `NEWUSER`, `SYSOP`, `AREAFIX`, `POSTMASTER`) return `"SYSOP"`. `ROOT` returns account 1's Mail ID.
**Returns:** Pointer to the Mail ID string, or `""` if not found.

---

#### CNetIDToUUCP
```c
char *CNetIDToUUCP(LONG userid);
```
**Registers:** D0
**Category:** IDENT
**Description:** Look up a user's UUCP/Mail ID from their serial ID number.
**Returns:** Pointer to the Mail ID string, or `""` if no match.

---

#### NumFromUnique
```c
short NumFromUnique(char *gokeyword);
```
**Registers:** A0
**Category:** SUB
**Description:** Find the physical subboard number for a GO keyword. Performs case-insensitive matching against `SubboardType4.SubDirName`.
**Returns:** Physical subboard number, or -1 if not found.

---

#### FindOrdinal
```c
struct Node *FindOrdinal(struct List *list, short n);
```
**Registers:** A0/D0
**Category:** MISC
**Description:** Return the Nth node from an Exec linked list.
**Returns:** Pointer to the Nth node.

---

#### InStr
```c
short InStr(char *string, char *isin);
```
**Registers:** A0/A1
**Category:** PARSE
**Description:** Search for substring `isin` within `string`.
**Returns:** Offset of the first match within `string`.

---

#### AllDigits
```c
BOOL AllDigits(char *string);
```
**Registers:** A0
**Category:** PARSE
**Description:** Test whether a string contains only numeric digits (`0-9`).
**Returns:** TRUE if all characters are digits.

---

#### CopyFilterText
```c
void CopyFilterText(char *dest, char *source);
```
**Registers:** A0/A1
**Category:** MCI
**Description:** Copy `source` to `dest`, stripping MCI codes and CR/LF characters, and converting TABs to 3 spaces. Used to prepare text for GUI display.

---

#### IsNowOnLine
```c
struct PortData *IsNowOnLine(struct MainPort *myp, short acc);
```
**Registers:** A0/D0
**Category:** SYS
**Description:** Check whether the user with account number `acc` is currently online.
**Returns:** Pointer to their `PortData` if online, NULL otherwise.

---

#### GetRFCDate
```c
void GetRFCDate(char *datedest);
```
**Registers:** A0
**Category:** DATE
**Description:** Generate an RFC 2822-format date string representing the current time. Uses `locale.library` for timezone information.

---

#### AppendToFile
```c
BOOL AppendToFile(char *filename, char *text, BOOL AddCR);
```
**Registers:** A0/A1/D0
**Category:** FILE
**Description:** Append `text` to the end of `filename`. If `AddCR` is TRUE, a newline (`0x0A`) is appended after the text.
**Returns:** TRUE on success, FALSE on error.

---

#### PItemToArgV
```c
void PItemToArgV(short portnum, char *argv[]);
```
**Registers:** D0/A0
**Category:** PARSE
**Description:** Convert CNet's parsed input items (`z->pitem[]`) for port `portnum` into a standard `argv` array. The array must hold at least `PITEM_MAX` (8) entries.

---

#### CreateDirEntry
```c
struct CNetFileEntry *CreateDirEntry(struct FileInfoBlock *fib);
```
**Registers:** A0
**Category:** DIR
**Description:** Allocate and initialize a `CNetFileEntry` from a `FileInfoBlock`.
**Returns:** Pointer to the new entry, or NULL on allocation failure.

---

#### InsertDirEntry
```c
void InsertDirEntry(struct CNetFileEntry **list, struct CNetFileEntry *newentry);
```
**Registers:** A0/A1
**Category:** DIR
**Description:** Insert `newentry` into a sorted `CNetFileEntry` list. Directories are sorted before files; entries are sorted alphabetically within each group.

---

#### CNetDisposeDir
```c
void CNetDisposeDir(struct CNetFileEntry **list);
```
**Registers:** A0
**Category:** DIR
**Description:** Free all entries in a `CNetFileEntry` list (as returned by `CNetReadDir()`). The list pointer is set to NULL.

---

#### CNetReadDir
```c
struct CNetFileEntry *CNetReadDir(char *path, UBYTE recurse);
```
**Registers:** A0/D0
**Category:** DIR
**Description:** Read a directory into a linked list of `CNetFileEntry` structures with parent/child directory linkage. If `recurse` is TRUE, subdirectories are read recursively.
**Returns:** Pointer to the first entry.
**See also:** `CNetDisposeDir()`

---

#### GetDirEntry
```c
struct CNetFileEntry *GetDirEntry(LONG ordinal, struct CNetFileEntry *list);
```
**Registers:** D0/A0
**Category:** DIR
**Description:** Retrieve the entry at position `ordinal` (1-based; entry 0 is the directory header).
**Returns:** Pointer to the entry, or NULL.

---

#### CountDirEntries
```c
LONG CountDirEntries(struct CNetFileEntry *list);
```
**Registers:** A0
**Category:** DIR
**Description:** Count the number of file/directory entries in the list (excluding parent/child sublists).
**Returns:** Entry count.

---

#### FindParent
```c
struct CNetFileEntry *FindParent(struct CNetFileEntry *list);
```
**Registers:** A0
**Category:** DIR
**Description:** Navigate to the parent directory of the current list.
**Returns:** Pointer to parent, or `list` itself if at root.

---

#### NextDirEntry
```c
struct CNetFileEntry *NextDirEntry(struct CNetFileEntry *list, struct CNetFileEntry *nowentry);
```
**Registers:** A0/A1
**Category:** DIR
**Description:** Get the entry after `nowentry` in `list`.
**Returns:** Pointer to the next entry.

---

#### PrevDirEntry
```c
struct CNetFileEntry *PrevDirEntry(struct CNetFileEntry *list, struct CNetFileEntry *nowentry);
```
**Registers:** A0/A1
**Category:** DIR
**Description:** Get the entry before `nowentry` in `list`.
**Returns:** Pointer to the previous entry.

---

#### HeaderEntry
```c
struct CNetFileEntry *HeaderEntry(struct CNetFileEntry *anyentry);
```
**Registers:** A0
**Category:** DIR
**Description:** Get the header entry (directory name node) for the list containing `anyentry`.
**Returns:** Pointer to the header entry.

---

#### LastEntry
```c
struct CNetFileEntry *LastEntry(struct CNetFileEntry *anyentry);
```
**Registers:** A0
**Category:** DIR
**Description:** Get the last entry in the list containing `anyentry`.
**Returns:** Pointer to the last entry.

---

#### EntryOrdinal
```c
LONG EntryOrdinal(struct CNetFileEntry *entry, struct CNetFileEntry *list);
```
**Registers:** A0/A1
**Category:** DIR
**Description:** Find the ordinal position of `entry` within `list`.
**Returns:** Ordinal number.

---

#### GetVolAssList
```c
struct CNetFileEntry *GetVolAssList(void);
```
**Registers:** (none)
**Category:** DIR
**Description:** Get a list of all mounted volumes and assigns as `CNetFileEntry` nodes.
**Returns:** Pointer to the list.

---

#### CNetSearchEntry
```c
struct CNetFileEntry *CNetSearchEntry(struct CNetFileEntry *list, struct CNetFileEntry **destlist, char *pattern, UBYTE recurse);
```
**Registers:** A0/A1/A2/D0
**Category:** DIR
**Description:** Search a `CNetFileEntry` list for entries matching `pattern`. Matches are appended to `*destlist` (created if NULL). If `recurse` is TRUE, subdirectories are searched recursively.
**Returns:** Pointer to the search result list.

---

#### MCIRemove
```c
char *MCIRemove(char *text);
```
**Registers:** A0
**Category:** MCI
**Description:** Strip all MCI codes from `text` in-place.
**Returns:** Pointer to the modified string (same as input).

---

#### UCacheDispose
```c
void UCacheDispose(void);
```
**Registers:** (none)
**Category:** CACHE
**Description:** Free all resources used by the user data cache. Used only during CNet shutdown. Calling at other times degrades performance.

---

#### UCacheInitUser
```c
struct CNUserCache *UCacheInitUser(short account);
```
**Registers:** D0
**Category:** CACHE
**Description:** Create a cache entry for the specified account. Do NOT call on an already-cached account; check with `UCacheLookup()` first. In general, prefer `LockAccount()`/`UnLockAccount()` which manage caching internally.
**Returns:** Pointer to the cache entry.

---

#### UCacheLookup
```c
struct CNUserCache *UCacheLookup(short account);
```
**Registers:** D0
**Category:** CACHE
**Description:** Retrieve the existing cache entry for an account.
**Returns:** Pointer to the cache entry, or NULL if not cached.

---

#### UCacheRead
```c
short UCacheRead(short account, struct UserData *user, short numaccounts);
```
**Registers:** D0/A0/D1
**Category:** CACHE
**Description:** Read `numaccounts` sequential user records from the cache, starting at `account`, into the `user` array.
**Returns:** Number of accounts actually read.

---

#### UCacheWrite
```c
short UCacheWrite(short account, struct UserData *user, short numaccounts);
```
**Registers:** D0/A0/D1
**Category:** CACHE
**Description:** Write `numaccounts` sequential user records from the `user` array into the cache, starting at `account`.
**Returns:** Number of accounts actually written.

---

#### StartupInitCache
```c
void StartupInitCache(void);
```
**Registers:** (none)
**Category:** CACHE
**Description:** Initialize the user cache at Control startup. There is no need to call this externally.

---

#### ZGetItemPtr
```c
struct ItemType3 *ZGetItemPtr(struct SubboardType4 *s, short c);
```
**Registers:** A0/D0
**Category:** SUB
**Description:** Get a direct pointer to item `c`'s `ItemType3` in the subboard's in-memory array.
**Returns:** Pointer to the `ItemType3`, or NULL.

---

#### ZGetIHeadPtr
```c
struct ItemHeader *ZGetIHeadPtr(struct SubboardType4 *s, short c);
```
**Registers:** A0/D0
**Category:** SUB
**Description:** Get a direct pointer to item `c`'s `ItemHeader` in the subboard's in-memory array.
**Returns:** Pointer to the `ItemHeader`, or NULL.

---

#### CNetStripChars
```c
void CNetStripChars(char *s, char *strip);
```
**Registers:** A0/A1
**Category:** PARSE
**Description:** Remove all characters found in `strip` from string `s` in-place.

---

## cnetc.library

Base name: `_CNetCBase` / `CNetCBase`

66 entries (1 private + 65 public). This is the primary PFile (door
program) API. Functions are invoked via CMessage IPC to the port's
BBS process. The library opens on a per-door basis via
`CNC_InitContext()`.

### Function Table

| # | LVO | Function | Category |
|---|-----|----------|----------|
| 1 | -30 | *Private1* | (reserved) |
| 2 | -36 | CNC_InitContext | SYS |
| 3 | -42 | CNC_DisposeContext | SYS |
| 4 | -48 | CNC_CallHost | SYS |
| 5 | -54 | CNC_PutText | IO |
| 6 | -60 | CNC_PutA | IO |
| 7 | -66 | CNC_ShutDown | SYS |
| 8 | -72 | CNC_EnterLine | IO |
| 9 | -78 | CNC_OneKey | IO |
| 10 | -84 | CNC_EnterPassword | IO |
| 11 | -90 | CNC_CommonCommands | IO |
| 12 | -96 | CNC_ReadFile | IO |
| 13 | -102 | CNC_SetDoing | SYS |
| 14 | -108 | CNC_CallEditor | IO |
| 15 | -114 | CNC_ReadGraphics | IO |
| 16 | -120 | CNC_MakeDate | DATE |
| 17 | -126 | CNC_ReadAccount | USER |
| 18 | -132 | CNC_SaveAccount | USER |
| 19 | -138 | CNC_AddCharge | ACCT |
| 20 | -144 | CNC_CheckBalance | ACCT |
| 21 | -150 | CNC_EnterText | IO |
| 22 | -156 | CNC_ConferenceWait | CONF |
| 23 | -162 | CNC_CheckChanges | SYS |
| 24 | -168 | CNC_ConvertAccess | PARSE |
| 25 | -174 | CNC_GetFree | FILE |
| 26 | -180 | CNC_FindAccount | USER |
| 27 | -186 | CNC_CheckFlowControl | IO |
| 28 | -192 | CNC_ListDir | DIR |
| 29 | -198 | CNC_Rnext | IO |
| 30 | -204 | CNC_ParseCommandLine | PARSE |
| 31 | -210 | CNC_FindCommand | PARSE |
| 32 | -216 | CNC_ReadMessagePoint | SUB |
| 33 | -222 | CNC_EditMessage | IO |
| 34 | -228 | CNC_LoadText | IO |
| 35 | -234 | CNC_WaitForInput | IO |
| 36 | -240 | CNC_SelectAndDownload | XFER |
| 37 | -246 | CNC_VisualDataEditor | IO |
| 38 | -252 | CNC_ExtSetMinFree | XFER |
| 39 | -258 | CNC_ExtSetProtocol | XFER |
| 40 | -264 | CNC_ExtDownload | XFER |
| 41 | -270 | CNC_ExtUpload | XFER |
| 42 | -276 | CNC_PutQ | IO |
| 43 | -282 | CNC_DoReturn | SYS |
| 44 | -288 | CNC_WriteUKeys | USER |
| 45 | -294 | CNC_DoANSI | IO |
| 46 | -300 | CNC_DoANSIOut | IO |
| 47 | -306 | CNC_InputSignalInit | IO |
| 48 | -312 | CNC_InputSignalDispose | IO |
| 49 | -318 | CNC_OWriteLog | LOG |
| 50 | -324 | CNC_VDEapply | IO |
| 51 | -330 | CNC_VDEclose | IO |
| 52 | -336 | CNC_EnterAccess | IO |
| 53 | -342 | CNC_CheckCarrier | IO |
| 54 | -348 | CNC_CheckOLM | IO |
| 55 | -354 | CNC_JMakeDate | DATE |
| 56 | -360 | CNC_GrabFileSize | FILE |
| 57 | -366 | CNC_SaveUser | USER |
| 58 | -372 | CNC_PutQMult | IO |
| 59 | -378 | CNC_MailSend | MAIL |
| 60 | -384 | CNC_ReadLog | LOG |
| 61 | -390 | CNC_OpenDisplay | SYS |
| 62 | -396 | CNC_GetSetFileNote | FILE |
| 63 | -402 | CNC_ExtractFile | XFER |
| 64 | -408 | CNC_DLoadSelect | XFER |
| 65 | -414 | CNC_FindForeignArea | SUB |
| 66 | -420 | CNC_OutScan | SUB |

### Function Reference -- cnetc.library

---

#### CNC_InitContext
```c
struct CNetCContext *CNC_InitContext(char *cport_a1);
```
**Registers:** A1
**Category:** SYS
**Description:** Initialize a door's connection to the BBS process. `cport_a1` is the CPort message port name passed as `argv[1]` by CNet when launching the door. Populates the `CNetCContext` with pointers to `myp`, `z`, and `cport`. Must be called before any other `CNC_*` function.
**Returns:** Pointer to `CNetCContext`, or NULL on failure.
**See also:** `CNC_DisposeContext()`, `CNCL_DoorInit()`

---

#### CNC_DisposeContext
```c
void CNC_DisposeContext(void);
```
**Registers:** (none)
**Category:** SYS
**Description:** Release the door context allocated by `CNC_InitContext()`. Should be called during door shutdown (normally handled by `CNCL_DoorCleanup()`).

---

#### CNC_CallHost
```c
void CNC_CallHost(UBYTE c);
```
**Registers:** D1
**Category:** SYS
**Description:** Send a raw CMessage command `c` to the BBS process. Used for low-level IPC. Most doors use the higher-level wrapper functions instead.

---

#### CNC_PutText
```c
void CNC_PutText(char *text);
```
**Registers:** A1
**Category:** IO
**Description:** Output a text string to the user's terminal. MCI codes in the text are processed. This is the primary output function for doors.

---

#### CNC_PutA
```c
void CNC_PutA(void);
```
**Registers:** (none)
**Category:** IO
**Description:** Send the contents of `z->ABuffer` to the user's terminal. Use `sprintf()` to format text into ABuffer before calling this function. MCI codes in the buffer are processed.

---

#### CNC_ShutDown
```c
void CNC_ShutDown(char *spawn);
```
**Registers:** A1
**Category:** SYS
**Description:** Signal the BBS process that the door is exiting. If `spawn` is non-NULL, it specifies a command to run next (door chaining). Pass NULL for normal exit.

---

#### CNC_EnterLine
```c
int CNC_EnterLine(UBYTE len, ULONG flags, char *prompt);
```
**Registers:** D1/D2/A1
**Category:** IO
**Description:** Prompt the user for a line of text input. `len` is the maximum length. `flags` is a combination of `ELINE_*` constants (see `io.h`). `prompt` is the prompt string. The entered text is placed in `z->InBuffer`.
**Returns:** Length of the entered text.

---

#### CNC_OneKey
```c
char CNC_OneKey(void);
```
**Registers:** (none)
**Category:** IO
**Description:** Wait for and return a single keypress from the user.
**Returns:** The character pressed (uppercase unless `z->NoCaseConvert` is set).

---

#### CNC_EnterPassword
```c
void CNC_EnterPassword(UBYTE len);
```
**Registers:** D1
**Category:** IO
**Description:** Prompt for password input (characters are not echoed). `len` is the maximum length. Result is in `z->InBuffer`.

---

#### CNC_CommonCommands
```c
long CNC_CommonCommands(void);
```
**Registers:** (none)
**Category:** IO
**Description:** Process common BBS commands typed by the user (OLM, WHO, etc.). Should be called after `CNC_EnterLine()` when the door allows standard commands.
**Returns:** Non-zero if a common command was processed.

---

#### CNC_ReadFile
```c
UBYTE CNC_ReadFile(char *file, UBYTE flags);
```
**Registers:** A1/D1
**Category:** IO
**Description:** Display a text file to the user with MCI processing. `flags` controls display options.
**Returns:** Non-zero on success.

---

#### CNC_SetDoing
```c
void CNC_SetDoing(char *what);
```
**Registers:** A1
**Category:** SYS
**Description:** Set the "Doing" / "Action" string displayed in the WHO list for this port.

---

#### CNC_CallEditor
```c
void CNC_CallEditor(short max, short inlines);
```
**Registers:** D1/D2
**Category:** IO
**Description:** Invoke the BBS text editor. `max` is the maximum number of lines. `inlines` is the number of pre-loaded lines (from `z->EdBuffer`).

---

#### CNC_ReadGraphics
```c
UBYTE CNC_ReadGraphics(char *path, char flags);
```
**Registers:** A1/D1
**Category:** IO
**Description:** Display an ANSI/graphics file to the user. Similar to `CNC_ReadFile()` but optimized for graphics content.
**Returns:** Non-zero on success.

---

#### CNC_MakeDate
```c
void CNC_MakeDate(struct IsDate *date, char *output);
```
**Registers:** A1/A2
**Category:** DATE
**Description:** Format an `IsDate` structure into a human-readable date string.

---

#### CNC_ReadAccount
```c
UBYTE CNC_ReadAccount(short id, struct UserData *udata);
```
**Registers:** D1/A1
**Category:** USER
**Description:** Read a user's account data into `udata`. Handles locking internally.
**Returns:** Non-zero on success.

---

#### CNC_SaveAccount
```c
UBYTE CNC_SaveAccount(struct UserData *udata, short id);
```
**Registers:** A1/D1
**Category:** USER
**Description:** Save modified user data for account `id`. Handles locking internally.
**Returns:** Non-zero on success.

---

#### CNC_AddCharge
```c
UBYTE CNC_AddCharge(short n, short a);
```
**Registers:** D1/D2
**Category:** ACCT
**Description:** Apply charge type `n` with multiplier `a` to the current user.
**Returns:** Non-zero if charge was applied.

---

#### CNC_CheckBalance
```c
UBYTE CNC_CheckBalance(short n, short a);
```
**Registers:** D1/D2
**Category:** ACCT
**Description:** Check if the current user has sufficient balance for charge type `n` with multiplier `a`.
**Returns:** Non-zero if the user can afford the charge.

---

#### CNC_EnterText
```c
int CNC_EnterText(char firstchar, short maxchars, short perline, short maxlines);
```
**Registers:** D1/D2/D3/D4
**Category:** IO
**Description:** Multi-line text entry. `firstchar` is an optional pre-typed character. `maxchars` is the character limit. `perline` is the line width. `maxlines` is the maximum number of lines.
**Returns:** Number of lines entered.

---

#### CNC_ConferenceWait
```c
long CNC_ConferenceWait(short a);
```
**Registers:** D1
**Category:** CONF
**Description:** Enter conference (chat) wait mode.
**Returns:** Result code.

---

#### CNC_CheckChanges
```c
void CNC_CheckChanges(void);
```
**Registers:** (none)
**Category:** SYS
**Description:** Check for and process pending changes (OLMs, time updates, etc.). Should be called periodically during long-running door operations.

---

#### CNC_ConvertAccess
```c
long CNC_ConvertAccess(char *s);
```
**Registers:** A1
**Category:** PARSE
**Description:** Door-side wrapper for `ConvertAccess()`. Converts a flag range string to a packed LONG.
**Returns:** Packed bitfield.

---

#### CNC_GetFree
```c
long CNC_GetFree(char *name, UBYTE quiet, char *deststring, char **bm);
```
**Registers:** A1/D1/A2/A3
**Category:** FILE
**Description:** Get the free space for a volume or path. If `quiet` is TRUE, suppresses output. `deststring` and `bm` are optional (may be NULL in the simplified `CGetFree()` wrapper).
**Returns:** Free space value.

---

#### CNC_FindAccount
```c
short CNC_FindAccount(char *a, struct UserData *b, UBYTE c);
```
**Registers:** A1/A2/D1
**Category:** USER
**Description:** Search for a user account by name/handle. Results are placed in `*b`. `c` controls quiet mode.
**Returns:** Account number found.

---

#### CNC_CheckFlowControl
```c
void CNC_CheckFlowControl(void);
```
**Registers:** (none)
**Category:** IO
**Description:** Check for space (pause/skip) and Ctrl-C (abort) keypresses. Updates `z->PrintFlags` accordingly. Call before long output loops to respect user abort requests.

---

#### CNC_ListDir
```c
long CNC_ListDir(UBYTE a, UBYTE b, struct IsDate *c);
```
**Registers:** D1/D2/A1
**Category:** DIR
**Description:** Display a directory listing to the user with BBS formatting.
**Returns:** Result code.

---

#### CNC_Rnext
```c
UBYTE CNC_Rnext(void);
```
**Registers:** (none)
**Category:** IO
**Description:** Advance to the next message/response in a read sequence.
**Returns:** Non-zero if there is a next item.

---

#### CNC_ParseCommandLine
```c
void CNC_ParseCommandLine(UBYTE numargs);
```
**Registers:** D1
**Category:** PARSE
**Description:** Parse the user's input line into `z->pitem[]` array elements. `numargs` specifies the maximum number of arguments to parse.

---

#### CNC_FindCommand
```c
short CNC_FindCommand(short num);
```
**Registers:** D1
**Category:** PARSE
**Description:** Look up a command number in the parsed command table.
**Returns:** Command index.

---

#### CNC_ReadMessagePoint
```c
void CNC_ReadMessagePoint(char *a, long b);
```
**Registers:** A1/D1
**Category:** SUB
**Description:** Read and display a message starting at seek position `b` in file `a`.

---

#### CNC_EditMessage
```c
void CNC_EditMessage(char *file);
```
**Registers:** A1
**Category:** IO
**Description:** Open the BBS editor for editing the specified file.

---

#### CNC_LoadText
```c
void CNC_LoadText(BPTR fh);
```
**Registers:** A1
**Category:** IO
**Description:** Load text from file handle `fh` into the editor buffer.

---

#### CNC_WaitForInput
```c
char CNC_WaitForInput(long mics);
```
**Registers:** D1
**Category:** IO
**Description:** Wait for user input with a timeout of `mics` microseconds.
**Returns:** The character received, or 0 on timeout.

---

#### CNC_SelectAndDownload
```c
UBYTE CNC_SelectAndDownload(char *file, UBYTE flags);
```
**Registers:** A1/D1
**Category:** XFER
**Description:** Add a file to the download selection list and optionally begin the download. `flags` is a combination of `SAD_FLAG_*` constants.
**Returns:** Non-zero on success.

---

#### CNC_VisualDataEditor
```c
short CNC_VisualDataEditor(char *file, void *data, long size);
```
**Registers:** A1/A2/D1
**Category:** IO
**Description:** Open the Visual Data Editor (VDE) for editing a binary data structure. `file` is the VDE definition file. `data` points to the data buffer. `size` is the buffer length.
**Returns:** Result code.

---

#### CNC_ExtSetMinFree
```c
void CNC_ExtSetMinFree(long free);
```
**Registers:** D1
**Category:** XFER
**Description:** Set the minimum free disk space required for external file transfers.

---

#### CNC_ExtSetProtocol
```c
UBYTE CNC_ExtSetProtocol(char a);
```
**Registers:** D1
**Category:** XFER
**Description:** Set the external file transfer protocol.
**Returns:** Non-zero on success.

---

#### CNC_ExtDownload
```c
char *CNC_ExtDownload(char *args);
```
**Registers:** A1
**Category:** XFER
**Description:** Initiate an external download using the currently set protocol.
**Returns:** Result string.

---

#### CNC_ExtUpload
```c
char *CNC_ExtUpload(char *args);
```
**Registers:** A1
**Category:** XFER
**Description:** Initiate an external upload using the currently set protocol.
**Returns:** Result string.

---

#### CNC_PutQ
```c
UBYTE CNC_PutQ(char *a);
```
**Registers:** A1
**Category:** IO
**Description:** Display a prompt and wait for a Yes/No response.
**Returns:** Non-zero if the user answered Yes.

---

#### CNC_DoReturn
```c
void CNC_DoReturn(void);
```
**Registers:** (none)
**Category:** SYS
**Description:** Signal the BBS process that the door has completed its current operation and control should return to the calling context.

---

#### CNC_WriteUKeys
```c
BYTE CNC_WriteUKeys(BYTE getsem);
```
**Registers:** D1
**Category:** USER
**Description:** Save the user's key file data. If `getsem` is TRUE, acquires the necessary semaphore.
**Returns:** Result code.

---

#### CNC_DoANSI
```c
void CNC_DoANSI(UBYTE n, USHORT a, USHORT b);
```
**Registers:** D1/D2/D3
**Category:** IO
**Description:** Execute an ANSI control sequence. `n` is the sequence type. `a` and `b` are parameters.

---

#### CNC_DoANSIOut
```c
void CNC_DoANSIOut(UBYTE n);
```
**Registers:** D1
**Category:** IO
**Description:** Output a predefined ANSI sequence number `n`.

---

#### CNC_InputSignalInit
```c
BYTE CNC_InputSignalInit(char *portname, ULONG timeout);
```
**Registers:** A1/D1
**Category:** IO
**Description:** Set up signal-based notification for user input. `portname` is the message port name. `timeout` is the wait timeout in microseconds. The door receives `CN_USERINPUT` (0xFF) signals when input arrives. Used for non-blocking input loops.
**Returns:** 0 on success.
**See also:** `CNC_InputSignalDispose()`

---

#### CNC_InputSignalDispose
```c
void CNC_InputSignalDispose(void);
```
**Registers:** (none)
**Category:** IO
**Description:** Tear down input signal notification set up by `CNC_InputSignalInit()`.

---

#### CNC_OWriteLog
```c
void CNC_OWriteLog(short n, char *text1, char *text2);
```
**Registers:** D1/A1/A2
**Category:** LOG
**Description:** Write a log entry from a door context. Parameters match `WriteLog()` but the PortData is derived from the door's context automatically.

---

#### CNC_VDEapply
```c
void CNC_VDEapply(void *data0, void *data1);
```
**Registers:** A1/A2
**Category:** IO
**Description:** Apply changes from VDE editing to the target data structures.

---

#### CNC_VDEclose
```c
void CNC_VDEclose(void);
```
**Registers:** (none)
**Category:** IO
**Description:** Close the Visual Data Editor and free its resources.

---

#### CNC_EnterAccess
```c
long CNC_EnterAccess(char *prompt, long def);
```
**Registers:** A1/D1
**Category:** IO
**Description:** Prompt the user to enter an access group selection. `def` is the default value.
**Returns:** The selected access value as a packed LONG.

---

#### CNC_CheckCarrier
```c
UBYTE CNC_CheckCarrier(void);
```
**Registers:** (none)
**Category:** IO
**Description:** Check if the modem carrier signal is still active. Should be called periodically to detect disconnections.
**Returns:** Non-zero if carrier is present.

---

#### CNC_CheckOLM
```c
char CNC_CheckOLM(void);
```
**Registers:** (none)
**Category:** IO
**Description:** Check for and display any pending Online Messages (OLMs).
**Returns:** Character code if OLM was present.

---

#### CNC_JMakeDate
```c
void CNC_JMakeDate(struct IsDate *date, char *output);
```
**Registers:** A1/A2
**Category:** DATE
**Description:** Format an `IsDate` into a Julian-style date string.

---

#### CNC_GrabFileSize
```c
long CNC_GrabFileSize(UBYTE quiet);
```
**Registers:** D1
**Category:** FILE
**Description:** Determine a file's size, optionally prompting the user. If `quiet` is TRUE, suppresses output.
**Returns:** File size value.

---

#### CNC_SaveUser
```c
void CNC_SaveUser(struct UserData *user, short id);
```
**Registers:** A1/D1
**Category:** USER
**Description:** Save user data for account `id`. Similar to `CNC_SaveAccount()`.

---

#### CNC_PutQMult
```c
UBYTE CNC_PutQMult(char *prompt, char *poss);
```
**Registers:** A1/A2
**Category:** IO
**Description:** Display `prompt` and accept one of the characters in `poss`.
**Returns:** 1-based index of the selected character.

---

#### CNC_MailSend
```c
void CNC_MailSend(short account);
```
**Registers:** D1
**Category:** MAIL
**Description:** Invoke the mail composition interface, pre-addressed to `account`.

---

#### CNC_ReadLog
```c
void CNC_ReadLog(char *logname);
```
**Registers:** A1
**Category:** LOG
**Description:** Display a CNet log file to the user.

---

#### CNC_OpenDisplay
```c
void CNC_OpenDisplay(short portnum);
```
**Registers:** D1
**Category:** SYS
**Description:** Open the display/screen for a port number.

---

#### CNC_GetSetFileNote
```c
void CNC_GetSetFileNote(char *pathfile);
```
**Registers:** A1
**Category:** FILE
**Description:** Get or set the AmigaOS file note (comment) for a file.

---

#### CNC_ExtractFile
```c
UBYTE CNC_ExtractFile(char *pathfile);
```
**Registers:** A1
**Category:** XFER
**Description:** Extract a file from an archive.
**Returns:** Non-zero on success.

---

#### CNC_DLoadSelect
```c
void CNC_DLoadSelect(UBYTE optdel);
```
**Registers:** D1
**Category:** XFER
**Description:** Begin downloading files from the selection list. If `optdel` is set, offers deletion after download.

---

#### CNC_FindForeignArea
```c
short CNC_FindForeignArea(char *gokey);
```
**Registers:** A1
**Category:** SUB
**Description:** Look up a subboard by GO keyword, including foreign (non-local) areas.
**Returns:** Subboard number, or -1 if not found.

---

#### CNC_OutScan
```c
void CNC_OutScan(void);
```
**Registers:** (none)
**Category:** SUB
**Description:** Perform an outbound scan for new messages in subscribed areas.

---

## cnet4.library

Base name: `_CNet4Base` / `CNet4Base`

15 functions providing date/time operations and range parsing for CNet 4/5.

### Function Table

| # | LVO | Function | Category |
|---|-----|----------|----------|
| 1 | -30 | CNetPrintTime | DATE |
| 2 | -36 | CNetTime | DATE |
| 3 | -42 | CNetExplodeTime | DATE |
| 4 | -48 | CNetImplodeTime | DATE |
| 5 | -54 | CNetDowDate | DATE |
| 6 | -60 | CNetPrintDate | DATE |
| 7 | -66 | CNetFindRange | PARSE |
| 8 | -72 | CNetNextRange | PARSE |
| 9 | -78 | CNetLongToRange | PARSE |
| 10 | -84 | CNetRangeToLong | PARSE |
| 11 | -90 | CNetTimeNowDiff | DATE |
| 12 | -96 | CNetEndOfRange | PARSE |
| 13 | -102 | CNetPrevRange | PARSE |
| 14 | -108 | CNetStartOfRange | PARSE |
| 15 | -114 | CNetUpTime | DATE |

### Function Reference -- cnet4.library

---

#### CNetPrintTime
```c
void CNetPrintTime(ULONG t, char *out, char **strings, struct UserData *user);
```
**Registers:** D0/A0/A1/A2
**Category:** DATE
**Description:** Format a CNet time value `t` (seconds since epoch) into a string according to the user's time format preference. `strings` points to format strings. `user` provides timezone/format preferences.

Note: The FD file declares the 4th parameter as `format` with register A2, but the prototype uses `struct UserData *user`.

---

#### CNetTime
```c
ULONG CNetTime(void);
```
**Registers:** (none)
**Category:** DATE
**Description:** Get the current time as a CNet time value (seconds since the Amiga epoch, 1978-01-01 00:00:00).
**Returns:** Current time in seconds.

---

#### CNetExplodeTime
```c
void CNetExplodeTime(ULONG t, struct IsDate *date);
```
**Registers:** D0/A0
**Category:** DATE
**Description:** Convert a CNet time value (seconds) into an `IsDate` structure.

---

#### CNetImplodeTime
```c
ULONG CNetImplodeTime(struct IsDate *date);
```
**Registers:** A0
**Category:** DATE
**Description:** Convert an `IsDate` structure into a CNet time value (seconds).
**Returns:** Seconds since epoch.

---

#### CNetDowDate
```c
USHORT CNetDowDate(struct IsDate *d);
```
**Registers:** A0
**Category:** DATE
**Description:** Determine the day of the week for a given date.
**Returns:** Day of week (0 = Sunday, 6 = Saturday).

---

#### CNetPrintDate
```c
void CNetPrintDate(struct IsDate *date, char *out, char **strings, char *format);
```
**Registers:** A1/A2/A3/A4
**Category:** DATE
**Description:** Format an `IsDate` into a string using a custom format pattern and localized strings.

---

#### CNetFindRange
```c
long CNetFindRange(char *buffer, long min, long max, struct RangeContext *rc);
```
**Registers:** A0/D0/D1/A1
**Category:** PARSE
**Description:** Parse a range string (e.g., `"1-5,8,10-20"`) and initialize a `RangeContext` for iteration. Values are clamped to `[min, max]`.
**Returns:** The first element in the range, or -1 if the range is empty.
**See also:** `CNetNextRange()`, `CNetPrevRange()`

---

#### CNetNextRange
```c
long CNetNextRange(struct RangeContext *rc);
```
**Registers:** A0
**Category:** PARSE
**Description:** Advance to the next element in a parsed range.
**Returns:** The next element, or -1 at end of range.
**See also:** `CNetFindRange()`

---

#### CNetLongToRange
```c
void CNetLongToRange(long def, char *where);
```
**Registers:** D0/A0
**Category:** PARSE
**Description:** Convert a packed LONG bitfield to a range string. Identical to `ExpandFlags()` in `cnet.library`.

---

#### CNetRangeToLong
```c
long CNetRangeToLong(char *s);
```
**Registers:** A0
**Category:** PARSE
**Description:** Convert a range string to a packed LONG bitfield. Identical to `ConvertAccess()` in `cnet.library`.
**Returns:** Packed LONG.

---

#### CNetTimeNowDiff
```c
LONG CNetTimeNowDiff(ULONG prevtime);
```
**Registers:** D0
**Category:** DATE
**Description:** Calculate the number of seconds between `prevtime` and the current time.
**Returns:** Difference in seconds.

---

#### CNetEndOfRange
```c
UBYTE CNetEndOfRange(struct RangeContext *rc);
```
**Registers:** A0
**Category:** PARSE
**Description:** Check whether the current element is the last in the range.
**Returns:** TRUE if at the end of the range.

---

#### CNetPrevRange
```c
long CNetPrevRange(struct RangeContext *rc);
```
**Registers:** A0
**Category:** PARSE
**Description:** Move to the previous element in a parsed range.
**Returns:** The previous element.

---

#### CNetStartOfRange
```c
UBYTE CNetStartOfRange(struct RangeContext *rc);
```
**Registers:** A0
**Category:** PARSE
**Description:** Check whether the current element is the first in the range.
**Returns:** TRUE if at the start of the range.

---

#### CNetUpTime
```c
char *CNetUpTime(char *diffstr, LONG timediff);
```
**Registers:** A1/D1
**Category:** DATE
**Description:** Format a time difference (in seconds) into a human-readable uptime string (e.g., `"2d 5h 30m"`).
**Returns:** Pointer to `diffstr`.

---

## cnetmail.library

Base name: `_CNetMailBase` / `CNetMailBase`

14 functions for MIME processing and mail routing.

### Function Table

| # | LVO | Function | Category |
|---|-----|----------|----------|
| 1 | -30 | MIMEParseBoundary | MAIL |
| 2 | -36 | MIMEIsBoundary | MAIL |
| 3 | -42 | MIMEParseContentType | MAIL |
| 4 | -48 | MIMEFindType | MAIL |
| 5 | -54 | MIMEParseName | MAIL |
| 6 | -60 | MIMEFromExt | MAIL |
| 7 | -66 | DisposeMailTo | MAIL |
| 8 | -72 | NameToList | MAIL |
| 9 | -78 | IsUUCP | MAIL |
| 10 | -84 | IsFido | MAIL |
| 11 | -90 | IsLocal | MAIL |
| 12 | -96 | SendMail | MAIL |
| 13 | -102 | GetMailSems | MAIL |
| 14 | -108 | GetMPSems | MAIL |

### Function Reference -- cnetmail.library

---

#### MIMEParseBoundary
```c
UBYTE MIMEParseBoundary(char *contentline, char *boundary);
```
**Registers:** A0/A1
**Description:** Extract the MIME boundary delimiter from a `Content-Type:` header line.
**Returns:** Non-zero if a boundary was found and placed in `boundary`.

---

#### MIMEIsBoundary
```c
UBYTE MIMEIsBoundary(char *line, char *boundary);
```
**Registers:** A0/A1
**Description:** Test whether `line` is a MIME boundary line matching the `boundary` string obtained from `MIMEParseBoundary()`.
**Returns:** Non-zero if it matches.

---

#### MIMEParseContentType
```c
UBYTE MIMEParseContentType(char *input, char *contentres);
```
**Registers:** A0/A1
**Description:** Extract the content-type string from a header line.
**Returns:** Non-zero if a content-type was found and placed in `contentres`.

---

#### MIMEFindType
```c
struct MIMEContentType *MIMEFindType(char *typestring, char *encodingstring);
```
**Registers:** A0/A1
**Description:** Look up the CNet `MIMEContentType` structure for the given content type and encoding. `encodingstring` may be NULL for a base content-type match.
**Returns:** Pointer to the `MIMEContentType`, or NULL.

---

#### MIMEParseName
```c
UBYTE MIMEParseName(char *line, char *name, char *base);
```
**Registers:** A0/A1/A2
**Description:** Extract the filename from a MIME attachment header. `base` is the base path where the attachment will be stored.
**Returns:** Non-zero if a filename was found.

---

#### MIMEFromExt
```c
struct MIMEContentType *MIMEFromExt(char *ext);
```
**Registers:** A0
**Description:** Look up the `MIMEContentType` for a file extension.
**Returns:** Pointer to the `MIMEContentType`, or NULL.

---

#### DisposeMailTo
```c
void DisposeMailTo(struct MailToList **mailtolist);
```
**Registers:** A0
**Description:** Free a `MailToList` linked list and set the pointer to NULL.

---

#### NameToList
```c
struct MailToList *NameToList(char *name, char *uucp, struct MailToList *mailtolist, UBYTE mailtype);
```
**Registers:** A0/A1/A2/D0
**Description:** Add a recipient to a `MailToList`. `name` is the display name, `uucp` is the address, `mailtype` is the address type.
**Returns:** Updated list pointer.

---

#### IsUUCP
```c
struct MailToList *IsUUCP(char *net, struct MailToList *mailtolist);
```
**Registers:** A0/A1
**Description:** Parse an internet-style address and add it to the mail list.
**Returns:** Updated list pointer.

---

#### IsFido
```c
struct MailToList *IsFido(char *net, USHORT *faddr, struct CNetNodeDesc *nd, struct MailToList *mailtolist);
```
**Registers:** A0/A1/A2/A3
**Description:** Parse a FidoNet address and add it to the mail list.
**Returns:** Updated list pointer.

---

#### IsLocal
```c
struct MailToList *IsLocal(char *name, struct MailToList *mailtolist);
```
**Registers:** A0/A1
**Description:** Check if `name` is a local user and add them to the mail list.
**Returns:** Updated list pointer.

---

#### SendMail
```c
UBYTE SendMail(char *from, char *to, char *subject, char *text, LONG smflags);
```
**Registers:** A0/A1/A2/A3/D0
**Description:** Compose and send a mail message programmatically.
**Returns:** Non-zero on success.

---

#### GetMailSems
```c
struct SignalSemaphore *GetMailSems(void);
```
**Registers:** (none)
**Description:** Get a pointer to the per-account mail semaphore array (`myp->MPE->MailSem`). Used to lock individual mailboxes during concurrent access.
**Returns:** Pointer to the semaphore array.

---

#### GetMPSems
```c
struct SignalSemaphore *GetMPSems(void);
```
**Registers:** (none)
**Description:** Get a pointer to the MainPort semaphore array (`myp->SEM[]`).
**Returns:** Pointer to the semaphore array.

---

## cnetgui.library

Base name: `_CNetGuiBase` / `CNetGuiBase`

2 functions for CNet GUI support.

### Function Table

| # | LVO | Function | Category |
|---|-----|----------|----------|
| 1 | -30 | CNGTextWidth | MISC |
| 2 | -36 | InitWindowStruct | MISC |

### Function Reference -- cnetgui.library

---

#### CNGTextWidth
```c
UWORD CNGTextWidth(UBYTE *str, struct TextAttr *Attr);
```
**Registers:** A0/A1
**Description:** Calculate the pixel width of a text string when rendered with the specified font.
**Returns:** Width in pixels.

---

#### InitWindowStruct
```c
APTR InitWindowStruct(void);
```
**Registers:** (none)
**Description:** Allocate and initialize a CNet window structure for GUI operations.
**Returns:** Pointer to the initialized structure.

---

## libcnet_door.a

The linker library linked into every CNet C door program. Provides door
lifecycle management, convenience wrappers around `CNC_*` shared library
functions, and standalone utility functions.

Source files: `lib/door_init.c`, `lib/door_printf.c`, `lib/cdoor_wrappers.c`,
`lib/cdoor_extra.c`, `lib/utility.c`

### Door Lifecycle Functions

#### CNCL_DoorInit
```c
BOOL CNCL_DoorInit(int argc, char **argv);
```
**Source:** `door_init.c`
**Description:** Initialize the door's CNet connection. Opens `cnetc.library`,
calls `CNC_InitContext(argv[1])`, and populates the global pointers
(`CNetCBase`, `context`, `z`, `myp`, `cport`). Registers `CNCL_DoorCleanup()`
as an `atexit()` handler.

Call at the top of `main()`. CNet passes the CPort name as `argv[1]`.

**Returns:** TRUE on success, FALSE on failure.

**API note:** The original SAS/C version used `ReadArgs()` with signature
`CNCL_DoorInit(LONG *myargs)`. The GCC version uses standard `argc`/`argv`.

---

#### CNCL_DoorCleanup
```c
void CNCL_DoorCleanup(void);
```
**Source:** `door_init.c`
**Description:** Shut down the door's CNet connection. Calls `CNC_ShutDown(NULL)`
and `CNC_DisposeContext()`, then closes the library. Safe to call at any
point, including during partial initialization. Registered as an `atexit()`
handler by `CNCL_DoorInit()`.

---

#### CNCL_Printf
```c
void CNCL_Printf(char *fmt, ...);
```
**Source:** `door_printf.c`
**Description:** Formatted output to the BBS terminal. Works like `printf()`
but sends the result through `CNC_PutText()`. Uses a static 4096-byte
buffer; output is silently truncated if it exceeds that length.

---

### CNC_* Wrapper Functions (cdoor_wrappers.c)

These are simple C wrappers that delegate to the corresponding `CNC_*`
shared library function. They provide the short-name API (`CPutText`,
`COneKey`, etc.) used by most door code.

| Wrapper | Delegates to | Notes |
|---------|-------------|-------|
| `CPutText(char *text)` | `CNC_PutText()` | |
| `CPutA(void)` | `CNC_PutA()` | |
| `COneKey(void)` | `CNC_OneKey()` | |
| `CEnterLine(UBYTE len, ULONG flags, char *prompt)` | `CNC_EnterLine()` | |
| `CEnterPassword(UBYTE len)` | `CNC_EnterPassword()` | |
| `CCommonCommands(void)` | `CNC_CommonCommands()` | |
| `CReadFile(char *path, UBYTE flags)` | `CNC_ReadFile()` | |
| `CSetDoing(char *what)` | `CNC_SetDoing()` | |
| `CCallEditor(short max, short inlines)` | `CNC_CallEditor()` | |
| `CReadGraphics(char *path, char flags)` | `CNC_ReadGraphics()` | |
| `CMakeDate(struct IsDate *date, char *output)` | `CNC_MakeDate()` | |
| `CReadAccount(short id, struct UserData *user)` | `CNC_ReadAccount()` | |
| `CSaveAccount(struct UserData *user, short id)` | `CNC_SaveAccount()` | |
| `CAddCharge(short n, short a)` | `CNC_AddCharge()` | |
| `CCheckBalance(short n, short a)` | `CNC_CheckBalance()` | |
| `CEnterText(char firstchar, short maxchars, short perline, short maxlines)` | `CNC_EnterText()` | |
| `CConferenceWait(short a)` | `CNC_ConferenceWait()` | |
| `CCheckChanges(void)` | `CNC_CheckChanges()` | |
| `CConvertAccess(char *s)` | `CNC_ConvertAccess()` | |
| `CGetFree(char *s, UBYTE q)` | `CNC_GetFree()` | Simplified: extra args passed as NULL |
| `CFindAccount(char *a, struct UserData *b, UBYTE quiet)` | `CNC_FindAccount()` | |
| `CCheckFlowControl(void)` | `CNC_CheckFlowControl()` | |
| `CListDir(UBYTE a, UBYTE b, struct IsDate *c)` | `CNC_ListDir()` | |
| `CRnext(void)` | `CNC_Rnext()` | |
| `CParseCommandLine(UBYTE numargs)` | `CNC_ParseCommandLine()` | |
| `CFindCommand(short num)` | `CNC_FindCommand()` | |
| `CReadMessagePoint(char *a, long b)` | `CNC_ReadMessagePoint()` | |
| `CEditMessage(char *file)` | `CNC_EditMessage()` | |
| `CLoadText(BPTR fh)` | `CNC_LoadText()` | |
| `CWaitForInput(long mics)` | `CNC_WaitForInput()` | |
| `CSelectAndDownload(char *file, UBYTE flags)` | `CNC_SelectAndDownload()` | |
| `CVisualDataEditor(char *file, void *data, long size)` | `CNC_VisualDataEditor()` | |
| `CExtSetMinFree(long free)` | `CNC_ExtSetMinFree()` | |
| `CExtSetProtocol(char a)` | `CNC_ExtSetProtocol()` | |
| `CExtDownload(char *args)` | `CNC_ExtDownload()` | |
| `CExtUpload(char *args)` | `CNC_ExtUpload()` | |
| `CPutQ(char *a)` | `CNC_PutQ()` | |
| `CDoReturn(void)` | `CNC_DoReturn()` | |
| `CWriteUKeys(BYTE getsem)` | `CNC_WriteUKeys()` | |
| `CDoANSI(UBYTE n, USHORT a, USHORT b)` | `CNC_DoANSI()` | |
| `CDoANSIOut(UBYTE n)` | `CNC_DoANSIOut()` | |
| `CCheckOLM(void)` | `CNC_CheckOLM()` | |
| `CGrabFileSize(UBYTE quiet)` | `CNC_GrabFileSize()` | |
| `CSaveUser(struct UserData *user, short id)` | `CNC_SaveUser()` | |
| `CPutQMult(char *prompt, char *poss)` | `CNC_PutQMult()` | |
| `PutQMult(char *prompt, char *possibilities)` | `CNC_PutQMult()` | Legacy alias |
| `CCheckCarrier(void)` | `CNC_CheckCarrier()` | |
| `COWriteLog(short n, char *text1, char *text2)` | `CNC_OWriteLog()` | |
| `CVDEapply(void *data0, void *data1)` | `CNC_VDEapply()` | |
| `CVDEclose(void)` | `CNC_VDEclose()` | |
| `CEnterAccess(char *prompt, long def)` | `CNC_EnterAccess()` | |
| `CShutDown(char *spawn)` | `CNC_ShutDown()` | |
| `CallHost(UBYTE c)` | `CNC_CallHost()` | v3-style raw IPC |

---

#### CLoadError
```c
void CLoadError(void);
```
**Source:** `cdoor_wrappers.c`
**Description:** Error handler for door load failures. Calls `exit(RETURN_FAIL)`.
The `atexit()` handler (`CNCL_DoorCleanup()`) handles resource teardown.

---

### Editor List Functions (cdoor_extra.c)

These manage the Exec-list-based editor used in CNetC doors. Each editor
line is an `EditorLine` node containing a standard `Node` header and an
81-byte text buffer.

#### CNC_MakeEd
```c
struct EditorLine *CNC_MakeEd(void);
```
**Description:** Allocate and initialize an empty `EditorLine` node.
**Returns:** Pointer to the new node, or NULL on failure.

---

#### CNC_DeleteEd
```c
void CNC_DeleteEd(struct EditorLine *ed);
```
**Description:** Free an `EditorLine` node, removing it from its list first if linked.

---

#### CNC_OpenEd
```c
BPTR CNC_OpenEd(char *filename);
```
**Description:** Open a text file for loading into the editor.
**Returns:** AmigaOS file handle, or ZERO on failure.

---

#### CNC_PrepEditor
```c
void CNC_PrepEditor(struct List *editlist);
```
**Description:** Initialize (or reinitialize) an Exec List for use as an editor line list. Frees any existing nodes.

---

#### CNC_SaveEditor
```c
void CNC_SaveEditor(struct List *editlist, char *filename);
```
**Description:** Write all editor lines to a file, one line per node, with newline terminators.

---

#### CMakeEd
```c
void CMakeEd(char *path);
```
**Description:** Construct an editor temp file path and invoke `CNC_EditMessage()`.
Caller must provide a valid path.

---

#### CNC_compstra
```c
LONG CNC_compstra(char *s1, char *s2);
```
**Description:** Case-insensitive string comparison.
**Returns:** 0 if equal, negative if `s1 < s2`, positive if `s1 > s2`.

---

### Utility Functions (utility.c)

Standalone functions that do not require `cnetc.library`. They operate
directly on CNet data structures or provide general-purpose helpers.

#### String Utilities

| Function | Prototype | Description |
|----------|-----------|-------------|
| `compstra` | `short compstra(char *s, char *t)` | Case-insensitive string comparison |
| `SToUpper` | `void SToUpper(char *s)` | Convert string to uppercase in-place |
| `SToLower` | `void SToLower(char *s)` | Convert string to lowercase in-place |
| `stristr` | `char *stristr(char *haystack, char *needle)` | Case-insensitive substring search |
| `RemoveLeadingSpace` | `UBYTE *RemoveLeadingSpace(UBYTE *s)` | Strip leading whitespace in-place |
| `RemoveNonAlNum` | `void RemoveNonAlNum(char *a)` | Remove non-alphanumeric characters in-place |
| `checkspaces` | `BOOL checkspaces(UBYTE *str, LONG checklen)` | Trim trailing spaces; returns TRUE if non-empty |

#### Port Queries

| Function | Prototype | Description |
|----------|-----------|-------------|
| `IsPortLoaded` | `BOOL IsPortLoaded(short portnum)` | Check if a BBS port is active |
| `IsPortOccupied` | `BOOL IsPortOccupied(short portnum)` | Check if a port has a connected user |
| `IsOnLine` | `struct PortData *IsOnLine(short acc)` | Find online user by account; returns their PortData |

#### User Helpers

| Function | Prototype | Description |
|----------|-----------|-------------|
| `CalculateAge` | `short CalculateAge(struct UserData *user)` | Calculate age from user's Birthdate field |
| `ReadAccount` | `UBYTE ReadAccount(short id, struct UserData *user)` | Read user record from disk (no locking) |
| `SaveAccount` | `UBYTE SaveAccount(struct UserData *user, short id)` | Write user record to disk (no locking) |
| `FindPosition` | `short FindPosition(struct List *list, struct Node *node)` | Find 0-based position of a node in an Exec list |

#### I/O Helpers

| Function | Prototype | Description |
|----------|-----------|-------------|
| `IsAbort` | `BOOL IsAbort(void)` | Check if user pressed space/^C (checks `z->PrintFlags`) |
| `AddKeyBuffer` | `void AddKeyBuffer(char *c)` | Inject characters into the port's keyboard buffer |

#### Date/Time

| Function | Prototype | Description |
|----------|-----------|-------------|
| `GetDate` | `void GetDate(struct IsDate *dest)` | Fill IsDate with current date/time |
| `PutDateHere` | `void PutDateHere(struct IsDate *dest)` | Alias for `GetDate()` |
| `LongToDate` | `void LongToDate(char *date, char *t, ULONG was)` | Convert seconds-since-epoch to `"MM-DD-YY"` and `"HH:MM:SS"` strings |

#### Memory

| Function | Prototype | Description |
|----------|-----------|-------------|
| `SafeFreeMem` | `void SafeFreeMem(void *b, ULONG s)` | NULL-safe wrapper for `FreeMem()` |
| `SafeFreeVec` | `void SafeFreeVec(void *b)` | NULL-safe wrapper for `FreeVec()` |

#### Global Variables

The linker library provides these global symbols, initialized by
`CNCL_DoorInit()`:

| Variable | Type | Description |
|----------|------|-------------|
| `CNetCBase` | `struct Library *` | cnetc.library base pointer |
| `context` | `struct CNetCContext *` | Door context (contains myp, z, cport) |
| `z` | `struct PortData *` | Current port's state structure |
| `myp` | `struct MainPort *` | System-global BBS state |
| `cport` | `struct CPort *` | Door communication port |
| `myrdargs` | `struct RDArgs *` | SAS/C compatibility symbol (unused in GCC doors) |
| `__stack` | `unsigned long` | Set to 65536 (libnix stack size) |
