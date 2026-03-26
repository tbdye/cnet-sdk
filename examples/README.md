# CNet SDK Examples

42 example programs demonstrating CNet API usage, ported from SAS/C to GCC.

## Categories

- **CNetC doors** -- Use CNCL_DoorInit and the cnetc.library door framework
- **Standalone utilities** -- Read BBS state via FindPort("cnetport"), no login needed
- **Raw CMessage doors** -- V3-style compatibility using direct message passing

## Not Compiled

- `CheckOnline.c` -- Code snippet (no main function)
- `mailsend.c` -- Code snippet with known bugs (see docs/known-issues.md)

See [docs/examples-guide.md](../docs/examples-guide.md) for annotated walkthroughs.
