#if !defined(VX_RESPONSERESULT_H)
#define VX_RESPONSERESULT_H

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


// VxResponseResult.h: interface for the VxResponseResult class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/Vx.h"

struct MdtBaseConstraint;

namespace Vx
{

/**
  * Base class for results in callbacks. An object of this class is passed in 
  * to the registered callback. It provides access non-collision properties 
  * at the time of contact. 
  *
  * @see VxIntersectResult for collision properties,
  * VxDynamicsResponse for dynamics properties.
  * 
  * @warning In the future, there will be results available for intersections with
  * particles and other types. Check the type of response to access
  * the right information!
  * 
  * 
  */
class VX_DLLSPEC VxResponseResult: public VxBase
{
  friend class VxFrame;
  friend class VxUniverse;

public:
    VxResponseResult();
    VxResponseResult(const VxResponseResult& copy);
    virtual ~VxResponseResult();

  virtual int getType(){return -1;}
  virtual const char* getClassName() const { return "VxResponseResult"; }

protected:
    MdtBaseConstraint *mContactFirst;
    MdtBaseConstraint *mContactNext;
};

} // namespace Vx

#endif // VX_RESPONSERESULT_H
