#if !defined(VX_DYNAMICSRESPONSE_H)
#define VX_DYNAMICSRESPONSE_H

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


// VxDynamicsResponse.h: interface for the VxDynamicsResponse class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/VxDynamicsContactInput.h"

struct MdtBaseConstraint;
struct McdIntersectResult;

namespace Vx
{

class VxGeometry;
class VxUniverse;

/**
 * Class containing dynamics information concerning collisions between
 * pairs of objects.
 * This class is passed into the call back the user registers for. It
 * provides access to the contact's dynamic properties.
 * These properties are initialized from the contact geometry and material
 * table and can be monitored in the callback or
 * modified for special effects.
 *  
 * For usage, see VxUniverse, VxMaterialPair
 *
 * For collision response types, see VxIntersectResult
 *
 * Other response types can be derived.
 * @see VxResponseResult
 *
 */
class VX_DLLSPEC VxDynamicsResponse : public VxDynamicsContactInput
{
public:
    VxDynamicsResponse(VxUniverse *u);
    VxDynamicsResponse(const VxDynamicsResponse &r);
    virtual ~VxDynamicsResponse();

    virtual const char* getClassName() const { return "VxDynamicsResponse"; }

    VxDynamicsResponse* getFirst();
    VxDynamicsResponse* getNext();
    
    //Low level access:

    bool operator==(const VxDynamicsResponse &r) const;
    
protected:
    friend class VxFrame;
    friend class VxUniverse;
    
    VxUniverse *mVxUniverse;
   
private:

};

} // namespace Vx

#endif // VX_DYNAMICSRESPONSE_H
