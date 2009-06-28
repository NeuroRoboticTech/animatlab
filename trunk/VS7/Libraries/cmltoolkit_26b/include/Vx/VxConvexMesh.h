#ifndef VX_CONVEXMESH_H
#define VX_CONVEXMESH_H

/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/12/03 01:21:29 $
   
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


// VxConvexMesh.h: interface for the VxConvexMesh class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/VxGeometry.h"
#include "MeMath.h"

namespace Vx
{

/** 
  The Convex mesh geometry type.

  Meshes can be loaded from Wavefront obj files.
*/
class VX_DLLSPEC VxConvexMesh : public VxGeometry
{
public:
    VxConvexMesh(VxReal3* vertices, int vertexCount, VxReal fatnessRadius=0);
    VxConvexMesh(const char *fileName, VxReal scale, VxReal fatnessRadius=0);
    
    virtual ~VxConvexMesh();
    
    static int getClassType();
    virtual const char* getClassName() const { return "VxConvexMesh"; }
    
    void saveToFile(const char *fileName);

    int getPolygonCount() const;
    void getPolygonNormal(int pIndex, VxReal3Ptr n) const;
    void getPolygonCenter(int pIndex, VxReal3Ptr c) const;
    int getPolygonVertexCount(int pIndex) const;
    void getPolygonVertex(int pIndex, int vIndex, VxReal3Ptr vertex) const;
        
protected:
    void loadFromFile(const char *fileName);
private:
    VxConvexMesh() {} // for cloning
};

} // namespace Vx

#endif // VX_CONVEXMESH_H
