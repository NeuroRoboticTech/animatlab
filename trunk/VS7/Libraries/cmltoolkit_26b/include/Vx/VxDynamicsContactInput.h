#if !defined(VX_DYNAMICSCONTACTINPUT_H)
#define VX_DYNAMICSCONTACTINPUT_H

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


// VxDynamicsContactInput.h: interface for the VxDynamicsContactInput class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/VxContactProperties.h"
#include "Vx/VxResponseResult.h"

struct MdtBaseConstraint;
struct McdIntersectResult;

namespace Vx
{

class VxGeometry;
class VxPart;
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
 * For usage, 
 * @see VxUniverse, VxMaterialPair
 *
 * For collision response types
 * @see VxIntersectResult
 *
 * Other response types can be derived.
 * @see VxResponseResult
 *
 */
class VX_DLLSPEC VxDynamicsContactInput : public VxResponseResult, public VxContactProperties
{
public:
    virtual const char* getClassName() const { return "VxDynamicsContactInput"; }
    int getType();

    void getCollisionGeometryPair(VxPart **part0, VxPart **part1) const;
    void setCollisionGeometryPair(VxPart *part0, VxPart *part1);
    void getContactPosition(VxReal3Ptr pos) const;
    void setContactPosition(VxReal3Ptr pos);
    void getContactNormal(VxReal3Ptr normal) const;
    void setContactNormal(VxReal3Ptr normal);
    VxReal getContactPenetration() const;
    void setContactPenetration(VxReal penetration);
    void getContactDirection(VxReal3Ptr v) const;
    void setContactDirection(VxReal3Ptr v);
    void getContactForce(int partID, VxReal3Ptr force) const;
    void getContactTorque(int partID, VxReal3Ptr torque) const;
    bool enableContact(bool enable);
    bool isEnabled() const;
    void setContactUserData(void* data);
    void* getContactUserData();

    VxDynamicsContactInput* getFirst();
    VxDynamicsContactInput* getNext();

    //Low level access:
    MdtBaseConstraint *&getMdtContacts() {return mContactFirst;}
    MdtBaseConstraint *&getCurrentMdtContact(){return mContactNext;}
    
    VxDynamicsContactInput(MdtBaseConstraint *contact);
//    VxDynamicsContactInput(const VxDynamicsContactInput& copy) : VxResponseResult(copy) {};
    virtual ~VxDynamicsContactInput();

    bool operator==(const VxDynamicsContactInput &r) const;

protected:
    friend class VxFrame;
    friend class VxUniverse;

private:

};

} // namespace Vx

#endif // VX_DYNAMICSCONTACTINPUT_H
