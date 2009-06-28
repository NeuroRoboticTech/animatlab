#if !defined(VX_SCREWJOINT_H)
#define VX_SCREWJOINT_H

/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/02/16 21:58:18 $
   
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

// VxScrewJoint.h: interface for the VxScrewJoint class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/VxConstraint.h"

namespace Vx
{

// tolua_begin
/**
 * screw joint constraint class.
 * 
 * This constraint enforces that the rotation of part 0 about its primary
 * axis is proportional, in the given ratio but with the opposite sign, to
 * the relative translation of part 1 and part 2 about their primary axis.  
 * if part 2 is absent, the absolute translation of part 1 is used. 
 * This is useful to
 * simulate screw but be aware that this constraint does not include the
 * attachment to attach the screw joint to a part.
 *
 * This constraint currently has no constraint coordinates with motors
 * or locks.
 * 
 */
class VX_DLLSPEC VxScrewJoint : public VxConstraint  
{

public:
    VxScrewJoint();
    VxScrewJoint(VxPart* part1, VxPart* part2, VxReal ratio, VxReal3 axis);
    virtual ~VxScrewJoint();

    virtual const char* getClassName() const { return "VxScrewJoint"; }
    static int getClassType();
    
    void setScrewJointRatio(VxReal value);
    VxReal getScrewJointRatio();

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

#endif // VX_SCREWJOINT_H
