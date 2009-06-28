#if !defined(VX_WINCHJOINT_H)
#define VX_WINCHJOINT_H

/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/02/17 17:00:48 $
   
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

// VxWinchJoint.h: interface for the VxWinchJoint class.
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
 * the relative translation of part 1 and two. The constraint also manages a
 * distance parameter which is proportional to the part 0 rotation to simulate
 * cable-winch behaviour.
 *
 * This constraint currently has no constraint coordinates with motors
 * or locks.
 * 
 */
class VX_DLLSPEC VxWinchJoint : public VxConstraint  
{

public:
    VxWinchJoint();
    VxWinchJoint(VxPart* part0, VxPart* part1, VxPart* part2, VxReal ratio, VxReal3 axis);
    virtual ~VxWinchJoint();

    virtual const char* getClassName() const { return "VxWinchJoint"; }
    static int getClassType();
    
    void setWinchJointRatio(VxReal value);
    VxReal getWinchJointRatio() const;

    void setAxis(VxReal3 v);
    void setAxis(int body, VxReal3 v);
    void getAxis(int body, VxReal3Ptr a) const;

    void setMaxTorque(VxReal value);
    VxReal getMaxTorque() const ;

    void setMinTorque(VxReal value);
    VxReal getMinTorque() const ;

    void setDifferential(VxReal value);
    VxReal getDifferential() const ;
    void setDistance(VxReal value);
    VxReal getDistance() const ;

    void setMinDistance(VxReal value);
    VxReal getMinDistance() const ;
    void setMaxDistance(VxReal value);
    VxReal getMaxDistance() const ;

    // tolua_end
protected:
    
private:
    void _reset();
    void init();

}; // tolua_export

} //namespace Vx

#endif // VX_WINCHJOINT_H
