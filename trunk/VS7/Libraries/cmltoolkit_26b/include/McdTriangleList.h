#ifndef _MCDTRIANGLELIST_H
#define _MCDTRIANGLELIST_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/12/17 21:25:11 $
   
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
  @file McdTriangleList.h
  The user-specified triangle list geometry type
*/

#include "McdGeometry.h"
#include "McdUserTriangle.h"

#ifdef __cplusplus
extern "C" {
#endif


MCD_DECLARE_GEOMETRY_TYPE( McdTriangleList );

/** McdTriangleList is a concrete McdGeometry type. */
typedef McdGeometryID McdTriangleListID;


/** User function to get triangles given a model containing a
    triangle list geometry. The triangle list will be the
    second model in the pair. */
typedef int (MEAPI * McdTriangleListFn)(McdModelPair* modelTriListPair,
                     MeVector3 pos, MeReal radius);

/** Geometry given by user-specified triangle list */
typedef struct McdTriangleList
{
  McdGeometry m_g;

  MeReal mR[3];
  MeReal mRadius;

  McdUserTriangle *triangleList;    /**< user allocated list of triangles */
  int triangleCount;           /**< current number of triangles */
  int triangleMaxCount;        /**< max number of triangles in list */

  void *userData;              /**< user geometry representation, or other user data */
  union {void *ptr; int tag;} element; /** user data */
  McdTriangleListFn triangleListGenerator; /**< function to calculate triangle list. Returns non-zero if there were any triangles to intersect */

} McdTriangleList;


/** Register triangle list geometry type. */
void              MEAPI McdTriangleListRegisterType();
McdGeometryID     MEAPI McdTriangleListCreate(MeReal dx, MeReal dy, MeReal dz, McdTriangleListFn f );

void              MEAPI McdTriangleListSetList( McdGeometryID g,
                        McdUserTriangle* triangleList, int size, int maxSize);
void              MEAPI McdTriangleListGetList( McdGeometryID g,
                        McdUserTriangle** triangleList, int *size, int *maxSize);
void          MEAPI McdTriangleListSetBBDimensions(McdGeometryID g,
                            MeReal dx, MeReal dy, MeReal dz );
void          MEAPI McdTriangleListGetBBDimensions(McdGeometryID g,
                            MeReal dx, MeReal dy, MeReal dz );

void*             MEAPI McdTriangleListGetUserData( McdGeometryID g );
void              MEAPI McdTriangleListSetUserData( McdGeometryID g, void* );
void              MEAPI McdTriangleListSetGenerator( McdGeometryID g, McdTriangleListFn f );

int McduTriangleListCreateFromObjWithAcceptCB( 
               const char *filename, MeReal scale,
               MeMatrix4Ptr tm, int flipNormals, 
               MeVector3** outVertices, int* outVertexCount, 
               MeVector3Ptr** outTriVertexPtrs, int* outTriCount,
               MeBool (*cb)(MeVector3 p0, MeVector3 p1, MeVector3 p2, void* data), void* userData);

int McduTriangleListCreateFromASEWithAcceptCB( 
           const char *filename, MeReal scale, MeMatrix4Ptr tm,
           int flipNormals, 
           MeVector3** outVertices, int* outVertexCount, 
           MeVector3Ptr** outTriVertexPtrs, int* outTriCount,
           MeBool (*cb)(MeVector3 p0, MeVector3 p1, MeVector3 p2, void* data), void* userData);

MeBool MEAPI McdTriangleListLineSegmentRegisterInteraction();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MCDTRIANGLELIST_H */
