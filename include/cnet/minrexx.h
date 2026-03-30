#ifndef CNET_MINREXX_H
#define CNET_MINREXX_H

#include <cnet/align.h>

// Some macro definitions

#define RXSNAME  "rexxsyslib.library"
#define RXSID    "rexxsyslib 1.06 (07 MAR 88)\n"
#define RXSDIR   "REXX"
#define RXSTNAME "ARexx"

// Global flag bit definitions for RexxMaster                          
#define RLFB_TRACE RTFB_TRACE          // interactive tracing?         
#define RLFB_HALT  RTFB_HALT           // halt execution?              
#define RLFB_SUSP  RTFB_SUSP           // suspend execution?           
#define RLFB_STOP  6                   // deny further invocations     
#define RLFB_CLOSE 7                   // close the master             

#define RLFMASK    (1<<RLFB_TRACE) | (1<<RLFB_HALT) | (1<<RLFB_SUSP)

// Initialization constants                                            
#define RXSVERS    34                  // main version                 
#define RXSREV     7                   // revision                     
#define RXSALLOC   0x800000            // maximum allocation           
#define RXSCHUNK   1024                // allocation quantum           
#define RXSNEST    32                  // expression nesting limit     
#define RXSTPRI    0                   // task priority                
#define RXSSTACK   4096                // stack size                   
#define RXSLISTH   5                   // number of list headers       

// Character attribute flag bits used in REXX.                         
#define CTB_SPACE   0                  // white space characters       
#define CTB_DIGIT   1                  // decimal digits 0-9           
#define CTB_ALPHA   2                  // alphabetic characters        
#define CTB_REXXSYM 3                  // REXX symbol characters       
#define CTB_REXXOPR 4                  // REXX operator characters     
#define CTB_REXXSPC 5                  // REXX special symbols         
#define CTB_UPPER   6                  // UPPERCASE alphabetic         
#define CTB_LOWER   7                  // lowercase alphabetic         
                                                                      
// Attribute flags                                                     
#define CTF_SPACE   (1 << CTB_SPACE)
#define CTF_DIGIT   (1 << CTB_DIGIT)
#define CTF_ALPHA   (1 << CTB_ALPHA)
#define CTF_REXXSYM (1 << CTB_REXXSYM)
#define CTF_REXXOPR (1 << CTB_REXXOPR)
#define CTF_REXXSPC (1 << CTB_REXXSPC)
#define CTF_UPPER   (1 << CTB_UPPER)
#define CTF_LOWER   (1 << CTB_LOWER)

//
//   Maximum messages that can be pending, and the return codes
//   for two bad situations.
//
#define MAXRXOUTSTANDING (300)
#define RXERRORIMGONE (100)
#define RXERRORNOCMD (30)

//
//   This is the association list you build up (statically or
//   dynamically) that should be terminated with an entry with
//   NULL for the name . . .
//
CNET_PACK_BEGIN

struct rexxCommandList {
   char *name ;
   char *(*userdata)() ;
};

CNET_PACK_END

#endif /* CNET_MINREXX_H */
