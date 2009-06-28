#ifndef VX_GEOMETRY_H
#define VX_GEOMETRY_H

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

// VxGeometry.h: interface for the VxGeometry class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/Vx.h"

struct McdGeometry;

namespace Vx
{

// tolua_begin
/**
 *  The base class from which all collision geometry classes are derived.
 *  
 *  The primary role of the VxGeometry classes is to define the
 *  three-dimensional surface that is to be associated with a VxSensor
 *  or VxPart for the purpose of collision detection.  For performance
 *  reasons, the collision geometry is often simpler than the rendered
 *  graphic geometry.
 *
 *  Available geometries include the primitives (VxBox, VxCylinder,
 *  VxPlane and VxSphere), and the more versatile, but less efficient,
 *  VxConvexMesh, VxTriangleMesh and VxTerrain.
 *
 *  Certain mesh geometries need to be built. This is done automatically
 *  at the first step, but might cause a delay. The user can call
 *  build() to do this earlier.
 *  
 */
class VX_DLLSPEC VxGeometry: public VxRefCountBase
{
public:
// tolua_end
    VxGeometry();
    virtual ~VxGeometry();
    
    bool isOfType(int classType) const;	// tolua_export
    virtual bool isComposite() const { return false; } 
    
    void getBoundingBox(const VxReal44Ptr tm,VxReal3Ptr minCorner, VxReal3Ptr maxCorner) const;
    void getBoundingSphere(VxReal3Ptr center, VxReal *radius) const;	
    VxReal getBoundingSphereRadius() const;
    void getExtremePoint(const VxReal44Ptr tm,VxReal3Ptr inDir, VxReal3Ptr outPoint) const;
    
    virtual bool build() { return true; }
    virtual bool isBuilt() const { return true; }

    inline McdGeometry *&getMcdGeometry()	{return mMcdGeometryID;}	
    inline McdGeometry *getMcdGeometry() const {return mMcdGeometryID;}	
    

protected:
    McdGeometry *mMcdGeometryID;
}; // tolua_export

} // namespace Vx

#endif // VX_GEOMETRY_H

