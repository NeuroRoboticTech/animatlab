#ifndef _MCDINTERSECTRESULT_H
#define _MCDINTERSECTRESULT_H
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

/** @file McdIntersectResult.h

    Output data structure for McdIntersect().
 */

#include "McdModelPair.h"
#include "McdContact.h"

#ifdef __cplusplus
extern "C" {
#endif


/*----------------------------------------------------------------
 * McdIntersectResult
 *---------------------------------------------------------------
 */

/** Intersection query data. Output format for McdIntersect().
    @see McdIntersect
 */
typedef struct McdIntersectResult
{
  McdModelPair *pair;
    McdContact *contacts;           /**< array of contacts to be filled */
    int contactMaxCount;            /**< size of array */
    int contactCount;               /**< number of contacts returned in array */
    int touch;                      /**< 1 if objects are in contact, 0 otherwise */
    MeReal normal[3];               /**< average normal of contacts returned */
    void *data;                     /**< auxiliary data */
} McdIntersectResult;

typedef int  (MEAPI *McdIntersectFn)(McdModelPair*, McdIntersectResult* );

int               MEAPI McdIntersectResultSimplify( McdIntersectResult*,
                            McdContact* inputBuffer, int inputBufferSize );


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* _MCDINTERSECTRESULT_H */
