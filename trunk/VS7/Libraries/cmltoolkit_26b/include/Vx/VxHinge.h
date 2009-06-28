#if !defined(VX_HINGE_H)
#define VX_HINGE_H

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

// VxHinge.h: interface for the VxHinge class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/VxConstraint.h"

namespace Vx
{

// tolua_begin
/**
 * Hinge joint class.
 *
 * A hinge joint leaves a pair of bodies free to rotate about a 
 * single axis but otherwise completely fixed with respect to each other. 
 * This joint removes five degrees of freedom, and is therefore more computationally
 * costly than, for example, a ball and socket joint. 
 * 
 * A Hinge joint could be used, for example, to attach a gate (or door) to a gatepost, 
 * a lever, drawbridge or seesaw to its fulcrum, or to attach rotating parts such as a 
 * wheel to a chassis, a propeller shaft to a ship or a turntable to a deck. 
 * 
 */
class VX_DLLSPEC VxHinge: public VxConstraint
{
public:
    VxHinge();
    VxHinge(VxPart* part1, VxPart* part2, VxReal3 pos, VxReal3 axis);
    virtual ~VxHinge();

    static int getClassType();
    virtual const char* getClassName() const { return "VxHinge"; }

    void setAttachmentPosition(VxReal3 v);
    void setAxis(VxReal3 v);

    // tolua_end

protected:

private:
    void _reset();
    void init();
}; // tolua_export

} // namespace Vx

#endif // VX_HINGE_H
