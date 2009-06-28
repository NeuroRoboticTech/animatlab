
#ifndef _VXPOWERTRAIN_H_
#define _VXPOWERTRAIN_H_



#include "VxTable.h"
#include "VxEngine.h"
#include "VxTransmission.h"
#include "Vx/VxPrecision.h"

namespace Vx
{
class VxPart;
class VxUniverse;
class VxConstraint;
}

namespace VxVehicle
{

class VxEngine;
class VxPowerTrain;


/** 
 * class VxPowerTrain, pure virtual power train base class.
 */
class VxPowerTrain
{
public:
    VxPowerTrain(Vx::VxUniverse* u) : mUniverse(u) {}
    virtual ~VxPowerTrain() {}

    virtual void init() = 0;
    virtual void reset() = 0;
    virtual void preUpdate() = 0;
    virtual void postUpdate() = 0;

    Vx::VxUniverse* mUniverse;
};

} // namespace Vx


#endif // _VXPOWERTRAIN_H_


