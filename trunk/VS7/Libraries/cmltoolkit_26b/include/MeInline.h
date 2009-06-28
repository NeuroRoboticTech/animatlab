#ifndef _MEINLINE_H
#define _MEINLINE_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:51:07 $
   
   This software and its accompanying manuals have been developed by CMLabs
   Simulations Inc. ("CMLabs") and MathEngine PLC ("MathEngine").  The
   copyright to the Vortex Toolkits and all associated materials belongs to
   CMLabs.
   
   All intellectual property rights in the software belong to CMLabs,
   except for portions that have been licensed from MathEngine.

   All rights conferred by law (including rights under international
   copyright conventions) are reserved to CMLabs. This software may also
   incorporate information which is confidential to CMLabs.

   Save to the extent permitted by law, or as otherwise expressly permitted
   by CMLabs, this software and the manuals must not be copied (in whole or
   in part), re-arranged, altered or adapted in any way without the prior
   written consent of CMLabs. In addition, the information contained in the
   software may not be disseminated without the prior written consent of
   CMLabs.

 */

/** @file
 *  Inlining control. If you want to use inlining in a public header, include
 *  this file and write "#define MePUT_FUNCTIONS_HERE_IF_NOT_INLINED 1" in the
 *  source file that includes the header.
 *
 *  Usage in the header that contains inlines is:
 *
 *  #if (!MeDEFINE)
 *    prototype inline functions here like this:
 *    void someFunc(void);
 *  #endif
 *
 *  #if (MeDEFINE)
 *    define inline functions here like this:
 *    MeINLINE someFunc(void)
 *    {
 *     ...
 *    }
 *
 *  Inlining can be turned off by defining MeDONTINLINE in the makefile.
 */

#if (MeDONTINLINE)
#   define MeDEFINE MePUT_FUNCTIONS_HERE_IF_NOT_INLINED
#   define MeINLINE
#elif (__cplusplus)
#   define MeDEFINE 1
#   define MeINLINE static inline
#elif (defined __GNUC__ || defined _MSC_VER)
#   define MeDEFINE 1
#   define MeINLINE static __inline
#elif (defined __MWERKS__)
#   define MeDEFINE 1
#   define MeINLINE static inline
#else
#   define MeDEFINE MePUT_FUNCTIONS_HERE_IF_NOT_INLINED
#   define MeINLINE
#endif


#endif /* _MEINLINE_H */
