# libcnet_door.a -- CNet Door Static Library

Reimplements the SAS/C linker libraries (cnet.lib and cnetcdoor.lib) as
GCC-compatible C source.

## Contents

- `door_init.c` -- CNCL_DoorInit() and CNCL_DoorCleanup() door lifecycle
- `door_printf.c` -- CNCL_Printf() formatted output wrapper
- `cdoor_wrappers.c` -- C* wrapper functions (CPutText, COneKey, etc.)
- `cdoor_extra.c` -- CNC_* editor functions (linker library, not shared library)
- `utility.c` -- Utility functions (string, file, date, port query helpers)
- `common/` -- CommonFuncs code snippets adapted for GCC

## Build

```sh
make        # produces libcnet_door.a
make clean  # removes object files and archive
```
