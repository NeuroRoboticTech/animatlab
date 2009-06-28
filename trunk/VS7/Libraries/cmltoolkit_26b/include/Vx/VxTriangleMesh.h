#if !defined(VX_TRIANGLEMESH_H)
#define VX_TRIANGLEMESH_H

/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/12/03 01:21:30 $
   
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


// VxTriangleMesh.h: interface for the VxTriangleMesh class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/VxGeometry.h"

namespace Vx
{

class TMVertexList;

/** 
  The triangle mesh geometry type.
  Triangle meshes allow the most flexibility in representing the collision
  geometry for an object but are also less efficient than primitives
  or convex meshes.
  TriangleMesh defines a mesh of triangles with shared vertices.
  The geometrical surface of an arbitrarily complex model can be specified
  as a collection of such triangles.
  Vertices are allocated and managed by the user unless loaded from file
  or converted from a scenegraph node, and the mesh triangles point
  to those vertices.
  If two triangles share a vertex, it is preferable if the corresponding
  vertex pointers are also the same in each triangle.
  Triangle vertices must be ordered counter-clockwise when seen from the outside
  or the object (right-hand rule) in order to produce correct normals.

  Meshes can be loaded from Wavefront obj files.

 */
class VX_DLLSPEC VxTriangleMesh : public VxGeometry
{

public:
    class AcceptTriangleSubscriber
    {
    public:
        virtual bool notify(MeVector3 p0, MeVector3 p1, MeVector3 p2) = 0;
    };
    
    VxTriangleMesh(int triCount);
    VxTriangleMesh(const char *fileName, VxReal scale, bool merge, VxReal mergeEps, int flipNormals, AcceptTriangleSubscriber* sub=0);
    
    virtual ~VxTriangleMesh();
    
    static int getClassType();
    virtual const char* getClassName() const { return "VxTriangleMesh"; }
    
    void saveToFile(const char *fileName, bool merge, VxReal mergeEps = 0.0);

    int getTriangleCount() const;

    int addTriangle(VxReal3Ptr v0, VxReal3Ptr v1, VxReal3Ptr v2);
    int addMergedTriangle(VxReal3Ptr v0, VxReal3Ptr v1, VxReal3Ptr v2, VxReal epsilon=0);

    void getTriangleNormal(int index, VxReal3Ptr n) const;
    void getTriangleVertexPtrs( int index, VxReal3Ptr *v1, VxReal3Ptr *v2, VxReal3Ptr *v3);
    void setTriangleAreaThreshold(VxReal eps);
    VxReal getTriangleAreaThreshold() const;

    bool build();
    bool isBuilt() const;
    void getVertexArray( VxReal3Ptr *v, int *vertexCount );
    void setVertexArray( VxReal3Ptr v, int vertexCount);
    void getOrientedBoundingBox(VxReal44Ptr relTM, VxReal *dx, VxReal *dy, VxReal *dz) const;

    void setMeshMeshContactCullingThresholds(VxReal distance, VxReal cosAngle);
    void getMeshMeshContactCullingThresholds(VxReal *distance, VxReal *cosAngle);


protected:
    void loadFromFile(const char *fileName, VxReal scale, bool merge, VxReal mergeEps, int flipNormals);
    VxReal3* mVerticesFromFile;
    AcceptTriangleSubscriber* mSubscriber;

    TMVertexList* mTMList;
    void setTMList(TMVertexList* list);
    
};

} // namespace Vx

#endif // VX_TRIANGLEMESH_H
