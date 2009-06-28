#if !defined(VX_HEIGHTFIELD_H)
#define VX_HEIGHTFIELD_H

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


// VxHeightField.h: interface for the VxHeightField class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/VxGeometry.h"

namespace Vx
{

// tolua_begin
/**
 * Geometry representing a heightfield given by a regular grid of heights along the local Z axis.
 */
class VX_DLLSPEC VxHeightField : public VxGeometry
{

public:
  VxHeightField();
  virtual ~VxHeightField();

  virtual const char* getClassName() const { return "VxHeightField"; }

  bool build(int xGrid, int yGrid, VxReal deltaX, VxReal deltaY,
             VxReal zVal, VxReal angHz);
  
  void getHeightFieldOrigin(VxReal *orgX, VxReal *orgY);
  void setHeightArray(VxReal *heightArray);

// tolua_end
  VxReal *getHeights(){ return mHeights;};

protected:
  void createDeformation(VxReal depth, VxReal frequency);
  VxReal *mHeights;
  VxReal mXOrigin;
  VxReal mYOrigin;
  VxReal mGridX;
  VxReal mGridY;
  VxReal mDeltaX;
  VxReal mDeltaY;
  VxReal mAngHz;

}; // tolua_export

} // namespace Vx

#endif // VX_HEIGHTFIELD_H
