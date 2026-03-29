# CNet 5 SDK - Top-level Makefile
#
# Cross-compile CNet doors, utilities, and tools for AmigaOS (m68k).
#
# Programs fall into three categories:
#   1. CNetC doors    - use CNCL_DoorInit or CNC_InitContext, link with libcnet_door.a
#   2. Standalone     - use FindPort("cnetport") directly, no door framework
#   3. Raw CMessage   - v3-style doors using CallHost(), link with libcnet_door.a
#
# Some standalone utilities also link against cnet.library or cnet4.library
# at runtime but do not need the door static library at compile time.

PREFIX  ?= /opt/amiga
CC      = $(PREFIX)/bin/m68k-amigaos-gcc
CFLAGS  = -noixemul -m68020 -O2 -Wall -Wno-pointer-sign -Iinclude -Iinclude/cnet
LDFLAGS = -noixemul
LIBS    = -Llib -lcnet_door

# ---------------------------------------------------------------------------
# CNetC door examples (use CNCL_DoorInit or CNC_InitContext, need libcnet_door.a)
# ---------------------------------------------------------------------------
CNETC_DOORS = \
    examples/empty \
    examples/CNetCTest \
    examples/NewEmptyCTest \
    examples/FindHandle \
    examples/PortStatus \
    examples/PortStatus2 \
    examples/PortStatus3 \
    examples/SubListVars \
    examples/WFI_Test \
    examples/RangeTest \
    examples/RangeTest2 \
    examples/dirsize \
    examples/ansidetect \
    examples/MenuBits \
    examples/identdtest \
    examples/TestOpenDisplay \
    examples/showunval

# ---------------------------------------------------------------------------
# Standalone utilities (FindPort for MainPort, no door library needed)
# ---------------------------------------------------------------------------
STANDALONE = \
    examples/GetMainPort \
    examples/mainporttest \
    examples/ShowTelnetHosts \
    examples/ShowIdleTime \
    examples/setIdleTime \
    examples/setsplitchat \
    examples/SetZPos \
    examples/OneKeyInject \
    examples/RegistrationInfo \
    examples/implist \
    examples/listsubsem \
    examples/identtestdos

# Standalone utilities that need cnet.library (opened at runtime, not linked)
STANDALONE_CNETLIB = \
    examples/ZGettest \
    examples/ReadDirTest \
    examples/ReadDirTest2 \
    examples/CNetFileFind \
    examples/verifymailsem

# Standalone utilities that need cnet.library AND libcnet_door.a (for LongToDate)
STANDALONE_CNETLIB_DOORUTIL = \
    examples/ListEvents

# Standalone utilities that need cnet.library AND cnet4.library
STANDALONE_CNET4 = \
    examples/dos_uptime \
    examples/DosDoor2

# Standalone utilities that need bsdsocket.library
STANDALONE_BSDSOCK = \
    examples/listdcc

# Standalone using DOS I/O only (no CNet libraries)
STANDALONE_DOSONLY = \
    examples/DosDoor

# Standalone needing custom DOSBase handling (no startup code)
STANDALONE_NOSTARTUP = \
    examples/listmailsem

# ---------------------------------------------------------------------------
# Raw CMessage doors (v3-style, link with libcnet_door.a for CallHost)
# ---------------------------------------------------------------------------
RAW_DOORS = \
    examples/WaitForC \
    examples/identdlookup

# ---------------------------------------------------------------------------
# Code snippets (not standalone programs, cannot be compiled alone)
# ---------------------------------------------------------------------------
# examples/CheckOnline.c  - function snippet, no main()
# examples/mailsend.c     - function snippet with syntax errors, no main()

# ---------------------------------------------------------------------------
# Templates
# ---------------------------------------------------------------------------
TEMPLATES = \
    templates/pfile_cnetc \
    templates/pfile_raw \
    templates/standalone

# ---------------------------------------------------------------------------
# Tools
# ---------------------------------------------------------------------------
TOOLS = \
    tools/guoffset \
    tools/libinfo \
    tools/structsizes

# ---------------------------------------------------------------------------
# Tests (CNetC doors that exercise SDK functionality)
# ---------------------------------------------------------------------------
TESTS_DOOR = \
    tests/test_hello \
    tests/test_userdata \
    tests/test_library \
    tests/test_structsizes

TESTS_STANDALONE = \
    tests/test_headers

TESTS = $(TESTS_DOOR) $(TESTS_STANDALONE)

# ---------------------------------------------------------------------------
# Aggregate targets
# ---------------------------------------------------------------------------
ALL_EXAMPLES = $(CNETC_DOORS) $(STANDALONE) $(STANDALONE_CNETLIB) \
    $(STANDALONE_CNETLIB_DOORUTIL) \
    $(STANDALONE_CNET4) $(STANDALONE_BSDSOCK) $(STANDALONE_DOSONLY) \
    $(STANDALONE_NOSTARTUP) $(RAW_DOORS)

ALL_TARGETS = $(ALL_EXAMPLES) $(TEMPLATES) $(TOOLS) $(TESTS)

.PHONY: all clean lib examples templates tools tests

all: lib examples templates tools

lib:
	$(MAKE) -C lib PREFIX=$(PREFIX)

examples: lib $(ALL_EXAMPLES)

templates: lib $(TEMPLATES)

tools: $(TOOLS)

# ---------------------------------------------------------------------------
# Build rules
# ---------------------------------------------------------------------------

# CNetC doors need the door library
$(CNETC_DOORS): %: %.c lib
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

# Standalone utilities don't need the door library
$(STANDALONE): %: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

$(STANDALONE_CNETLIB): %: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

# Standalone with cnet.library AND libcnet_door.a for utility functions
$(STANDALONE_CNETLIB_DOORUTIL): %: %.c lib
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

$(STANDALONE_CNET4): %: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

$(STANDALONE_BSDSOCK): %: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

$(STANDALONE_DOSONLY): %: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

$(STANDALONE_NOSTARTUP): %: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

# Raw doors need the door library for CallHost/CMessage support
$(RAW_DOORS): %: %.c lib
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

# CNetC and raw door templates need the door library
templates/pfile_cnetc: templates/pfile_cnetc.c lib
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

templates/pfile_raw: templates/pfile_raw.c lib
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

# Standalone template does NOT need the door library
templates/standalone: templates/standalone.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

# Tools
tools/guoffset: tools/guoffset.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

tools/libinfo: tools/libinfo.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

tools/structsizes: tools/structsizes.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

# Tests
tests: lib $(TESTS)

# Door tests need the door library
$(TESTS_DOOR): %: %.c lib
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

# Standalone tests don't need the door library
$(TESTS_STANDALONE): %: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	$(MAKE) -C lib PREFIX=$(PREFIX) clean
	rm -f $(ALL_TARGETS)
