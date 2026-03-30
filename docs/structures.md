# CNet 5 SDK Structure Reference

Field-by-field documentation for all major CNet data structures. All
structs use SAS/C 2-byte (word) alignment, which is the natural
alignment on m68k. On other architectures (x86, ARM, etc.), this
alignment is enforced via `include/cnet/align.h`, which provides
`CNET_PACK_BEGIN` / `CNET_PACK_END` macros wrapping
`#pragma pack(push, 2)` / `#pragma pack(pop)`.

**Offset caveat:** Header offset comments in the original SDK source are
not authoritative and contain known errors. Offsets verified through
binary testing on a live CNet v5.36c system are marked as confirmed.
Unverified offsets are listed as they appear in the headers but should
not be relied upon for binary-level access.

---

## Table of Contents

1. [IsDate](#isdate) -- 6 bytes
2. [UserData](#userdata) -- 672 bytes
3. [Privs](#privs) -- 92 bytes
4. [AccessGroup](#accessgroup) -- 156 bytes
5. [PortData](#portdata) -- ~10 KB
6. [PortDataExtension](#portdataextension)
7. [MainPort](#mainport) -- system-global state
8. [MainPortExtension](#mainportextension)
9. [SubboardType4](#subboardtype4) -- ~696 bytes
10. [ItemType3](#itemtype3) -- 168 bytes
11. [ItemHeader](#itemheader) -- 34 bytes
12. [HeaderType](#headertype) -- response/post header
13. [MessageType3](#messagetype3) -- message response record
14. [BaseUser](#baseuser) -- per-user-per-subboard
15. [SelectType](#selecttype) -- file selection entry
16. [CPort](#cport) -- door communication port
17. [CMessage](#cmessage) -- door IPC message
18. [CNetCContext](#cnetccontext) -- cnetc.library context
19. [SerPort4](#serport4) -- modem/port configuration
20. [RangeContext](#rangecontext) -- range parsing state
21. [CNetFileEntry](#cnetfileentry) -- directory listing node
22. [MailHeader4](#mailheader4) -- v4.12+ mail header
23. [NewMailFolder](#newmailfolder) -- mail folder
24. [ChargeSet / ChargeType](#chargeset--chargetype) -- accounting
25. [CNUserCache](#cnusercache) -- user cache entry
26. [KeyElement4](#keyelement4) -- user key index
27. [NewConfig1](#newconfig1) -- global configuration
28. [PortConfig](#portconfig) -- per-port configuration
29. [FreeType](#freetype) -- subboard free-space node
30. [Other Structures](#other-structures)

---

## IsDate

**Header:** `include/cnet/dates.h`
**Size:** 6 bytes

CNet's compact date/time representation. `Year` is stored as an offset
from 1900 (`ISDATE_BASE_YEAR`), so year 2000 is stored as 100.

| Offset | Field | Type | Description |
|--------|-------|------|-------------|
| 0 | Year | UBYTE | Year minus 1900 |
| 1 | Month | UBYTE | Month (1-12) |
| 2 | Date | UBYTE | Day of month (1-31) |
| 3 | Hour | UBYTE | Hour (0-23) |
| 4 | Minute | UBYTE | Minute (0-59) |
| 5 | Second | UBYTE | Second (0-59) |

**Usage:** Found throughout the system -- user records, item timestamps,
subboard metadata, port state. Functions like `CNetExplodeTime()` and
`CNetImplodeTime()` convert between `IsDate` and seconds-since-epoch.

**Related defines:**
- `ISDATE_BASE_YEAR` = 1900
- `SECONDS_IN_DAY` = 86400
- `SECONDS_IN_HOUR` = 3600
- `SECONDS_IN_MINUTE` = 60

---

## UserData

**Header:** `include/cnet/users.h`
**Size:** 672 bytes (LONG-aligned)

The complete user account record, stored in `sysdata:bbs.udata4`. One
record per account, indexed by account number. Contains identity,
statistics, preferences, and an embedded `Privs` structure.

| Offset | Field | Type | Size | Description |
|--------|-------|------|------|-------------|
| 0 | IDNumber | long | 4 | Unique serial ID (never reused) |
| 4 | Handle | char[] | 21 | User's handle/nickname |
| 25 | RealName | char[] | 26 | Real name |
| 51 | Address | char[] | 31 | Street address |
| 82 | CityState | char[] | 31 | City and state |
| 113 | ZipCode | char[] | 11 | Zip/postal code |
| 124 | Country | char[] | 8 | Country code |
| 132 | PhoneNo | char[] | 17 | Data phone (Euro 8-7 format possible) |
| 149 | PassWord | char[] | 15 | Encrypted password |
| 164 | Comments | char[] | 34 | Sysop comments |
| 198 | Macro[3] | char[][36] | 108 | User-defined macro keys |
| 306 | Organ | char[] | 31 | Organization |
| 337 | PhoneVerified | UBYTE | 1 | Phone verification flag |
| 338 | z | PortData * | 4 | Transient: set during LockAccount() |
| 342 | Birthdate | IsDate | 6 | User's birthday |
| 348 | FirstCall | IsDate | 6 | Date of first call |
| 354 | LastCall | IsDate | 6 | Date of most recent call |
| 360 | EMailFlags | ULONG | 4 | Email preference flags |
| 364 | InterfaceType | USHORT | 2 | 0 = CNet/4 style, 1 = CNet/3 style |
| 366 | ConnectDate | IsDate | 6 | Date of current connection |
| 372 | YankDate | IsDate | 6 | Date of last yank operation |
| 378 | Access | BYTE | 1 | Current access group level (0-31) |
| 379 | ExpireAccess | BYTE | 1 | Access level to expire to |
| 380 | ShowNewsHead | UBYTE | 1 | Show UUCP news headers |
| 381 | PfileTimeToday | BYTE | 1 | PFile time used today |
| 382 | SplitChatFlags | UBYTE | 1 | Bit 0: split-screen chat |
| 383 | AutoDecodeMail | BYTE | 1 | Auto-decode MIME in email |
| 384 | TextSet | BYTE | 1 | BBSTEXT/MENU translation set |
| 385 | TimeForm | BYTE | 1 | Time display format |
| 386 | MoreMode | BYTE | 1 | "More?" prompt mode |
| 387 | LineFeeds | BYTE | 1 | User requires line feeds |
| 388 | FracBal | BYTE | 1 | Balance fraction (1/100 cent) |
| 389 | TimeZone | BYTE | 1 | Timezone offset from system time |
| 390 | Colors | BYTE | 1 | ANSI color support |
| 391 | Tabs | BYTE | 1 | ANSI tab support |
| 392 | ANSI | BYTE | 1 | ANSI emulation type |
| 393 | Sex | BYTE | 1 | Gender |
| 394 | AutoMore | BYTE | 1 | Auto "More?" and CLS between messages |
| 395 | TermType | BYTE | 1 | Terminal emulation type |
| 396 | HelpLevel | BYTE | 1 | Help level (novice/intermediate/expert) |
| 397 | CompType | BYTE | 1 | Computer type (index into BBSMENU section 30) |
| 398 | DefProtocol | BYTE | 1 | Default transfer protocol |
| 399 | TermWidth | BYTE | 1 | Terminal width in characters |
| 400 | TermLength | BYTE | 1 | Terminal height in lines |
| 401 | MailBoxOpen | BYTE | 1 | Mailbox open/closed state |
| 402 | MailForward | short | 2 | Forward mail to this account (0 = disabled) |
| 404 | TotalCalls | long | 4 | Lifetime call count |
| 408 | PubMessages | long | 4 | Public messages posted |
| 412 | PriMessages | long | 4 | Private messages sent |
| 416 | UpBytes | long | 4 | Total uploaded kilobytes |
| 420 | UpFiles | long | 4 | Total uploaded files |
| 424 | DownBytes | long | 4 | Total downloaded kilobytes |
| 428 | DownFiles | long | 4 | Total downloaded files |
| 432 | FileCredits | long | 4 | File credit balance |
| 436 | ByteCredits | long | 4 | Byte credit balance |
| 440 | TimeCredits | long | 4 | Time credit balance |
| 444 | Balance | long | 4 | Account financial balance |
| 448 | NetCredits | long | 4 | Network credits |
| 452 | DoorPoints | long | 4 | Door/game point balance |
| 456 | DayUpBytes | long | 4 | Bytes uploaded today |
| 460 | DayDownBytes | long | 4 | Bytes downloaded today |
| 464 | DayUpFiles | short | 2 | Files uploaded today |
| 466 | DayDownFiles | short | 2 | Files downloaded today |
| 468 | CallsToday | short | 2 | Calls made today |
| 470 | TimeToday | short | 2 | Minutes online today |
| 472 | LogonAttempts | short | 2 | Failed logon attempts |
| 474 | BaudRate | USHORT | 2 | Current connection baud rate |
| 476 | HighBaud | USHORT | 2 | Highest baud ever connected |
| 478 | UUCP | char[] | 11 | Mail ID (max 9 chars + terminator) |
| 489 | PName | UBYTE | 1 | Privacy: name |
| 490 | PAge | UBYTE | 1 | Privacy: age |
| 491 | PAddress | UBYTE | 1 | Privacy: street address |
| 492 | PVoice | UBYTE | 1 | Privacy: phone numbers |
| 493 | PData | UBYTE | 1 | Privacy: data |
| 494 | Banner | char[] | 43 | User's banner/tagline |
| 537 | VoiceNo | char[] | 17 | Voice phone number |
| 554 | YankCount | short | 2 | Number of yanks |
| 556 | ExpireDate | IsDate | 6 | Account expiration date |
| 562 | FNewDate | IsDate | 6 | "Files new since" date |
| 568 | more | char[] | 7 | Reserved |
| 575 | YankCR | UBYTE | 1 | Add CRs to yank text |
| 576 | YankArc | UBYTE | 1 | Yank archiver preference |
| 577 | DefEditor | BYTE | 1 | Default editor |
| 578 | AutoHide | BYTE | 1 | Auto-hide flag |
| 579 | MailSort | BYTE | 1 | Mail sorting preference |
| 580 | MyPrivs | Privs | 92 | Effective privilege set |

**Access:** Use `LockAccount()`/`UnLockAccount()` to read/modify safely.
Doors should use `CNC_ReadAccount()`/`CNC_SaveAccount()`. The `z->user1`
field in `PortData` contains the currently logged-in user's data (offset 68).

---

## Privs

**Header:** `include/cnet/users.h`
**Size:** 92 bytes

Privilege and access control flags. Each user has an embedded `Privs` in
their `UserData` (at offset 580). Access groups (`AccessGroup`) also
contain a `Privs` that serves as the template.

| Offset | Field | Type | Size | Description |
|--------|-------|------|------|-------------|
| 0 | MBaseFlags | long | 4 | Message base access flags |
| 4 | FBaseFlags | long | 4 | File base access flags |
| 8 | LBaseFlags | long | 4 | GFiles/PFiles/Telnet/Other flags |
| 12 | ABits | ULONG | 4 | Primary ability bitfield (32 flags) |
| 16 | DailyDownBytes | long | 4 | Daily download byte limit |
| 20 | DailyUpBytes | long | 4 | Daily upload byte limit |
| 24 | XFreeBytes | long | 4 | (Unused) |
| 28 | XFreeFiles | long | 4 | (See global default in Config1) |
| 32 | Calls | short | 2 | Maximum calls per day |
| 34 | CallMinutes | short | 2 | Maximum minutes per call |
| 36 | DailyMinutes | short | 2 | Maximum minutes per day |
| 38 | DailyDownloads | short | 2 | Maximum downloads per day |
| 40 | DailyUploads | short | 2 | Maximum uploads per day |
| 42 | Messages | short | 2 | Maximum messages per call |
| 44 | Feedbacks | short | 2 | Maximum feedbacks per call |
| 46 | EditorLines | short | 2 | Maximum editor lines |
| 48 | Idle | short | 2 | Idle timeout (minutes) |
| 50 | MaxMailKBytes | short | 2 | Maximum mailbox size (KB) |
| 52 | PurgeDays | short | 2 | Account purge threshold (days) |
| 54 | FileRatio | UBYTE | 1 | File ratio set 1 |
| 55 | ByteRatio | UBYTE | 1 | Byte ratio set 1 |
| 56 | FileRatio2 | UBYTE | 1 | File ratio set 2 |
| 57 | ByteRatio2 | UBYTE | 1 | Byte ratio set 2 |
| 58 | FileRatio3 | UBYTE | 1 | File ratio set 3 |
| 59 | ByteRatio3 | UBYTE | 1 | Byte ratio set 3 |
| 60 | ABits2 | ULONG | 4 | Extended ability bitfield (32 more flags) |
| 64 | SigLines | short | 2 | Maximum signature lines |
| 66 | AllowAliases | UBYTE | 1 | Tri-state: allow aliases |
| 67 | DeleteOwn | UBYTE | 1 | Tri-state: delete own posts |
| 68 | Anonymous | UBYTE | 1 | Tri-state: anonymous posting |
| 69 | PrivateArea | UBYTE | 1 | Tri-state: private areas |
| 70 | DailyPfileMinutes | short | 2 | Daily PFile time limit |
| 72 | LogToMail | short | 2 | Account to receive log copies |
| 74 | LogFlags | long | 4 | Log event flags |
| 78 | CallBack | UBYTE | 1 | Tri-state: callback verification |
| 79 | TermLink | UBYTE | 1 | TermLink access |
| 80 | CallerID | UBYTE | 1 | Caller ID requirement |
| 81 | PageSysop | UBYTE | 1 | Sysop page permission |
| 82 | notused | LONG | 4 | Reserved |
| 86 | exp | char[] | 2 | Reserved |
| 88 | Alias | short | 2 | Alias settings |
| 90 | Dictionary | short | 2 | Dictionary settings |

### ABits Bitfield

| Bit | Name | Description |
|-----|------|-------------|
| 0 | EMAIL | Can use email |
| 1 | PFILE | Can run PFiles (doors) |
| 2 | GFILE | Can access GFiles |
| 3 | ULIST | Can access user list |
| 4 | SYSOP | Has sysop privileges |
| 5 | REWARDS | Receives download rewards |
| 6 | FCREDEX | File credit exempt |
| 7 | BCREDEX | Byte credit exempt |
| 8 | EXPIREMAIL | Send expiration notice via mail |
| 9 | BULKMAIL | Can send bulk mail |
| 11 | URGENTMAIL | Can send urgent mail |
| 12 | TIMESBP | Time credits for uploads |
| 13 | FILEADD | Can add files |
| 14 | READANY | Can read any message |
| 15 | DELETEANY | Can delete any message |
| 16 | SUSPENDACCT | Account suspended |
| 18 | AUTOVALID | Auto-validate uploads |
| 20 | SEEANON | Can see anonymous posters |
| 22 | CONF | Can access conference |
| 23 | CONFCON | Conference control |
| 24 | MCI1 | MCI level 1 access |
| 25 | MCI2 | MCI level 2 access |
| 26 | RELOGON | Can re-logon |
| 27 | RECEIVEMAIL | Can receive mail |
| 28 | FORWARD | Can forward mail |
| 29 | VOTETOPIC | Can create vote topics |
| 30 | VOTECHOICE | Can add vote choices |
| 31 | NOLOCKS | Bypass subboard locks |

### ABits2 Bitfield

| Bit | Name | Description |
|-----|------|-------------|
| 0 | EDHANDLE | Can edit own handle |
| 1 | EDREALNAME | Can edit own real name |
| 2 | EDADDRESS | Can edit own address |
| 3 | EDVOICE | Can edit own voice phone |
| 4 | EDDATA | Can edit own data phone |
| 5 | BANNER | Can set banner |
| 7 | PORTMONITOR | Can monitor ports |
| 8 | VOTEMAINT | Vote maintenance |
| 9 | ALARMSYSOP | Can alarm sysop |
| 10 | OPENSCREEN | Can open port screens |
| 11 | OPENCAPTURE | Can open captures |
| 12 | NETMAIL | Can send netmail |
| 13 | FREQSEND | Can send file requests |
| 14 | NETCOSTEXEMPT | Net cost exempt |
| 20 | NETCOSTCREDITS | Use credits for net costs |
| 21 | HOLDANDCRASH | Hold and crash flags |
| 22 | FEMPTYTRASH | Force empty trash |
| 23 | BROADCAST | Can send broadcasts |
| 24 | SUPERUSER | Superuser access |

---

## AccessGroup

**Header:** `include/cnet/users.h`
**Size:** 156 bytes

Access level template. CNet supports up to 32 access groups, stored in
`myp->AGC[0..31]`. When a user's access level is set, their `MyPrivs` is
typically initialized from the corresponding `AccessGroup.DefPrivs`.

| Offset | Field | Type | Size | Description |
|--------|-------|------|------|-------------|
| 0 | Name | char[] | 32 | Access group name |
| 32 | DefPrivs | Privs | 92 | Default privilege set |
| 124 | ExpireDays | short | 2 | Days until access expires |
| 126 | ExpireAccess | UBYTE | 1 | Group to expire to |
| 127 | exp | char[] | 29 | Reserved |

---

## PortData

**Header:** `include/cnet/ports.h`
**Size:** Approximately 10 KB

Per-port state structure -- one per loaded BBS port. Contains the current
user's data, I/O state, editor buffers, subboard context, transfer state,
screen/window resources, and much more. Referenced via `z` in door code.

This is the largest and most complex CNet structure. Fields are grouped
by functional area below. Due to the structure's size and the number of
embedded sub-structures, only the most commonly accessed fields are
documented.

### Current Session State (offsets 0-67)

| Offset | Field | Type | Description |
|--------|-------|------|-------------|
| 0 | Today | IsDate | Current date/time |
| 6 | VConnectDate | IsDate | Virtual connect date (e.g., midnight reset) |
| 12 | id | short | Current user's account number |
| 14 | InPort | short | Port number |
| 16 | Age | short | User's age |
| 18 | CurrentCPS | short | Current characters-per-second rate |
| 20 | OnType | short | Logon type |
| 22 | OffType | short | Logoff type |
| 24 | CallMessages | short | Messages written this call |
| 26 | CallFeedbacks | short | Feedbacks written this call |
| 28 | TimeLeft | short | Time remaining (tenths of minutes) |
| 32 | TimeOnLine | short | Time online (tenths of minutes) |
| 34 | TimeCredit | short | Bonus time from uploads, etc. |
| 38 | Carrier | short | Carrier detect / local mode flag |
| 40 | AName | char * | Pointer to access group name string |
| 44 | SysMaint | long | TRUE if user has SYSOP_FLAG |
| 64 | Caller | long | Caller number for this session |

### User Data (offsets 68-1411)

| Offset | Field | Type | Description |
|--------|-------|------|-------------|
| 68 | user1 | UserData | Current user's account data (672 bytes) |
| 740 | user2 | UserData | Secondary user data (672 bytes) |

### Navigation State

| Offset | Field | Type | Description |
|--------|-------|------|-------------|
| 1412 | MyMail | short | User's mail count |
| 1414 | MyNewMail | short | User's new mail count |
| 1416 | bn | short | Current subboard number |

### I/O Flags (various offsets)

Key I/O-related fields scattered through the structure:

| Field | Type | Description |
|-------|------|-------------|
| OnLine | UBYTE | User is logged in (1742+5 area) |
| PrintFlags | UBYTE | Space/^C abort state |
| AtPrompt | UBYTE | At input prompt |
| InTransfer | UBYTE | In file transfer |
| ConfMode | UBYTE | In conference mode |
| EditorOn | char | Editor is active |
| NoMCI | UBYTE | Suppress MCI processing |
| FreezeTime | UBYTE | Stop time counting |

### Buffers

| Field | Type | Description |
|-------|------|-------------|
| InBuffer | char[256] | User input after CEnterLine() |
| KBuff | char[256] | Keyboard input buffer (circular, KB1/KB2) |
| GBuffer[16][81] | char[][] | General-purpose text buffers |
| pitem[8][61] | char[][] | Parsed input items |
| npitems | short | Number of parsed items |

### Editor State

| Field | Type | Description |
|-------|------|-------------|
| EdBuffer | char ** | Array of pointers to editor lines |
| EdBase | char * | Pointer to editor memory block |
| CR | UBYTE * | Carriage return flags per line |
| txline | long | Number of lines in editor buffer |
| edbuff | char | Editor save state: 1=saved, 0=empty, -1=aborted |

### Door Use

| Field | Type | Description |
|-------|------|-------------|
| uz[10] | long[] | General purpose longs for PFile use (initialized to 0) |
| nPdepth | UBYTE | PFile nesting depth |
| CRunCount | short | C PFile depth count |
| CSpawn | char[84] | Door chain command |

### Subboard Context

| Field | Type | Description |
|-------|------|-------------|
| sb | SubboardType4 * | Pointer to current subboard |
| sorder | short[240] | Subboard ordering array |
| bz | short | Number of subs on current list |
| dz | short | Directory depth |
| br | short | Last used item number |

### Mail State

| Field | Type | Description |
|-------|------|-------------|
| Folders | NewMailFolder * | Linked list of mail folders |
| Folder0 | NewMailFolder * | Current folder pointer |
| rmhead | MailHeader4 * | Linked list of mail headers |
| Aliases | MailAlias * | Linked list of mail aliases |
| OMail0 | MailHeader4 | General purpose mail header |

### System Pointers

| Field | Type | Description |
|-------|------|-------------|
| ThisTask | Task * | AmigaOS task pointer |
| cnp | MainPort * | Pointer to system MainPort |
| Window | Window * | Primary Intuition window |
| Screen | Screen * | Intuition screen |
| SocketBase | Library * | bsdsocket.library base (when opened) |

### Extension

| Field | Type | Description |
|-------|------|-------------|
| PDE | PortDataExtension * | Extension structure pointer |

---

## PortDataExtension

**Header:** `include/cnet/ports.h`

Extension fields for `PortData`, introduced in v4 to avoid breaking the
`PortData` binary layout. Accessed via `z->PDE`.

| Field | Type | Description |
|-------|------|-------------|
| TNHost0 | TNHostEntry * | Telnet host being listed/edited |
| CurrentTNHost | TNHostEntry * | Currently connected telnet host |
| GoMatches | LONG * | GO keyword match array |
| DynamicGoKey | char * | Current GO keyword for listings |
| DynamicGoTitle | char * | Current subboard title for GO listings |
| DynamicGoNum | LONG | Current GO match ordinal |
| InMailFwd | UBYTE | Currently forwarding mail |
| def_part | short | Default file partition |
| MailArrived | short | New mails pending for online user |
| lastby | char[52] | "Last message by" display field |
| sp | SerPort4 | Port serial/modem configuration |
| AtOLMPrompt | UBYTE | At OLM read prompt |
| mydloc | char[128] | Current directory for dmaint |
| dflag | LONG | dmaint control flags (see `DM_FLAG_*`) |
| dmaintup | UBYTE | dmaint is running |
| DefItem | ItemType3 | ARexx default item template |
| DefHeader | HeaderType | ARexx default header template |
| DefMessage | MessageType3 | ARexx default message template |
| DefIHead | ItemHeader | ARexx default item header |
| TelnetDMode | UBYTE | In telnetd I/O mode |
| dsock | int | Socket for telnetd session I/O |
| telnetdata | TelnetdPort * | Telnetd connection data |

**DM_FLAG defines:**
- `DM_FLAG_DIRMAINT` (0x01) -- full directory maintenance
- `DM_FLAG_CDROM` (0x02) -- use CDROM temp directory
- `DM_FLAG_NODATA` (0x04) -- hide CNet data files

---

## MainPort

**Header:** `include/cnet/control.h`

System-global state structure. One per BBS instance, located via
`FindPort("cnetport")`. Contains the Exec MsgPort, system statistics,
configuration, subboard array, port pointers, semaphores, and more.
Referenced via `myp` in door code.

### Core Fields

| Field | Type | Description |
|-------|------|-------------|
| mp | MsgPort | Standard Exec message port (34 bytes) |
| portName | char[22] | Port name (typically `"cnetport"`) |
| LastOn | char[32] | Name of last user to log on |
| Nums[5] | long[] | System counters (see `NUMS_*`) |
| SAG[2][72] | ULONG[][] | System Activity Graph data |
| SAM[5][15] | long[][] | System Activity Monitor data |
| SAMDate[6] | IsDate[] | SAM date stamps; [1] = boot time |
| um[10] | long[] | General purpose longs for PFile use |

### NUMS Array Indices

| Index | Define | Description |
|-------|--------|-------------|
| 0 | NUMS_CURRENT_ACCOUNTS | Total physical user accounts |
| 1 | NUMS_INUSE_ACCOUNTS | Currently active accounts |
| 2 | NUMS_HIGH_ID | Last assigned serial ID |
| 3 | NUMS_CALLS_TOTAL | Total calls ever |
| 4 | NUMS_CALLS_LOGGED | Currently logged-in users |

### Pointers and Resources

| Field | Type | Description |
|-------|------|-------------|
| PWindow | Window * | Control panel window |
| Subboard | SubboardType4 * | Array of all subboards |
| Key | KeyElement4 * | User key index array |
| IName | short * | Name-sorted index into Key[] |
| IPhone | short * | Phone-sorted index into Key[] |
| SubTitle | char * | Subboard title string |
| FirstCharge | ChargeSet * | Charge schedule linked list |
| ns | long | Number of allocated subboards |

### Task State

| Field | Type | Description |
|-------|------|-------------|
| toss_up | UBYTE | Toss/import task state (`TASK_UP_*`) |
| yank_up | UBYTE | Yank task state |
| mail_up | UBYTE | Mail task state |
| fileman_up | UBYTE | File task state |
| ftpd_up | UBYTE | FTPd state |
| serv_exit | UBYTE | Set TRUE to signal all servers to quit |
| OpenPfiles | long | Number of currently running doors |

### Semaphores

`myp->SEM[0..18]` -- critical section locks:

| Index | Purpose |
|-------|---------|
| 0 | MainPort itself |
| 1 | User Key[], IName, Phone arrays |
| 2 | Upload/selection lists |
| 3 | Serial device |
| 4 | NUMS[1] (active account count) |
| 5 | Subboard data files and structures |
| 7 | Conference/join chat |
| 8 | Conference rooms |
| 10 | PFile locking |
| 11 | bbs.udata, bbs.cuser, userlist |
| 12 | Log files |
| 13 | LIST-type subboard items |
| 14 | Vote data |
| 16 | CopyCDRom() function |
| 18 | SAM data |

### Port Arrays

| Field | Type | Description |
|-------|------|-------------|
| AGC[32] | AccessGroup[] | Access group definitions |
| gc | NewConfig1 | Global configuration |
| pc[101] | PortConfig[] | Per-port config (0-100) |
| PortZ[100] | PortData *[] | Per-port PortData pointers |
| CRoom[100] | Room *[] | Per-port room pointers |
| HiPort | short | Highest loaded port number |
| nPorts | short | Number of loaded ports |

### Extension

| Field | Type | Description |
|-------|------|-------------|
| MPE | MainPortExtension * | Extension structure |

### TASK_UP States

| Value | Define | Meaning |
|-------|--------|---------|
| 0 | TASK_UP_NOTRUNNING | Not running |
| 1 | TASK_UP_GUIOPEN | GUI is open |
| 2 | TASK_UP_HIDDEN | Totally hidden |
| 3 | TASK_UP_MENUFIED | Menu-fied |
| 4 | TASK_UP_ICONIFIED | Iconified |
| 5 | TASK_UP_RUNNING | Starting up (temporary) |

---

## MainPortExtension

**Header:** `include/cnet/control.h`

Extension structure for MainPort, accessed via `myp->MPE`.

### Semaphores

`myp->MPE->sem[0..19]` -- additional critical section locks:

| Index | Purpose |
|-------|---------|
| 0 | reload_text variable |
| 1 | Identd data (idd) insertion/removal |
| 2 | Global MailKill list |
| 3 | IMP list |
| 4 | Hostname/SPAM filter config |
| 5 | uumail directory (pauses mail-task) |
| 6 | Global MIME list |
| 7 | FTPd list / processes |
| 8 | TxProcess list |
| 9-19 | Unassigned |

### Key Fields

| Field | Type | Description |
|-------|------|-------------|
| reload_text | UBYTE | Set to 1 to trigger text/menu reload |
| newsman_up | UBYTE | News task state |
| newstask | Task * | News task pointer |
| idd | IdentdData * | Identd data linked list |
| MailKillList | List * | Mail kill pattern list |
| gc2 | ConfigExtension | Extended configuration |
| IMPList | List | IMP (Inter-Module Process) list |
| statline | char * | Control stats line buffer |
| MailSem | SignalSemaphore * | Per-account mail semaphores |
| mimelist | MIMEContentType * | MIME type linked list |
| DCCList | List | DCC request list (IRC) |
| pubscreenname | char * | Public screen name for GUIs |
| FTPdList | List | Active FTPd process list |
| TxProcessList | List | File test/transform process list |
| telnetd_acnt | short | Auto-loaded telnetd port count |

---

## SubboardType4

**Header:** `include/cnet/subs.h`
**Size:** ~696 bytes

Subboard/area configuration structure. Stored in `myp->Subboard[]` array
and on disk in the subboard data files. Encompasses message bases, file
areas, doors, text files, and subdirectories.

### Identity and Paths

| Offset | Field | Type | Size | Description |
|--------|-------|------|------|-------------|
| 0 | Title | char[] | 60 | Display title |
| 60 | SubDirName | char[] | 75 | GO keyword / subdirectory name |
| 135 | DataPath | char[] | 95 | Path to data files |

### Configuration

| Offset | Field | Type | Description |
|--------|-------|------|-------------|
| 230 | Parts | long | Partition bitmap (bits 0-31) |
| 234 | SubOpIDs[6] | long[] | Subop user serial IDs |
| 258 | MRewardBytes | short | Upload reward: % of byte size |
| 260 | NRewardFiles | short | Upload reward: file credits per download |
| 274 | SubOpAccs[6] | short[] | Subop account numbers |
| 286 | Filler[6] | char[] | Shown during SCAN |
| 294 | Direct | UBYTE | Disk-exchange type |
| 295 | Vote | UBYTE | Non-sysop vote topic creation |
| 296 | FreeDays | short | Days until files become free |
| 298 | Subdirectory | UBYTE | Is this a subdirectory? |
| 299 | Closed | UBYTE | Subboard is closed |

### Access Control

| Offset | Field | Type | Description |
|--------|-------|------|-------------|
| 300 | UnionFlags | long | Required user flags (all must match) |
| 304 | Access | long | Groups that may enter |
| 308 | PostAccess | long | Groups that may post |
| 312 | RespondAccess | long | Groups that may respond |
| 316 | UploadAccess | long | Groups that may upload |
| 320 | DownloadAccess | long | Groups that may download |
| 324 | ComputerTypes | long | Computer type filter |
| 328 | Hours | long | Restricted entry hours |
| 340 | Baud | long | Minimum baud rate |
| 348 | Gender | UBYTE | Gender restriction (0/'M'/'F') |
| 349 | AllowAliases | UBYTE | Alias permission |

### Accounting

| Offset | Field | Type | Description |
|--------|-------|------|-------------|
| 360 | MDownBytes | short | % of download bytes to subtract |
| 362 | NDownFiles | short | Files to subtract per download |
| 364 | MTimeCredit | short | % of time to credit for uploads |
| 366 | MinFreeBytes | long | Minimum free space for uploads |
| 383 | SDownFiles | BYTE | Download charge file schedule |
| 384 | SDownBytes | BYTE | Download charge byte schedule |

### Tree Structure

| Offset | Field | Type | Description |
|--------|-------|------|-------------|
| 462 | Parent | short | Physical number of parent list |
| 464 | Child | short | Physical number of child list (-1 if none) |
| 466 | Next | short | Physical number of next sibling (-1 if none) |

### Runtime State

| Offset | Field | Type | Description |
|--------|-------|------|-------------|
| 262 | free0 | FreeType * | Free-text hole list |
| 424 | count | ULONG | Item count (replaces _COUNT file) |
| 456 | Users | short | Current active users in this sub |
| 458 | Created | long | Creation timestamp |
| 620 | MaxItems | USHORT | Maximum items allowed |
| 624 | Item | ItemType3 * | In-memory item array |
| 628 | ihead | ItemHeader * | In-memory item header array |
| 632 | NewMess | MessageType3 * | New message array |
| 636 | sem | SignalSemaphore * | Subboard semaphore |

### Marker Field

The `Marker` field (offset 407) encodes the subboard type:

| Value | Define | Meaning |
|-------|--------|---------|
| 0 | MRK_MSG_BASE | Message base (subdirectories are also type 0 with `Subdirectory=1`) |
| 1 | MRK_FILE_TXFER | File transfer area |
| 3 | MRK_TEXT_DOOR | Text door |
| 4 | MRK_TEXT_FILE | Text file |
| 5 | MRK_CNETC_DOOR | CNetC (C language) door |
| 6 | MRK_AREXX_DOOR | ARexx door |
| 7 | MRK_ADOS_DOOR | AmigaDOS door |
| 8 | MRK_BBS_MACRO | BBS macro |
| 9 | MRK_DIRECT_COMMANDER | Direct commander |

Flags in the Marker byte:
- Bit 7 (0x80): `MRK_SUBBOARD_KILLED` -- deleted subboard
- Bit 6 (0x40): `MRK_SUBBOARD_ROOT` -- root subboard

Use `(Marker & MRK_SUBBOARD_BASE)` (mask 0x3F) to extract the type.

---

## ItemType3

**Header:** `include/cnet/subs.h`
**Size:** 168 bytes

Item (post or file) metadata. Stored in the subboard's `_Items` data
file and cached in `SubboardType4.Item[]`.

| Offset | Field | Type | Size | Description |
|--------|-------|------|------|-------------|
| 0 | Title | char[] | 42 | Item title |
| 42 | ByAccount | short | 2 | Uploader/poster's account number |
| 44 | ByID | long | 4 | Poster's serial ID |
| 48 | ToID | long | 4 | Recipient's serial ID |
| 52 | UsedDate | IsDate | 6 | Last access date |
| 58 | Part | short | 2 | Partition number |
| 60 | Downloads | long | 4 | Download count |
| 64 | ByteCharges | long | 4 | Byte charges applied |
| 68 | FileCharges | short | 2 | File charges applied |
| 70 | Private | UBYTE | 1 | Private item |
| 71 | DLnotifyULer | UBYTE | 1 | Notify uploader on download |
| 73 | Frozen | UBYTE | 1 | Item is frozen |
| 74 | Free | UBYTE | 1 | Free download |
| 76 | Favorite | UBYTE | 1 | Marked as favorite |
| 77 | Validated | UBYTE | 1 | Sysop validated |
| 78 | Finished | UBYTE | 1 | Upload is complete |
| 79 | Described | UBYTE | 1 | Has description |
| 80 | Transformed | UBYTE | 1 | Transform script has run |
| 81 | PurgeKill | UBYTE | 1 | Delete file when purged |
| 82 | MissingFile | UBYTE | 1 | File is offline |
| 83 | Integrity | BYTE | 1 | Test result (see `ITEMTYPE_INTEGRITY_*`) |
| 84 | AutoGrab | UBYTE | 1 | ASCII-grab when read |
| 85 | PurgeStatus | UBYTE | 1 | Purge behavior (see `ITEMTYPE_PURGE_*`) |
| 86 | ShowDate | IsDate | 6 | Display date |
| 92 | order | short | 2 | On-disk ordering |
| 99 | VirusChecked | UBYTE | 1 | Virus check complete |
| 102 | FilePayBack | short | 2 | File credits per download |
| 104 | BytePayBack | long | 4 | Byte credits per download |
| 108 | VoteLink | long | 4 | Linked vote serial number |
| 116 | ByteRewards | long | 4 | Total byte rewards given |
| 120 | FileRewards | long | 4 | Total file rewards given |
| 124 | ByteDownload | long | 4 | Byte credits required to download |
| 128 | FileDownload | short | 2 | File credits required to download |
| 130 | DLableFrom | IsDate | 6 | Download window start |
| 136 | DLableTo | IsDate | 6 | Download window end |
| 142 | PurgeOn | IsDate | 6 | Purge date |
| 148 | BestCPS | long | 4 | Best download CPS achieved |
| 152 | First | long | 4 | Seek position of post in _Text |
| 156 | Last | long | 4 | Seek position of last response |
| 160 | InfoX | long | 4 | Seek position in _Short file |
| 164 | InfoLen | long | 4 | Length of short description |

---

## ItemHeader

**Header:** `include/cnet/subs.h`
**Size:** 34 bytes

Compact item header for scan/listing operations. Stored in the
subboard's `_Headers` data file and cached in `SubboardType4.ihead[]`.

| Offset | Field | Type | Size | Description |
|--------|-------|------|------|-------------|
| 0 | Number | ULONG | 4 | Unique item number |
| 4 | Size | long | 4 | File size (0 = post, not file) |
| 8 | Responses | long | 4 | Response/reply count |
| 12 | PostDate | IsDate | 6 | Original post date |
| 18 | RespDate | IsDate | 6 | Date of latest response |
| 24 | TitleSort | UBYTE[] | 9 | Sort key |
| 33 | Killed | UBYTE | 1 | Deleted flag |

---

## HeaderType

**Header:** `include/cnet/subs.h`

Full message/post header. Contains author/recipient information,
timestamps, and text location data.

| Field | Type | Size | Description |
|-------|------|------|-------------|
| ShowDate | IsDate | 6 | Display date |
| EditDate | IsDate | 6 | Last edit date |
| PostDate | IsDate | 6 | Original post date |
| ByAccount | short | 2 | Author's account number |
| NetAddress | char[] | 52 | Network address |
| UUCPnumber | long | 4 | UUCP message number |
| other | char[] | 16 | Reserved |
| By | char[] | 36 | Author display name |
| ByUser | char[] | 24 | Author handle |
| ByID | long | 4 | Author serial ID |
| To | char[] | 36 | Recipient display name |
| ToUser | char[] | 24 | Recipient handle |
| ToID | long | 4 | Recipient serial ID |
| ByNotParanoid | UBYTE | 1 | Show author's real name |
| ByAnonymous | UBYTE | 1 | Author is anonymous |
| ToAnonymous | UBYTE | 1 | Recipient is anonymous |
| Private | UBYTE | 1 | Private message |
| Imported | UBYTE | 1 | Imported from network |
| ByAlias | UBYTE | 1 | "By" is an alias |
| ToNotParanoid | UBYTE | 1 | Show recipient's real name |
| ToAlias | UBYTE | 1 | "To" is an alias |
| ToAccount | short | 2 | Recipient account number |
| Magic | ULONG | 4 | Alignment/validation magic |
| Number | ULONG | 4 | Unique message number |
| Text | long | 4 | Seek position in _Text |
| TextLen | long | 4 | Text length in bytes |
| Organ | char[] | 31 | Organization |
| Received | UBYTE | 1 | Message has been read |
| Next | long | 4 | Seek position of next response |
| Previous | long | 4 | Seek position of previous response |

---

## MessageType3

**Header:** `include/cnet/subs.h`

Message (response) record. Compact structure linking a response to its
parent item.

| Field | Type | Size | Description |
|-------|------|------|-------------|
| ItemNumber | ULONG | 4 | Parent item's serial ID |
| Seek | long | 4 | Position in text file |
| ByID | long | 4 | Author serial ID |
| ToID | long | 4 | Recipient serial ID |
| Number | ULONG | 4 | Response serial number |
| PostDate | IsDate | 6 | Post date |
| Imported | UBYTE | 1 | Imported from network |
| IsFile | UBYTE | 1 | Response includes a file |

---

## BaseUser

**Header:** `include/cnet/subs.h`

Per-user-per-subboard membership data. Tracks whether a user has joined
a subboard, their scan state, aliases, and new message tracking.

| Field | Type | Size | Description |
|-------|------|------|-------------|
| UID | long | 4 | User's serial ID (0 if never visited) |
| Joined | BYTE | 1 | Join state: -1=dropped, 0=sub default, 1=joined |
| Invited | BYTE | 1 | Invite state: -1=denied, 0=sub default, 1=invited |
| Sort | BYTE | 1 | Sort preference |
| scan_flags | UBYTE | 1 | Scan marking flags |
| LastMNew | IsDate | 6 | Last "new messages" check date |
| MNewDate | IsDate | 6 | New message reference date |
| Created | long | 4 | Subboard creation timestamp |
| Alias | char[] | 22 | User's alias for this subboard |
| Carbons | BYTE | 1 | Carbon copy: -1=no, 0=sub default, 1=yes |
| MNewSave | UBYTE | 1 | Save new message date flag |
| tome0 | short | 2 | Messages to user count (persistent) |
| tome1 | short | 2 | Messages to user count (this session) |
| expansion | char[] | 12 | Reserved |

---

## SelectType

**Header:** `include/cnet/subs.h`

File selection list entry. Used for both download selection lists
(`z->Select[]`) and upload lists (`z->Upload[]`).

| Field | Type | Size | Description |
|-------|------|------|-------------|
| Size | long | 4 | File size |
| Title | char[] | 32 | Display title |
| Location | char[] | 96 | Full filesystem path |
| Base | short | 2 | Physical subboard number |
| SDownFiles | UBYTE | 1 | File charge schedule |
| SDownBytes | UBYTE | 1 | Byte charge schedule |
| temp_rem | UBYTE | 1 | Selected from DS list |
| CDROM | UBYTE | 1 | Copy from CDROM before download |
| FreeStuff | BYTE | 1 | Free file flag |
| AutoKill | UBYTE | 1 | Post-download action (see `ST_AKILL_*`) |
| ByteDownload | long | 4 | Byte credits to subtract |
| FileDownload | short | 2 | File credits to subtract |
| ADate | IsDate | 6 | Adopt date |
| Number | ULONG | 4 | Item number (for regular select) or ordinal (for upload) |

---

## CPort

**Header:** `include/cnet/doors.h`

Communication port structure created by CNet when launching a door. The
door finds this via `FindPort(argv[1])`.

| Offset | Field | Type | Size | Description |
|--------|-------|------|------|-------------|
| 0 | mport | MsgPort | 34 | Exec message port |
| 34 | align | short | 2 | Alignment padding |
| 36 | myp | MainPort * | 4 | Pointer to MainPort |
| 40 | zp | PortData * | 4 | Pointer to this port's PortData |
| 44 | ack | UBYTE | 1 | Door initialization acknowledgment |
| 45 | align2 | UBYTE[] | 3 | Alignment padding |

**Usage:** `CNCL_DoorInit()` calls `CNC_InitContext(argv[1])` which finds the
CPort and uses its `myp` and `zp` fields to populate the door's global
pointers.

---

## CMessage

**Header:** `include/cnet/doors.h`

IPC message passed between door programs and the BBS process via the
CPort. The `cnetc.library` functions fill this structure internally.

| Offset | Field | Type | Size | Description |
|--------|-------|------|------|-------------|
| 0 | cn_Message | Message | 20 | Standard Exec Message |
| 20 | arg1 | ULONG | 4 | Command argument 1 |
| 24 | arg2 | ULONG | 4 | Command argument 2 |
| 28 | arg3 | ULONG | 4 | Command argument 3 |
| 32 | arg4 | ULONG | 4 | Command argument 4 |
| 36 | result | ULONG | 4 | Return value |
| 40 | command | UBYTE | 1 | Command code (see `CMESS_CMD_*`) |
| 41 | align | UBYTE[] | 3 | Alignment padding |

### CMESS_CMD Command Codes

| Value | Define | CNC_* Function |
|-------|--------|----------------|
| 0 | CMESS_CMD_SHUTDOWN | CNC_ShutDown |
| 1 | CMESS_CMD_PUTTEXT | CNC_PutText |
| 2 | CMESS_CMD_ENTERLINE | CNC_EnterLine |
| 3 | CMESS_CMD_ONEKEY | CNC_OneKey |
| 4 | CMESS_CMD_ENTERPASS | CNC_EnterPassword |
| 5 | CMESS_CMD_CCOMMANDS | CNC_CommonCommands |
| 6 | CMESS_CMD_READFILE | CNC_ReadFile |
| 7 | CMESS_CMD_SETDOING | CNC_SetDoing |
| 8 | CMESS_CMD_CALLEDITOR | CNC_CallEditor |
| 9 | CMESS_CMD_READGRAPHICS | CNC_ReadGraphics |
| 10 | CMESS_CMD_MAKEDATE | CNC_MakeDate |
| 11 | CMESS_CMD_READACCOUNT | CNC_ReadAccount |
| 12 | CMESS_CMD_SAVEACCOUNT | CNC_SaveAccount |
| 13 | CMESS_CMD_ADDCHARGE | CNC_AddCharge |
| 14 | CMESS_CMD_CHECKBALANCE | CNC_CheckBalance |
| 15 | CMESS_CMD_ENTERTEXT | CNC_EnterText |
| 16 | CMESS_CMD_CONFWAIT | CNC_ConferenceWait |
| 17 | CMESS_CMD_CHECKCHANGES | CNC_CheckChanges |
| 18 | CMESS_CMD_CONVERTACCESS | CNC_ConvertAccess |
| 19 | CMESS_CMD_GETFREE | CNC_GetFree |
| 20 | CMESS_CMD_FINDACCOUNT | CNC_FindAccount |
| 21 | CMESS_CMD_CHECKFLOW | CNC_CheckFlowControl |
| 22 | CMESS_CMD_LISTDIR | CNC_ListDir |
| 24 | CMESS_CMD_RNEXT | CNC_Rnext |
| 25 | CMESS_CMD_PARSECMDLINE | CNC_ParseCommandLine |
| 26 | CMESS_CMD_FINDCOMMAND | CNC_FindCommand |
| 27 | CMESS_CMD_READMSGPOINT | CNC_ReadMessagePoint |
| 28 | CMESS_CMD_EDITMESSAGE | CNC_EditMessage |
| 29 | CMESS_CMD_PREPEDITOR | (internal) |
| 30 | CMESS_CMD_FILESIZE | CNC_GrabFileSize |
| 31 | CMESS_CMD_WAITFORINPUT | CNC_WaitForInput |
| 34 | CMESS_CMD_PUTITEM | (ZPutItem via door) |
| 35 | CMESS_CMD_GETITEM | (ZGetItem via door) |
| 39 | CMESS_CMD_SELANDDLOAD | CNC_SelectAndDownload |
| 40 | CMESS_CMD_VDE | CNC_VisualDataEditor |
| 41 | CMESS_CMD_WRITELOG | CNC_OWriteLog |
| 42 | CMESS_CMD_SETMINFREE | CNC_ExtSetMinFree |
| 43 | CMESS_CMD_SETPROTO | CNC_ExtSetProtocol |
| 44 | CMESS_CMD_DLOAD | CNC_ExtDownload |
| 45 | CMESS_CMD_ULOAD | CNC_ExtUpload |
| 56 | CMESS_CMD_VDECLOSE | CNC_VDEclose |
| 57 | CMESS_CMD_VDEAPPLY | CNC_VDEapply |
| 58 | CMESS_CMD_WRITEUKEYS | CNC_WriteUKeys |
| 59 | CMESS_CMD_DOANSI | CNC_DoANSI |
| 60 | CMESS_CMD_DOANSIOUT | CNC_DoANSIOut |
| 61 | CMESS_CMD_INPUTSIGINIT | CNC_InputSignalInit |
| 62 | CMESS_CMD_INPUTSIGDISP | CNC_InputSignalDispose |
| 63 | CMESS_CMD_CHECKOLMS | CNC_CheckOLM |
| 64 | CMESS_CMD_PUTQMULT | CNC_PutQMult |
| 65 | CMESS_CMD_CHECKCARRIER | CNC_CheckCarrier |
| 66 | CMESS_CMD_MAILSEND | CNC_MailSend |
| 67 | CMESS_CMD_READLOG | CNC_ReadLog |
| 68 | CMESS_CMD_OPENDISPLAY | CNC_OpenDisplay |
| 69 | CMESS_CMD_GSETFILENOTE | CNC_GetSetFileNote |
| 70 | CMESS_CMD_EXTRACTFILE | CNC_ExtractFile |
| 71 | CMESS_CMD_DLOADSELECTED | CNC_DLoadSelect |
| 72 | CMESS_CMD_FINDFOREIGNAREA | CNC_FindForeignArea |
| 73 | CMESS_CMD_OUTSCAN | CNC_OutScan |

---

## CNetCContext

**Header:** `include/cnet/doors.h`

Context structure returned by `CNC_InitContext()`. Contains the
essential pointers a door needs to interact with the BBS.

| Field | Type | Description |
|-------|------|-------------|
| myp | MainPort * | System-global state |
| z | PortData * | Current port's state |
| cport | CPort * | Communication port |
| replyp | MsgPort * | Library-allocated reply port |
| cmess | CMessage | CMessage for IPC communication |

**Usage:** The door code rarely accesses `context` directly. Instead, it
uses the global `myp`, `z`, and `cport` pointers set by `CNCL_DoorInit()`.

---

## SerPort4

**Header:** `include/cnet/ports.h`

Modem/serial port configuration. Stored in `PortDataExtension.sp`.

| Field | Type | Size | Description |
|-------|------|------|-------------|
| unit | long | 4 | Serial device unit number |
| flags | long | 4 | Serial device flags |
| idlebaud | short | 2 | Idle baud rate |
| escape | short | 2 | Escape character |
| answerpause | short | 2 | Answer pause (ms) |
| seconds | short | 2 | Timeout seconds |
| name | char[] | 40 | Device name |
| init1 | char[] | 50 | Modem init string 1 |
| ppass | char[] | 11 | Port password |
| init2 | char[] | 50 | Modem init string 2 |
| TrapHours | ULONG | 4 | Trap hours bitmask |
| MailHours | ULONG | 4 | Mail hours bitmask |
| hangup | char[] | 21 | Hangup command |
| dialout | char[] | 21 | Dial command |
| answer | char[] | 21 | Answer command |
| offhook | char[] | 21 | Off-hook command |
| terminal | char[] | 50 | Terminal program path |
| callerid | char[] | 11 | Caller ID prefix |
| ring | char[] | 9 | Ring string |
| connect | char[] | 11 | Connect string |
| termlink | char[] | 21 | TermLink configuration |
| null | UBYTE | 1 | Local terminal flag |
| IdleWho | char[] | 21 | WHO display text when idle |
| portflags | long | 4 | Port flags (see `SP_PF_*`) |
| expansion | BYTE[] | 103 | Reserved |

**Port flag defines:**
- `SP_PF_SHOWONWHO` (bit 0) -- show port on WHO display
- `SP_PF_TELNETD` (bit 1) -- telnetd port
- `SP_PF_OFFCLOSE` (bit 2) -- unload port after logoff

---

## RangeContext

**Header:** `include/cnet/ranges.h`

State for iterating over a parsed range string. Used with the range
functions in `cnet4.library`.

| Field | Type | Size | Description |
|-------|------|------|-------------|
| nparts | long | 4 | Number of part sets in range |
| parts[10][2] | long[][] | 80 | Min/max pairs for each set |
| set | long | 4 | Current set index |
| element | long | 4 | Current element value |

**Usage:**
```c
struct RangeContext rc;
long val = CNetFindRange("1-5,8,10-20", 1, 100, &rc);
while (val >= 0) {
    /* process val */
    val = CNetNextRange(&rc);
}
```

---

## CNetFileEntry

**Header:** `include/cnet/files.h`

Linked list node for directory listings. Created by `CNetReadDir()` and
related functions. Supports hierarchical directory trees with parent/child
linkage.

| Field | Type | Size | Description |
|-------|------|------|-------------|
| DirHeader | CNetFileEntry * | 4 | Root directory entry (contains full path) |
| filename | char[] | 256 | File or directory name |
| size | LONG | 4 | File size in bytes |
| filenote | char[] | 80 | AmigaOS file comment |
| ChangedDate | IsDate | 6 | Last modification date |
| ftype | UBYTE | 1 | `CNFE_TYPE_FILE` (0) or `CNFE_TYPE_DIR` (1) |
| flag | LONG | 4 | User flag (CNet uses as deleted marker) |
| userdata | APTR | 4 | User-defined data pointer |
| parent | CNetFileEntry * | 4 | Parent directory node |
| child | CNetFileEntry * | 4 | First child (subdirectory contents) |
| prevfile | CNetFileEntry * | 4 | Previous entry in this directory |
| nextfile | CNetFileEntry * | 4 | Next entry in this directory |

---

## MailHeader4

**Header:** `include/cnet/mail.h`

Mail message header for CNet v4.12+. Used for reading, composing, and
storing mail messages.

**WARNING:** This structure is reconstructed from usage patterns in
`ports.h` and `control.h`. Field sizes and offsets are approximate.
Use CNet library API functions for mail operations rather than direct
struct access.

| Field | Type | Size | Description |
|-------|------|------|-------------|
| From | char[] | 40 | Sender name |
| To | char[] | 40 | Recipient name |
| Subject | char[] | 80 | Subject line |
| Date | IsDate | 6 | Send date |
| ReadDate | IsDate | 6 | Date message was read |
| Seek | long | 4 | Position in mail data file |
| Length | long | 4 | Message text length |
| Flags | ULONG | 4 | Mail flags |
| Number | ULONG | 4 | Unique message number |
| FromAccount | short | 2 | Sender account number |
| ToAccount | short | 2 | Recipient account number |
| FolderName | char[] | 32 | Containing folder name |
| UUCP_From | char[] | 32 | Sender's UUCP/Mail ID |
| UUCP_To | char[] | 32 | Recipient's UUCP/Mail ID |
| expansion | UBYTE[] | 240 | Reserved |

---

## NewMailFolder

**Header:** `include/cnet/mail.h`

Mail folder structure. Stored as a linked list in `z->Folders`.

**NOTE:** This structure is reconstructed. The actual binary layout
may differ.

| Field | Type | Size | Description |
|-------|------|------|-------------|
| FolderName | char[] | 64 | Folder name |
| NumMessages | long | 4 | Message count in folder |
| temporary | UBYTE | 1 | Temporary folder flag |
| expansion | UBYTE[] | 63 | Reserved |
| next | NewMailFolder * | 4 | Next folder in list |

---

## ChargeSet / ChargeType

**Header:** `include/cnet/charges.h`

### ChargeSet

Accounting schedule. Contains a 32x80 matrix of charge rates indexed
by access group and charge type.

| Field | Type | Size | Description |
|-------|------|------|-------------|
| next | ChargeSet * | 4 | Next schedule in list |
| schedule | short | 2 | Schedule number |
| ACC[32][80] | short[][] | 5120 | Charge amounts: ACC[group][type] |

### ChargeType

Individual charge record in the queue.

| Field | Type | Description |
|-------|------|-------------|
| next | ChargeType * | Next charge in list |
| id | short | User account number |
| schedule | short | Schedule used |
| type | short | Charge type (see `CHARGETYPE_*`) |
| units | short | Number of units |
| fcredits | ULONG | File credits |
| bcredits | ULONG | Byte credits |

### CHARGETYPE Defines

| Value | Define | Description |
|-------|--------|-------------|
| 0 | CHARGETYPE_MINBALANCE | Minimum balance charge |
| 2 | CHARGETYPE_LOGONSUCCESS | Successful logon |
| 7 | CHARGETYPE_CONNECTMINUTE | Per-minute connection |
| 8 | CHARGETYPE_OLM | Online message |
| 10 | CHARGETYPE_CONFERENCEMINUTE | Per-minute conference |
| 24 | CHARGETYPE_FEEDBACK | Feedback sent |
| 25 | CHARGETYPE_EMAILSENT | Email sent |
| 26 | CHARGETYPE_EMAILREAD | Email read |
| 36 | CHARGETYPE_SUBBDITEMREAD | Subboard item read |
| 37 | CHARGETYPE_SUBBDITEMPOST | Subboard item posted |
| 38 | CHARGETYPE_SUBBDITEMRESP | Subboard item response |
| 41 | CHARGETYPE_YANK | Yank operation |
| 47-49 | CHARGETYPE_FILEDLOAD1-3 | File download (3 schedules) |
| 53-55 | CHARGETYPE_FILEUPLOAD1-3 | File upload (3 schedules) |

See `charges.h` for the complete list of 63 charge types.

---

## CNUserCache

**Header:** `include/cnet/users.h`

User data cache entry. This is a **read-only** structure; modifying
its fields directly will cause inconsistencies. Use `UCacheRead()`,
`UCacheWrite()`, `LockAccount()`, and `UnLockAccount()` instead.

| Field | Type | Description |
|-------|------|-------------|
| udata | UserData * | Cached user data |
| account | short | Account number |
| accessed | LONG | CNet time of last access |
| next | CNUserCache * | Next cache entry |

---

## KeyElement4

**Header:** `include/cnet/users.h`

User index record stored in `myp->Key[]`. Provides fast lookup of
user identity fields without loading full `UserData` records.

| Field | Type | Size | Description |
|-------|------|------|-------------|
| UUCP | char[] | 11 | Mail ID (10 chars + terminator) |
| IDNumber | long | 4 | Serial ID (high byte always 0) |
| phone1 | long | 4 | Data phone area code |
| phone2 | long | 4 | Data phone local number |
| Handle | char[] | 21 | Handle/nickname |
| RealName | char[] | 26 | Real name |
| PName | UBYTE | 1 | Name privacy flag |
| Access | BYTE | 1 | Access group |

`myp->IName[]` and `myp->IPhone[]` are index arrays that map sorted
order to positions in the `Key[]` array.

---

## NewConfig1

**Header:** `include/cnet/config.h`

Global BBS configuration. Stored in `myp->gc`. Contains system
identity, path settings, protocol/archiver/editor/FidoNet
configuration, and many system-wide defaults.

### Key Fields

| Field | Type | Description |
|-------|------|-------------|
| MySystemName | char[26] | BBS name |
| MySysopName | char[26] | Sysop name |
| MyLocation | char[26] | BBS location |
| MyPhoneNumber | char[20] | BBS phone number |
| MyBBSID | char[10] | BBS identifier |
| maxUserAccounts | long | Maximum user accounts |
| MaxOpenPfiles | long | Maximum concurrent doors |
| nsub | long | Total subboard count |
| ZIPpath | char[40] | Temp/work directory path |
| RAMpath | char[40] | RAM directory path |
| OLMpath | char[40] | OLM storage path |
| narc | long | Number of configured archivers |
| nproto | long | Number of configured protocols |

The `ConfigExtension` structure (`myp->MPE->gc2`) contains additional
settings for mail server, SMTP, news, telnetd, FTPd, and caching.

---

## PortConfig

**Header:** `include/cnet/control.h`

Per-port configuration stored in `myp->pc[0..100]`.

| Field | Type | Size | Description |
|-------|------|------|-------------|
| online | UBYTE | 1 | Port is loaded |
| open | UBYTE | 1 | Screen state (see `SCR_OPEN_*`) |
| check | UBYTE | 1 | Toggle button states (see `CHECKBIT_*`) |
| idle | UBYTE | 1 | Idle display mode |
| offline | UBYTE | 1 | Port loaded but modem offline |
| not_used | UBYTE | 1 | Reserved |
| bplanes | UBYTE | 1 | Bitplane count |
| lace | UBYTE | 1 | 0=none, 1=24 line, 2=49 line |
| sizes[8] | USHORT[] | 16 | Window and zoom dimensions |

**Screen open modes:** `SCR_OPEN_PERMANENT` (1), `SCR_OPEN_FORCALL` (2), `SCR_OPEN_WORKBENCH` (3)

**Check bits:** `CHECKBIT_DOORSCLOSED` (0), `CHECKBIT_FILESCLOSED` (1), `CHECKBIT_MSGSCLOSED` (2), `CHECKBIT_NONUSER` (3), `CHECKBIT_SYSOPIN` (4)

---

## FreeType

**Header:** `include/cnet/subs.h`

Linked list node tracking free (reusable) space in subboard `_Text`
data files. Managed by `AllocText()`, `FreeText()`, and `SaveFree()`.

| Field | Type | Size | Description |
|-------|------|------|-------------|
| start | long | 4 | Byte offset in _Text file |
| length | long | 4 | Length of free block |
| next | FreeType * | 4 | Next free block |

---

## Other Structures

### CNetPort (ports.h)

Display-only port information shown in WHO listings.

| Field | Type | Size | Description |
|-------|------|------|-------------|
| UserName | char[] | 32 | Display username |
| From | char[] | 32 | Location/city |
| Area | char[] | 32 | Current area |
| Action | char[] | 32 | Current action |
| Baud | ULONG | 4 | Connection speed |
| Logon | ULONG | 4 | Logon time |
| Port | USHORT | 2 | Port number |
| Access | USHORT | 2 | Access level |

### IMPStruct (control.h)

Inter-Module Process list entry. Stored in `myp->MPE->IMPList`.

| Field | Type | Size | Description |
|-------|------|------|-------------|
| node | Node | 14 | Exec list linkage |
| Name | char[] | 80 | IMP entry name |
| ProcessName | char[] | 80 | Associated process name |
| ProcessReply | char[] | 80 | Reply port name |
| dflag | UBYTE | 1 | Display flag |
| pflag | UBYTE | 1 | Process flag |

### GETUSER Codes (getusers.h)

Constants for the ARexx GETUSER command, specifying which structure
and data type to query.

**Structure selectors (arg 1):**
| Value | Define | Structure |
|-------|--------|-----------|
| 1 | GU1_PORTDATA | PortData |
| 2 | GU1_MAINPORT | MainPort |
| 3 | GU1_PORTDATA_EXT | PortDataExtension |
| 4 | GU1_MAINPORT_EXT | MainPortExtension |

**Type selectors (arg 2):**
| Value | Define | C Type |
|-------|--------|--------|
| 1 | GU2_CHAR | char / BYTE / UBYTE |
| 2 | GU2_SHORT | short / USHORT |
| 3 | GU2_STRING | char[] |
| 4 | GU2_LONG | long / ULONG |
| 5 | GU2_DATE | IsDate |
| 6 | GU2_CHAR_PTR | char * |

### ENTERLINE Flags (io.h)

Input flags for `CNC_EnterLine()` / `CEnterLine()`:

| Flag | Value | Description |
|------|-------|-------------|
| ELINE_NONE | 0x0 | No special flags |
| ELINE_ALLCAPS | 0x00001 | Force uppercase |
| ELINE_FILENAME | 0x00002 | Filename-safe characters |
| ELINE_USEINBUFF | 0x00004 | Pre-fill with InBuffer content |
| ELINE_NOLEADSPACE | 0x00008 | Strip leading spaces |
| ELINE_CAPWORDSTART | 0x00010 | Capitalize word starts |
| ELINE_RESTWORDLOWER | 0x00020 | Lowercase rest of words |
| ELINE_NUMBERSONLY | 0x00040 | Digits only |
| ELINE_INPUTBOX | 0x00080 | Draw input box |
| ELINE_ALLOWMCI | 0x00100 | Allow MCI in input |
| ELINE_HANDLESPECIAL | 0x00200 | Handle special characters |
| ELINE_XSLASHSTART | 0x00400 | Disallow leading slash |
| ELINE_XBKSPACESTART | 0x00800 | Disallow leading backspace |
| ELINE_NOOLMS | 0x01000 | Suppress OLM display |
| ELINE_ALLOWCHAT | 0x02000 | Allow chat interruption |
| ELINE_NOSPACES | 0x04000 | Disallow spaces |
| ELINE_NOCURSORMOVE | 0x08000 | Disable cursor movement |
| ELINE_NOSLASHES | 0x10000 | Disallow slashes |
| ELINE_NOPATH | 0x20000 | Disallow path separators |
| ELINE_ALPHAONLY | 0x40000 | Letters only |
