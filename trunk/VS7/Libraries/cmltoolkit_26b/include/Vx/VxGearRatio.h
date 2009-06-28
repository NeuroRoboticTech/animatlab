#if !defined(VX_GEARRATIO_H)
#define VX_GEARRATIO_H

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

// VxGearRatio.h: interface for the VxGearRatio class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/VxConstraint.h"

namespace Vx
{

// tolua_begin
/**
 * gear ratio constraint class.
 * 
 * This constraint enforces that the rotation of part 0 about the primary
 * axis is proportional, in the given ratio but with the opposite sign, to
 * the rotation of part 1 about the secondary axis.  This is useful to
 * simulate gears but be aware that this constraint does not include the
 * hinge joint necessary to attach a gear to a part.
 *
 * This constraint currently has no constraint coordinates with motors
 * or locks.
 * 
 */
class VX_DLLSPEC VxGearRatio : public VxConstraint  
{

public:
    VxGearRatio();
    VxGearRatio(VxPart* part1, VxPart* part2, VxReal gearRatio, VxReal3 axis);
    virtual ~VxGearRatio();

    virtual const char* getClassName() const { return "VxGearRatio"; }
    static int getClassType();
    
    void setGearRatio(VxReal value);
    VxReal getGearRatio();

    void setAxis(VxReal3 v);
    void setAxis(int body, VxReal3 v);
    void getAxis(int body, VxReal3Ptr a) const;

    void setMaxTorque(VxReal value);
    VxReal getMaxTorque();

    void setMinTorque(VxReal value);
    void setDifferential(VxReal value);
    VxReal getDifferential();
    VxReal getMinTorque();
    // tolua_end
protected:
    
private:
    void _reset();
    void init();

}; // tolua_export

} //namespace Vx

#endif // VX_GEARRATIO_H
