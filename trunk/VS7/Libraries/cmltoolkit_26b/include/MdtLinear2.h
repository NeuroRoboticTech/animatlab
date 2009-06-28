#ifndef _MDTLINEAR2_H
#define _MDTLINEAR2_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/09/27 21:30:57 $
   
   This software and its accompanying manuals have been developed by CMLabs
   Simulations Inc. ("CMLabs") and MathEngine PLC ("MathEngine").  The
   copyright to the Vortex Toolkits and all associated materials belongs to
   CMLabs.
   
   All intellectual property rights in the software belong to CMLabs,
   except for portions that have been licensed from MathEngine.

   All rights conferred by law (including rights under international
   copyright conventions) are reserved to CMLabs. This software may also
   incorporate information which is confidential to CMLabs.

   Save to the extent permitted by law, or as otherwise expressly permitted
   by CMLabs, this software and the manuals must not be copied (in whole or
   in part), re-arranged, altered or adapted in any way without the prior
   written consent of CMLabs. In addition, the information contained in the
   software may not be disseminated without the prior written consent of
   CMLabs.

 */

/** @file
 * MdtLinear2 API functions.
 
 For this joint, a point p0 fixed in body b0 is constrained to move
 along a line l1 fixed in body b1.  For convenience, the origin of that
 line is called the joint position and the direction vector is called
 the joint axis. 

 */

#include "MePrecision.h"
#include "MdtTypes.h"


#ifdef __cplusplus
extern "C"
{
#endif


MdtLinear2ID      MEAPI MdtLinear2Create(const MdtWorldID w);
MdtLinear2ID      MEAPI MdtLinear2CreateInPool(MePool* pool);
void              MEAPI MdtLinear2Reset(MdtLinear2ID j);


/*
  Linear2 joint accessors.
*/

#define MdtLinear2GetPosition(j, x) MdtConstraintGetPosition((j), (x))
#define MdtLinear2GetDirection(j, n) MdtConstraintGetAxis((j), (n))


  /* deprecated: this here for backwards compatibility */
#define Linear2SetBodies(j, b1, b2) \
  MdtConstraintSetBodies((j), (b1), (b2)); \
  MdtConstraintSetAxis((j), (1), (0), (0)) 

#define MdtLinear2SetPosition(j, x, y, z) \
  MdtConstraintSetPosition((j), (x), (y), (z))

#define MdtLinear2SetDirection(j, x, y, z) \
  MdtConstraintSetAxis((j), (x), (y), (z))




#define MdtLinear2Destroy(j)  MdtConstraintDestroy(j)
#define MdtLinear2Enable(j)  MdtConstraintEnable(j)
#define MdtLinear2Disable(j)  MdtConstraintDisable(j)
#define MdtLinear2IsEnabled(j)  MdtConstraintIsEnabled(j)
#define MdtLinear2GetBody(j, bodyindex)  MdtConstraintGetBody(j, bodyindex)
#define MdtLinear2GetUserData(j)  MdtConstraintGetUserData(j)
#define MdtLinear2GetWorld(j)  MdtConstraintGetWorld(j)
#define MdtLinear2GetForce(j, bodyindex, f)  MdtConstraintGetForce(j, bodyindex, f)
#define MdtLinear2GetTorque(j, bodyindex, t)  MdtConstraintGetTorque(j, bodyindex, t)
#define MdtLinear2SetBodies(j, b1, b2)  MdtConstraintSetBodies(j, b1, b2)
#define MdtLinear2SetUserData(j, d)  MdtConstraintSetUserData(j, d)


#ifdef __cplusplus
}
#endif


#endif
