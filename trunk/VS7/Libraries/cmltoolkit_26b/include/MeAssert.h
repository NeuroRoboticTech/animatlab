#ifndef _MEASSERT_H
#define _MEASSERT_H
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
 * Cross configuration and cross platform wrapper for assertions
 */

#ifdef _MECHECK

#  ifdef WIN32
#    include <crtdbg.h>
#    define MEASSERT(x)  _ASSERTE(x);
#  else
#    include <assert.h>
#    define MEASSERT(x)  assert(x);
#  endif

#else

#  define MEASSERT(x)

#endif

#define MEASSERTALIGNED(x,y) MEASSERT((((MeUintPtr)x)%((MeUintPtr)y)) == 0)

#endif /* _MEASSERT_H */
