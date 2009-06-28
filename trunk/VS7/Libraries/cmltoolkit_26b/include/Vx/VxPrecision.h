
#ifndef _VXPRECISION_H_
#define _VXPRECISION_H_

#include "MePrecision.h"

namespace Vx
{

// tolua_begin
typedef MeReal          VxReal;

typedef MeVector3       VxReal3;
typedef MeVector3Ptr    VxReal3Ptr;

typedef MeVector4       VxReal4;
typedef MeVector4Ptr    VxReal4Ptr;

typedef MeMatrix3       VxReal33;
typedef MeMatrix3Ptr    VxReal33Ptr;

typedef MeMatrix4       VxReal44;
typedef MeMatrix4Ptr    VxReal44Ptr;

// tolua_end

#define Vx_IS_ZERO      ME_IS_ZERO
#define Vx_ARE_EQUAL_TOL ME_ARE_EQUAL_TOL
#define Vx_ARE_EQUAL    ME_ARE_EQUAL

#define VX_PI ME_PI
#define VX_HALF_PI (ME_PI/2.0f)
#define VX_TWO_PI (ME_PI*2.0f)

/*
typedef const VxReal     VxReal4Const[4];
typedef VxReal4Const* const    VxReal44ConstPtr;
*/

} // namespace Vx

#endif // _VXPRECISION_H_
