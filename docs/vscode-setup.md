# VS Code Setup Guide

This guide walks you through setting up Visual Studio Code for CNet 5 SDK
development with full IntelliSense, one-key builds, and GCC error navigation.

Two environments are supported:

- **Windows** via WSL (Windows Subsystem for Linux)
- **Native Linux**

By the end, you will have a working cross-compiler, code completion for all
CNet and AmigaOS APIs, and the ability to build AmigaOS 68k executables with
a single keypress.

## Windows Setup (WSL)

Windows users develop inside WSL -- a lightweight Linux environment that runs
alongside Windows. VS Code connects to WSL transparently, so the editor UI
runs on Windows while all compilation happens inside Linux.

### Step 1: Install WSL

1. Open the Start menu and search for **Turn Windows features on or off**.
   Click it.
2. In the dialog, check these two boxes:
   - **Virtual Machine Platform**
   - **Windows Subsystem for Linux**
3. Click **OK**. Windows will install the components.
4. Restart your computer when prompted.
5. After the restart, open the **Microsoft Store** app.
6. Search for **Ubuntu** and select the "Ubuntu" app (by Canonical Group
   Limited).
7. Click **Get** (or **Install**). Wait for the download to complete, then
   click **Open**.
8. Ubuntu will perform first-time setup. It will ask you to create a Linux
   username and password. These are separate from your Windows credentials --
   pick whatever you like.

Verify the installation by opening a terminal (PowerShell or Command Prompt)
and running:

```powershell
wsl --list --verbose
```

You should see Ubuntu listed with a state of "Running" or "Stopped".

Check the **VERSION** column. If Ubuntu shows version 1, upgrade it to WSL 2
for significantly better performance:

```powershell
wsl --set-version Ubuntu 2
```

This conversion takes a few minutes. Verify by running `wsl --list --verbose`
again -- Ubuntu should now show version 2.

> **If you prefer the command line:** Opening PowerShell as Administrator and
> running `wsl --install -d Ubuntu` performs all of the above in a single
> step. However, this can be unreliable if WSL was previously installed or
> partially configured on the system.

### Step 2: Install VS Code

Download and install VS Code from <https://code.visualstudio.com/>.

After installation, open VS Code and install the **WSL** extension:

1. Press **Ctrl+Shift+X** to open the Extensions panel.
2. Search for "WSL" (publisher: Microsoft).
3. Click **Install**.

This extension lets VS Code run its backend inside WSL while the UI stays on
Windows. All file operations, terminals, and builds happen inside the Linux
environment.

### Step 3: Build the Cross-Compiler

The CNet SDK requires the Bebbo GCC cross-compiler for AmigaOS 68k. This
step builds it from source inside WSL.

Open an Ubuntu terminal. You can do this by:
- Searching for "Ubuntu" in the Start menu, or
- Opening VS Code, pressing **Ctrl+\`** to open the terminal, and selecting
  "Ubuntu (WSL)" from the terminal dropdown.

Install the build prerequisites:

```sh
sudo apt update
sudo apt install make wget git gcc g++ lhasa libgmp-dev libmpfr-dev \
    libmpc-dev flex bison gettext texinfo ncurses-dev autoconf rsync \
    libreadline-dev
```

Create the installation directory:

```sh
sudo mkdir -p /opt/amiga
sudo chown -R $USER:$USER /opt/amiga
```

Clone the toolchain and build it:

```sh
git clone https://codeberg.org/bebbo/amiga-gcc
cd amiga-gcc
make update
make all -j$(nproc)
```

> GitHub mirrors are also available at
> [AmigaPorts/m68k-amigaos-gcc](https://github.com/AmigaPorts/m68k-amigaos-gcc)
> and [BlitterStudio/m68k-amigaos-gcc](https://github.com/BlitterStudio/m68k-amigaos-gcc).

This installs the cross-compiler, NDK headers, and runtime libraries to
`/opt/amiga/`. The build takes roughly 15--30 minutes depending on your
hardware.

Verify the installation:

```sh
/opt/amiga/bin/m68k-amigaos-gcc --version
```

You should see version output from `m68k-amigaos-gcc`.

### Step 4: Use the WSL Filesystem

> **WARNING:** Never clone or build projects on `/mnt/c/` (the Windows
> filesystem mounted inside WSL). File I/O on the Windows mount is 10--50x
> slower than the native WSL filesystem, and `inotify` does not work, which
> breaks file watching and VS Code's change detection.
>
> Always work inside your WSL home directory (e.g., `~/projects/`).

VS Code's WSL extension handles this transparently. When you open a folder
from VS Code connected to WSL, you are browsing the WSL filesystem, not the
Windows filesystem.

### Step 5: Clone the SDK

Inside the WSL terminal:

```sh
mkdir -p ~/projects
cd ~/projects
git clone https://github.com/tbdye/cnet-sdk.git
cd cnet-sdk
```

### Step 6: Open in VS Code

From the WSL terminal inside the `cnet-sdk` directory, run:

```sh
code .
```

This opens VS Code connected to WSL with the SDK as the workspace. If this
is the first time, VS Code will install its server component inside WSL
automatically.

Alternatively, you can open VS Code on Windows and use the Remote Explorer
sidebar (click the monitor icon in the left bar) to browse WSL and open
folders.

When the workspace opens, VS Code will prompt you to install recommended
extensions. Click **Install All** to get the C/C++ extension (required for
IntelliSense), the Hex Editor (useful for inspecting compiled Amiga
binaries), and EditorConfig (enforces consistent formatting).

### Step 7: Verify IntelliSense

Wait a moment for the C/C++ extension to initialize. On the first open, it
queries the cross-compiler to discover system include paths and predefined
macros. This takes a few seconds.

To verify everything is working:

1. Open `templates/pfile_cnetc.c` from the Explorer panel.
2. Hover over `struct PortData` -- you should see the struct definition.
3. Hover over `UBYTE` -- it should resolve to `unsigned char`.
4. Check that there are no red squiggles on `#include` lines.

If system includes show errors, check the toolchain path in
`.vscode/c_cpp_properties.json` (see the [Custom Toolchain Path](#custom-toolchain-path)
section below).

### Step 8: Build

Press **Ctrl+Shift+B** (or use **Terminal > Run Build Task** from the menu).

The default "Build All" task runs `make all`. Compiler errors and warnings
appear in the **Problems** panel at the bottom of the window, with clickable
links that jump to the exact file and line.

Other build tasks are available through the Command Palette
(**Ctrl+Shift+P > Tasks: Run Task**):

| Task | What it builds |
|------|----------------|
| Build All | Everything (library, examples, templates, tools) |
| Build Library | `lib/libcnet_door.a` only |
| Build Examples | All example programs |
| Build Templates | Template programs |
| Build Tools | SDK tools (libinfo, structsizes, guoffset) |
| Build Tests | Test programs |
| Clean | Remove all build artifacts |

The output files are native AmigaOS 68k executables, ready to deploy to an
Amiga or emulator.

### Step 9: Deploy to WinUAE or Amiberry

Several methods are available for getting compiled binaries to the Amiga:

**WinUAE shared folder (simplest for Windows users):**
In WinUAE settings, add a Windows directory as an Amiga volume (Settings >
Hardware > CD & Hard drives > Add Directory). Copy binaries from WSL to the
Windows filesystem for this purpose:

```sh
cp examples/mypfile /mnt/c/Users/yourname/amiga-share/
```

Note: using `/mnt/c/` for deployment copies is fine -- the performance
warning applies to *building* on the Windows mount, not occasional file
copies.

**Network share (Samba):**
Set up a Samba share on the Linux host and mount it from the Amiga side using
a TCP/IP stack with SMB support.

**amigactl:**
If you have [amigactl](https://github.com/tbdye/amigactl) set up:

```sh
amigactl put examples/mypfile PFiles:mypfile
```

### Include Ordering: The `__asm` Conflict

The AmigaOS NDK header `<dos/dos.h>` defines `__asm` as a macro, which
conflicts with GCC's built-in `__asm` keyword. If you include Amiga headers
before standard C headers, you may see cryptic parse errors in libnix or
system headers.

**Rule:** Always include standard C headers first, then Amiga headers:

```c
/* Correct order */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <exec/types.h>
#include <cnet/cnet.h>
```

The SDK templates and snippets already follow this convention. See
[getting-started.md](getting-started.md) for the full explanation of this
constraint and how the SDK handles it with wrapper functions.

## Native Linux Setup

Native Linux users skip WSL entirely. The cross-compiler builds and runs
directly on the host system.

### Install the Cross-Compiler

Install the same prerequisites as the WSL section. On Debian/Ubuntu:

```sh
sudo apt update
sudo apt install make wget git gcc g++ lhasa libgmp-dev libmpfr-dev \
    libmpc-dev flex bison gettext texinfo ncurses-dev autoconf rsync \
    libreadline-dev
```

Create the installation directory and build:

```sh
sudo mkdir -p /opt/amiga
sudo chown -R $USER:$USER /opt/amiga
git clone https://codeberg.org/bebbo/amiga-gcc
cd amiga-gcc
make update
make all -j$(nproc)
```

Verify:

```sh
/opt/amiga/bin/m68k-amigaos-gcc --version
```

### Install VS Code

Download a `.deb` or `.rpm` package from <https://code.visualstudio.com/>, or
install via snap:

```sh
sudo snap install code --classic
```

No WSL extension is needed on native Linux.

### Clone, Open, and Build

```sh
git clone https://github.com/tbdye/cnet-sdk.git
cd cnet-sdk
code .
```

Accept the recommended extensions when prompted, then press **Ctrl+Shift+B**
to build. IntelliSense and build tasks work identically to the WSL setup.

### Deploy

Transfer compiled binaries to the Amiga via
[amigactl](https://github.com/tbdye/amigactl), a Samba share, or an Amiberry
shared directory.

## Custom Toolchain Path

If your cross-compiler is installed somewhere other than `/opt/amiga/`, two
things need updating: IntelliSense configuration and build tasks.

### IntelliSense (c_cpp_properties.json)

Open `.vscode/c_cpp_properties.json` and change the `amigaToolchain` value in
the `env` block:

```json
"env": {
    "amigaToolchain": "/your/toolchain/path"
}
```

This single value controls the compiler path that the C/C++ extension uses
for IntelliSense queries.

### Build Tasks (tasks.json)

The Makefile's `PREFIX` variable controls the toolchain path for builds.
There are two ways to override it:

**Option A (recommended): Edit tasks.json directly.**
Open `.vscode/tasks.json` and add the `PREFIX` argument to each task. For
example, change the "Build All" task from:

```json
"args": ["all"]
```

to:

```json
"args": ["all", "PREFIX=/your/toolchain/path"]
```

Repeat for each task (Build Library, Build Examples, etc.). The Clean task
works regardless of PREFIX. This approach keeps the override explicit and
version-controllable.

**Option B: Set PREFIX in your shell profile.**
Add the following to your `~/.bashrc`:

```sh
export PREFIX=/your/toolchain/path
```

Then configure VS Code's integrated terminal to use a login shell so the
profile is sourced. Add this to your VS Code user settings
(**Ctrl+Shift+P > Preferences: Open User Settings (JSON)**):

```json
"terminal.integrated.defaultProfile.linux": "bash (login)"
```

### Example

For a toolchain installed at `/home/user/amiga-gcc`:

- **c_cpp_properties.json:** Set `"amigaToolchain": "/home/user/amiga-gcc"`
- **tasks.json (Option A):** Change `"args": ["all"]` to
  `"args": ["all", "PREFIX=/home/user/amiga-gcc"]` for each task
- **Command line:** `make all PREFIX=/home/user/amiga-gcc`

## Using Snippets

The SDK includes workspace-scoped code snippets for common CNet development
patterns. Type a snippet prefix in a `.c` file and press **Tab** to expand
it.

| Prefix | Description |
|--------|-------------|
| `cnet-door` | Full CNetC door scaffold (use in a new empty file) |
| `cnet-standalone` | Full standalone utility scaffold (use in a new empty file) |
| `cput` | `CPutText()` call |
| `center` | `CEnterLine()` with input handling |
| `cputa` | Format into `z->ABuffer` and call `CPutA()` |
| `conekey` | `COneKey()` single keypress |
| `cputq` | `CPutQ()` yes/no question |
| `cnet-ports` | Iterate over active CNet ports |

The full scaffold snippets (`cnet-door` and `cnet-standalone`) insert an
entire file skeleton with tabstops for the filename, description, and main
body. These are best used in a new, empty `.c` file.

The snippets are defined in `.vscode/cnet.code-snippets`. You can add your
own snippets to this file or create a separate `.code-snippets` file in the
`.vscode/` directory.

**A note on line endings in CPutText:** The snippets use `\n` in
`CPutText()` calls. Both `\n` and `\r\n` work -- CNet handles CR/LF
translation internally. The [getting-started.md](getting-started.md)
examples use `\r\n` for historical reasons, but either form is correct.

## Troubleshooting

**"Cannot find compiler" in IntelliSense / red squiggles on all includes:**
The C/C++ extension cannot find the cross-compiler. Open
`.vscode/c_cpp_properties.json` and verify that the `amigaToolchain` path
points to your toolchain installation. Check the Output panel (**View >
Output**, then select "C/C++" from the dropdown) for detailed error messages.

**"make: m68k-amigaos-gcc: No such file or directory":**
The Makefile cannot find the cross-compiler. Either the toolchain is not
installed, or it is installed at a non-default path. Run
`/opt/amiga/bin/m68k-amigaos-gcc --version` to verify the installation. If
the toolchain is elsewhere, pass `PREFIX=/your/path` to make or update
`tasks.json` (see [Custom Toolchain Path](#custom-toolchain-path)).

**Build succeeds but IntelliSense shows errors:**
IntelliSense and the actual compiler can disagree on some Amiga-specific
constructs. The cross-compiler is the ground truth -- if `make` succeeds,
your code is correct. IntelliSense errors on AmigaOS-specific syntax are
cosmetic and can be ignored.

**Red squiggles only on system includes (exec/types.h, dos/dos.h, etc.):**
The compiler query failed during IntelliSense initialization. This usually
means the `compilerPath` in `c_cpp_properties.json` is wrong. Check that
the path resolves to an actual executable:

```sh
ls -l /opt/amiga/bin/m68k-amigaos-gcc
```

**Slow builds on WSL:**
Make sure you are building on the WSL filesystem (`~/projects/...`), not on
the Windows mount (`/mnt/c/...`). Building on the Windows mount is 10--50x
slower due to the filesystem translation layer.

**Ctrl+Shift+B does nothing or shows "No build task defined":**
Ensure the `.vscode/tasks.json` file exists in the workspace. If you cloned
the SDK, it should be present. Try reopening the folder (**File > Open
Folder** and select the `cnet-sdk` directory).

## Next Steps

- [getting-started.md](getting-started.md) -- Full development walkthrough:
  writing, compiling, deploying, and testing your first PFile, including the
  `__asm` include-ordering constraint
- [templates/](../templates/) -- Starter code for doors and utilities
- [architecture.md](architecture.md) -- Understanding the CNet process model,
  MainPort, and PortData
