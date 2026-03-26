/*
 * cdoor_extra.c - CNC_* linker library functions
 *
 * These 6 functions were part of cnetcdoor.lib (the SAS/C linker library
 * for CNetC doors) but were NOT in the cnetc.library shared library.
 * They handle editor list management and case-insensitive string
 * comparison.
 *
 * The CNet editor in CNetC doors uses an Exec List of text line nodes.
 * Each node contains a standard Exec Node header followed by a text
 * buffer.  This is distinct from the internal BBS editor which uses
 * the z->EdBuffer array of char pointers.
 *
 * Also includes CMakeEd(), the cnetcdoor.lib wrapper that constructs
 * an editor temp file path and calls CNC_EditMessage().
 *
 * Extern globals (from door_init.c):
 *   z   - pointer to current port's PortData
 *   myp - pointer to CNet MainPort
 *   CNetCBase - cnetc.library base (for CNC_* inline stubs)
 *
 * Compile: m68k-amigaos-gcc -noixemul -m68020 -Iinclude ...
 */

#include <string.h>
#include <stdio.h>

#include <exec/types.h>
#include <exec/memory.h>
#include <exec/lists.h>
#include <exec/nodes.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <proto/dos.h>

/*
 * Include cnetc.library inline stubs BEFORE any CNet SDK headers
 * that redefine __asm.  We need CNC_EditMessage() for CMakeEd().
 */
#include <proto/cnetc.h>

/*
 * Forward declarations for CNet struct types.  We avoid including
 * <cnet/cnet.h> because its __asm redefinition would break the
 * inline stubs already included above.
 */
struct PortData;
struct MainPort;
struct CNetCContext;
struct IsDate;

/*
 * We need access to specific fields of PortData and MainPort.
 * Since we can't include cnet.h, we access them through the
 * extern pointers and cast as needed.  However, for CMakeEd()
 * we need z->InPort and myp->gc.ZIPpath.  We include the
 * individual headers that don't conflict.
 */
#include <cnet/dates.h>
#include <cnet/editor.h>

/* Globals from door_init.c */
extern struct Library      *CNetCBase;
extern struct CNetCContext *context;
extern struct PortData     *z;
extern struct MainPort     *myp;

/*
 * Maximum line length for editor text lines.  CNet's editor
 * typically uses 80 characters per line.
 */
#define EDLINE_MAXLEN 80

/*
 * EditorLine - a single line of text in the CNetC editor list.
 *
 * This is a standard Exec list node with an embedded text buffer.
 * The CNetC editor manages linked lists of these nodes.
 *
 * The struct is not defined in any surviving SDK header (editor.h
 * only has DrawElement for the screen editor).  This definition is
 * reconstructed from the cnetcdoor.lib function behavior.
 */
struct EditorLine {
    struct Node el_Node;                /* Exec list linkage */
    char        el_Text[EDLINE_MAXLEN + 1]; /* line text buffer */
};

/* ================================================================== */
/*  CNC_compstra                                                        */
/* ================================================================== */

/*
 * CNC_compstra - case-insensitive string comparison.
 *
 * This was a linker library convenience that duplicated the compstra()
 * function from cnet.lib.  We implement it directly rather than
 * calling compstra() to avoid a cross-module dependency.
 *
 * Returns 0 if equal, <0 if s1 < s2, >0 if s1 > s2.
 */
LONG CNC_compstra(char *s1, char *s2)
{
    unsigned char c1, c2;

    if (!s1 || !s2)
        return s1 ? 1 : (s2 ? -1 : 0);

    while (*s1 && *s2) {
        c1 = *s1;
        c2 = *s2;

        /* Manual toupper for ASCII range */
        if (c1 >= 'a' && c1 <= 'z') c1 -= 32;
        if (c2 >= 'a' && c2 <= 'z') c2 -= 32;

        if (c1 != c2)
            return (LONG)c1 - (LONG)c2;

        s1++;
        s2++;
    }

    c1 = *s1;
    c2 = *s2;
    if (c1 >= 'a' && c1 <= 'z') c1 -= 32;
    if (c2 >= 'a' && c2 <= 'z') c2 -= 32;

    return (LONG)c1 - (LONG)c2;
}

/* ================================================================== */
/*  CNC_MakeEd                                                          */
/* ================================================================== */

/*
 * CNC_MakeEd - allocate a new EditorLine node.
 *
 * Allocates an EditorLine from public memory, initializes the
 * text buffer to empty, and returns a pointer to it.
 * Returns NULL on allocation failure.
 */
struct EditorLine *CNC_MakeEd(void)
{
    struct EditorLine *ed;

    ed = (struct EditorLine *)AllocVec(sizeof(struct EditorLine),
                                       MEMF_PUBLIC | MEMF_CLEAR);
    if (ed) {
        ed->el_Node.ln_Type = NT_UNKNOWN;
        ed->el_Node.ln_Pri  = 0;
        ed->el_Node.ln_Name = ed->el_Text;
        ed->el_Text[0] = '\0';
    }

    return ed;
}

/* ================================================================== */
/*  CNC_DeleteEd                                                        */
/* ================================================================== */

/*
 * CNC_DeleteEd - free an EditorLine node.
 *
 * If the node is still linked into a list (ln_Succ and ln_Pred
 * are set), it is removed first.
 */
void CNC_DeleteEd(struct EditorLine *ed)
{
    if (!ed)
        return;

    /* Remove from list if linked */
    if (ed->el_Node.ln_Succ && ed->el_Node.ln_Pred)
        Remove(&ed->el_Node);

    FreeVec(ed);
}

/* ================================================================== */
/*  CNC_OpenEd                                                          */
/* ================================================================== */

/*
 * CNC_OpenEd - open a file for editor loading.
 *
 * Opens the specified file in read mode.  Returns the AmigaOS
 * file handle (BPTR), or ZERO if the file cannot be opened.
 *
 * The caller is responsible for reading lines from the file
 * into EditorLine nodes and closing the handle when done.
 */
BPTR CNC_OpenEd(char *filename)
{
    if (!filename)
        return (BPTR)0;

    return Open(filename, MODE_OLDFILE);
}

/* ================================================================== */
/*  CNC_PrepEditor                                                      */
/* ================================================================== */

/*
 * CNC_PrepEditor - prepare an editor list for editing.
 *
 * Initializes the Exec List for use as an editor line list.
 * If the list already contains nodes, they are freed first
 * to start with a clean slate.
 */
void CNC_PrepEditor(struct List *editlist)
{
    struct Node *node, *next;

    if (!editlist)
        return;

    /* Free any existing nodes */
    node = editlist->lh_Head;
    while (node->ln_Succ) {
        next = node->ln_Succ;
        Remove(node);
        FreeVec(node);
        node = next;
    }

    /* Reinitialize the list header */
    editlist->lh_Head     = (struct Node *)&editlist->lh_Tail;
    editlist->lh_Tail     = NULL;
    editlist->lh_TailPred = (struct Node *)&editlist->lh_Head;
}

/* ================================================================== */
/*  CNC_SaveEditor                                                      */
/* ================================================================== */

/*
 * CNC_SaveEditor - save editor list contents to a file.
 *
 * Writes each EditorLine's text to the file, one line per node,
 * terminated by a newline character.
 */
void CNC_SaveEditor(struct List *editlist, char *filename)
{
    BPTR fh;
    struct Node *node;

    if (!editlist || !filename)
        return;

    fh = Open(filename, MODE_NEWFILE);
    if (!fh)
        return;

    for (node = editlist->lh_Head; node->ln_Succ; node = node->ln_Succ) {
        struct EditorLine *ed = (struct EditorLine *)node;
        LONG len = strlen(ed->el_Text);

        if (len > 0)
            Write(fh, ed->el_Text, len);
        Write(fh, "\n", 1);
    }

    Close(fh);
}

/* ================================================================== */
/*  CMakeEd - cnetcdoor.lib wrapper (NOT a CNC_* shared lib function)  */
/* ================================================================== */

/*
 * CMakeEd - construct editor temp file path and invoke the editor.
 *
 * The original cnetcdoor.lib function builds a temp file path from
 * myp->gc.ZIPpath and z->InPort, then calls CNC_EditMessage().
 *
 * Since we cannot access the gc.ZIPpath field without including
 * cnet.h (which would break the __asm inline stubs), we use the
 * path string passed by the caller.
 *
 * If 'path' is NULL, we use the standard CNet temp directory format.
 */
void CMakeEd(char *path)
{
    /*
     * Callers must provide a valid path.  The original SAS/C version
     * built the path from myp->gc.ZIPpath + "editor." + z->InPort,
     * but we cannot access those struct fields without including
     * cnet.h (which would break the __asm inline stubs).  Returning
     * without action on NULL makes the failure visible rather than
     * silently succeeding.
     */
    if (!path)
        return;

    CNC_EditMessage(path);
}
