#ifndef _MCDTRIANGLEMESH_H
#define _MCDTRIANGLEMESH_H
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
   @file McdTriangleMesh.h

    The triangle mesh geometry type.
    McdTriangleMesh defines a mesh of triangles with shared vertices.
    The geometrical surface of an arbitrarily complex model can be specified
    as a collection of such triangles.
    Vertices are allocated and managed by the user and the mesh triangle point
    to those vertices. This allows the vertex data to be shared and
    synchronized between the collision package and the user's 3D graphics
    package.
    If two triangles share a vertex, it is preferable if the corresponding
    vertex pointers are also the same in each triangle.
    Otherwise, if each triangle has its own copy of its vertices,
    the system will function but will not be able to exploit connectivity
    information, and as a result contact generation might be less precise or
    less efficient in the future.

    Meshes can be loaded from Wavefront obj files using the
    McduTriangleMeshCreateFromObj function found in
    Mcd/include/McduTriangleMeshIO.h.
    In addition, meshes can be created from IRIS Performer scene graph
    nodes, interfacing to any file format that can be loaded by Performer,
    including pfb and flt files.

 */

#include "McdGeometry.h"
#include "McdModelPair.h"
#include "McdIntersectResult.h"
#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------
 *  McdTriangleMesh
 *----------------------------------------------------------------
 */

typedef McdGeometryID McdTriangleMeshID;

  /** @internal */
MCD_DECLARE_GEOMETRY_TYPE( McdTriangleMesh );
/* Declares
void              MEAPI McdTriangleMeshDestroy( McdGeometryID);\
void              MEAPI McdTriangleMeshGetAABB(McdGeometry*, MeMatrix4 transform,\
                            MeVector3 minCorner, MeVector3 maxCorner );\
void              MEAPI McdTriangleMeshGetBSphere(McdGeometryID,\
                            MeVector3 center, MeReal *radius );\
MeI16             MEAPI McdTriangleMeshGetMassProperties( McdGeometryID, MeMatrix4, \
                            MeMatrix3, MeReal* );\
char*             MEAPI McdTriangleMeshGetTypeName( McdGeometryID )
*/

/** Specifications for triangle mesh building options. In all cases collision
with the mesh will work, only distance and deformation are optional.
*/
typedef 
enum { McdTriangleMeshOptionNoDistance = 0, /*!< No distance computation or deformations are allowed on this mesh. The default */
       McdTriangleMeshOptionDistanceAllowed = 1, /*!< Distance computation only is allowed in this mesh. Takes more memory. */
       McdTriangleMeshOptionDeformAllowed = 2  /*!< Deformation only (vertex position change) is allowed in this mesh. Produces a slightly less efficiently colliding mesh. */
} McdTriangleMeshBuildOptions;

/** Structure to hold options for building triangle meshes. */
typedef struct McdTriangleMeshOptions
{
   int buildOptions; /*!< value from McdTriangleMeshBuildOptions. 
                     Can be one of 
                     - McdTriangleMeshOptionNoDistance: No distance computation or deformations are allowed on this mesh. The default.
                     - McdTriangleMeshOptionDistanceAllowed: Distance computation only is allowed in this mesh. Takes more memory.
                     - McdTriangleMeshOptionDeformAllowed: Deformation only (vertex position change) is allowed in this mesh. Produces a slightly less efficiently colliding mesh. 
                    */
} McdTriangleMeshOptions;



MeBool            MEAPI McdTriangleMeshRegisterInteractions();

/** Register collision interaction between triangle meshes */
MeBool            MEAPI McdTriangleMeshTriangleMeshRegisterInteraction();

/** Register interaction between triangle meshes and line segments. */
MeBool            MEAPI McdTriangleMeshLineSegmentRegisterInteraction();
MeBool            MEAPI McdTriangleMeshSphereRegisterInteraction();
MeBool            MEAPI McdTriangleMeshBoxRegisterInteraction();
MeBool            MEAPI McdTriangleMeshCylinderRegisterInteraction();
MeBool            MEAPI McdTriangleMeshPlaneRegisterInteraction();

/** Register collision interaction between triangle meshes and primitives. */
MeBool            MEAPI McdTriangleMeshPrimitivesRegisterInteractions();


McdTriangleMeshID MEAPI McdTriangleMeshCreate( int triCount );

McdTriangleMeshID MEAPI McdTriangleMeshCreateWithOptions(int triMaxCount,
                    McdTriangleMeshOptions* options);

int               MEAPI McdTriangleMeshAddTriangle( McdTriangleMeshID mesh,
                            MeVector3 v0, MeVector3 v1, MeVector3 v2 );
#define McdTriangleMeshGetNumberOfTriangles( m ) McdTriangleMeshGetTriangleCount( m )
int               MEAPI McdTriangleMeshGetTriangleCount( McdTriangleMeshID );

void              MEAPI McdTriangleMeshGetAuxData( McdTriangleMeshID, int** );

void              MEAPI McdTriangleMeshGetTriangleVertexPtrs( McdTriangleMeshID,
                            int index,
                            MeVector3Ptr *v1, MeVector3Ptr *v2, MeVector3Ptr *v3 );

void              MEAPI McdTriangleMeshGetTriangleNormalPtr( McdTriangleMeshID,
                            int index, MeVector3Ptr *n );
void              MEAPI McdTriangleMeshGetTriangleNormal( McdTriangleMeshID,
                            int index, MeVector3 n );

void              MEAPI McdTriangleMeshSetMinAreaThreshold(McdTriangleMeshID mesh, MeReal eps);
MeReal            MEAPI McdTriangleMeshGetMinAreaThreshold(McdTriangleMeshID mesh);

void              MEAPI McdTriangleMeshSetContactCullingThresholds(McdTriangleMeshID mesh, MeReal distance, MeReal cosAngle);
void              MEAPI McdTriangleMeshGetContactCullingThresholds(McdTriangleMeshID mesh, MeReal *distance, MeReal *cosAngle);


unsigned int      MEAPI McdTriangleMeshBuild( McdTriangleMeshID );
unsigned int      MEAPI McdTriangleMeshIsBuilt( McdTriangleMeshID mesh);
void              MEAPI McdTriangleMeshDeform(McdTriangleMeshID mesh);
void              MEAPI McdTriangleMeshSetOBBStartDepth(McdGeometryID g, int depth);
void              MEAPI McdTriangleMeshGetDepth(McdGeometryID g, int *minDepth, int *maxDepth);


MeI16             MEAPI McdTriangleMeshGetMassProperties( McdTriangleMeshID mesh, MeMatrix4 relTM, MeMatrix3 m, MeReal* volume);

void              MEAPI McdTriangleMeshSetDestroyCallback( McdTriangleMeshID, void (*)(void*), void* );

void              MEAPI McdTriangleMeshSetVertexArray( McdTriangleMeshID mesh, MeVector3Ptr v, int vertexCount );
void              MEAPI McdTriangleMeshGetVertexArray( McdTriangleMeshID mesh, MeVector3Ptr *v, int *vertexCount );

struct  McdModelPair;
struct McdIntersectResult;
struct McdDistanceResult;
struct McdContact;

int               MEAPI McdTriangleMeshTriangleMeshIntersect( struct McdModelPair *p,
                            struct McdIntersectResult* r );

int               MEAPI McdTriangleMeshBoxIntersect(McdModelPair *p, 
                            struct McdIntersectResult *result);
  
int               MEAPI McdTriangleMeshSphereIntersect(McdModelPair *p, 
                            struct McdIntersectResult *result);
  
void              MEAPI McdTriangleMeshTriangleMeshDistance( struct McdModelPair *p,
                            struct McdDistanceResult* r );

void              MEAPI McdTriangleMeshTriangleMeshApproximateDistance( struct McdModelPair *p,
                            struct McdDistanceRequest *dr, struct McdDistanceResult* r );

void              MEAPI McdTriangleMeshPrimitiveDistance( struct McdModelPair *p,
                            struct McdDistanceResult* r );

void              MEAPI McdTriangleMeshPrimitiveApproximateDistance( struct McdModelPair *p, 
                            struct McdDistanceRequest *dr, struct McdDistanceResult* r );

void              MEAPI McdTriangleMeshTriangleMeshPenetration( struct McdModelPair *p, MeVector3 direction,
                            struct McdDistanceResult* r );

void              MEAPI McdTriangleMeshTriangleMeshPenetrationSphereBounded( 
                                                                struct McdModelPair *p, MeVector3 direction,
                            struct McdDistanceResult* r, MeVector3 bPosition, MeReal bRadius );

void              MEAPI McdTriangleMeshTriangleMeshGetClosestTriangles( struct McdDistanceResult* r, int *triangle1, int *triangle2);

McdGeometryID     MEAPI McdTriangleMeshGetOBB( McdTriangleMeshID mesh, MeMatrix4Ptr relTM );

void              MEAPI McdTriangleMeshTriangleMeshGetIntersectingTriangles( struct McdContact* c, int *triangle1, int *triangle2);

int MEAPI         McdTriangleMeshPrimitiveGetIntersectingTriangle( McdContact* c );

void MEAPI        McdTriangleMeshTriangleMeshRegisterApproximateDistance();
void MEAPI        McdTriangleMeshTriangleMeshRegisterDistance();
void MEAPI        McdTriangleMeshPrimitiveRegisterDistance();
void MEAPI        McdTriangleMeshPrimitiveRegisterApproximateDistance();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MCDTRIANGLEMESH_H */
