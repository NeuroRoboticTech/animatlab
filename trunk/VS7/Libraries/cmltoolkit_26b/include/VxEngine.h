
#ifndef _VX_ENGINE_H_
#define _VX_ENGINE_H_

#include "VxTable.h"
#include "VxVehicleUtils.h"
#include "Vx/VxPrecision.h"

namespace Vx
{
class VxHinge;
class VxPart;
class VxGearRatio;
class VxUniverse;
}

namespace VxVehicle
{

/**
 * VxEngine class
 * This class links the chassis to the engine shaft.
 * The preUpdate converts the input throttle and engine rpm into a torque
 * applied to the shaft.
 */
class VxEngine
{
public:
    VxEngine();

    void init(Vx::VxUniverse *u);
    void reset();

    void setMaxRpm(Vx::VxReal rpm) { mMaxVel= RPM_TO_ANGVEL(rpm); }
    void setMaxTorque(Vx::VxReal mt) { mMaxTorque = mt; }
    void setIdleRpm(Vx::VxReal idle) { mIdleVel = RPM_TO_ANGVEL(idle); }
    void setShaftMassAndInertia(Vx::VxReal mass, Vx::VxReal inertia);
    void setShaftAngularVelocityDamping(Vx::VxReal damp, Vx::VxReal damp_exp);
    void setTorqueTable(int sizeThrottle, int sizeRpm, Vx::VxReal* throttle, Vx::VxReal* rpm, Vx::VxReal* torque);
    void scaleTorqueTable(VxReal scaleThrottle, VxReal scaleRpm, VxReal scaleTorque);
    void adaptTorqueTable(VxReal desiredMaxRpm, VxReal desiredMaxTorque);

    void updateShaftAxis(VxReal3Ptr axis);

    void setChassis(VxPart* chassis, const Vx::VxReal44Ptr shaft_rel_tm, const Vx::VxReal3Ptr shaft_axis);
    
    void setThrottle(Vx::VxReal t);
    void setThrottleLock(bool t);
    void setEngineRunning(bool r);

    /** Returns engine running state */
    bool getEngineRunning() const { return mEngineRunning; }
    /** Returns current engine horse power */
    inline Vx::VxReal getHP() const { return ENGINE_HP(mShaftVel, mTorque); }
    /** Sets a scale factor that will be applied to the computed torque. */
    inline void setTorqueScaler(Vx::VxReal scaler) { mTorqueScaler = scaler; }
    /** Returns scale factor that will be applied to the computed torque. */
    Vx::VxReal getTorqueScaler() const { return mTorqueScaler; }

    void preUpdate();
    void postUpdate();

    Vx::VxReal getShaftTorque() const;
    /** Returns the current shaft angular velocity */
    Vx::VxReal getShaftSpeed() const { return mShaftVel; }

    bool mEngineRunning;

    Vx::VxReal mShaftVel;
    Vx::VxReal mIdleVel;
    Vx::VxReal mMaxVel;

    Vx::VxReal mTorque;
    Vx::VxReal mMaxTorque;

    Vx::VxReal mTorqueScaler;

    bool mConstantSpeedFlag;
    bool mBackDrive;                // true if occurs

    VxPart* mChassisPart;
    VxPart* mShaftPart;
    VxHinge* mShaftHinge;           // between chassis and shaft

    VxTable2D mTorqueTable;          // table of torque vs throttle and rpm

    Vx::VxReal44 mShaftPartRelTm;
    Vx::VxReal3 mShaftAxis;
    Vx::VxReal mShaftDamping;           
    Vx::VxReal mShaftDampingExp;  

    void setIdleController(Vx::VxReal start, Vx::VxReal inc);
    
    Vx::VxReal mIdleControllerStart;
    Vx::VxReal mIdleController;
    Vx::VxReal mIdleControllerInc;
    
    void updateTransform();

    /** Return throttle set by user. */
    Vx::VxReal getInputThrottle() const { return mInputThrottle; }

    /** Return inputThrottle + contribution from the idle controller or throttleLock controller. */
    Vx::VxReal getThrottle() const { return mThrottle; }

    void setRpmResponseFrequency(Vx::VxReal rp);

    bool mShaftAttachedToChassis;

protected:

    void computeEngineTorque();

    Vx::VxReal mInputThrottle;
    Vx::VxReal mThrottle;
    bool mThrottleLock;
    Vx::VxReal mShaftVelLock;

    Vx::VxUniverse* mUniverse;
    Vx::VxReal mRpmResponseFrequency;

private:
    Vx::VxReal tmpX[5];
    Vx::VxReal tmpY[5];
    Vx::VxReal tmpZ[5*5];
};

} // namespace VxVehicle

#endif // _VX_ENGINE_H_
