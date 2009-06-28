#if !defined(VX_DOUBLE_HINGE_H)
#define VX_DOUBLE_HINGE_H

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

// VxDoubleHinge.h: interface for the VxDoubleHinge class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/VxConstraint.h"

namespace Vx
{

// tolua_begin
/**
 * Double Hinge joint.  Useful for special types of wheels.
 *
 */
class VX_DLLSPEC VxDoubleHinge : public VxConstraint
{
public:
    static const int kDistanceLimit;
    static const int kTorsionLimit;
    static const int kWheelLimit;
    
    VxDoubleHinge();
    VxDoubleHinge(VxPart* part1, VxPart* part2, VxReal3 pos1, VxReal3 pos2, VxReal3 axis1, VxReal3 axis2);
    virtual ~VxDoubleHinge();

    static int getClassType();
    virtual const char* getClassName() const { return "VxDoubleHinge"; }
    
    virtual void enable(bool val);

// tolua_end
protected:

private:
    void _reset();
    void init();
}; // tolua_export

} // namespace Vx

#endif // VX_DOUBLE_HINGE_H
