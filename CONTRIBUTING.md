# Contributing to the CNet 5 SDK

Contributions are welcome. This project benefits most from testing on real
(or accurately emulated) hardware, documentation improvements, and additional
working examples.

## Testing on Real Hardware

The most valuable contributions are test results from CNet systems other than
the v5.36c installation this SDK was developed against. If you have access to
a CNet BBS:

1. Build the SDK examples with `make all`.
2. Deploy the standalone tools (`tools/libinfo`, `tools/structsizes`) to
   your Amiga and run them from a CLI shell.
3. Compare the output against the values in `tests/TEST_RESULTS.md` and
   `docs/version_delta.md`.
4. If you can configure a test subboard, try the door test programs
   (`tests/test_hello`, etc.) through a BBS login session.
5. Report any differences -- even small ones -- as issues. Include your CNet
   version, Amiga model (or emulator), and the full program output.

### What to watch for

- Struct size mismatches (different `sizeof()` values from `structsizes`).
- Library version or LVO count differences (from `libinfo`).
- Crashes when calling specific library functions.
- Doors that compile but behave differently than expected at runtime.

## Documentation Improvements

The SDK documentation was written from reverse-engineering the headers, FD
files, and the `CNet_Library_doc.guide`. If you have experience with CNet
internals or access to additional documentation:

- Corrections to function descriptions or parameter semantics.
- Missing information about struct fields (especially the undocumented
  expansion fields).
- Clarifications about CNet's runtime behavior (semaphore locking rules,
  subboard lifecycle, editor buffer management).
- Usage notes from real-world door development experience.

## Additional Examples

New example programs are welcome, especially:

- Doors that demonstrate CNet features not covered by the existing examples
  (e.g., file transfer protocols, conference mode, OLM handling).
- Standalone utilities that extract or display BBS data.
- Examples showing integration with other Amiga libraries (ARexx,
  bsdsocket.library, etc.).

### Guidelines for examples

- Follow the patterns established in the existing examples and templates.
- Use the `libcnet_door.a` wrapper functions (not direct `CNC_*` calls)
  in door code.
- Include a comment header explaining what the program does.
- Test on a live CNet system if possible, or at minimum verify it compiles
  cleanly with `make`.

## Bug Reports

File issues for:

- Compilation failures (include the full compiler output).
- Runtime crashes (include the Amiga alert code or crash address if
  available).
- Incorrect function behavior compared to the original SAS/C SDK.
- Struct layout mismatches discovered through testing.

Include your toolchain version (`m68k-amigaos-gcc --version`), CNet version,
and enough context to reproduce the problem.

## What This Project Does Not Accept

- CNet BBS binaries or proprietary source code.
- Changes that break compilation with the Bebbo m68k-amigaos-gcc toolchain.
- Features that require runtime dependencies beyond the CNet libraries and
  standard AmigaOS.
