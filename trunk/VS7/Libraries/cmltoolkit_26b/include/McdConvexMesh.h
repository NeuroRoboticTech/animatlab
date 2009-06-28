#ifndef _MCDCONVEXMESH_H
#define _MCDCONVEXMESH_H
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
  @file McdConvexMesh.h
  The convex mesh geometry type
*/

#include "McdGeometry.h"
#include "McdInteractions.h"

#ifdef __cplusplus
extern "C" {
#endif

/** McdConvexMesh is a concrete McdGeometry type. */
typedef McdGeometryID McdConvexMeshID;

/** Structure to hold options for building convex meshes. */
typedef struct McdConvexMeshOptions
{
   char * qhullFlags; /*< flags to be passed to QHull. Use with extreme caution! */
} McdConvexMeshOptions;

void              MEAPI McdConvexMeshRegisterType();
int               MEAPI McdConvexMeshGetTypeId();


/* creation */

McdConvexMeshID   MEAPI McdConvexMeshCreate(int vertexCount, int edgeCount,
                            int polyCount, MeReal fatnessRadius);
void              MEAPI McdConvexMeshDestroy(McdConvexMeshID);

/**
    Create new convex polyhedron object from its vertices. Computes
    the convex hull polygons and edges.
*/

McdConvexMeshID   MEAPI McdConvexMeshCreateHull(MeVector3* vertices,
                            int vertexCount, MeReal fatnessRadius );
McdConvexMeshID   MEAPI McdConvexMeshCreateHullWithOptions(MeVector3* vertices,
                            int vertexCount, MeReal fatnessRadius, McdConvexMeshOptions *options );
McdConvexMeshID   MEAPI McdConvexMeshCreateSphyl(MeReal radius, MeReal height);

/* accessors */

int               MEAPI McdConvexMeshGetPolygonCount(McdConvexMeshID );
int               MEAPI McdConvexMeshGetPolygonVertexCount(McdConvexMeshID,
                            int polyID);
void              MEAPI McdConvexMeshGetPolygonVertex(McdConvexMeshID,
                            int polyID, int vertexID, MeVector3);
MeReal*           MEAPI McdConvexMeshGetPolygonVertexPtr(McdConvexMeshID,
                            int polyID, int vertexID);
void              MEAPI McdConvexMeshGetPolygonNormal(McdConvexMeshID, int,
                            MeVector3);
MeI16             MEAPI McdConvexMeshGetMassProperties( McdConvexMeshID mesh, MeMatrix4 relTM,
                            MeMatrix3 m, MeReal* volume);



  /* interactions */


MeBool               MEAPI McdConvexMeshConvexMeshHello( McdModelPair* );

int MEAPI McdConvexMeshConvexMeshIntersect( McdModelPair*,
                        McdIntersectResult* );

void              MEAPI McdConvexMeshConvexMeshGoodbye(McdModelPair*);


  /** registration of interactions with Mcd system */

MeBool              MEAPI McdConvexMeshConvexMeshRegisterInteraction();

/** register the ConvexMesh interactions with all primitives */
MeBool              MEAPI McdConvexMeshPrimitivesRegisterInteractions();

MeBool              MEAPI McdConvexMeshRegisterBoxAndSphereFns();

/** register the Box ConvexMesh interaction */
MeBool              MEAPI McdBoxConvexMeshRegisterInteraction();

/** register the Cylinder ConvexMesh interaction */
MeBool              MEAPI McdCylinderConvexMeshRegisterInteraction();

/** register the Sphere ConvexMesh interaction */
MeBool              MEAPI McdSphereConvexMeshRegisterInteraction();

/** register the RGHeightField ConvexMesh interaction */
MeBool              MEAPI McdConvexMeshRGHeightFieldRegisterInteraction();

/** register the TriangleList ConvexMesh interaction */
MeBool              MEAPI McdConvexMeshTriangleListRegisterInteraction();


  /* "AsConvex" interactions for primitive-primitive */

MeBool              MEAPI McdBoxBoxAsConvexRegisterInteraction();
MeBool              MEAPI McdBoxSphereAsConvexRegisterInteraction();
MeBool              MEAPI McdCylinderCylinderAsConvexRegisterInteraction();
MeBool              MEAPI McdCylinderBoxAsConvexRegisterInteraction();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MCDCONVEXMESH_H */
