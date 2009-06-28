#ifndef _MCDSAFETIMERESULT_H
#define _MCDSAFETIMERESULT_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:50:41 $
   
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

/**
  @file McdSafeTimeResult.h
  McdSafeTimeResult for determining estimated time of contact
*/

#include "McdModelPair.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------
 * McdSafeTimeResult
 *---------------------------------------------------------------
 */

/** SafeTime query result. Might be merged with IntersectResult */
typedef struct
{
  McdModelPair *pair; /**< pair involved in SafeTime query */
  MeReal time; /**< estimated time of contact given linear and angular
                    velocities */
  /* int touch;   **< if 1, indicates that collision has already occured.
            However if the velocities are small, the SafeTime
            functions exit without testing for collision, so
                    touch might might be 0, even if the objects are colliding.
            Use McdIntersect to determine colliding state.
        */

} McdSafeTimeResult ;

typedef
int (MEAPI *McdSafeTimeFn)(McdModelPair*, MeReal maxTime, McdSafeTimeResult*);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* _MCDSAFETIMERESULT_H */
