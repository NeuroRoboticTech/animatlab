
#ifndef _VXVEHICLEUTILS_H_
#define _VXVEHICLEUTILS_H_

#include "Vx/VxPrecision.h"

namespace VxVehicle
{

inline Vx::VxReal ANGVEL_TO_RPM(Vx::VxReal vel) { return vel * 60.0f / (2.0f*ME_PI); }
inline Vx::VxReal RPM_TO_ANGVEL(Vx::VxReal rmp) { return rmp / 60.0f * (2.0f*ME_PI); }
inline Vx::VxReal HP_TO_WATT(Vx::VxReal hp)     { return hp * 745.7f; }
inline Vx::VxReal WATT_TO_HP(Vx::VxReal w)      { return w / 745.7f; }
inline Vx::VxReal ENGINE_HP(Vx::VxReal torque, VxReal w) 
                                                { return WATT_TO_HP(torque * w); }
inline Vx::VxReal ENGINE_TORQUE(Vx::VxReal hp, VxReal rpm) 
                                                { return HP_TO_WATT(hp) / RPM_TO_ANGVEL(rpm); }
inline Vx::VxReal FP_TO_NM(Vx::VxReal fp)       { return fp * .737562f; }
inline Vx::VxReal KMPH_TO_MPS(Vx::VxReal kph)   { return kph / (0.001f * 60.0f*60.0f); }
inline Vx::VxReal MPS_TO_KMPH(Vx::VxReal mps)   { return mps * (0.001f * 60.0f*60.0f); }
inline Vx::VxReal MILLEPH_TO_KMPH(Vx::VxReal mps)   { return mps * 1.609344; }
inline Vx::VxReal KMPH_TO_MILLEPH(Vx::VxReal mps)   { return mps / 1.609344; }
inline VxReal RPM_TO_MPS(VxReal rpm, VxReal radius)
                                                { return RPM_TO_ANGVEL(rpm) * radius; }

inline Vx::VxReal RPM_TO_KMPH(Vx::VxReal rpm, Vx::VxReal radius) 
                                                { return MPS_TO_KMPH(RPM_TO_MPS(rpm, radius)); }
//#define DEBUG_MESSAGE(X,Y) fprintf(stderr, X, Y)
#define DEBUG_MESSAGE 

} // namespace VxVehicle

#endif // _VXVEHICLEUTILS_H_
