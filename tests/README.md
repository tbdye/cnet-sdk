# CNet SDK Tests

Test programs that exercise SDK functionality.

| Test | What It Verifies |
|------|-----------------|
| `test_hello.c` | Basic door lifecycle (init, output, cleanup) |
| `test_userdata.c` | UserData struct field access and layout |
| `test_library.c` | cnet.library inline stub function calls |
| `test_structsizes.c` | Struct sizes match expected values |
| `test_headers.c` | SDK headers compile cleanly |

Build with `make tests` from the project root. Door tests require a CNet
login session; standalone tests can be run from the Amiga CLI.
