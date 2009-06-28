
#ifndef _VXTRACKEDVEHICLE_H_
#define _VXTRACKEDVEHICLE_H_

#include "VxVehicle.h"
#include "VxTrackedPowerTrain.h"
#include "Vx/VxAssembly.h"
#include "Vx/VxUniverse.h"


namespace Vx { 
class VxGeometry;
}

namespace VxVehicle
{



/**
  The VxTrackedVehicle encapsulates the physics and collision of
  a tracked vehicle consisting of a chassis, an engine (see VxPowerTrain),
  a number of road wheels
  with suspension specified by the configuration, two sprockets and
  two idler wheels. The wheels are appropriately constrained to
  simulate tracks. Extra parts between the front wheels and the sprockets 
  and between the rear wheels and the idlers are used to simulate track tension
  during collision with external objects.
*/
class VxTrackedVehicle : public VxVehicleBase
{
public:
    VxTrackedVehicle(const char* name, Vx::VxUniverse *U);
    virtual ~VxTrackedVehicle();
    
    enum TrackedVehicleIDEnum { kSprocketID=kVehicleIDEnumCount, kIdlerID, kFrontBoxID, kRearBoxID, TrackedVehicleIDEnumCount };
            
    // creation 
    void createSuspension(Vx::VxReal3 wpos[], Vx::VxReal3 watt[], 
                        Vx::VxReal3Ptr longAxis, Vx::VxReal3Ptr verticalAxis,
                        VxTable1D* stiffness, VxTable1D* damping, Vx::VxReal length);
    void setSuspensionStops(Vx::VxReal minAngle, Vx::VxReal maxAngle, Vx::VxReal stiff, Vx::VxReal damp);
    
    void createSprockets(Vx::VxReal3 pos[2], Vx::VxReal mass, Vx::VxGeometry* g, 
                        Vx::VxReal3Ptr axis, Vx::VxReal3Ptr axis2, Vx::VxReal radius);
    void createIdlers(Vx::VxReal3 pos[2], Vx::VxReal mass, Vx::VxGeometry* g, 
                        Vx::VxReal3Ptr axis, Vx::VxReal3Ptr axis2, Vx::VxReal radius);
    void createFrontTrackPart(Vx::VxReal mass, Vx::VxReal thick, Vx::VxReal width, Vx::VxReal stiff, Vx::VxReal damp);
    void createRearTrackPart(Vx::VxReal mass, Vx::VxReal thick, Vx::VxReal width, Vx::VxReal stiff, Vx::VxReal damp);
    
    // setting of damping and brake
    void setWheelDamping(Vx::VxReal l, Vx::VxReal r);
    void setSuspensionDampingScale(int i, Vx::VxReal scale);
    void setSuspensionStiffnessScale(int i, Vx::VxReal scale);
    Vx::VxReal getSuspensionDampingScale(int i) const { return mSuspensionDampingScale[i]; }
    Vx::VxReal getSuspensionStiffnessScale(int i) const { return mSuspensionStiffnessScale[i]; }

    // run-time api
    void reset();
    void preUpdate();
    void postUpdate();
    void transform(const Vx::VxReal44Ptr tm);


    // utility and accessors api
    void attachShaftToFrame(Vx::VxReal44Ptr shaft_rel_tm, Vx::VxReal3Ptr axis);
    virtual void disableCollisionWith(Vx::VxPart* p);

    Vx::VxReal getLeftSprocketTorque();
    Vx::VxReal getRightSprocketTorque();
    void setExtraTrackMaterialID(int id);

    /** Sets the track softness. */
    void setTrackSoftness(Vx::VxReal s) { mTrackSoftness = s; }

    /** Returns the current track softness. */
    Vx::VxReal getTrackSoftness() const { return mTrackSoftness; }

    Vx::VxReal getGroundHeightAtPart(Vx::VxPart* p) const;
    
    /** Returns the engine. */
    VxEngine* getEngine()             { return &mPowerTrain.mEngine; }
    /** Returns the power train. */
    VxPowerTrain* getPowerTrain()     { return &mPowerTrain; }
    /** Returns the transmission. */
    VxTransmission* getTransmission() { return &mPowerTrain.mTransmission; }


    // direct access to the most of components instead on hundreds of accessors...
    VxTrackedPowerTrain mPowerTrain;
    
    Vx::VxConstraint  **mRoadArm;               // 
    Vx::VxConstraint  *mSprocketHinge[2];       // 
    Vx::VxConstraint  *mSprocketWheelGear[2];   //
    Vx::VxPart        *mSprocketPart[2];        // 
    Vx::VxPart        *mFrontTrackPart[2];      // 
    Vx::VxPart        *mRearTrackPart[2];       // 
    
protected:

    Vx::VxConstraint  *mIdlerHinge[2];          // 
    Vx::VxPart        *mIdlerPart[2];           //
    Vx::VxConstraint  *mFrontrackHinge[2];      // 
    Vx::VxConstraint  *mRearTrackHinge[2];      // 

    VxTable1D     mSuspensionStiffnessTable;// table of stiffnes vs limit position
    VxTable1D     mSuspensionDampingTable;  // table of damping vs limit position
    
    Vx::VxReal*       mSuspensionStiffnessScale;// extra scaling factor added to the suspension stiffness
    Vx::VxReal*       mSuspensionDampingScale;  // extra scaling factor added to the suspension damping

    Vx::VxReal3       mSprocketAxis;            // sprocket rotation axis in local frame
    Vx::VxReal3       mIdlerAxis;               // idler rotation axis in local frame
    
    Vx::VxReal        mSprocketRadius;
    Vx::VxReal        mIdlerRadius;

    Vx::VxReal        mTrackSoftness;

//private:
    inline int getSprocketID()    const { return mChassisID+kSprocketID; }
    inline int getIdlerID()       const { return mChassisID+kIdlerID; }
    inline int getFrontBoxesID()  const { return mChassisID+kFrontBoxID; }
    inline int getRearBoxesID()   const { return mChassisID+kRearBoxID; }
};

} // namespace Vx

#endif // _VXTRACKEDVEHICLE_H_

