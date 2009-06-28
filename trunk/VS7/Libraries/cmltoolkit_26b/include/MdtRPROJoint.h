#ifndef _MDTRPROJOINT_H
#define _MDTRPROJOINT_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/02/23 15:42:03 $
   
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
 * MdtRPROJoint API functions.

 
 This geometry of this constraint is that of a fullly controlled
 spherical joint in which the user gets to set the relative orientation
 between a coordinate system S0 with origin at point p0 in body b0 and
 a coordinate system S1 with origid at point p1 in body b1.  The joint
 really consists of a positional or linear component which maintains p0
 coincident with p1, and an angular part which controls the relative
 rotation matrix between frames S0 and S1.  The latter is specified
 using a quaternion (also called Euler parameters, which are different
 from Euler angles).  
 
 To clarify this, assume that S0 is specified witn an orthonormal
 coordinate system u0, v0, n0 which is assumed fixed in body b0.
 Likewise, u1, v1, n1 is an orthonormal basis fixed in body b1.  The
 orthogonal rotation matrices for bodies b0 and b1 and reference
 systems S0 and S1 are R0, R1, T0 and T1 respectively.  Matrices T0 and
 T1 have vectors u0, v0, n0 and u1, v1, n1 as columns,
 respectively. The relative orientation between the two coordinate
 systems is defined as:
 
 $R = (R1 * T1 )^{-1}  * ( R0 * T0 ) $
 
 
 In worlds: relative rotation R is the product of  [ the inverse of the
 product of R1 with T1  ] with [ the product of R0 with T0 ]. 
 

 Special API calls are provided here to access and set the variables in
 relative cooridinates.

 Since this constraint is useful when mixing animations with simulations,
 the user can specify the maximum strenght of each of the constraint
 equations.   

 */

#include "MePrecision.h"
#include "MdtTypes.h"


#ifdef __cplusplus
extern "C"
{
#endif


MdtConstraintID    MEAPI MdtRPROJointCreate(const MdtWorldID w);
void              MEAPI MdtRPROJointReset(MdtConstraintID j);



/*
  Relative position, relative orientation joint accessors.
*/


void  MEAPI MdtRPROJointGetAttachmentPosition(const MdtConstraintID j, const unsigned int bodyindex, MeVector3 position);

void  MEAPI MdtRPROJointGetAttachmentQuaternion(const MdtConstraintID j, const unsigned int bodyindex, MeVector4 quaternion);

void  MEAPI MdtRPROJointGetRelativeQuaternion(const MdtConstraintID j, MeVector4 q);

void  MEAPI MdtRPROJointGetRelativeAngularVelocity(const MdtConstraintID j, MeVector3 w);

void  MEAPI MdtRPROJointGetAngularStrength(const MdtConstraintID j, MeVector3 v);

void  MEAPI MdtRPROJointGetLinearStrength(const MdtConstraintID j, MeVector3 v);

void MEAPI MdtRPROJointSetRelativeQuaternionFromBodies(MdtConstraintID J);



/*
  Relative position, relative orientation joint mutators.
*/




void  MEAPI MdtRPROJointSetAttachmentQuaternion(MdtConstraintID j, const
						MeReal q0, const MeReal q1,
						const MeReal q2, 
						const MeReal q3, const unsigned int bodyindex);

void  MEAPI MdtRPROJointSetRelativeQuaternion(MdtConstraintID j, const MeVector4 q);
void  MEAPI MdtRPROJointSetRelativeAngularVelocity(MdtConstraintID j, MeVector3 w);


void  MEAPI MdtRPROJointSetAngularStrength(MdtConstraintID j, const MeReal sX, const MeReal sY, const MeReal sZ);
void  MEAPI MdtRPROJointSetLinearStrength(MdtConstraintID j, const MeReal sX, const MeReal sY, const MeReal sZ);

#define  MdtRPROJointDestroy(j)  MdtConstraintDestroy(j)
#define  MdtRPROJointEnable(j)  MdtConstraintEnable(j)
#define  MdtRPROJointDisable(j)  MdtConstraintDisable(j)
#define  MdtRPROJointIsEnabled(j)  MdtConstraintIsEnabled(j)
#define  MdtRPROJointGetBody(j, bodyindex)  MdtConstraintGetBody(j, bodyindex)
#define  MdtRPROJointGetUserData(j)  MdtConstraintGetUserData(j)
#define  MdtRPROJointGetWorld(j)  MdtConstraintGetWorld(j)
#define  MdtRPROJointGetForce(j, bodyindex, f)  MdtConstraintGetForce(j, bodyindex, f)
#define  MdtRPROJointGetTorque(j, bodyindex, t)  MdtConstraintGetTorque(j, bodyindex, t)
#define  MdtRPROJointSetPosition(j, x, y, z)  MdtConstraintSetPosition(j, x, y, z)
#define  MdtRPROJointSetBodies(j, b1, b2)  MdtConstraintSetBodies(j, b1, b2)
#define  MdtRPROJointSetUserData(j, d)  MdtConstraintSetUserData(j, d)
#define  MdtRPROJointSetAttachmentPosition(j, x, y, z, bodyindex) MdtConstraintBodySetPosition(j, bodyindex, x,y, z)


#ifdef __cplusplus
}
#endif

#endif
