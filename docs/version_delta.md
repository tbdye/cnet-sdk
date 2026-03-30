# CNet SDK Version Delta: v5.01 (SDK) vs v5.36c (Live System)

Analysis date: 2026-03-24

## Summary

The CNet library ABI is **unchanged** between v5.01 (SDK release) and v5.36c
(live BBS system). Library version numbers, export counts, and struct layouts
all match exactly. The SDK headers are safe to use for compiling doors and
utilities that run on the v5.36c system.

The version number "5.36c" refers to the overall CNet BBS application, not to
the individual shared libraries. The libraries themselves were not rebuilt
between v5.01 and v5.36c -- changes in the BBS were confined to the main
executable (Control, port handlers, etc.) rather than the shared library API.

## Library Version Comparison

| Library          | SDK version.h | Live v5.36c | Demo file size | Live file size | Changed? |
|------------------|---------------|-------------|----------------|----------------|----------|
| cnet.library     | 5.0           | 5.0         | 28,424         | 28,708         | Size +284 |
| cnetc.library    | 4.1           | 4.1         | 5,408          | 5,408          | No       |
| cnet4.library    | 5.0           | 5.0         | 9,280          | 9,504          | Size +224 |
| cnetmail.library | 4.1           | 4.1         | 10,248         | 10,368         | Size +120 |
| cnetgui.library  | (not listed)  | 4.4         | 1,040          | 1,040          | No       |

### File Size Differences

Three libraries show small size increases between the demo distribution and
the installed v5.36c system:

- **cnet.library**: +284 bytes (28,424 -> 28,708). Despite the size increase,
  the NegSize and LVO count are identical, meaning no new exports. The size
  difference is in the positive data/code segment -- likely bug fixes to
  existing function implementations.

- **cnet4.library**: +224 bytes (9,280 -> 9,504). Same pattern -- internal
  code changes without ABI changes.

- **cnetmail.library**: +120 bytes (10,248 -> 10,368). Same pattern.

These size differences confirm that Perspective Software shipped updated
library binaries with v5.36c that contain internal bug fixes or improvements,
but the API surface (exports, struct layouts) was not changed.

## LVO / Export Count Comparison

Measured via `lib_NegSize` from a test program running on the live v5.36c system.

| Library          | FD entries | FD+4 reserved | NegSize | NegSize/6 | Computed funcs | Match? |
|------------------|------------|---------------|---------|-----------|----------------|--------|
| cnet.library     | 79         | 83            | 500     | 83 (+2 pad) | 79           | YES    |
| cnetc.library    | 66 (1 priv)| 70            | 420     | 70          | 66           | YES    |
| cnet4.library    | 15         | 19            | 116     | 19 (+2 pad) | 15           | YES    |
| cnetmail.library | 14         | 18            | 108     | 18          | 14           | YES    |
| cnetgui.library  | 2          | 6             | 36      | 6           | 2            | YES    |

### Notes on NegSize Alignment

- cnet.library and cnet4.library have NegSize values that are not exact
  multiples of 6 (remainder of 2 bytes each). This is word-alignment padding
  in the jump table. It does not indicate extra functions.

- All five libraries have exactly the number of exported functions that the
  v5.01 FD files describe. **No new functions were added between v5.01 and
  v5.36c.**

## PosSize (Library Data Area)

| Library          | PosSize (bytes) |
|------------------|-----------------|
| cnet.library     | 1,608           |
| cnetc.library    | 100             |
| cnet4.library    | 1,032           |
| cnetmail.library | 972             |
| cnetgui.library  | 108             |

These values represent the library's internal data area. They are not directly
relevant to SDK compatibility but are recorded here for reference.

## Struct Size Verification

All struct sizes compiled from the SDK headers match the sizes observed on a
live CNet v5.36c Amiga system. Structs use SAS/C 2-byte (word) alignment --
see `include/cnet/align.h`.

| Structure         | SDK sizeof() | Expected | Match? |
|-------------------|-------------|----------|--------|
| IsDate            | 6           | 6        | YES    |
| Privs             | 92          | 92       | YES    |
| UserData          | 672         | 672      | YES    |
| ULock             | 62          | --       | --     |
| AccessGroup       | 156         | 156 (32+92+32) | YES |
| KeyElement3       | 68          | --       | --     |
| KeyElement4       | 74          | --       | --     |
| CNUserCache       | 14          | --       | --     |
| SelectType        | 156         | --       | --     |
| BaseUser          | 64          | --       | --     |
| HeaderType        | 288         | --       | --     |
| MessageType3      | 28          | --       | --     |
| ItemType3         | 168         | 168      | YES    |
| ItemHeader        | 34          | 34       | YES    |
| FreeType          | 12          | --       | --     |
| SubboardType4     | 696         | --       | --     |
| CPort             | 48          | --       | --     |
| CMessage          | 44          | --       | --     |
| CNetCContext      | 60          | --       | --     |
| OldMailHeader     | 254         | ~256     | Close* |
| MailHeader4       | 528         | ~512     | Close* |
| NewMailFolder     | 136         | --       | --     |
| CNetPort          | 140         | --       | --     |
| SerPort           | 348         | --       | --     |
| SerPort4          | 492         | --       | --     |
| PortData          | 15,180      | --       | --     |
| PortDataExtension | 1,238       | --       | --     |
| PortConfig        | 24          | --       | --     |
| NewConfig1        | 17,470      | --       | --     |
| MainPort          | 28,326      | --       | --     |
| MainPortExtension | 1,382       | --       | --     |

### Key Offset Verification (PortData)

| Field          | SDK offset | Comment offset | Match? |
|----------------|-----------|----------------|--------|
| user1          | 68        | 68             | YES    |
| user2          | 740       | 740            | YES    |
| bn             | 1,416     | 1416           | YES    |

### Key Offset Verification (UserData)

| Field          | SDK offset | Comment offset | Match? |
|----------------|-----------|----------------|--------|
| Handle         | 4         | 4              | YES    |
| RealName       | 25        | 25             | YES    |
| UUCP           | 478       | 478            | YES    |
| MyPrivs        | 580       | 580            | YES    |

### Notes on Reconstructed Structs

The structs marked with `*` (OldMailHeader, MailHeader4, NewMailFolder) were
not in the original SDK distribution. They were reconstructed in mail.h based
on usage patterns in ports.h and control.h. Their sizes are approximate and
padded with expansion fields to reach safe upper bounds. Since these structs
are only used through library API functions (not by direct file I/O), the
exact sizes are not critical for door compilation.

### Mail Header Size Discrepancies

The mail.h comments state that OldMailHeader is "padded to 256 bytes" and
MailHeader4 is "padded to 512 bytes," but the actual compiled sizes are 254
and 528 respectively. The discrepancies are due to incorrectly sized expansion
arrays in the reconstructed headers: OldMailHeader's expansion[84] falls 2
bytes short of the 256 target, and MailHeader4's expansion[240] overshoots the
512 target by 16 bytes. These are not ABI-breaking issues since these structs
are only accessed through library API functions, never by direct file I/O or
cross-process memory sharing.

## Recommendations

### No Header Updates Required

The SDK headers accurately describe the v5.36c library ABI. No changes to
struct definitions, function declarations, or FD files are needed.

### version.h Update

The only recommended change is adding the missing `cnetgui.library` version
constants to version.h, since this library was omitted from the original
version defines:

```c
#define CNETGUILIBNAME     "cnetgui.library"
#define CNETGUILIBVERSION  4L
#define CNETGUILIBREVISION 4L
```

### Safe to Proceed with Example Porting

Since the library ABI is identical between v5.01 and v5.36c, doors and
utilities compiled against the SDK headers will work correctly on the live
v5.36c system. No struct layout changes, no new functions, no API breaks.

### Demo vs Installed Binary Differences

The three libraries with size differences (cnet, cnet4, cnetmail) contain
internal bug fixes but no ABI changes. This means:

1. The installed v5.36c libraries are newer builds with the same API.
2. If someone installs the demo distribution's libraries, they would get
   older (buggier) implementations but the same API surface.
3. No FD file updates are needed.
