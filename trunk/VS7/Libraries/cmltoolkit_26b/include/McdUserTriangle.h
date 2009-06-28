#ifndef _MCDUSERTRIANGLE_H
#define _MCDUSERTRIANGLE_H
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
  @file McdUserTriangle.h
  The user-specified triangle. Not part of the geometry framework.
  Used by McdTriangleList geometry. The user must call
  McdUserTriangleComputeNormal before feeding it to the triangle list.
  Subject to change
*/

#include "McdModel.h"
#include "McdIntersectResult.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct McdUserTriangle
{
  MeVector3 *vertices[3]; /**< pointers to vertices */
  MeVector3 *normal;    /**< triangle normal. Subject to change */
  MeVector3 *mateNormalCos; /**< dot products of triangle normal with neighbour triangle normal across edges. Subject to change. */ 
  MeVector3 *mateNormalSin; /**< cross products of triangle normal with neighbour triangle normal across edges dotted with edge. Subject to change. */ 

  union {void *ptr; int tag;} element1;
  union {void *ptr; int tag;} element2;

} McdUserTriangle;

void MEAPI McdUserTriangleInit(McdUserTriangle *tri);
int MEAPI McdUserTriangleComputeNormal(McdUserTriangle *tri);
void MEAPI McdUserTriangleComputeEdgeAngles(McdUserTriangle *t0, int e0, McdUserTriangle *t1, int e1);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MCDUSERTRIANGLE_H */
