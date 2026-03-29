# lib/Makefile - Build libcnet_door.a static library
#
# Cross-compile CNet door library for AmigaOS (m68k).
# Combines door lifecycle, CNC_* wrappers, utility functions,
# and CommonFuncs snippets into a single archive.

PREFIX  ?= /opt/amiga
CC      = $(PREFIX)/bin/m68k-amigaos-gcc
AR      = $(PREFIX)/bin/m68k-amigaos-ar
RANLIB  = $(PREFIX)/bin/m68k-amigaos-ranlib
CFLAGS  = -noixemul -m68020 -O2 -Wall -Wno-pointer-sign -I../include -I../include/cnet
ARFLAGS = rcs

# Source files
SRCS = door_init.c door_printf.c cdoor_wrappers.c utility.c cdoor_extra.c \
       common/CheckAccess.c common/CheckFlags.c common/ClonePortPri.c \
       common/Datestamp_to_IsDate.c common/IsAbort.c

OBJS = $(SRCS:.c=.o)

TARGET = libcnet_door.a

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
