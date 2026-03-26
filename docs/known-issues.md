# CNet 5 SDK Known Issues and Errata

This document catalogs known issues, quirks, and workarounds discovered during
the modernization of the CNet 5 SDK for the m68k-amigaos-gcc cross-compiler.
Items are listed roughly in order of impact to door developers.

---

## 1. The `__asm` Conflict (Critical for GCC Users)

**Problem:** CNet's master header (`cnet.h`) redefines `__asm` to nothing for
SAS/C compatibility. GCC's AmigaOS NDK headers use `__asm` as a built-in
keyword in `__REG__()` macros for register-binding inline library stubs. If
`__asm` is redefined before the NDK headers are included, all inline stubs
for Exec, DOS, Graphics, Intuition, and other system libraries break silently
-- the register bindings are stripped out, and function calls pass arguments on
the stack instead of in registers, causing crashes or data corruption at
runtime.

**Current mitigation:** `cnet.h` includes all required AmigaOS system headers
*before* the `#define __asm` line (line 49). This means the NDK inline stubs
are compiled correctly, but `__asm` is then undefined for the rest of the
translation unit.

**Consequence:** You cannot `#include <proto/cnetc.h>` (or any CNet library
proto header) in the same source file as `#include <cnet/cnet.h>`. The CNet
proto headers contain inline stubs that use `__asm`, and the redefinition
breaks them.

**Workarounds:**

1. **Use `libcnet_door.a` wrapper functions (recommended).** The door library
   provides `CPutText()`, `CEnterLine()`, `CPutA()`, etc. that forward to the
   corresponding `CNC_*` library calls. These wrappers are compiled in a
   separate translation unit where `__asm` is not redefined. See
   `templates/pfile_cnetc.c` for the full list.

2. **Separate compilation units.** Put CNet struct access (which needs
   `cnet.h`) in one `.c` file, and CNet library calls (which need
   `proto/cnetc.h`) in another. Link them together.

3. **Standalone pattern.** For utilities that don't use the door framework,
   include system proto headers first, then define only the SAS/C compat
   macros you need (excluding `__asm`), then include CNet struct headers
   individually. See `templates/standalone.c` for this approach.

**See also:** [Getting Started -- The `__asm` conflict](getting-started.md)

---

## 2. cnet.h Offset Comments

**Problem:** Field offset comments in the CNet struct headers (e.g., `// 68`,
`// 740`) contain errors in some structures. These comments were part of the
original SDK and were not systematically verified.

**Verified correct offsets** (from `test_structsizes` running on v5.36c):

| Structure | Field      | Actual Offset | Comment Offset | Match? |
|-----------|------------|---------------|----------------|--------|
| PortData  | user1      | 68            | 68             | YES    |
| PortData  | user2      | 740           | 740            | YES    |
| PortData  | bn         | 1416          | 1416           | YES    |
| UserData  | Handle     | 4             | 4              | YES    |
| UserData  | RealName   | 25            | 25             | YES    |
| UserData  | UUCP       | 478           | 478            | YES    |
| UserData  | MyPrivs    | 580           | 580            | YES    |

**Recommendation:** Do not rely on offset comments for binary structure
parsing or direct memory access. Use `offsetof()` from `<stddef.h>` to
compute offsets at compile time. The struct definitions themselves are
verified correct -- only the comments may be wrong.

---

## 3. TextToEditor Stub

**Problem:** `TextToEditor()` in `libcnet_door.a` is an unimplemented stub
that does nothing. The original implementation directly manipulated
`z->EdBuffer`, `z->EdBase`, `z->MaxLines`, and `z->txline`, but the editor
buffer allocation scheme is not documented in the surviving SDK headers.

**Workaround:** Use `CCallEditor()` to open the full-screen editor, or
`CEnterText()` to accept multi-line text input. These work through
`cnetc.library` and handle buffer management internally.

---

## 4. OldMailHeader / MailHeader4 Size Discrepancies

**Problem:** The `mail.h` comments state that `OldMailHeader` is "padded to
256 bytes" and `MailHeader4` is "padded to 512 bytes." The actual compiled
sizes are 254 and 528 bytes respectively.

The discrepancies come from incorrectly sized expansion arrays in the
reconstructed headers:
- `OldMailHeader.expansion[84]` falls 2 bytes short of the 256-byte target.
- `MailHeader4.expansion[240]` overshoots the 512-byte target by 16 bytes.

**Impact:** Low. These structs were reconstructed from usage patterns in
`ports.h` and `control.h` (they were not in the original SDK distribution).
They are accessed only through library API functions, never by direct file I/O
or cross-process memory sharing. The size mismatches do not affect door
compilation or runtime behavior.

---

## 5. UUCPMAIL_BIT Definition

**Problem:** `users.h` line 205 defines `UUCPMAIL_BIT` as 29, but the
surrounding bit definitions suggest it should be 19:

```c
#define EXPANSION4_BIT     18
#define UUCPMAIL_BIT       29    /* gap: should this be 19? */
#define NETCOSTCREDITS_BIT 20
#define HOLDANDCRASH_BIT   21
```

There is a gap from bit 18 to bit 29, then it resumes at 20. The value 29
may be a typo for 19, or it may be intentionally placed at bit 29 (skipping
over bits 19-28 for other purposes). Without access to the CNet source code,
the correct value cannot be determined.

**Recommendation:** This definition is preserved as-is from the original SDK.
If you use `UUCPMAIL_BIT` or `UUCPMAIL_FLAG`, be aware that it may not match
the BBS's actual flag position. Test against a live system before relying on
it.

---

## 6. v3 FD Register Errors

**Problem:** The v3 `cnet.fd` file specified incorrect registers for
`OneLessUser` and `OneMoreUser`:

| Function     | v3 FD register | v5 FD register | Actual |
|-------------|---------------|---------------|--------|
| OneLessUser | `(D0)`        | `(A0)`        | A0     |
| OneMoreUser | `(D0)`        | `(A0/D0)`     | A0/D0  |

The v3 FD passed the subboard pointer in `D0` (a data register), but the
library implementation expected it in `A0` (an address register). This would
cause crashes in any v3 door that actually called these functions through the
FD-generated stubs.

**Status:** Fixed. The v5 FD files in this SDK use the correct registers.
This is only relevant if you are porting old v3 doors -- see
[Migration Guide](migration-v3-to-v5.md) for details.

---

## 7. cnet4.fd A3/A4 Register Usage

**Problem:** `CNetPrintDate` in `cnet4_lib.fd` uses registers A1, A2, A3,
and D7. The use of A3 is unusual for AmigaOS library calls. In some GCC
configurations, A4 is the small data base pointer, and A3 may be reserved
for similar purposes.

The original SDK included a `LP4NRA4` macro (a SAS/C a4-relative variant of
the 4-argument no-return inline macro). This SDK aliases it to the standard
`LP4NR` since GCC with `-noixemul` (libnix) does not use A4 for small data.

**Status:** `CNetPrintDate` compiles and links correctly. The `LP4NRA4`
alias is defined in `include/inline/cnet4.h`. No A4 conflicts have been
observed with the `-noixemul -m68020` configuration used by this SDK.

**Recommendation:** If you encounter register conflicts with `CNetPrintDate`
(unlikely with the standard build flags), compile the calling source file
with `-fno-baserel`.

---

## 8. mailsend.c Example Bugs

**Problem:** The `examples/mailsend.c` code snippet has multiple issues:

- Missing closing quote on line 26.
- References undefined variables: `FromAccount`, `fromid`, `toid`, `text`,
  `tempmailto`, `mailto`.
- No `main()` function -- it is a code fragment, not a compilable program.

The original source explicitly states it is "uncompiled and untested."

**Status:** Not fixed. This file is preserved as-is from the original SDK
for reference. It is listed in the Makefile as a code snippet (not compiled).
Use `examples/ListEvents.c` or the door templates as working examples of
CNet API usage.

---

## 9. Datestamp_to_IsDate.c SAS/C Dependency

**Problem:** The original `Datestamp_to_IsDate.c` (a CommonFuncs snippet)
called `utunpk()`, which is a SAS/C runtime function for unpacking Unix
timestamps. This function does not exist in libnix (GCC's AmigaOS C
runtime).

**Status:** Fixed. The function was rewritten using manual date arithmetic
against the Amiga epoch (1 January 1978). The replacement is in
`lib/common/Datestamp_to_IsDate.c` and compiles cleanly with GCC.

---

## 10. netinclude: Paths in telnet.h

**Problem:** The original `telnet.h` used SAS/C-style assign paths:

```c
#include "netinclude:sys/types.h"
#include "netinclude:netinet/in.h"
```

The `netinclude:` assign is specific to Amiga TCP/IP stacks and SAS/C build
environments. GCC's cross-compiler uses standard NDK include paths.

**Status:** Fixed. The includes were changed to standard paths:

```c
#include <sys/types.h>
#include <netinet/in.h>
```

These resolve correctly with the Bebbo GCC toolchain's NDK headers.

---

## 11. CNC_ConvertAccess Duplicate Declaration

**Problem:** The original `cnetclibrary_protos.h` (in the SAS/C SDK)
declared `CNC_ConvertAccess` twice -- once at line 24 and again at line 59
with an identical signature. This caused compiler warnings with `-Wall`.

**Status:** Fixed. The duplicate was removed from
`include/clib/cnetc_protos.h`. The function appears once, matching the
single FD entry.

---

## Informational Notes

### Library ABI Stability

The CNet library ABI is unchanged between v5.01 (original SDK release) and
v5.36c (latest known installed version). Library version numbers, export
counts, and struct layouts all match. See [Version Delta](version_delta.md)
for the full comparison.

### Mail Header Structs Are Reconstructed

The `OldMailHeader`, `MailHeader4`, and `NewMailFolder` structs in `mail.h`
were not part of the original SDK distribution. They were reconstructed from
usage patterns in `ports.h` and `control.h`. Their layouts are approximate.
Since these structs are only accessed through library API functions, the
exact field layout is not critical for door compilation.

### ZGettest Semaphore Deadlock

`ZGettest.c` calls `OneMoreUser()` which acquires a subboard semaphore. If
run from a non-CNet context (e.g., `amigactl exec` or a CLI shell), the
semaphore acquisition can deadlock when the subboard is locked by a CNet
process. This blocks the calling process and may require a system restart.

Programs that acquire CNet semaphores must be run from a proper CNet door
context or an interactive CLI where they can be interrupted with Ctrl-C.
