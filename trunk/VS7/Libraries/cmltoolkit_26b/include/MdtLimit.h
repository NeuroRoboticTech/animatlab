#ifndef _MDTLIMIT_H
#define _MDTLIMIT_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/03/09 16:26:23 $
   
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
  MdtLimit and MdtSingleLimit API functions.

  This API allows to fully control the one degree of freedom of the
  MdtHingeID and MdtPrismaticID constraint as well as most of the behavior
  of the MdtSpringID constraint.

  Use this to set up upper and lower limits on prismatic and hinge joints.
  You can also control the behavior of the limit itself i.e., make it a
  hard or soft limit.  In the case of a soft limit, you may control the
  stiffness and damping coefficients of the oscillations around the limit
  point.

  This API also provide ways to control the limited joint variable itself
  with a motor or a lock.  The MdtLimitSetLimitedForceMotor() will drive
  the joint variable at a desired velocity but only deliver up to the
  specified maximum force.  This is a much more stable and efficient way to
  control your joint variables than is obtained by supplying torques on the
  constrained bodies directly.

 */

#include "MePrecision.h"
#include "MdtTypes.h"


#ifdef __cplusplus
extern "C"
{
#endif



/*
  Individual limit initialisation:
*/

/*
 * This implements hard and soft limits for the remaining degrees of
 * freedom of constraints.
 *
 * All angles are specified in radians.
 */

void              MEAPI MdtSingleLimitReset(const MdtSingleLimitID limit);

/*
  Individual limit accessors:
*/

MeReal            MEAPI MdtSingleLimitGetStop(
                            const MdtSingleLimitID sl);
MeReal            MEAPI MdtSingleLimitGetStiffness(
                            const MdtSingleLimitID sl);
MeReal            MEAPI MdtSingleLimitGetDamping(
                            const MdtSingleLimitID sl);
MeReal            MEAPI MdtSingleLimitGetRestitution(
                            const MdtSingleLimitID sl);
MeReal            MEAPI MdtSingleLimitGetVelocity(
                            const MdtSingleLimitID sl);
/*
  Individual limit mutators:
*/

void              MEAPI MdtSingleLimitSetStop(const MdtSingleLimitID sl,
                            const MeReal NewStop);
void              MEAPI MdtSingleLimitSetStiffness(const MdtSingleLimitID sl,
                            const MeReal NewStiffness);
void              MEAPI MdtSingleLimitSetDamping(const MdtSingleLimitID sl,
                            const MeReal NewDamping);
void              MEAPI MdtSingleLimitSetRestitution(const MdtSingleLimitID sl,
                            const MeReal NewRestitution);
void              MEAPI MdtSingleLimitSetVelocity(const MdtSingleLimitID sl,
                            const MeReal NewVelocity);

/*
  Joint limit functions:
*/

void              MEAPI MdtLimitReset(const MdtLimitID limit);

/*
  Joint limit accessors:
*/

MeBool            MEAPI MdtLimitIsActive(const MdtLimitID l);

MdtSingleLimitID  MEAPI MdtLimitGetLowerLimit(const MdtLimitID l);
MdtSingleLimitID  MEAPI MdtLimitGetUpperLimit(const MdtLimitID l);
MeReal            MEAPI MdtLimitGetPosition(const MdtLimitID l);
MeReal            MEAPI MdtLimitGetOffset(const MdtLimitID l);
MeReal            MEAPI MdtLimitGetOvershoot(const MdtLimitID l);
MeReal            MEAPI MdtLimitGetVelocity(const MdtLimitID l);
MeReal            MEAPI MdtLimitGetStiffnessThreshold( const MdtLimitID l);
MeBool            MEAPI MdtLimitIsMotorized(const MdtLimitID l);
MeBool            MEAPI MdtLimitIsLocked(const MdtLimitID l);
MeBool            MEAPI MdtLimitIsFree(const MdtLimitID l);
MeReal            MEAPI MdtLimitGetMotorDesiredVelocity( const MdtLimitID l);
MeReal            MEAPI MdtLimitGetDesiredPosition( const MdtLimitID l);
MeReal            MEAPI MdtLimitGetMotorMaxForce( const MdtLimitID l);
MeReal            MEAPI MdtLimitGetMotorMinForce( const MdtLimitID l);
MeReal            MEAPI MdtLimitGetLockMaxForce( const MdtLimitID l);
MeReal            MEAPI MdtLimitGetMotorLoss(const MdtLimitID l);



MdtLimitStateEnum MEAPI MdtLimitGetState( const MdtLimitID l);
void              MEAPI MdtLimitSetState( const MdtLimitID l, MdtLimitStateEnum newState);


/*
  Joint limit mutators:
*/

void              MEAPI MdtLimitSetLowerLimit(const MdtLimitID l, const MdtSingleLimitID sl);
void              MEAPI MdtLimitSetUpperLimit(const MdtLimitID l, const MdtSingleLimitID sl);
void              MEAPI MdtLimitSetOffset(const MdtLimitID l, MeReal NewOffset );
void              MEAPI MdtLimitActivateLimits(const MdtLimitID l, const MeBool NewActivationState);
void              MEAPI MdtLimitSetStiffnessThreshold(const MdtLimitID l, const MeReal NewStiffnessThreshold);

void              MEAPI MdtLimitActivateMotor(const MdtLimitID l, const MeBool NewActivationState);
void              MEAPI MdtLimitSetMotorParameters(const MdtLimitID l, 
                                                   const MeReal desiredVelocity, 
                                                   const MeReal f_min, 
                                                   const MeReal f_max);

void              MEAPI MdtLimitSetMotor(const MdtLimitID l, 
					 const MeReal desiredVelocity, 
                                         const MeReal f_min, 
                                         const MeReal f_max);
void              MEAPI MdtLimitSetMotorLoss(MdtLimitID l, MeReal loss);

void              MEAPI MdtLimitSetLimitedForceMotor(const MdtLimitID l, 
						     const MeReal desiredVelocity, 
                                                     const MeReal f_min,  
                                                     const MeReal f_max);

void              MEAPI MdtLimitActivateLock(const MdtLimitID l, const MeBool NewActivationState);
void              MEAPI MdtLimitSetLockParameters(const MdtLimitID l, const MeReal lock_value, 
                                                  const MeReal f_min,
                                                  const MeReal f_max);
void              MEAPI MdtLimitSetLock(const MdtLimitID l, const MeReal lock_value, 
                                        const MeReal f_min, 
                                        const MeReal f_max);
void              MEAPI MdtLimitSetLockVelocity(const MdtLimitID l, const MeReal lock_velocity);
MeReal            MEAPI MdtLimitGetLockVelocity(const MdtLimitID l);

void              MEAPI MdtLimitSetLockSpring(const MdtLimitID l, const MeReal stiffness, const MeReal damping);
MeReal            MEAPI MdtLimitGetLockSpringStiffness(const MdtLimitID l);
MeReal            MEAPI MdtLimitGetLockSpringDamping(const MdtLimitID l);
void              MEAPI MdtLimitController(const MdtLimitID limit,
                                           const MeReal desiredPosition, const MeReal gap,
                                           const MeReal maxSpeed, 
                                           const MeReal minForce, 
                                           const MeReal maxForce);

MdtLimitTypeEnum  MEAPI MdtLimitGetType(const MdtLimitID l);
MeBool            MEAPI MdtLimitIsLinear(const MdtLimitID l);
MeBool            MEAPI MdtLimitIsAngular(const MdtLimitID l);
MeBool            MEAPI MdtLimitIsDistance(const MdtLimitID l);

MeBool            MEAPI MdtLimitGetForceCalculatePosition(const MdtLimitID l);
void              MEAPI MdtLimitSetForceCalculatePosition(const MdtLimitID l,  const MeBool NewState);

MeBool            MEAPI MdtLimitGetMotorStopAtLock(const MdtLimitID l);
void              MEAPI MdtLimitSetMotorStopAtLock(const MdtLimitID l,  const MeBool NewState);

#ifdef __cplusplus
}
#endif


#endif
