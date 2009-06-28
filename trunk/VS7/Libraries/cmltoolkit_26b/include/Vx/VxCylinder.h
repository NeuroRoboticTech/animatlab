#ifndef VX_CYLINDER_H
#define VX_CYLINDER_H

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

#include "Vx/VxGeometry.h"

namespace Vx
{

// tolua_begin
/**
 * Cylinder collision geometry with an axis along the
 * local Z axis, and centered at the local origin.
 */
class VX_DLLSPEC VxCylinder : public VxGeometry
{

public:
    VxCylinder(VxReal radius, VxReal height);
    virtual ~VxCylinder();
    
    static int getClassType();
    virtual const char* getClassName() const { return "VxCylinder"; }
    
    void setRadius(VxReal radius);
    VxReal getRadius() const;
    void setHeight( VxReal height );
    VxReal getHeight() const;
};
// tolua_end

} // namespace Vx

#endif // VX_CYLINDER_H
