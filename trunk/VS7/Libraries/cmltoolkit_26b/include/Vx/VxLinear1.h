#if !defined(VX_LINEAR1_H)
#define VX_LINEAR1_H

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

// VxLinear1.h: interface for the VxLinear1 class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/VxConstraint.h"

namespace Vx
{

// tolua_begin
/**
 * Linear1 constraint class.
 * 
 * The linear one constraint will keep a point on body 0 in the plane perpendicular
 * to the first axis of body 1 and intersecting with the attachment position.
 * There is no constraint on the bodies orientation.
 * 
 */
class VX_DLLSPEC VxLinear1 : public VxConstraint  
{

public:
    VxLinear1();
    VxLinear1(VxPart* part1, VxPart* part2, VxReal3 pos, VxReal3 axis);
    virtual ~VxLinear1();

    virtual const char* getClassName() const { return "VxLinear1"; }
    static int getClassType();
    
    void setAttachmentPosition(VxReal3 v);
    void setAxis(VxReal3 v);
    // tolua_end
protected:
    
private:
    void _reset();
    void init();
}; // tolua_export

} // namespace Vx

#endif // VX_LINEAR1_H
