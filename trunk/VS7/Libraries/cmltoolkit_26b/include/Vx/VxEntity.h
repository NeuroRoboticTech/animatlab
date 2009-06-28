#if !defined(VX_ENTITY_H)
#define VX_ENTITY_H

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

// VxEntity.h: interface for the VxEntity class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/Vx.h"

namespace Vx
{

class VxCollisionGeometry;
class VxConstraint;
class VxAssembly;

struct VxID2Entity;

// tolua_begin
/**
 * The VxEntity class is responsible for communicating with 
 * nodes in a scene graph. 
 * A node may be set, thus associating this entity with a graphics object,
 * and taking its initial position from it.
 * It is the base class for all simulation objects in Vx.
 * Facilities are provided to position the entity,
 * follow another entity.
 * A VxEntity cannot be instantiated, rather the user creates VxParts or
 * VxSensors.
 * @see VxPart, VxSensor
 */
class VX_DLLSPEC VxEntity: 
public VxBase                                                               
{
    friend class VxUniverse;
    
public:
    virtual ~VxEntity();

    virtual void setName(const char* name) { setNameBase(name, &sNameMap); }
    static VxEntity* findFromName(const char* name) { return sNameMap ? (VxEntity*)VxBase::findFromName(name, sNameMap) : 0; }
    
    bool setNode(VxNode node = 0);
    VxNode getNode();
    void updateNode();
    void updateToNode();
    void updateFromNode();
    void updateFollowers();
    void preStepUpdateFromNode(VxReal44Ptr tm);
    static VxEntity* getEntityFromNode(VxNode node);

    virtual void freeze(bool ) {}
    
    // this can follow a given entity/geometry only if not dynamics
    // Adds an entity to follow with optionnal offsets such as tm = offset1 * Leader->tm * offset2.
    bool followEntity(VxEntity *pVxEntity, VxReal44Ptr pMatrixOffset1=0, VxReal44Ptr pMatrixOffset2=0);
    // Adds an geometry to follow, kicks out current Leader if any
    void followCollisionGeometry(VxCollisionGeometry *pVxCollisionGeometry);
    VxEntity* getLeader() { return mLeader; }
    const VxEntity* getLeader() const { return mLeader; }
    
    virtual int getConstraintCount() const { return 0; }
    virtual VxConstraint *getConstraint(int jointID) const { return 0; }

    // The universe the entity belongs to.
    inline VxUniverse* getUniverse() { return mUniverse; }
    
    inline VxReal44Ptr getTransformPtr() const {return mEntityTM;}
    void getTransform(VxReal44Ptr tm) const;
    void getTransformLocal(VxReal44Ptr localTM) const;
    virtual void setTransform(const VxReal44Ptr tm);
    virtual void setTransform(const VxReal44Ptr tm, VxReal dt) { setTransform(tm); }
    virtual void updateTransform() {}

    void getPosition(VxReal3Ptr pos) const;
    void setPosition(VxReal3 pos);
    void setPosition(VxReal x, VxReal y, VxReal z);

    void getOrientationQuaternion(VxReal4 q) const;
    void setOrientationQuaternion(VxReal4 q);
    void setOrientationQuaternion(VxReal w, VxReal x, VxReal y, VxReal z);

    void setAbsoluteScale(VxReal3Ptr scale);
    void getAbsoluteScale(VxReal3Ptr scale) const;
    
    void setRelativeToParentTM(VxReal44Ptr relTM);
    inline VxReal44Ptr getRelativeToParentTransformPtr() const {
        return relToParentTM;
    }

    // Get a entity that follows.
    virtual VxEntity * getFollower(int entityID) const;
    int getFollowerID(VxEntity *entity) const;

    // The number of entities that follow.
    int getFollowerCount() const;

    // vpVxtraversal still need some internal methods...
    void setParent(VxEntity *parent) { _setParent(parent); } 
    VxEntity * getParent() { return _getParent(); } 
    void setIsLastKinematic(bool b) { _setIsLastKinematic(b); }
    bool getIsLastKinematic() const { return _getIsLastKinematic(); }
    
    virtual int getNewResponseID() = 0;
    virtual int getResponseID() const = 0;
    virtual bool setResponseID(int newID) = 0;

    virtual int getCollisionCount() const { return 0; }

    /** Query if a part is dynamic.. */
    virtual bool isDynamic() const =0;
    /** Query if a part has been made dynamic, independently of its frozen attribute. */
    virtual bool hasDynamicFlag() const =0;
    /** Query if a part is kinematic.. */
    virtual bool isKinematic() const =0;
    /** Query if a part is a sensor without dynamics response. */
    virtual bool isSensor() const =0;
    /** Query if a part is frozen, which means it wont be moved. */
    virtual bool isFrozen() const =0;

    /** Query if the entity is a part. */
    virtual bool isPart() const { return false; }

    virtual void getBoundingBox(VxReal3Ptr minCorner, VxReal3Ptr maxCorner) const = 0;
    virtual void getBoundingSphere(VxReal3Ptr center, VxReal *radius) const = 0;

    /** returns true if the entity needs to be updated from scenegraph during the prestep. */
    //bool isUpdatedFromSceneGraph() const { return !isFrozen() && (isKinematic() || isSensor()); }
    bool isUpdatedFromSceneGraph() const { return !getLeader() && (isKinematic() || isSensor()); }

    VxAssembly* getAssembly() { return mAssembly; }

    // tolua_end
protected:
    friend class VxAssembly;

    VxEntity();

    // Graphics TM is relative to the parent.
    void _setParent(VxEntity *parent);
    inline VxEntity * _getParent() const { return mParent; }

    virtual void _cleanDependencies();
    
    virtual void _removeFromUniverse() { _setUniverse(0); }
    virtual void _addToUniverse(VxUniverse *pUniverse) { _setUniverse(pUniverse); }
    
    virtual void _setTransformPtr(VxReal44Ptr tm);
    void _setIsLastKinematic(bool);
    bool _getIsLastKinematic() const {return mLastKinematic;}

    VxNode              mNode;
    VxUniverse         *mUniverse;
    VxEntity           *mParent;
    VxID2Entity        *mFollowerEntitiesArr;
    
    VxReal44Ptr        mEntityTM;
    VxReal44Ptr        relToParentTM;
    
    VxEntity           *mLeader;
    VxReal44           mLeaderOffset1;
    VxReal44           mLeaderOffset2;
    
    VxReal3            mReferenceScale;
    bool               mLastKinematic;

    VxAssembly         *mAssembly;
    
private:
    static VxNameMap*  sNameMap;
    VxReal44           mEntityTMLocal;
    void _setUniverse(VxUniverse *pUniverse);
}; // tolua_export

} //namespace Vx

#endif //VX_ENTRY_H
