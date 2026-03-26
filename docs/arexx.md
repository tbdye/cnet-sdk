# CNet 5 ARexx Integration Guide

CNet exposes a comprehensive ARexx command interface for scripting,
automation, and door development. This guide covers port naming,
the most important commands, the GETUSER code table, and how to use
ARexx from doors and external scripts.

---

## Table of Contents

1. [ARexx Port Names](#arexx-port-names)
2. [Using ARexx from Doors vs External Scripts](#using-arexx-from-doors-vs-external-scripts)
3. [Key Commands](#key-commands)
4. [GETUSER Codes](#getuser-codes)
5. [Advanced GETUSER: Raw Structure Access](#advanced-getuser-raw-structure-access)
6. [PUTUSER and SETOBJECT](#putuser-and-setobject)
7. [Scratch Account Access](#scratch-account-access)
8. [Examples](#examples)

---

## ARexx Port Names

CNet publishes one ARexx port per active process:

| Port name | Process | Purpose |
|-----------|---------|---------|
| `CONTROLREXX.1` | Control (supervisor) | System-level commands |
| `CNETREXX0` | bbs port 0 | User session commands |
| `CNETREXX1` | bbs port 1 | User session commands |
| `CNETREXX2` | bbs port 2 | User session commands |
| ... | ... | ... |
| `CNETREXX99` | bbs port 99 | User session commands |

The per-port names follow the pattern `CNETREXX{portnum}` with no
separator between the prefix and the port number. Port numbers are
0-based and match the port numbers shown in the Control panel and WHO
displays.

---

## Using ARexx from Doors vs External Scripts

### From an ARexx door

When CNet launches an ARexx script as a door (subboard type 6 /
`MRK_AREXX_DOOR`), the script automatically addresses the correct
CNet port. No `address` command is needed:

```rexx
/* ARexx door -- automatically addresses the launching port */
options results
'GETUSER' 1
say "Hello," result
```

### From an external script

External ARexx scripts (run from a CLI shell or via `RX`) must
explicitly address a CNet port before sending commands:

```rexx
/* External script -- must address a specific port */
options results
address CNETREXX0

'GETUSER' 1
say "User on port 0:" result
```

Replace `0` with the port number you want to interact with.

### From a C-language door

C doors do not use the ARexx interface directly. They access the same
data through the `z` (PortData) and `myp` (MainPort) struct pointers,
which provide the same information more efficiently. For example,
`GETUSER 1` (handle) in ARexx corresponds to `z->user1.Handle` in C.

However, a C door can execute an ARexx script using the AmigaDOS
`Execute()` function or by sending a message to the ARexx master port.
This is rarely needed.

---

## Key Commands

All commands return their result in the ARexx `RESULT` variable. Use
`options results` at the top of your script to enable result capture.

### GETUSER {n}

Retrieve a user data value by numeric code. See the full code table
below.

```rexx
'GETUSER' 1       /* returns the user's handle */
handle = result
```

### GETCARRIER

Check whether the user is still connected. Returns `1` if carrier is
present (or in local mode), `0` otherwise.

```rexx
'GETCARRIER'
if result = 0 then exit   /* user disconnected */
```

### GETPORTID {port}

Get the account number of the user on a specific port. Returns `-1` if
no user is logged in on that port.

```rexx
'GETPORTID' 0
if result = -1 then say "Port 0 is empty"
else say "Port 0 has account #" result
```

### BBSIDENTIFY {arg}

Query system identity information. The `{arg}` selects what to return:

| Argument | Returns | Example |
|----------|---------|---------|
| `USER` | `"handle" "location" access_level` | `"Samoht" "Home" SYSOP` |
| `TERM` | `baud columns rows line` | `38400 80 24 0` |
| `BBS` | CNet version string | `5.36c` |
| `NAME` | BBS name (from bbstext line 9) | `My BBS` |
| `SYSOP` | Sysop name (from bbstext line 10) | `Samoht` |
| `EMULATION` | Terminal type | `ANSI` or `ASCII` |
| `ABBEREXX` | ARexx interface version | `1.0` |

Access levels returned by `USER`: `GUEST` (new user), `MEMBER`
(verified), `SYSOP` (account #1), `CO-SYSOP` (other maintenance
accounts).

### TRANSMIT / PRINT {s}

Send text to the user with MCI code translation, followed by a newline.

```rexx
'TRANSMIT' "Hello, world!"
'TRANSMIT' "c1Red text c0Default text"
```

### SENDSTRING {s}

Send text without a trailing newline.

### SEND {s}

Send raw text with no MCI translation.

### PROMPT {length} {type} '"{prompt}"'

Input a line from the user.

```rexx
'PROMPT' 40 NORMAL '"Enter your name: "'
name = result
```

Types: `NORMAL`, `HIDE` (password), `YESNO` (default yes), `NOYES`
(default no).

Note the quoting: the prompt string must be enclosed in single quotes
wrapping double quotes so ARexx passes the quotes to CNet.

### QUERY {s}

Input a line with a prompt (up to 80 characters). Simpler than PROMPT
but less flexible.

```rexx
'QUERY' "What is your favorite color?"
color = result
```

### GETCHAR

Wait for a single keypress. Returns the character.

```rexx
'GETCHAR'
key = result
```

### FINDACCOUNT {s}

Search for a user by handle or account number. Returns the account
number, or `0` if not found.

```rexx
'FINDACCOUNT' "Samoht"
if result > 0 then say "Found account #" result
```

### CHANGEWHERE {s}

Set the user's "where" field in WHO displays. Returns the previous
value.

### LOGENTRY {s} / SYSOPLOG {s}

Write a line to the call log.

### ADDPOINTS {n}

Add or subtract door game points. Use negative numbers (in parentheses)
to subtract.

```rexx
'ADDPOINTS' 50         /* add 50 points */
'ADDPOINTS' (-10)      /* subtract 10 points */
```

### ADDTIME {n}

Add or subtract time in tenths of minutes.

### CALLEDITOR {n}

Open the text editor with a maximum of `{n}` lines (0 for default).
Returns `1` if the buffer is not empty after editing, `0` otherwise.

### LOADEDITOR {path}

Load a file into the editor buffer before calling `CALLEDITOR`.

### SAVEEDITOR {path}

Save the editor buffer to a file. Returns `1` on success.

### LOADSCRATCH {n} / GETSCRATCH {n} / PUTSCRATCH {n} / SAVESCRATCH {n}

Account inspection commands. See the Scratch Account Access section.

### WRITEMAIL {account}

Send the editor buffer contents as mail to the specified account. Use
`SETMAILSUBJ` first.

### SPAWN {script}

Transfer control to another ARexx script. Usually the last command in
a door.

---

## GETUSER Codes

The numeric code table for `GETUSER` and `GETSCRATCH`. These are the
predefined access codes (1-92):

### User identity

| Code | Returns |
|------|---------|
| 1 | Handle |
| 2 | Password |
| 3 | Real name |
| 4 | City and state |
| 5 | Zip code |
| 6 | Address |
| 39 | UUCP id |
| 40 | Account number |
| 41 | ID (serial) number |
| 56 | Country |
| 59 | Organization |

### Session info

| Code | Returns |
|------|---------|
| 7 | Time left (this session) |
| 11 | Last call date and time |
| 12 | Today's date and time |
| 15 | Access group (number) |
| 16 | Access group name |
| 17 | Maintenance access (0=No) |
| 18 | Minutes online today |
| 23 | Port number |
| 46 | Morning, afternoon, or evening |
| 47 | System caller number |
| 57 | Logon time |

### Contact

| Code | Returns |
|------|---------|
| 9 | Voice phone |
| 10 | Data phone |
| 58 | Birth date |

### Statistics

| Code | Returns |
|------|---------|
| 20 | Balance (credits) |
| 21 | Door points |
| 22 | Total calls |
| 30 | Uploaded kbytes |
| 31 | Uploaded files |
| 32 | Downloaded kbytes |
| 33 | Downloaded files |
| 34 | File credits |
| 35 | Byte credits |
| 36 | Public messages |
| 37 | Private messages |

### Daily statistics

| Code | Returns |
|------|---------|
| 42 | Day uploaded bytes |
| 43 | Day downloaded bytes |
| 44 | Day uploaded files |
| 45 | Day downloaded files |

### Transfer settings

| Code | Returns |
|------|---------|
| 24 | Current CPS |
| 25 | File upload ratio 1 |
| 26 | Byte upload ratio 1 |
| 29 | Default protocol |

### Terminal

| Code | Returns |
|------|---------|
| 19 | Screen clear code (always 12) |
| 27 | Terminal width |
| 28 | Terminal type |

### Mail

| Code | Returns |
|------|---------|
| 13 | Number of mail messages |
| 14 | Number of new mail messages |

### Display

| Code | Returns |
|------|---------|
| 8 | Sysop comment |
| 38 | User's WHO banner |
| 50 | Last user's name |
| 55 | Help level |

### Subboard context

| Code | Returns |
|------|---------|
| 48 | Current subboard number |
| 49 | Current subboard title |
| 51 | Post access (0/1) |
| 52 | Respond access (0/1) |
| 53 | Download access (0/1) |
| 54 | Upload access (0/1) |

### MCI registers

| Code | Returns |
|------|---------|
| 60-69 | MCI numeric registers (10 values) |
| 70-74 | MCI character registers (5 strings) |

### Message context

| Code | Returns |
|------|---------|
| 75 | Primary author name (by0) |
| 76 | Secondary author name (by1) |
| 77 | Primary addressee name (to0) |
| 78 | Secondary addressee name (to1) |
| 79 | Message date (date0) |
| 80 | Message organization (organ) |
| 81 | Current item number (br) |
| 82 | Current response number (rs) |
| 83 | Number of responses |
| 84 | File size |
| 85 | Number of downloads |
| 86 | Best CPS rate |
| 87 | Current item title |
| 88 | Fancy file size (with K/M suffix) |
| 89-92 | Scan output fields |

---

## Advanced GETUSER: Raw Structure Access

Beyond the predefined codes, `GETUSER` supports a 7-digit format for
direct access to any field in the PortData or MainPort structures:

```
GETUSER XT#####
```

Where:
- **X** = structure selector: `1` for PortData, `2` for MainPort
- **T** = type code: `1`=BYTE, `2`=SHORT, `3`=CHAR*, `4`=LONG,
  `5`=IsDate*, `6`=CHAR** (pointer to pointer)
- **#####** = five-digit byte offset into the structure

Example: to read a LONG at offset 612 in PortData (total calls):

```rexx
'GETUSER' 1400612
calls = result
```

This is a powerful but fragile mechanism. Offsets depend on the exact
struct layout, which differs between CNet versions. Prefer the
predefined codes (1-92) whenever possible.

---

## PUTUSER and SETOBJECT

To modify user data, use `SETOBJECT` followed by `PUTUSER`:

```rexx
/* Set the user's door points to 100 */
'SETOBJECT' 100
'PUTUSER' 21
```

`SETOBJECT` sets the value to write; `PUTUSER` writes it to the field
specified by the same numeric code as `GETUSER`. Not all fields are
writable.

---

## Scratch Account Access

The scratch account mechanism lets ARexx scripts read (and optionally
modify) user accounts other than the current user:

```rexx
/* Load account #2 into scratch buffer (locks the account) */
'LOADSCRATCH' 2
if result = 1 then do
    'GETSCRATCH' 1              /* get handle */
    say "Account 2 handle:" result
    'GETSCRATCH' 22             /* get total calls */
    say "Total calls:" result

    /* Release lock WITHOUT saving changes */
    'SAVESCRATCH' (-2)          /* negative = unlock without save */
end
```

Important: `LOADSCRATCH` locks the account, blocking other ports from
modifying it. Always follow with `SAVESCRATCH`:
- Positive account number: save changes and unlock.
- Negative account number: discard changes and unlock.

---

## Examples

### Query current user info from a door

```rexx
/* user_info.rexx -- display user information */
options results

'GETUSER' 1
handle = result

'GETUSER' 3
realname = result

'GETUSER' 15
group = result

'GETUSER' 16
groupname = result

'GETUSER' 22
calls = result

'GETUSER' 7
timeleft = result

'TRANSMIT' ""
'TRANSMIT' "   Handle: " || handle
'TRANSMIT' "Real Name: " || realname
'TRANSMIT' "   Access: " || group || " (" || groupname || ")"
'TRANSMIT' "    Calls: " || calls
'TRANSMIT' "Time Left: " || timeleft || " minutes"
'TRANSMIT' ""
```

### Check all ports from an external script

```rexx
/* portcheck.rexx -- show who is online (run from CLI) */
options results
address CNETREXX0

do port = 0 to 5
    'GETPORTID' port
    if result >= 0 then
        say "Port" port ": account #" result
    else
        say "Port" port ": empty"
end
```

### Look up another user's info

```rexx
/* lookup.rexx -- look up a user by handle */
options results

'QUERY' "Enter handle to look up:"
search = result

'FINDACCOUNT' search
acct = result
if acct = 0 then do
    'TRANSMIT' "User not found."
    exit
end

'LOADSCRATCH' acct
if result = 1 then do
    'GETSCRATCH' 1
    handle = result
    'GETSCRATCH' 3
    realname = result
    'GETSCRATCH' 22
    calls = result
    'TRANSMIT' ""
    'TRANSMIT' "     Handle: " || handle
    'TRANSMIT' "  Real Name: " || realname
    'TRANSMIT' "Total Calls: " || calls
    'TRANSMIT' ""
    'SAVESCRATCH' (-acct)    /* unlock without saving */
end
```

### Simple door game with points

```rexx
/* guessnum.rexx -- number guessing game */
options results

target = random(1, 100, time('S'))
guesses = 0

'TRANSMIT' ""
'TRANSMIT' "I'm thinking of a number between 1 and 100."
'TRANSMIT' ""

do forever
    'GETCARRIER'
    if result = 0 then exit    /* check for disconnect */

    'PROMPT' 3 NORMAL '"Your guess: "'
    if result = "###PANIC" then exit   /* carrier loss or timeout */
    guess = result
    guesses = guesses + 1

    if guess = target then do
        'TRANSMIT' "Correct! You got it in" guesses "guesses."
        points = 100 - (guesses * 5)
        if points < 5 then points = 5
        'ADDPOINTS' points
        'TRANSMIT' "You earned" points "door points!"
        leave
    end
    else if guess < target then
        'TRANSMIT' "Too low."
    else
        'TRANSMIT' "Too high."
end
```

### Key safety pattern

ARexx input commands return `###PANIC` when carrier is lost or the
user's time expires. Always check for this:

```rexx
'QUERY' "Enter something:"
if result = "###PANIC" then exit
```

Or check carrier periodically in loops:

```rexx
'GETCARRIER'
if result = 0 then exit
```
