#ifndef _MCDTRIANGLENE_H
#define _MCDTRIANGLENE_H
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
  @file McdTriangleNE.h
  The user-specified triangle. Not part of the geometry.
  Subject to change
*/

#include "McdModel.h"
#include "McdIntersectResult.h"

#ifdef __cplusplus
extern "C" {
#endif

#if 1
typedef struct McdTriangleNE
{
  MeReal *vertices; /**< pointers to vertices */
  MeVector3 normal;    /**< triangle normal. Subject to change */
  MeVector3 edges[3];  /**< edge vector. Subject to change */
  int tag;    /**< user triangle id. */
  MeVector3 mateNormalCos; /**< dot products of triangle normal with neighbour triangle normal across edges. Subject to change. */
  MeVector3 mateNormalSin; /**< cross products of triangle normal with neighbour triangle normal across edges dotted with edge. Subject to change. */
} McdTriangleNE;
#endif

/**
    Initialize a McdTriangleNE struct to hold the triangle represented
    by @a vertex1, @a vertex2, @a vertex3. The vertices must be in
    counterclockwise order when seen from the outside of the model.
    Collision detection with other geometries is one-sided, that is
    it is assumed that triangles are part of a model.
    Normal and edges are computed in this call. This part of the API
    is subject to change; for example, the normal might become
    a pointer to a normal that needs to be computed by the user.
*/
void              MEAPI McdTriangleNEInit(McdTriangleNE* t,
                           MeReal* vertex1, MeReal* vertex2, MeReal* vertex3);

/**
    Produce contacts for a model with box geometry with a triangle.
*/
void MEAPI McdBoxTriangleNEIntersect(McdModelID box, McdTriangleNE* t, McdIntersectResult *result);

/**
    Produce contacts for a model with sphere geometry with a triangle.
*/
void MEAPI McdSphereTriangleNEIntersect(McdModelID sphere, McdTriangleNE* t, McdIntersectResult *result);

/**
    Produce contacts for a model with cylinder geometry with a triangle.
*/
void MEAPI McdCylinderTriangleNEIntersect(McdModelID cylinder, McdTriangleNE* t, McdIntersectResult *result);

/**
    Produce contacts for a model with any geometry for which the triangle
    functions are implemented. The return value is 0 if the corresponding
    function is not implemented. See the guide or this header file
    for a listing of implemented functions.
*/
int MEAPI McdTriangleNEIntersect(McdModelID model, McdTriangleNE* t, McdIntersectResult *result);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MCDTRIANGLENE_H */
