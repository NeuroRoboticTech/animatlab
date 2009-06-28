
#ifndef _VxWheeledPowerTrain_H_
#define _VxWheeledPowerTrain_H_



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
 * VxWheeledPowerTrain class
 * This class contains the engine, the torque converter, the transmission and
 * the differential.
 */
class VxWheeledPowerTrain : public VxPowerTrain
{
public:
    VxWheeledPowerTrain(Vx::VxUniverse* u);
    virtual ~VxWheeledPowerTrain() {}

    void setChassis(Vx::VxPart* chassis, const Vx::VxReal44Ptr shaft_rel_tm, const Vx::VxReal3Ptr shaft_axis);
    void setMainPoweredWheel(Vx::VxPart* left, Vx::VxPart* right, const Vx::VxReal3Ptr axis);

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

#endif // _VxWheeledPowerTrain_H_


