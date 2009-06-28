
#ifndef _VX_ACDSTRANSMISSION_H_
#define _VX_ACDSTRANSMISSION_H_

#include "VxTransmission.h"
#include "VxTable.h"
#include "Vx/VxPrecision.h"


// tolua_begin

/** automatic controlled differential transmission configuration */
#define MAX_GEARS 10


// tolua_end

namespace Vx
{
class VxDifferentialJoint;
class VxGearRatio;
class VxPart;
class VxUniverse;
}

namespace VxVehicle
{

class VxEngine;

/**
 * VxAutomaticNGearTransmission class
 * This class simulates a N gears Automatic transmission, N<MAX_GEARS being the number of gear 
 * ratios and includes 0 or 1 reverse, a neutral and one or more forward gear ordered as: 
 * R, N, F1, F2, F3, ... .
 * Park is not a gear state and sould be simulated, for now at least, 
 * by setting the gear to neutral and applying brake. Park functionality might come later.
 */
class VxAutomaticNGearTransmission : public VxTransmission
{
public:

    VxAutomaticNGearTransmission();
    virtual ~VxAutomaticNGearTransmission();

    void init(Vx::VxUniverse *u);
    void reset();
    void setEngineAndSprocket(VxEngine* engine, Vx::VxPart* leftSprocket, Vx::VxPart* rightSprocket, 
                                const Vx::VxReal3Ptr sprocketAxis);

    void setCtrTable(int size, Vx::VxReal* wratio, Vx::VxReal* ctr);

    void setQuickShiftParameters(Vx::VxReal minThrottle, Vx::VxReal dthrottle, Vx::VxReal dt);
    void setShiftDelay(Vx::VxReal d);
    void setFinalGearRatio(Vx::VxReal g);

    void setGearCountAndRatios(int gearCount, Vx::VxReal* gearRatio);
    void setForwardGearChangeRpm(Vx::VxReal *up, Vx::VxReal *qup, Vx::VxReal *down, Vx::VxReal *qdown);

    void setDifferentialMaxTorque(Vx::VxReal mt);

    void setTransmissionDamping(Vx::VxReal damp, Vx::VxReal dampExp);
    void setFinalDriveDamping(Vx::VxReal damp, Vx::VxReal dampExp);

    void preUpdate();
    void postUpdate();

    bool setShifterPosition(int ming, int maxg);
    void setTC(Vx::VxReal inTC);
    Vx::VxReal getCsr() const { return mCsr; }

    /** returns the average of the sprockets angular velocities */
    inline Vx::VxReal getSprocketAverageVelocity() const 
                    { return 0.5f * (mRightSprocketVel + mLeftSprocketVel); }

    int getGearState() const { return mGearState; }
    int getNeutralGear() const { return mNeutralGear; }
    Vx::VxReal getCurrentGearRatio() const { return mCurrentGearRatio; }
    int getGearCount() const { return mGearCount; }

    void getShifterPosition(int *ming, int *maxg) const { *ming = mShifterMinG; *maxg = mShifterMaxG; }

    /** return the left sprocket angular velocity. */
    Vx::VxReal getLeftSprocketVel() const { return mLeftSprocketVel; }
    /** return the right sprocket angular velocity. */
    Vx::VxReal getRightSprocketVel() const { return mRightSprocketVel; }

    VxPart* getLeftSprocket() { return mLeftSprocket; }
    VxPart* getLeftSprocket() const { return mLeftSprocket; }
    VxPart* getRightSprocket() { return mRightSprocket; }
    VxPart* getRightSprocket() const { return mRightSprocket; }

    void getTrainDamping(Vx::VxReal *leftTrainDamp, Vx::VxReal *rightTrainDamp) const;
    VxDifferentialJoint* mDifferentialGear;
    VxGearRatio* mDifferentialLockGear;

    void setDifferentialLockTorque(Vx::VxReal torque);
    Vx::VxReal getDifferentialLockTorque() const;

protected:

    VxPart* mLeftSprocket;
    VxPart* mRightSprocket;

    Vx::VxTable1D mCtrTable;

    Vx::VxReal mCurrentInterShiftDelay; // remaining time transmission is forced to neutral
    Vx::VxReal mShiftAgainDelay;        // remaining time before transmission tries to change gear again
    Vx::VxReal mInitShiftAgainDelay;    // time transmission goes to neutral between shifts

    Vx::VxReal mLeftSprocketVel;
    Vx::VxReal mRightSprocketVel;

    void setGear(int gear);

    void checkForNextGear(Vx::VxReal upRpm);
    void checkForPreviousGear(Vx::VxReal downRpm, bool quickShift, Vx::VxReal quickDownRpm);
    void setGearBetween(int lo, int hi);
    void checkForTransmissionShift();
    void adjustInternalDamping();
    void setDifferentialAndTorqueConverter();

    Vx::VxReal getGearChangeRpm(Vx::VxReal rpm, Vx::VxReal rpmQuick) const;

    bool checkForQuickShift();
    void resetQuickShift();

    Vx::VxReal* mThrottleArray;
    int mThrottleArraySize;
    Vx::VxReal mCsr;
    Vx::VxReal mTCParam;
    Vx::VxReal mCurrentGearRatio;
    Vx::VxReal mTCwout;
    bool mQuickShift;


    int mGearCount;                   /* total number of gear state */
    int mNeutralGear;                 /* index of neutral gear */

    int mGearState;                   /* current gear state */
    int mShifterMinG;                 /* mininum gear corresponding to the shifter position */
    int mShifterMaxG;                 /* maximum gear corresponding to the shifter position */

    Vx::VxReal mGearRatios[MAX_GEARS];    /* gear ratio for all gears from 0 to MAX_GEARS */
    Vx::VxReal mFinalGearRatio;           /* final gear ratio between the transmission and the sprockets */

    Vx::VxReal mGearUpRpm[MAX_GEARS];       /* upshift rpm threshold from first gear*/
    Vx::VxReal mGearUpRpmQuick[MAX_GEARS];  /* upshift rpm threshold from first gear at full throttle */
    Vx::VxReal mGearDownRpm[MAX_GEARS];     /* downshift rpm threshold from second gear */
    Vx::VxReal mGearDownRpmQuick[MAX_GEARS];/* downshift rpm threshold due to high throttle up from second gear */

    Vx::VxReal mMinThrottleQuickShift;    /* throttle above which quick down shift mode starts */
    Vx::VxReal mDeltaThrottleQuickShift;  /* positive throttle change for quick downshift */
    Vx::VxReal mTimeIntervalQuickShift;   /* time interval in which throttle change must be achieved for quick down shift */

    Vx::VxReal mShiftDelay;               /* transmission gear change delay */

    Vx::VxReal mTransmissionDamping;      /* transmission power loss: friction */
    Vx::VxReal mTransmissionDampingExp;   /* transmission power loss: velocity exponent */
    Vx::VxReal mFinalDriveDamping;        /* final drive power loss; friction */
    Vx::VxReal mFinalDriveDampingExp;     /* final drive power loss: velocity exponent */
};

} // namespace Vx

#endif // _VX_ACDSTRANSMISSION_H_
