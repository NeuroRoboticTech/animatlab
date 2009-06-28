
#ifndef _VXWHEELEDVEHICLE_H_
#define _VXWHEELEDVEHICLE_H_

#include "VxVehicle.h"
#include "VxWheeledPowerTrain.h"
#include "Vx/VxAssembly.h"
#include "Vx/VxUniverse.h"


namespace Vx 
{ 
class VxGeometry;
class CollisionSubscriber;
class VxDynamicsResponse;
class VxCarWheel;
class VxGearRatio;
}

namespace VxVehicle
{


#define VxWheeledVehicle_MAX_PAIR_COUNT 10


/**
  The VxWheeledVehicle encapsulates the physics and collision of
  a wheeled vehicle consisting of a chassis, an engine (see VxPowerTrain),
  a number of road wheels. The wheels are attached to the chassis using 
  VxCarWheel joint. The wheel pairs are number from 0 to getWheelsPerSide(),
  0 being the rearmost pair.
  Powered and steered wheel paired are specified by the user.
*/
class VxWheeledVehicle : public VxVehicleBase
{
public:
    VxWheeledVehicle(const char* name, Vx::VxUniverse *U, int additionalID=0);
    virtual ~VxWheeledVehicle();
                
    void createSuspension(Vx::VxReal3 watt[], VxReal3Ptr suspensionAxis,
                        VxTable1D* stiffness, VxTable1D* damping);
    void setSuspensionStops(Vx::VxReal minPos, Vx::VxReal maxPos, Vx::VxReal stiff, Vx::VxReal damp);
        
    void attachShaftToFrame(Vx::VxReal44Ptr shaft_rel_tm, Vx::VxReal3Ptr axis);

    // setting of damping and brake
    void setWheelsDamping(Vx::VxReal damping);
    void setWheelDamping(int index, Vx::VxReal damping);
    void setSuspensionDampingScale(int i, Vx::VxReal scale);
    void setSuspensionStiffnessScale(int i, Vx::VxReal scale);
    Vx::VxReal getSuspensionDampingScale(int i) const { return mWheelData[i].mSuspensionDampingScale; }
    Vx::VxReal getSuspensionStiffnessScale(int i) const { return mWheelData[i].mSuspensionStiffnessScale; }

    // steered and powered wheels setting api
    void setPairWheelPowered(int pair, bool activate);
    bool isPairWheelPowered(int pair) const;

    void setWheelSteeringAngle(int wheel, Vx::VxReal angle);
    Vx::VxReal getWheelSteeringAngle(int wheel) const;

    //void setWheelPairRadius(int pair, Vx::VxReal radius);
    //Vx::VxReal getWheelPairRadius(int pair) const;

    VxReal getSuspensionPosition(int i);

    // run-time api
    void reset();
    void preUpdate();
    void postUpdate();
    void transform(const Vx::VxReal44Ptr tm);


    // utility and accessors api
    virtual void disableCollisionWith(VxPart* p);

    VxEngine* getEngine()             { return &mPowerTrain.mEngine; }
    VxPowerTrain* getPowerTrain()     { return &mPowerTrain; }
    VxTransmission* getTransmission() { return &mPowerTrain.mTransmission; }

    void setWheelsGearRatioMaxTorque(Vx::VxReal mt);

    /** Returns average angular velocity between left and right wheels. */
    VxReal getWheelsAverageVelocity() const { return mPowerTrain.mTransmission.getSprocketAverageVelocity(); }

    // direct access to the main components...
    VxWheeledPowerTrain mPowerTrain;

protected:

    /** @internal data holder class */
    class WheelData
    {
    public:
        WheelData() 
            : mCarWheelJoint(0), mWheelGearRatio(0), 
            mSuspensionStiffnessScale(1), mSuspensionDampingScale(1), 
            mWheelDamping(0) {}

        Vx::VxCarWheel*  mCarWheelJoint;
        Vx::VxGearRatio* mWheelGearRatio;

        Vx::VxReal       mSuspensionStiffnessScale;// extra scaling factor added to the suspension stiffness
        Vx::VxReal       mSuspensionDampingScale;  // extra scaling factor added to the suspension damping
        Vx::VxReal       mWheelDamping;            // individual damping factor applied to the wheels
    };

    void preUpdateWheelsDamping();
    void preUpdateSuspension();

    VxTable1D         mSuspensionStiffnessTable;// table of stiffnes vs limit position
    VxTable1D         mSuspensionDampingTable;  // table of damping vs limit position

    WheelData* mWheelData;
    bool *mPairPowered;
    int mMainPoweredWheelPair;
    Vx::VxReal mGearRatioMaxTorque;

private:
    void setGearRatio(Vx::VxGearRatio* gr, int w0, int w1, Vx::VxReal ratio);
    
};

} // namespace VxVehicle

#endif // _VXWHEELEDVEHICLE_H_

