/*
 * guoffset.c - CNet GETUSER offset finder
 *
 * History:
 *   - Initially coded by Spaceman (M. Bruford)
 *   - Released by Dotoran of Frontiers BBS as part of his CNet GETUSER
 *     offsets package
 *   - Modified and re-released by Ray A. Akey as part of CNet SDK
 *   - Ported to GCC cross-compiler (2026)
 *
 * Original SAS/C usage:
 *   The original tool was compiled with DEF flags to substitute struct
 *   and field names at compile time:
 *     sc DEF=STRUCT=PortData DEF=VARIABLE=InBuffer off.c
 *
 * GCC version:
 *   Since GCC cannot inject struct/field references via defines in the
 *   same way, this version prints a comprehensive offset table for the
 *   most commonly needed CNet structures.  Run it on the Amiga and
 *   look up the field you need.
 *
 * Build:
 *   m68k-amigaos-gcc -noixemul -m68020 -O2 -Wall -Wno-pointer-sign \
 *       -Iinclude -Iinclude/cnet -o tools/guoffset tools/guoffset.c
 */

#include <stddef.h>
#include <stdio.h>

#include <cnet/cnet.h>

unsigned long __stack = 65536;

/* Print one offset entry */
#define PRINT_OFFSET(type, field) \
    printf("  %-30s  offset: %5lu  (0x%04lx)\n", \
           #field, \
           (unsigned long)offsetof(type, field), \
           (unsigned long)offsetof(type, field))

int main(int argc, char **argv)
{
    printf("\033[1;32mCNet SDK - Structure Offset Reference\033[0m\n");
    printf("Use these offsets with CNet's GETUSER/PUTUSER commands.\n");

    /* PortData - the most commonly needed struct for GETUSER */
    printf("\n\033[1;31m=== PortData (size: %lu) ===\033[0m\n",
           (unsigned long)sizeof(struct PortData));
    PRINT_OFFSET(struct PortData, InBuffer);
    PRINT_OFFSET(struct PortData, ABuffer);
    PRINT_OFFSET(struct PortData, InPort);
    PRINT_OFFSET(struct PortData, OnLine);
    PRINT_OFFSET(struct PortData, Carrier);
    PRINT_OFFSET(struct PortData, LocalMode);
    PRINT_OFFSET(struct PortData, TimeLeft);
    PRINT_OFFSET(struct PortData, TimeIdle);
    PRINT_OFFSET(struct PortData, Dumped);
    PRINT_OFFSET(struct PortData, pos);
    PRINT_OFFSET(struct PortData, bn);
    PRINT_OFFSET(struct PortData, br);
    PRINT_OFFSET(struct PortData, id);
    PRINT_OFFSET(struct PortData, loaded);
    PRINT_OFFSET(struct PortData, user1);
    PRINT_OFFSET(struct PortData, user2);
    PRINT_OFFSET(struct PortData, sb);
    PRINT_OFFSET(struct PortData, bm);
    PRINT_OFFSET(struct PortData, CSpawn);
    PRINT_OFFSET(struct PortData, npitems);
    PRINT_OFFSET(struct PortData, pitem);
    PRINT_OFFSET(struct PortData, KBuff);
    PRINT_OFFSET(struct PortData, KB1);
    PRINT_OFFSET(struct PortData, KB2);
    PRINT_OFFSET(struct PortData, keyin);
    PRINT_OFFSET(struct PortData, Window);
    PRINT_OFFSET(struct PortData, Item0);
    PRINT_OFFSET(struct PortData, ihead0);
    PRINT_OFFSET(struct PortData, MCIcreg);

    /* UserData */
    printf("\n\033[1;31m=== UserData (size: %lu) ===\033[0m\n",
           (unsigned long)sizeof(struct UserData));
    PRINT_OFFSET(struct UserData, Handle);
    PRINT_OFFSET(struct UserData, RealName);
    PRINT_OFFSET(struct UserData, UUCP);
    PRINT_OFFSET(struct UserData, CityState);
    PRINT_OFFSET(struct UserData, ZipCode);
    PRINT_OFFSET(struct UserData, PhoneNo);
    PRINT_OFFSET(struct UserData, MyPrivs);
    PRINT_OFFSET(struct UserData, Birthdate);
    PRINT_OFFSET(struct UserData, LastCall);
    PRINT_OFFSET(struct UserData, PassWord);

    /* MainPort - key fields for standalone utilities */
    printf("\n\033[1;31m=== MainPort (size: %lu) ===\033[0m\n",
           (unsigned long)sizeof(struct MainPort));
    PRINT_OFFSET(struct MainPort, HiPort);
    PRINT_OFFSET(struct MainPort, PortZ);
    PRINT_OFFSET(struct MainPort, Subboard);
    PRINT_OFFSET(struct MainPort, Key);
    PRINT_OFFSET(struct MainPort, Nums);
    PRINT_OFFSET(struct MainPort, SEM);
    PRINT_OFFSET(struct MainPort, MPE);
    PRINT_OFFSET(struct MainPort, gc);
    PRINT_OFFSET(struct MainPort, pc);
    PRINT_OFFSET(struct MainPort, regto);
    PRINT_OFFSET(struct MainPort, regco);
    PRINT_OFFSET(struct MainPort, z0);

    /* SubboardType4 */
    printf("\n\033[1;31m=== SubboardType4 (size: %lu) ===\033[0m\n",
           (unsigned long)sizeof(struct SubboardType4));
    PRINT_OFFSET(struct SubboardType4, Title);
    PRINT_OFFSET(struct SubboardType4, SubDirName);
    PRINT_OFFSET(struct SubboardType4, Marker);
    PRINT_OFFSET(struct SubboardType4, rn);
    PRINT_OFFSET(struct SubboardType4, sem);

    printf("\n");
    return 0;
}
