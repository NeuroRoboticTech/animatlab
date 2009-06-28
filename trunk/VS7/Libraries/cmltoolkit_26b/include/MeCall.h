#ifndef _MECALL_H
#define _MECALL_H
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
    Calling convention for standard library functions.  This is
    needed on WIN32 platform where the system provides 3 different modes for
    invoking functions.  For callbacks, users must declare their functions as:
    
    my_return_type MEAPI my_function(my_argument_list); 
    
    On other platforms, the different calling conventions are all identical.  

*/

#if defined(WIN32) && !defined(__MAKECINT__)
/** @def MEAPI_CDECL The standard C linking mode: use this for the main function */
#define MEAPI_CDECL    __cdecl
/** @def MEAPI_STDCALL */
#define MEAPI_STDCALL  __stdcall
/** document fast call */
#define MEAPI_FASTCALL __fastcall
/** @def MEAPI Use this for all the call back functions used by MeViewer */
#define MEAPI          MEAPI_STDCALL
#else
/** @def MEAPI_CDECL The standard C linking mode: use this for the main function : irrelevant except on win32*/
#define MEAPI_CDECL
/** @def MEAPI_STDCALL document stdcall : only relevant on win32*/
#define MEAPI_STDCALL
/** document fast call: only relevant on win32 */
#define MEAPI_FASTCALL
/** @def MEAPI the call back functions used by MeViewer: only relevant on  win32 */
#define MEAPI
#endif

#endif /* _MECALL_H */
