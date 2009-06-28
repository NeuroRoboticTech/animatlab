#ifndef _MCDEACH_H
#define _MCDEACH_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:50:40 $
   
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

/** @file McdEach.h

    Apply functions in McdInteractions.h to a whole set of pairs in a
    single call. The McdModelPair argument is replaced by a
    McdModelPairContainer argument.

    This layer simplifies application code, and provides a "batch mode"
    operation that may be significant on multiprocessor or client-server
    architectures.
*/

#include "McdInteractions.h"
#include "McdModelPairContainer.h"

#ifdef __cplusplus
extern "C" {
#endif

void MEAPI McdHelloEach( McdModelPairContainer* );

void MEAPI McdGoodbyeEach( McdModelPairContainer* );

MeBool MEAPI
McdIntersectEach( McdModelPairContainer* pairs,
          McdModelPairContainerIterator* pairsIter,
          McdIntersectResult* resultArray, int resultArraySize,
          int *resultCount,
          McdContact* contactArray, int contactArraySize,
          int *contactCount );


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* _MCDEACH_H */
