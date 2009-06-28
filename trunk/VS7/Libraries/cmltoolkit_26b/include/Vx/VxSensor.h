#if !defined(VX_SENSOR_H)
#define VX_SENSOR_H

/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/05/04 21:17:32 $
   
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

// VxSensor.h: interface for the VxSensor class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/VxEntity.h"

namespace Vx
{

class VxCollisionGeometry;
class VxUniverse;
class VxGeometry;
struct VxID2COLLISION;

// tolua_begin
/**
 * Sensor corresponds to a kinematically moving geometry
 * that can be used to flag collision by the user.
 * It has no effect on the dynamics of other parts (does not
 * generate contacts).
 */
class VX_DLLSPEC VxSensor : public VxEntity
{
    friend class VxUniverse;
    
    
public:
    VxSensor();
    virtual ~VxSensor();

    virtual const char* getClassName() const { return "VxSensor"; }
    
    // Add a collision to the Sensor.
    virtual void addCollisionGeometry(VxCollisionGeometry* pVxCollision);

    virtual void addGeometry(VxGeometry* pVxGeometry, int materialID, VxReal44 relativeTransform = 0,  VxReal relativeDensity = 1);
    virtual void addGeometry(VxGeometry* pVxGeometry, int materialID, VxReal3 relPosition, VxReal3 relOrientationXYZEulerAngles,  VxReal relativeDensity = 1);
    
    // Remove a collision from the Sensor.
    virtual void removeCollision(int collisionID);
    virtual bool removeCollision(VxCollisionGeometry* pVxCollision);
    // Get a Sensor collision by ID.
    VxCollisionGeometry *getCollision(int collisionID)const;
    // Number of collisions.
    int getCollisionCount() const;

    virtual void setName(const char* name);

    // Disable/enable collision with another part.
    void disablePairIntersect(VxSensor *pVxSensor);
    void enablePairIntersect(VxSensor *pVxSensor);

    void getGeometryMassProperties(VxReal *mass, VxReal33Ptr inertiaTensor,
        VxReal3Ptr COM, VxReal *volume);
    virtual void setLinearVelocity(VxReal lv1,  VxReal lv2, VxReal lv3 );
    virtual void getLinearVelocity(VxReal *lv1, VxReal *lv2, VxReal *lv3) const;
    virtual void setAngularVelocity(VxReal av1, VxReal av2, VxReal av3 );
    virtual void getAngularVelocity(VxReal *av1, VxReal *av2, VxReal *av3) const;
    
    // Make Entry not moveable. e.g. Terrain.
    virtual void freeze( bool freeze);
    
    /** Returns true if entity has been frozen, false otherwise. */
    bool isFrozen() const { return mFrozen; }

    /** Query if a part is dynamic.. */
    virtual bool isDynamic() const { return false; }
    /** Query if a part has been made dynamic, independently of its frozen attribute. */
    virtual bool hasDynamicFlag() const { return false; }
    /** Query if a part is kinematic.. */
    virtual bool isKinematic() const { return false; }
    /** Query if a part is a sensor without dynamics response. */
    virtual bool isSensor() const { return true; }
    
    virtual void setTransform(const VxReal44Ptr tm);
    virtual void _setTransformPtr(VxReal44Ptr tm);

    virtual void updateTransform();

    void getBoundingBox(VxReal3Ptr minCorner, VxReal3Ptr maxCorner) const;
    void getBoundingSphere(VxReal3Ptr center, VxReal *radius) const;
    
    int getNewResponseID() { return getNewResponseID(this); }
    int getResponseID() const { return mResponseID; }
    bool setResponseID(int newID);


    // tolua_end
protected:
    static int getNewResponseID(VxSensor* entity);

    virtual void _removeFromUniverse();
    virtual void _addToUniverse(VxUniverse *pUniverse);
    
    VxID2COLLISION *collisionsArr;
    // Should be moved to a new struct.
    VxReal3     mLinearVelocity;
    VxReal3     mAngularVelocity;
    
    bool        mFrozen;
    int         mResponseID;
    
private:
    
}; // tolua_export

} // namespace Vx

#endif // VX_SENSOR_H
