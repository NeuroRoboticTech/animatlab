#ifndef _MCDDISTANCERESULT_H
#define _MCDDISTANCERESULT_H
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

/** @file McdDistanceResult.h

    Output data structure for McdDistance().
 */

#include "McdModelPair.h"

#ifdef __cplusplus
extern "C" {
#endif


/*----------------------------------------------------------------
 * McdDistanceResult
 *---------------------------------------------------------------
 */

/** Distance query data. Output structure for McdDistance().
    @see McdDistance
 */
typedef struct McdDistanceResult
{
    McdModelPair *pair;   /**< pair of models */
    MeReal distanceLB;    /**< lower bound on distance, same as distanceUB for exact distance */
    MeReal distanceUB;    /**< upper bound on distance */
    MeVector3 point1;    /**< point on model 1 realizing this distance, valid only for exact distance */
    MeVector3 point2;    /**< point on model 2 realizing this distance, valid only for exact distance */

  /** auxiliary data. Used by McdTriangleMesh distance function to return triangle index on closest triangle on model 1. */
  union {void *ptr; int tag;} element1;
  /** auxiliary data. Used by McdTriangleMesh distance function to return triangle index on closest triangle on model 2. */
  union {void *ptr; int tag;} element2;

} McdDistanceResult;

typedef struct McdDistanceRequest
{
  MeReal min;   /**< Minimum distance below which computation stops. 
          If an internal estimate determines the distance to be smaller than min, distance estimation
          does not attempt to produce a more precise estimate and stops. */
  MeReal max;   /**< Maximum distance above which computation of more precision distance stops.
          If an internal estimate determines the distance to be greater than max, distance estimation
          does not attempt to produce a more precise estimate and stops. */
  MeReal relativeError; /**< For certain types of distance estimates (eg. mesh-primitives), 
          distance estimate will guarantee that (distanceUB-distanceLB)/distanceLB < relativeError. */
} McdDistanceRequest;


typedef void (MEAPI *McdDistanceFn)( struct McdModelPair*, struct McdDistanceResult* );

typedef void (MEAPI *McdApproximateDistanceFn)( struct McdModelPair*, struct McdDistanceRequest*, struct McdDistanceResult* );

void MEAPI McdDistanceRequestInit( McdDistanceRequest* );

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* _MCDDISTANCERESULT_H */
