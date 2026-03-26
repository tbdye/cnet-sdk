# CNet 5 SDK for GCC

A modernized SDK for developing doors, PFiles, and utilities for the
[CNet Amiga BBS](https://en.wikipedia.org/wiki/CNet_(BBS)) system,
targeting the m68k-amigaos-gcc cross-compiler.

The original CNet 5 SDK was written for SAS/C on native AmigaOS. This
project ports the headers, library stubs, linker library, and examples to
compile cleanly under the [Bebbo GCC](https://github.com/bebbo/amiga-gcc)
cross-compiler toolchain, making it possible to develop CNet doors from a
modern Linux or macOS workstation.

## Who Is This For?

- CNet BBS sysops who want to write or modify doors and utilities
- Retro-computing enthusiasts exploring the Amiga BBS scene
- Door developers building new PFiles for CNet 5 systems

## Why GCC?

The original SDK required SAS/C, a commercial compiler that ran natively on
AmigaOS. The Bebbo GCC toolchain offers several advantages for modern
development:

- **Cross-compilation** from Linux, macOS, or WSL -- no Amiga hardware needed for compilation
- **Modern tooling** -- use your preferred editor, debugger, and version control
- **Free and open source** -- no commercial compiler license required
- **Active maintenance** -- the Bebbo toolchain is actively maintained and supports modern AmigaOS NDK headers
- **CI/CD friendly** -- automated builds are straightforward on any Linux host

The trade-off is the `__asm` compatibility issue documented in
[known-issues.md](docs/known-issues.md) -- a minor include-ordering constraint
that the templates and examples already handle.

## Quick Start

```sh
git clone <repo-url> cnet-sdk
cd cnet-sdk
make lib        # build libcnet_door.a
make examples   # build all example programs
```

See [docs/getting-started.md](docs/getting-started.md) for a complete
walkthrough of writing, compiling, deploying, and testing your first PFile.

## Build Requirements

- **m68k-amigaos-gcc** (Bebbo GCC toolchain) installed at `/opt/amiga/`
  (or edit `CC` in the Makefile)
- **GNU Make**
- A **CNet 5 BBS** (v5.x) running on a real or emulated Amiga for testing
- **[amigactl](https://github.com/tbdye/amigactl)** (optional) -- Remote Amiga management tool, useful for deploying compiled binaries

## SDK Contents

### Libraries

| Library | Description |
|---------|-------------|
| `lib/libcnet_door.a` | Static linker library: door lifecycle, CNC_* wrappers, utility functions |

Five CNet shared libraries are accessed at runtime via inline stubs (not
included in this repo -- they are part of the CNet BBS installation):

| Library | Functions | Purpose |
|---------|-----------|---------|
| cnet.library | 79 | System services (subboards, accounts, files, semaphores) |
| cnetc.library | 66 | Door I/O and BBS services (text, input, editor, protocols) |
| cnet4.library | 15 | Date/time and range utilities |
| cnetmail.library | 14 | MIME and mail routing |
| cnetgui.library | 2 | GUI support |

### Headers

- `include/cnet/` -- CNet struct definitions (PortData, MainPort, UserData, etc.)
- `include/clib/` -- C prototypes for all CNet library functions
- `include/fd/` -- Function definition files (register assignments)
- `include/inline/` -- GCC inline stubs (generated from FD files)
- `include/pragma/` -- SAS/C pragmas (for reference; GCC uses inline stubs)
- `include/proto/` -- Proto headers that pull in the correct stubs

### Examples

42 example programs demonstrating CNet API usage:

- CNetC doors (the modern door framework)
- Standalone utilities (read BBS state without a login session)
- Raw CMessage doors (v3-style compatibility)

### Templates

Three starter templates for new projects:

| Template | Use Case |
|----------|----------|
| `templates/pfile_cnetc.c` | CNetC door with full wrapper function list |
| `templates/pfile_raw.c` | Raw CMessage door (v3 compatible) |
| `templates/standalone.c` | CLI utility accessing CNet data structures |

### Documentation

| Document | Description |
|----------|-------------|
| [getting-started.md](docs/getting-started.md) | First PFile walkthrough |
| [architecture.md](docs/architecture.md) | CNet process model and data structures |
| [library-reference.md](docs/library-reference.md) | All 176 library functions |
| [structures.md](docs/structures.md) | Struct field reference |
| [pfile-guide.md](docs/pfile-guide.md) | Door development guide |
| [examples-guide.md](docs/examples-guide.md) | Annotated example catalog |
| [migration-v3-to-v5.md](docs/migration-v3-to-v5.md) | Porting v3 doors to v5 |
| [arexx.md](docs/arexx.md) | ARexx interface reference |
| [known-issues.md](docs/known-issues.md) | Known issues and errata |
| [version_delta.md](docs/version_delta.md) | v5.01 vs v5.36c ABI comparison |

### Tools

| Tool | Purpose |
|------|---------|
| `tools/libinfo` | Display CNet library version and export info |
| `tools/structsizes` | Report sizeof() for all CNet structs |
| `tools/guoffset` | Verify GetUsers field offsets |

## Compatibility

Tested against CNet v5.36c running under [Amiberry](https://github.com/BlitterStudio/amiberry).
The library ABI is unchanged between v5.01 (SDK release) and v5.36c, so doors
compiled with this SDK work on any CNet v5.x installation.

All 52 programs (42 examples, 3 templates, 3 tools, 4 tests) compile with zero
errors. Standalone utilities have been verified on a live system.
See [docs/version_delta.md](docs/version_delta.md) for ABI details.

## Credits

CNet BBS was originally created by **Ken Pletzer** under **Perspective Software**.
Development was continued by **Ray Akey** and **Todd Knight** under
**ZenMetal Software**, with Ray Akey serving as the primary developer from v4.x onward.
**Kelly Cochran** contributed to the cnetc.library and SDK examples.
CNet is currently maintained by **Dan FitzGerald**.

The original v5.01 SDK was distributed as part of `cnet5demo.lha` on
[Aminet](https://aminet.net/).

GCC porting, documentation, and tooling by **Thomas Dye**.

## License

Original CNet SDK materials are redistributed under their original terms
(see headers for per-file copyright notices). New work (GCC porting code,
documentation, build system, tools, tests) is licensed under the
[MIT License](LICENSE).
