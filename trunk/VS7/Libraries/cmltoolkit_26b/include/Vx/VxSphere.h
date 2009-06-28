#ifndef VX_SPHERE_H
#define VX_SPHERE_H

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

// VxSphere.h: interface for the VxSphere class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/VxGeometry.h"

namespace Vx
{

// tolua_begin
/**
 * Sphere collision geometry
 *
 */
class VX_DLLSPEC VxSphere : public VxGeometry
{

public:
    VxSphere(VxReal radius);
    virtual ~VxSphere();

    static int getClassType();
    virtual const char* getClassName() const { return "VxSphere"; }

    void setRadius(VxReal inRadius);
    VxReal getRadius() const;
};
// tolua_end

} // namespace Vx

#endif // VX_SPHERE_H
