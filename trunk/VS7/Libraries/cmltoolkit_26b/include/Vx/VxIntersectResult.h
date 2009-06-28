#if !defined(VX_INTERSECTRESULT_H)
#define VX_INTERSECTRESULT_H

/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/02/21 16:01:20 $
   
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


// VxIntersectResult.h: interface for the VxIntersectResult class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/Vx.h"

struct MdtBaseConstraint;
struct McdIntersectResult;
struct McdModelPair;

namespace Vx
{

class VxUniverse;
class VxCollisionGeometry;
class VxResponseResult;
class VxPart;
class VxEntity;

/**
 * Collision information to be read in a callback. 
 * An object of this class is passed to the user in callbacks
 * registers for. It provides access to the pair's properties at the
 * time of contact.
 * @see VxUniverse for setting callbacks.
 *
 */
class VX_DLLSPEC VxIntersectResult: public VxBase
{
    
    friend class VxFrame;
    friend class VxUniverse;
    
public:
    VxIntersectResult(McdIntersectResult* intersectResult): mIntersectResult(intersectResult), mOwnedIntersectResult(0) {};
    VxIntersectResult(int contactMaxCount);
    virtual ~VxIntersectResult();
    
    virtual const char* getClassName() const { return "VxIntersectResult"; }
    
    void getEntityPair(VxEntity **e1, VxEntity **e2);
    void getCollisionGeometryPair(VxPart **p1, VxPart **p2);
    void getCollisionGeometryPair(VxCollisionGeometry **geometry1, VxCollisionGeometry **geometry2);
    int getContactMaxCount() const;
    int getContactCount() const;
    bool isPairTouching() const;
    void getContactsNormal(VxReal3Ptr normal) const;
    
    void getContactNormal(int i, VxReal3Ptr normal) const;
    void getContactPosition(int i, VxReal3Ptr position) const;
    VxReal getContactPenetration(int i) const;
    void getContactDimensions(int i, short *dim1, short* dim2) const;
    void getContactIndices(int i, int *tag1, int *tag2) const;
    
    //Low level access:
    McdIntersectResult *&getMcdIntersectResult() {return mIntersectResult;};
    
protected:
    McdIntersectResult *mIntersectResult;
    McdIntersectResult *mOwnedIntersectResult;
    
};

} // namespace Vx

#endif
