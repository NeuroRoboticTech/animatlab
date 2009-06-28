
#ifndef _VXVEHICLE_H_
#define _VXVEHICLE_H_

#include "Vx/VxAssembly.h"

namespace Vx
{
class VxPart;
class VxCollisionGeometry;
class VxUniverse;
class VxGeometry;
class VxDynamicsResponse;
}

namespace VxVehicle
{

    class VxEngine;
    class VxPowerTrain;
    class VxTransmission;

//#define USE_COLL_SUB
//class CollisionSubscriberList;
class UpdateSubscriberList;


/**
  Vehicle base class.
  All Vortex vehicle classes are derived from this class.
*/
class VxVehicleBase
{
public:
    enum VehicleIDEnum { kChassisID, kWheelID, kVehicleIDEnumCount };

    VxVehicleBase(const char* name, Vx::VxUniverse *U, int matCount=kVehicleIDEnumCount);
    virtual ~VxVehicleBase();

    /** Returns the vehicle name. */
    const char* getName() const { return mAssembly.getName(); }

    /** Allows access to the internal assembly. */
    VxAssembly* getAssembly() { return &mAssembly; }

    void addAccessory(VxPart* p, VxConstraint* c);
    

    // creation of the vehicle
    virtual void setChassis(Vx::VxPart* part);
    virtual void createChassis(Vx::VxReal mass, const Vx::VxReal33Ptr inertia, Vx::VxReal3 comOffset);
    virtual void addCollisionGeometryToChassis(Vx::VxCollisionGeometry* cg);

    void createWheels(Vx::VxReal mass, int countPerSide, Vx::VxGeometry* g, 
                      Vx::VxReal3 pos[], Vx::VxReal radius, Vx::VxReal3Ptr wheelAxis);
    void setWheelParts(VxPart** parts, int countPerSide, VxReal radius, VxReal3Ptr wheelAxis);
    void setWheelsRelativeOrientation(Vx::VxReal3 e);
    void setWheelRelativeOrientation(int wheelIndex, Vx::VxReal3 e);
    void setWheelsRelativePosition(Vx::VxReal3 e);
    void setWheelMaxEdgeCosForEdgeContact(VxReal val);

    void setVehicleForwardAxis(Vx::VxReal3Ptr axis);
    void setVehicleUpAxis(Vx::VxReal3Ptr axis);
    void setWorldUpDirection(Vx::VxReal3Ptr axis);


    // run-time api
    virtual void reset();
    void saveState();
    virtual void preUpdate();
    virtual void postUpdate();
    virtual void transform(const Vx::VxReal44Ptr tm);
    

    // utility and accessors api
    void disableCollisionWith(Vx::VxPart* p);

    /** Returns the vehicle forward axis in local frame */
    inline const Vx::VxReal3& getVehicleForwardAxis()          const { return mVehicleForwardAxis; }
    /** Returns the vehicle forward direction in world frame */
    inline const Vx::VxReal3& getVehicleForwardDirection()     const { return mVehicleForwardDir; }
    /** Returns the vehicle up direction in world frame */
    inline const Vx::VxReal3& getVehicleUpDirection()          const { return mVehicleUpDir; }
    /** Returns the world up direction in world frame */
    inline const Vx::VxReal3& getWorldUpDirection()            const { return mWorldUpDir; }

    /** returns the vehicle general wheel axis (for unsteered wheel) in vehicle frame */
    inline const Vx::VxReal3& getWheelAxisDir()                const { return mWheelAxisDir; }
    /** returns the vehicle general wheel axis (for unsteered wheel) in world frame */
    inline const Vx::VxReal3& getWheelAxis()                   const { return mWheelAxis; }
    /** Returns the current rotation axis for sprcified wheel. */
    void getCurrentWheelAxis(int wheel, VxReal3 axis)          const;

    bool isOwnPart(Vx::VxPart* p);
    inline int convertID(int id)  const { return id - mChassisID; }

    void setChassisMaterialID(int id);
    void setWheelsMaterialID(int id);

    void setMaterialID(int partID, int matID);
    int getMaterialID(int partID) const;

    Vx::VxReal getLowestWheelGroundContact() const;
    Vx::VxReal getGroundNormalForceAtWheel(Vx::VxPart* wheel) const;

    int getWheelsInContact() const { return mWheelsInContact; }
    int getContactCountAtWheel(int w) const { return mContactPerWheel[w]; }
    Vx::VxReal getWheelRadius() const { return mWheelRadius; }
    int getWheelsPerSide() const { return mWheelsPerSide; }

    bool getCollectWheelContactForce() const { return mCollectWheelContactForce; }
    void setCollectWheelContactForce(bool b) { mCollectWheelContactForce = b; }

    Vx::VxReal getChassisForwardVelocity() const;
    virtual VxEngine* getEngine() = 0;
    virtual VxPowerTrain* getPowerTrain() = 0;
    virtual VxTransmission* getTransmission() = 0;

    void setBrakeTorque(Vx::VxReal torquel, Vx::VxReal torquer);
    void getBrakeTorque(Vx::VxReal* torquel, Vx::VxReal* torquer) const;

    bool contains(VxEntity* e) { return mAssembly.contains(e); }
    bool contains(VxConstraint* c) { return mAssembly.contains(c); }

    /** Class UpdateSubscriber. Use this class to add function to the vehicle that requires 
        to be update at each step. */
    class UpdateSubscriber
    {
    public:
        virtual ~UpdateSubscriber() {}
        virtual void update(VxReal dt) = 0;
        virtual void reset() = 0;
        virtual int getTypeID() const = 0;
        virtual int getPriority() const { return 0; }
    };

    void addUpdateSubscriber(UpdateSubscriber* sub);
    void removeUpdateSubscriber(UpdateSubscriber* sub);
    bool containUpdateSubscriber(UpdateSubscriber* sub) const;
    int getUpdateSubscriberCount() const;
    UpdateSubscriber* getUpdateSubscriber(int i);



    #ifdef USE_COLL_SUB
    // external collision subscribers utilities
    class CollisionSubscriber
    {
    public:
        CollisionSubscriber() {}
        virtual ~CollisionSubscriber() {}
        //virtual void notify(VxVehicleBase* vehicle, int part_index, VxPart* part_vehicle, VxUniverse::eIntersectEventType type, VxIntersectResult* ires, VxResponseResult* dres)=0;
    };

    void addCollisionSubscriber(CollisionSubscriber* sub);
    void removeCollisionSubscriber(CollisionSubscriber* sub);
    bool containCollisionSubscriber(CollisionSubscriber* sub) const;
    #endif

    virtual Vx::VxReal getGroundHeightAtPart(Vx::VxPart* p) const;
    void setGroundHeightAtWheel(VxPart* wheel, VxReal height);

    Vx::VxPart        *mChassisPart;
    Vx::VxPart        **mRoadWheel;         // pointers to wheels 

protected:

    Vx::VxReal3       mVehicleForwardAxis;  // vehicle forward axis in local frame
    Vx::VxReal3       mVehicleForwardDir;   // vehicle forward axis in world frame
    Vx::VxReal3       mWorldUpDir;          // world up dir
    Vx::VxReal3       mVehicleUpAxis;       // vehicle up dir in local frame
    Vx::VxReal3       mVehicleUpDir;        // vehicle up dir in world frame

    Vx::VxUniverse    *mUniverse;

    int               mWheelsPerSide;       // number of wheels per side
    Vx::VxReal        mWheelRadius;
    Vx::VxReal3       mWheelAxis;           // wheel rotation axis in local frame
    Vx::VxReal3       mWheelAxisDir;        // wheel rotation axis in world frame

    #ifdef USE_COLL_SUB
    LocalIntersectSubscriber mIntersectSub;
    #endif

    UpdateSubscriberList* mUpdateSubscriberList;


    void addPart(Vx::VxPart* p, int partID);
    void addConstraint(Vx::VxConstraint* c);
    inline int getChassisID()     const { return mChassisID+kChassisID; }
    inline int getWheelID()       const { return mChassisID+kWheelID; }

    Vx::VxReal        mBrakeLeft;               // left brake torque
    Vx::VxReal        mBrakeRight;              // right brake torque

//private:
    void addWheelContactNormalForce(Vx::VxDynamicsResponse* r, int i, Vx::VxPart* p, Vx::VxReal dir);

    Vx::VxReal *mLastWheelContactHeight;
    Vx::VxReal *mLastWheelGroundNormalForce;
    int    *mContactPerWheel;
    int    mWheelsInContact;
    bool   mCollectWheelContactForce;

    Vx::VxAssembly mAssembly;
    int        mChassisID;
    int        mIDCount;
    int        *mMaterialIDArray;

    #ifdef USE_COLL_SUB
    class LocalIntersectSubscriber : public Vx::VxUniverse::VxIntersectSubscriber
    {
    public:
        LocalIntersectSubscriber() : mSubscriberList(0) {};
        ~LocalIntersectSubscriber();

        void addCollisionSubscriber(CollisionSubscriber* sub);
        void removeCollisionSubscriber(CollisionSubscriber* sub);
        bool containCollisionSubscriber(CollisionSubscriber* sub) const;

        void preStep();

        virtual void notify(Vx::VxUniverse::eIntersectEventType type, Vx::VxIntersectResult* res);
        virtual void notify(Vx::VxUniverse::eIntersectEventType type, Vx::VxIntersectResult* ires, Vx::VxResponseResult* dres);

        VxVehicleBase* mVehicle;

        //CollisionSubscriberList* mSubscriberList;
    };
    #endif

};

} // namespace Vx

#endif // _VXVEHICLE_H_

