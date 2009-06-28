
#ifndef _VXTRACKEDPOWERTRAIN_H_
#define _VXTRACKEDPOWERTRAIN_H_



#include "VxPowerTrain.h"
#include "VxTable.h"
#include "VxEngine.h"
#include "VxAutomaticNGearTransmission.h"
#include "Vx/VxPrecision.h"


namespace Vx {   
class VxPart;
}

namespace VxVehicle
{


/**
 * VxTrackedPowerTrain class
 * This class contains the engine, the torque converter, the transmission and
 * the differential.
 */
class VxTrackedPowerTrain : public VxPowerTrain
{
public:
    VxTrackedPowerTrain(Vx::VxUniverse* u);
    virtual ~VxTrackedPowerTrain() {}

    void setChassis(VxPart* chassis, const Vx::VxReal44Ptr shaft_rel_tm, const Vx::VxReal3Ptr shaft_axis);
    void setSprockets(VxPart* left, VxPart* right, const Vx::VxReal3Ptr axis);

    Vx::VxReal getMass() const;

    void reset();
    void updateTransform();

    void preUpdate();
    void postUpdate();

    VxEngine mEngine;
    VxAutomaticNGearTransmission mTransmission;

private:
    void init();
};

} // namespace VxVehicle

#endif // _VXTRACKEDPOWERTRAIN_H_


