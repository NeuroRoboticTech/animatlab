#ifndef _MCDINTERACTIONS_H
#define _MCDINTERACTIONS_H
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

/**
    @file McdInteractions.h

    Queries on the geometrical relationship between a pair of McdModel
    objects. The principal query is McdIntersect(), which computes contacts.

    All pairwise queries are performed using McdModelPair objects.  The
    interactions rely on a retained-mode mechanism: Before a McdModelPair
    object is used in any of the interaction functions, you must prepare it
    by calling McdHello(). After that point, the object can be used on any
    available interaction functions, over multiple time steps in the
    simulation. When the McdModelPair object is no longer needed ( for
    example the pair of models involved are no longer in close proximity ),
    McdGoodbye() must be called on it.

    The continuity of McdModelPair identity, and keeping track of
    hello/goodbye events are managed automatically when you obtain your
    McdModelPair objects using McdSpace.

    The actual underlying algorithms depend on the actual geometry types of
    each McdModel object in the pair.
    The retained-mode mechanism  is exploited by
    some of these algorithms, which may employ
    coherence-based techniques, or perform pairwise pre-processing
    operations when a given pair is first encountered.

    Both the geometry types and the
    interaction algorithms must be explicitly registered with the Mcd
    system at initialization. If, for a particular geometry type - geometry
    type combination, there is no algorithm registered or available, the
    interaction will simply be ignored.

*/

#include "McdIntersectResult.h"
#include "McdSafeTimeResult.h"
struct McdDistanceResult;
struct McdDistanceRequest;

#ifdef __cplusplus
extern "C" {
#endif


MeBool             MEAPI McdNearby( McdModelPair* );

void               MEAPI McdHello( McdModelPair* );
void               MEAPI McdHelloWithRequest(McdModelPair *p, McdRequest* request);
void               MEAPI McdGoodbye( McdModelPair* );


MeBool             MEAPI McdIntersect( McdModelPair*, McdIntersectResult* );

MeBool             MEAPI McdIntersectAt( McdModelPair *p, McdIntersectResult *result, MeReal time );

MeBool             MEAPI McdSafeTime( McdModelPair*, MeReal maxTime, McdSafeTimeResult* );

MeBool             MEAPI McdDistance( McdModelPair*, struct McdDistanceResult* r );

MeBool             MEAPI McdApproximateDistance( McdModelPair*, struct McdDistanceRequest*, struct McdDistanceResult* r );


/*---------------------------------------------------------------
 * McdHelloCallbackFn
 *---------------------------------------------------------------
 */

  /* McdHelloCallbackFn used by McdHello to match a pair
     with a given request. (unless request already set?, eg manually) */
  /* Notes: important that this signature copy the values into the given
     location, instead of passing back a pointer to some callback-reserved
     portion of memory. Safer as well, if multiple pairs referencing the
     same request, and it gets changed or deleted */

  /* user-controlled setting of the Request field of p */
typedef void (MEAPI *McdHelloCallbackFn)(McdModelPair*, void* userData);

/* void              MEAPI McdDefaultRequestCallback( McdModelPair* ); */

McdHelloCallbackFn MEAPI McdGetHelloCallback();

void              MEAPI McdSetHelloCallback(
                            McdHelloCallbackFn, void* userData );

/** @internal */
extern McdHelloCallbackFn gMcdHelloCallbackFn;
extern void * gMcdHelloCallbackFnData;

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* _MCDINTERACTIONS_H */
