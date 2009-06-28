#ifndef _MDTCONELIMIT_H
#define _MDTCONELIMIT_H
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

/** @file    MdtConeLimit.h

    MdtConeLimit API functions.
    
    This constraint is really a special type of joint limit which can be
    added to any of the MdtBSJoint, MdtUniversal, MdtFixedPath joint
    types.  It can be used on it's own or on other joints as well though
    the behavior may not be what one might expect in cases not explicitly
    covered in the documentation.  Given an axis n0 which is fixed in body
    b0 and another axis n1 which is fixed in body b1, the constraint will
    prevent the angle between these two unit vectors to grow larger than
    the specified value.  The angle can go either positive or negative.
    The axis n1 specifies the axis of the constraint as reported by
    MdtConstraintGetAxis.  The position of this constraint is unspecified.

*/

#include "MePrecision.h"
#include "MdtTypes.h"


#ifdef __cplusplus
extern "C"
{
#endif


/*
  Cone Limit joint functions.
*/

MdtConeLimitID    MEAPI MdtConeLimitCreate(const MdtWorldID w);
MdtConeLimitID    MEAPI MdtConeLimitCreateInPool(MePool* pool);
void              MEAPI MdtConeLimitReset(MdtConeLimitID j);



/*
  Cone Limit accessors.
*/


void    MEAPI MdtConeLimitSetConeHalfAngle(MdtConeLimitID j, const MeReal theta);
void    MEAPI MdtConeLimitSetStiffness(MdtConeLimitID j, const MeReal kp);
void    MEAPI MdtConeLimitSetDamping(MdtConeLimitID j, const MeReal kd);

/*
  Cone Limit joint mutators.
*/
MeReal    MEAPI MdtConeLimitGetConeHalfAngle(const MdtConeLimitID j);
MeReal    MEAPI MdtConeLimitGetStiffness(const MdtConeLimitID j);
MeReal    MEAPI MdtConeLimitGetDamping(const MdtConeLimitID j);



#define  MdtConeLimitDestroy(j)   MdtConstraintDestroy(j)
#define  MdtConeLimitEnable(j)    MdtConstraintEnable(j)
#define  MdtConeLimitDisable(j)   MdtConstraintDisable(j)
#define  MdtConeLimitIsEnabled(j) MdtConstraintIsEnabled(j)
#define  MdtConeLimitGetAxis(j, v)  MdtConstraintGetAxis(j, (v))
#define  MdtConeLimitGetAxes(j, p, o)  MdtConstraintGetAxes(j, (p), (o))
#define  MdtConeLimitBodyGetAxes(j, b, p, o)  MdtConstraintBodyGetAxes(j, (b), (p), (o))
#define  MdtConeLimitGetBody(j, bodyindex)  MdtConstraintGetBody(j, (bodyindex))
#define  MdtConeLimitGetUserData(j)  MdtConstraintGetUserData(j)
#define  MdtConeLimitGetWorld(j)  MdtConstraintGetWorld(j)
#define  MdtConeLimitGetForce(j, bodyindex, f)  MdtConstraintGetForce(j, (bodyindex), (f))
#define  MdtConeLimitGetTorque(j, bodyindex, t)  MdtConstraintGetTorque(j, (bodyindex), (t))
#define  MdtConeLimitSetBodies(j, b1, b2)  MdtConstraintSetBodies(j, (b1), (b2))
#define  MdtConeLimitSetUserData(j, d)  MdtConstraintSetUserData(j, (d))
#define  MdtConeLimitSetAxis(j, x, y, z)  MdtConstraintSetAxis(j, (x), (y), (z))
#define  MdtConeLimitSetAxes(j, px, py, pz, ox, oy, oz)  MdtConstraintSetAxes(j, (px), (py), (pz), (ox), (oy), (oz))
#define  MdtConeLimitBodySetAxes(j, b, px, py, pz, ox, oy, oz)  MdtConstraintBodySetAxes(j, (b), (px), (py), (pz), (ox), (oy), (oz))
#define  MdtConeLimitBodySetAxesRel(j, b, px, py, pz, ox, oy, oz)  MdtConstraintBodySetAxesRel(j, (b), (px), (py), (pz), (ox), (oy), (oz))


#ifdef __cplusplus
}
#endif


#endif
