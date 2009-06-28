#ifndef _MDTBSJOINT_H
#define _MDTBSJOINT_H
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
 * MdtBSJoint API functions.

     The geometry of this joint is such that during the simulation, a point
     p0 fixed in the reference frame of body 0 will be kept coincident to a
     point p1 fixed in the reference frame of body 1.  This means that the
     world coordinates of point p0 will be almost identical to the world
     coordinates of point p1.  This common point is called the joint
     position: it is the current center of rotation about which body 0 and
     body 1 can move freely.  

*/

#include "MePrecision.h"
#include "MdtTypes.h"


#ifdef __cplusplus
extern "C"
{
#endif


/*
  Ball and socket joint functions.
*/

MdtBSJointID      MEAPI MdtBSJointCreate(const MdtWorldID w);
void              MEAPI MdtBSJointReset(const MdtBSJointID j);
MdtBSJointID      MEAPI MdtBSJointCreateInPool(MePool *pool);






#define                 MdtBSJointDestroy(j)  MdtConstraintDestroy(j)
#define                 MdtBSJointEnable(j)  MdtConstraintEnable(j)
#define                 MdtBSJointDisable(j)  MdtConstraintDisable(j)
#define                 MdtBSJointIsEnabled(j)  MdtConstraintIsEnabled(j)

/*
  Ball and socket joint accessors.
*/


#define                 MdtBSJointGetBody(j, bodyindex)  MdtConstraintGetBody(j, bodyindex)
#define                 MdtBSJointGetUserData(j)  MdtConstraintGetUserData(j)
#define                 MdtBSJointGetWorld(j)  MdtConstraintGetWorld(j)
#define                 MdtBSJointGetForce(j, bodyindex, f)  MdtConstraintGetForce(j, bodyindex, f)
#define                 MdtBSJointGetTorque(j, bodyindex, t)  MdtConstraintGetTorque(j, bodyindex, t)
#define                 MdtBSJointGetPosition(j, v)  MdtConstraintGetPosition(j, v)
#define                 MdtBSJointGetAxis(j, v)  MdtConstraintGetAxis(j, v)
/*
  Ball and socket joint mutators.
*/
#define                 MdtBSJointSetPosition(j, x, y, z) MdtConstraintSetPosition(j, x, y, z)
#define                 MdtBSJointSetAxis(j, x, y, z) MdtConstraintSetAxis(j, x, y, z)
#define                 MdtBSJointSetBodies(j, b1, b2) MdtConstraintSetBodies(j, b1, b2)
#define                 MdtBSJointSetUserData(j, d) MdtConstraintSetUserData(j, d)


void MEAPI MdtBSJointSetTwistDampingParameter(MdtBSJointID j,MeReal twistDamping);
void MEAPI MdtBSJointSetSwingDampingParameter(MdtBSJointID j,MeReal swingDamping);
void MEAPI MdtBSJointSetTwistDamping(MdtBSJointID j,MeReal twistDamping);
void MEAPI MdtBSJointSetSwingDamping(MdtBSJointID j,MeReal swingDamping);
void MEAPI MdtBSJointEnableTwistDamping(MdtBSJointID j);
void MEAPI MdtBSJointDisableTwistDamping(MdtBSJointID j);
void MEAPI MdtBSJointEnableSwingDamping(MdtBSJointID j);
void MEAPI MdtBSJointDisableSwingDamping(MdtBSJointID j);
MeReal MEAPI MdtBSJointGetTwistDampingParameter(MdtBSJointID j);
MeReal MEAPI MdtBSJointGetSwingDampingParameter(MdtBSJointID j);


#ifdef __cplusplus
}
#endif


#endif
