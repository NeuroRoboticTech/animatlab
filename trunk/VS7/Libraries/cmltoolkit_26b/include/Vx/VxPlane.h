#ifndef VX_PLANE_H
#define VX_PLANE_H

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

// VxPlane.h: interface for the VxPlane class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/VxGeometry.h"

namespace Vx
{

// tolua_begin
/** 
 * Plane collision geometry
 * 
 */
class VX_DLLSPEC VxPlane : public VxGeometry
{
    
public:
    VxPlane();
    virtual ~VxPlane();
    
    static int getClassType();
    virtual const char* getClassName() const { return "VxPlane"; }
    
    void getGetNormal(const VxReal44Ptr tm, VxReal3Ptr normal) const;
    void getDistanceToPoint(const VxReal44Ptr tm, const VxReal3Ptr point,
        VxReal *distance) const;
    void getBoundingBox(VxReal44Ptr tm, VxReal3Ptr minCorner, VxReal3Ptr maxCorner) const;
    void getBoundingSphere(VxReal3Ptr center, VxReal *radius) const;
};
// tolua_end

} // namespace Vx

#endif // VX_PLANE_H 
