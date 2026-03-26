
#include "stdio.h"
#include "string.h"
#include "proto/all.h"
#include "fctype.h"
#include "stdlib.h"
#include "stddef.h"
#include "time.h"

#include "minrexx.h"
#include "xproto.h"

#include "dos.h"
#include "dos/dosasl.h"
#include "dos/dostags.h"

#include "cnetfuncs.h"

#include "devices/serial.h"
#include "devices/timer.h"
#include "devices/console.h"
#include "exec/exec.h"
#include "graphics/display.h"
#include "graphics/gfxbase.h"
#include "intuition/screens.h"
#include "intuition/intuition.h"
#include "intuition/intuitionbase.h"
#include "libraries/dos.h"
#include "libraries/dosextens.h"
#include "workbench/startup.h"
#include "workbench/workbench.h"	// This has DiskObject and AppIcon structs

// #include "nl.h"
#include "traplist/nl.h"			// 27-Jan-97
#include "traplist/nl_pragmas.h"	// 27-Jan-97
#include "traplist/nltags.h"		// 27-Jan-97

#include "cnet.h"
