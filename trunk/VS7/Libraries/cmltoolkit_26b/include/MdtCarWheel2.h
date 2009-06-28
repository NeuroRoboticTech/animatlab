#ifndef _MDTCarWheel2_H
#define _MDTCarWheel2_H
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

/** @file MdtCarWheel2.h 

    MdtCarWheel2 API functions.
  
     The geometry of this constraint refers to a body b0 called chassis and
     a body b1 called wheel.  It is useful to think of this constraint as
     two main components namely, the steering column and driving axle.  In
     the API, these are referred to as the steering and hinge components
     respectively.  The steering column is attached at point p0 which is
     fixed on the chassis and at point p1 which is the center of mass of
     the wheel.  The steering column has a rest lenght, upper and lower
     limits, as well as a stiffness and damping spring constants for
     oscillations about the rest length.  The axle can be made to rotate
     about the steering column in a plane perpendicular to it. This angle
     is controlled with either a position conrol or a limited force
     constant velocity motor i.e., a motor which applies up to the
     specified maximum torque to achieve the specified velocity.  The
     motion of the wheel about the axle is either free or controlled with
     another limited force constant velocity motor.

     The generic funtion MdtConstraintGetPosition() will report the center of
     mass of the wheel.  The MdtConstraintGetAxis() function will report
     the steering axis.  The function MdtConstraintBodyGetAxes(ct, 1, steering, axle);
     will report the current steering and axle axis. 
     

*/

#include "MePrecision.h"
#include "MdtTypes.h"


#ifdef __cplusplus
extern "C"
{
#endif


  MdtCarWheel2ID    MEAPI MdtCarWheel2Create(const MdtWorldID w);
  void              MEAPI MdtCarWheel2Reset(MdtCarWheel2ID j);
  MdtCarWheel2ID    MEAPI MdtCarWheel2CreateInPool(MePool *pool);



/*
  Car wheel joint accessors.
*/

#define MdtCarWheel2GetPosition(j, v)  MdtConstraintGetPosition((j), (v)) 

  /* 1.0 steering angle */
  MeReal            MEAPI MdtCarWheel2GetSteeringAngle(const MdtCarWheel2ID j);
  MeReal            MEAPI MdtCarWheel2GetDesiredSteeringAngle(const MdtCarWheel2ID j);
  MeReal            MEAPI MdtCarWheel2GetSteeringAngleRate(const MdtCarWheel2ID j);
  void              MEAPI MdtCarWheel2GetSteeringAxis(const MdtCarWheel2ID j, MeVector3 v);
  MeReal            MEAPI MdtCarWheel2GetSteeringMotorDesiredVelocity( const MdtCarWheel2ID j);
  MeReal            MEAPI MdtCarWheel2GetSteeringMotorMaxForce( const MdtCarWheel2ID j);
  MeBool            MEAPI MdtCarWheel2IsSteeringLocked(MdtCarWheel2ID j);
  

  /* 2.0 turning wheel */
  MeReal            MEAPI MdtCarWheel2GetHingeAngle(const MdtCarWheel2ID j);
  MeReal            MEAPI MdtCarWheel2GetHingeAngleRate(const MdtCarWheel2ID j);
  void              MEAPI MdtCarWheel2GetHingeAxis(const MdtCarWheel2ID j, MeVector3 v);
  MeReal            MEAPI MdtCarWheel2GetHingeMotorDesiredVelocity( const MdtCarWheel2ID j);
  MeReal            MEAPI MdtCarWheel2GetHingeMotorMaxForce( const MdtCarWheel2ID j);
  

  /* 3.0 Suspension */
  MeReal            MEAPI MdtCarWheel2GetSuspensionHeight(const MdtCarWheel2ID j);
  MeReal            MEAPI MdtCarWheel2GetSuspensionRate(const MdtCarWheel2ID j);
  MeReal            MEAPI MdtCarWheel2GetSuspensionHighLimit( const MdtCarWheel2ID j);
  MeReal            MEAPI MdtCarWheel2GetSuspensionLowLimit( const MdtCarWheel2ID j);
  MeReal            MEAPI MdtCarWheel2GetSuspensionReference( const MdtCarWheel2ID j);
  MeReal            MEAPI MdtCarWheel2GetSuspensionKp(const MdtCarWheel2ID j);
  MeReal            MEAPI MdtCarWheel2GetSuspensionKd(const MdtCarWheel2ID j);
  




  /*
    Car wheel joint mutators.
  */
  
  void MEAPI MdtCarWheel2SetBodies(MdtConstraintID c, const MdtBodyID b1, const MdtBodyID b2);
  void MEAPI MdtCarWheel2SetPosition(MdtCarWheel2ID j, const MeReal x, const MeReal y, const MeReal z);


  /* steering */
  void MEAPI MdtCarWheel2SetSteeringAxis(MdtCarWheel2ID j, const MeReal x, const MeReal y, const MeReal z);
  void MEAPI MdtCarWheel2SetSteeringLimitedForceMotor( MdtCarWheel2ID j, 
                                                       const MeReal desiredVelocity, 
                                                       const MeReal f_min, 
                                                       const MeReal f_max);
  void MEAPI MdtCarWheel2SetSteeringLock(MdtCarWheel2ID j, const MeBool lock);
  void MEAPI MdtCarWheel2SetSteeringAngle(MdtCarWheel2ID j, MeReal angle);
  void MEAPI MdtCarWheel2SetSteeringMaxForce(MdtCarWheel2ID j, MeReal max_force);
  void MEAPI MdtCarWheel2SetSteeringHiLimit(MdtCarWheel2ID j,MeReal hilimit, const MeReal Kp, const MeReal Kd);
  void MEAPI MdtCarWheel2SetSteeringLoLimit(MdtCarWheel2ID j,MeReal lolimit, const MeReal Kp, const MeReal Kd);
  void MEAPI MdtCarWheel2SteeringLimitIsActivate(MdtCarWheel2ID J);
  void MEAPI MdtCarWheel2ActivateSteeringLimit(MdtCarWheel2ID J, MeBool activate);

  /* hinge for driving. */
  void MEAPI MdtCarWheel2SetHingeAxis(MdtCarWheel2ID j, const MeReal x, const MeReal y, const MeReal z);
  void MEAPI MdtCarWheel2SetHingeLimitedForceMotor(MdtCarWheel2ID j, 
                                                   const MeReal desiredVelocity, 
                                                   const MeReal f_min, 
                                                   const MeReal f_max);

  /* SetSuspension */
  void MEAPI MdtCarWheel2SetSuspension(MdtCarWheel2ID j, const MeReal Kp, const MeReal Kd, const MeReal reference);
  void MEAPI MdtCarWheel2SetSuspensionHiLimit(MdtCarWheel2ID j,MeReal hilimit, const MeReal Kp, const MeReal Kd);
  void MEAPI MdtCarWheel2SetSuspensionLoLimit(MdtCarWheel2ID j,MeReal lolimit, const MeReal Kp, const MeReal Kd);
  void MEAPI MdtCarWheel2ActivateSuspensionLimit(MdtCarWheel2ID J, MeBool activate);
  void MEAPI MdtCarWheel2SuspensionLimitIsActivate(MdtCarWheel2ID J);
  
#define  MdtCarWheel2Destroy(j)  MdtConstraintDestroy(j)
#define  MdtCarWheel2Enable(j)  MdtConstraintEnable(j)
#define  MdtCarWheel2Disable(j)  MdtConstraintDisable(j)
#define  MdtCarWheel2IsEnabled(j) MdtConstraintIsEnabled(j)
#define  MdtCarWheel2SetUserData(j, d)  MdtConstraintSetUserData(j, d)
#define  MdtCarWheel2GetBody(j, bodyindex)  MdtConstraintGetBody(j, bodyindex)
#define  MdtCarWheel2GetUserData(j)  MdtConstraintGetUserData(j)
#define  MdtCarWheel2GetWorld(j)  MdtConstraintGetWorld(j)
#define  MdtCarWheel2GetForce(j, bodyindex, f)  MdtConstraintGetForce(j, bodyindex, f)
#define  MdtCarWheel2GetTorque(j, bodyindex, t)  MdtConstraintGetTorque(j, bodyindex, t)


#ifdef __cplusplus
}
#endif


#endif
