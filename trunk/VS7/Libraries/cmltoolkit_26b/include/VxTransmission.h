
#ifndef _VX_TRANSMISSION_H_
#define _VX_TRANSMISSION_H_

#include "VxTable.h"
#include "Vx/VxPrecision.h"

namespace Vx
{
class VxDifferentialJoint;
class VxPart;
class VxUniverse;
}

namespace VxVehicle
{
class VxEngine;

/**
 * Vxtransmission class, pure virtual transmission base class.
 */
class VxTransmission
{
public:
    VxTransmission() : mEngine(0), mUniverse(0) {}
    virtual ~VxTransmission() {}

    virtual void init(Vx::VxUniverse *u) = 0;
    virtual void reset() = 0;
    virtual void preUpdate() = 0;
    virtual void postUpdate() = 0;

    VxEngine* mEngine;
    Vx::VxUniverse* mUniverse;

    Vx::VxReal mPrevEps;
    Vx::VxReal mTCRatio;
};

} // namespace VxVehicle

#endif // _VX_TRANSMISSION_H_
