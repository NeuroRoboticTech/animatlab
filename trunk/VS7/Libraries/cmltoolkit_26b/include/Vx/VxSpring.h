#if !defined(VX_SPRING_H)
#define VX_SPRING_H

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

// VxSpring.h: interface for the VxSpring class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/VxConstraint.h"

namespace Vx
{

// tolua_begin
/**
 * Spring joint class.
 *
 * A Spring joint connects two body with a spring-like constraint. 
 * 

 By default, this constraint tries to maintain a fixed distance between 
 the parts attachment points.
 It can be relaxed to serve as a model for a spring and this is done with the
 setStiffness() and setDamping() functions.  

 Getting constraint positions or axis on this constraint will return
 arbitrary values.

 A single limit can be used to setup hard or
 soft lower and upper limits on the extension which is always positive.  
 */
class VX_DLLSPEC VxSpring: public VxConstraint
{

public:
  VxSpring();
  VxSpring(VxPart* part1, VxPart* part2, VxReal naturalLength, VxReal stiffness, VxReal damping);
  virtual ~VxSpring();

  static int getClassType();
  virtual const char* getClassName() const { return "VxSpring"; }

  void  setNaturalLength(VxReal newNaturalLength);
  void  setStiffness(VxReal newStiffness);
  void  setDamping(VxReal newDamping);

  VxReal  getLength() const;
  VxReal  getNaturalLength() const;
  VxReal  getStiffness() const;
  VxReal  getDamping() const;

  // tolua_end

protected:

private:
    void _reset();
    void init();
}; // tolua_export

} // namespace Vx

#endif // VX_SPRING_H
