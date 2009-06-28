#if !defined(VX_CARWHEEL_H)
#define VX_CARWHEEL_H

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

// VxCarWheel.h: interface for the VxCarWheel class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/VxConstraint.h"

namespace Vx
{

// tolua_begin
/**
 * Car Wheel joint.
 *
 */
class VX_DLLSPEC VxCarWheel: public VxConstraint
{
public:
    static const int kSuspensionLimit;
    static const int kWheelLimit;
    static const int kSteeringLimit;
    
    VxCarWheel();
    VxCarWheel(VxPart* part1, VxPart* part2, VxReal3 pos, VxReal3 steeringAxis, VxReal3 wheelAxis);
    virtual ~VxCarWheel();

    static int getClassType();
    virtual const char* getClassName() const { return "VxCarWheel"; }
    
    void setAttachmentPosition(VxReal3 v);
    void setAttachmentPosition(VxReal x, VxReal y, VxReal z);
    void setAttachmentAxes(VxReal3 wheel, VxReal3 steering);
    void setAttachmentAxes(int partID, VxReal3Ptr primary, VxReal3Ptr secondary);
    void setSuspensionParams(VxReal Kp, VxReal Kd, VxReal refPoint);
    void getSuspensionParams(VxReal *Kp, VxReal *Kd, VxReal *refPoint) const;
    virtual void enable(bool val);

    // virtual overrides
    void setPartAttachmentAxes(int partID, VxReal3 primary, VxReal3 secondary);
    void setPartAttachmentPosition(int partID, VxReal x, VxReal y, VxReal z);

// tolua_end
protected:

private:
  void _reset();

}; // tolua_export

} // namespace Vx

#endif // VX_CARWHEEL_H
