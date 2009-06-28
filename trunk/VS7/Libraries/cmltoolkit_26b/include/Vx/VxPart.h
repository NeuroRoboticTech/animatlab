#if !defined(VX_PART_H)
#define VX_PART_H

/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/02/22 23:24:37 $
   
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

// VxPart.h: interface for the VxPart class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/VxSensor.h"

struct MdtBody;

namespace Vx
{

struct VxID2CONSTRAINT;

// tolua_begin
/**
 * A part corresponds to a single moving object in a mechanism. It can be either
 * kinematic or dynamic.
 * In either case it is possible to freeze the part.
 * For kinematic parts, this indicates the part is static and will not
 * be moved by the user.
 * Frozen dynamic parts will not be moved by the dynamics.
 * To move a frozen part, the user should unfreeze it first.
 */
class VX_DLLSPEC VxPart : public VxSensor
{
    friend class VxConstraint;
    
public:
    VxPart();
    VxPart(VxReal mass);
    virtual ~VxPart();

    virtual const char* getClassName() const { return "VxPart"; }
    
    void resetDynamics();

    virtual void addCollisionGeometry(VxCollisionGeometry* pVxCollision);
    virtual bool removeCollision(VxCollisionGeometry* pVxCollision);
    
    virtual void setLinearVelocity(VxReal3 lv);
    virtual void setLinearVelocity(VxReal lv1, VxReal lv2, VxReal lv3 );
    virtual void getLinearVelocity(VxReal3Ptr lv) const;
    virtual void setAngularVelocity(VxReal3 av);
    virtual void setAngularVelocity(VxReal av1, VxReal av2, VxReal av3 );
    virtual void getAngularVelocity(VxReal3Ptr av) const;
    
    bool   getMassPropertiesLocal(VxReal *mass, VxReal33Ptr inertiaTensor, VxReal3Ptr COM) const;
    bool   getMassPropertiesAbsolute(VxReal *mass, VxReal33Ptr inertiaTensor, VxReal3Ptr COM) const;
    void   getInertia(VxReal33Ptr inertiaTensor) const;
    void   getOrthogonalInertia(VxReal33Ptr inertiaTensor, VxReal44Ptr tm) const;
    void   getInertiaAbsolute(VxReal33Ptr inertiaTensor) const;
    void   getCOMOffset(VxReal3 v) const;
    void   getCOMPosition(VxReal3 v) const;
    
    
    VxReal getMass() const;
    
    void   setMassAndInertia(VxReal mass, VxReal33Ptr inertiaTensor);
    void   setCOMPosition(VxReal3 v);
    void   setCOMOffset(VxReal3 v);
    void   setCOMOffset(VxReal x, VxReal y, VxReal z);
    void   setMassPropertiesFromGeometry(VxCollisionGeometry* pVxCollision);
    void   setInertiaAndCOMFromGeometry(VxCollisionGeometry* geom);
    void   setMass(VxReal mass);
    void   setMassFromPart(const VxPart* src);
    
    void   resetMassPropertiesFromCollisionGeometry();
    void   lockMassProperties() { mMassLocked = mCOMLocked = mInertiaLocked = true; }
    
    void   unlockMass();
    void   unlockCOM();
    void   unlockInertia();
    
    void   setLinearVelocityDamping(VxReal damping);
    void   setAngularVelocityDamping(VxReal damping);
    VxReal getLinearVelocityDamping() const;
    VxReal getAngularVelocityDamping() const;
    void   getVelocityPointers(VxReal3Ptr* linear, VxReal3Ptr* angular);
    VxReal3Ptr getLinearVelocityPtr() const;
    VxReal3Ptr getAngularVelocityPtr() const;
    
    void   getForce(VxReal3 v) const;
    void   getTorque(VxReal3 v) const;
    VxReal getKineticEnergy() const;
    void   getVelocityAtPoint(const VxReal3Ptr point, VxReal3 lv) const;
    void   getLinearAcceleration( VxReal3 la) const;
    void   getAngularAcceleration(  VxReal3 aa) const;
    void   addForceAtPosition(VxReal3 f, VxReal3 p);
    void   addForce(VxReal3 f);
    void   addTorque(VxReal3 t);
    void   addImpulse(VxReal3 impulse);
    void   addImpulseAtPosition(VxReal3 impulse, VxReal3 p);
    
    // Make Part not moveable. e.g. Terrain.
    virtual void freeze( bool freeze = true);
    
    // Disable or enable part dynamics.
    void wakeDynamics(bool enable = true);
    bool isDynamicsAwake() const;
    
    // Create a dynamic body for the part.
    bool makeDynamic();
    bool makeKinematic();
    
    // Number of joints the Entry is attached to.
    int getConstraintCount() const;
    // Get the joint by the index.
    VxConstraint *getConstraint(int id) const;
    
    virtual void setTransform(const VxReal44Ptr tm);
    virtual void setTransform(const VxReal44Ptr tm, VxReal dt);
    
    /** Query if a part is dynamic. @warning a frozen object cannot be dynamic*/
    virtual bool isDynamic() const;
    /** Query if a part has been made dynamic, independently of its frozen attribute. */
    virtual bool hasDynamicFlag() const { return mHasDynamicFlag; }
    /** Query if a part is kinematic. */
    virtual bool isKinematic() const { return !isDynamic(); }
    /** Query if a part is a sensor without dynamics response. */
    virtual bool isSensor() const { return false; }

    virtual bool isPart() const { return true; }

    void noFastSpinAxis();
    void setFastSpinAxis(VxReal3 v);
    void setFastSpinAxis(VxReal x, VxReal y, VxReal z);
    bool getFastSpinAxis(VxReal3 v);
    
    // tolua_end
    
    /** Returns MdtBody */
    inline MdtBody *& getMdtBody(int item = 0) { return mMdtBodyID; }
    
    void _updateMass();
    
protected:
    VxID2CONSTRAINT *constraintsArr;
    // Internal use:
    void _init();
    void _addConstraint(VxConstraint *j);
    void _removeConstraint(VxConstraint *j);
    virtual void _cleanDependencies();

    virtual void _removeFromUniverse();
    virtual void _addToUniverse(VxUniverse *pUniverse);

    MdtBody *mMdtBodyID;

    bool mDynamicsAwake;

    void _setMass(VxReal mass);
    void _setInertia(const VxReal33Ptr inertia);
    void _setCOMRelativeTM(const VxReal44Ptr tm);
    void _checkInertia();

    // those are needed until kin and dyn part are separated
    VxReal   mMass;
    VxReal44 mCOMRelativeTM;
    VxReal33 mInertiaTensor;

    bool mMassLocked;
    bool mCOMLocked;
    bool mInertiaLocked;

    bool mHasDynamicFlag;

private:

}; // tolua_export

} // namespace Vx

#endif //VX_PART_H 
