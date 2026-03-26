/* Test that the CNet SDK headers compile cleanly with GCC */
#include <exec/types.h>
#include <exec/memory.h>
#include <exec/ports.h>
#include <dos/dos.h>

#include <cnet/cnet.h>

/* Verify some key struct sizes are non-zero */
int main(void) {
    struct UserData ud;
    struct CPort cp;
    struct CMessage cm;
    struct IsDate id;

    (void)ud;
    (void)cp;
    (void)cm;
    (void)id;

    return 0;
}
