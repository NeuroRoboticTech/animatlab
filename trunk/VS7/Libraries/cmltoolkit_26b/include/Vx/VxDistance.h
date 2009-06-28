#if !defined(VX_DISTANCE_H)
#define VX_DISTANCE_H

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

// VxDistance.h: interface for the VxDistance class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/VxConstraint.h"

namespace Vx
{

// tolua_begin
/**
 * Ball-and-socket constraint class.
 * 
 * A ball and socket joint constrains a point offset from one part
 * to be at the same location as that of a point offset from another body
 * This removes three degrees of freedom. 
 *
 * This constraint currently has no constraint coordinates with motors
 * or locks.
 * 
 */
class VX_DLLSPEC VxDistance : public VxConstraint  
{

public:
    VxDistance();
    VxDistance(VxPart* part1, VxPart* part2, VxReal distance);
    virtual ~VxDistance();

    virtual const char* getClassName() const { return "VxDistance"; }
    static int getClassType();
    
    void setDistance(VxReal value);
    VxReal getDistance();

    void setSwingDamping(VxReal value);
    void setTwistDamping(VxReal value);
    void setLinearDamping(VxReal value);
    VxReal getSwingDamping();
    VxReal getTwistDamping();
    VxReal getLinearDamping();
    // tolua_end
protected:
    
private:
    void _reset();
    void init();
}; // tolua_export

} // namespace Vx

#endif // VX_DISTANCE_H
