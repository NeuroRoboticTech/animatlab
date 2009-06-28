
#ifndef VXDEFAULTSTEERINGSUB_H_
#define VXDEFAULTSTEERINGSUB_H_

#include "VxWheeledVehicle.h"

namespace VxVehicle
{

    static const int VxDefaultSteeringSubID = 1; // tmp
    static const int VxDefaultSteeringSubPriority = 10; // tmp


/**
 *  Class VxDefaultSteeringSub: Implements default steering behavious for the 
 *  VXWheeledVehicle. One or more pair can be steered. The class simulates
 *  a range of steering behaviour going from Ackermann to reverse Ackermann.
 */
class VxDefaultSteeringSub : public VxVehicleBase::UpdateSubscriber
{
public:

    VxDefaultSteeringSub();
    ~VxDefaultSteeringSub();

    int getTypeID() const { return VxDefaultSteeringSubID; }
    int getPriority() const { return VxDefaultSteeringSubPriority; }

    void init(VxWheeledVehicle* veh);

    void reset();

    void setPairWheelSteered(int pair, bool activate);
    bool isPairWheelSteered(int pair) const;

    void setReferencePosition(Vx::VxReal pos);
    Vx::VxReal getReferencePosition() const { return mReferencePosition; }
    void computeReferencePosition();

    void setDesiredAngle(Vx::VxReal angle);
    Vx::VxReal getDesiredAngle() const { return mDesiredAngle; }

    void setMaxVelocity(Vx::VxReal vel);
    Vx::VxReal getMaxVelocity() const { return mMaxVelocity; }

    void setMaxAngle(Vx::VxReal angle);
    Vx::VxReal getMaxAngle() const { return mMaxAngle; }

    void getWheelAngle(int parIndex, VxReal* left, VxReal right);

    void setAckerMann(VxReal ackermann);
    Vx::VxReal getAckerMann() const { return mAckerMann; }

    void update(Vx::VxReal dt);

protected:
    Vx::VxReal mReferencePosition;
    Vx::VxReal mMaxPositionFromRef;
    Vx::VxReal mMaxAngle;
    Vx::VxReal mCurrentAngle;
    Vx::VxReal mDesiredAngle;
    Vx::VxReal mMaxVelocity;
    Vx::VxReal mVehicleHalfWidth;

    /* @internal data holder class */
    class WheelPairData
    {
    public:
        WheelPairData::WheelPairData() 
            : mDamping(0), mLongitudinalPosition(0), mSteerRatio(1.0f), 
              mSteered(false) {}
        VxReal mDamping;
        VxReal mLongitudinalPosition;
        VxReal mSteerRatio;
        VxReal mRadius;
        bool mSteered;
    };

    WheelPairData*    getWheelPairData() { return mWheelPairData; }
    WheelPairData*    mWheelPairData;
    VxReal            mAckerMann;

    VxWheeledVehicle* mVehicle;
};

} // namespace VxVehicle


#endif // VXDEFAULTSTEERINGSUB_H_
