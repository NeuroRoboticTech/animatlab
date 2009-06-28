#ifndef _MCDLINESEGINTERSECT_H
#define _MCDLINESEGINTERSECT_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/05/20 22:33:44 $
   
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
  @file
  Line segment intersections
*/

#include "McdModel.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------
 *  McdLineSegIntersect
 *----------------------------------------------------------------
 */

/** Line segment query data. */
typedef struct {
  McdModelID model;           /**< Collision model intersecting with the line segment */
  MeVector3  position;        /**< Intersection point */
  MeVector3  normal;          /**< Model normal at intersection point */
  MeReal     distance;        /**< Distance from the first end point of line segment
                                   to the intersection point. */
  McdModelID subModel;        /**< Collision model member of a composite intersecting with the line segment, or 0 if not a submodel. */
  int        tag;             /**< Element id, triangle index for a McdTriangleMesh. */

} McdLineSegIntersectResult;


/** Callback allowing the user to specify whether intersection
    with a linesegment with a specific model is enabled. Return 0 if disabled,
    non-zero otherwise. filterData is passed to the callback. */
typedef int (MEAPI *McdLineSegIntersectEnableCallback)(McdModelID, void*filterData);


/** Intersect a linesegment with a collision model */
unsigned int  MEAPI McdLineSegIntersect( const McdModelID cm,
                      MeReal* const inOrig, MeReal* const inDest,
                      McdLineSegIntersectResult *outOverlap);

/** Intersect a linesegment with a collision model. The submodels in a composite are filtered with the filterCB. */
unsigned int  MEAPI McdLineSegFirstEnabledIntersect( const McdModelID cm,
                      MeReal* const inOrig, MeReal* const inDest,
                      McdLineSegIntersectEnableCallback filterCB,
                      void * filterData,
                      McdLineSegIntersectResult *outOverlap);

typedef int  (MEAPI *McdLineSegIntersectFn)(const McdModelID cm,
                      MeReal* const inOrig, MeReal* const inDest,
                      McdLineSegIntersectResult *outOverlap);


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* _MCDLINESEGINTERSECT_H */
