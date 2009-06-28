#if !defined(VX_BOX_H)
#define VX_BOX_H

/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/12/03 01:21:28 $
   
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


// VxBox.h: interface for the VxBox class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/VxGeometry.h"

namespace Vx
{

// tolua_begin
/**
 * Box geometry used for collision. Centered at the local origin.
 *
 */
class VX_DLLSPEC VxBox : public VxGeometry
{
public:
    VxBox(VxReal3 d);
    VxBox(VxReal dx,VxReal dy,VxReal dz);
    virtual ~VxBox();

    virtual const char* getClassName() const { return "VxBox"; }
    static int getClassType();

    void setDimensions(VxReal3 d);
    void setDimensions(VxReal dx, VxReal dy, VxReal dz);
    void getDimensions(VxReal3 d) const;
};
// tolua_end

} // namespace Vx

#endif // VX_BOX_H

