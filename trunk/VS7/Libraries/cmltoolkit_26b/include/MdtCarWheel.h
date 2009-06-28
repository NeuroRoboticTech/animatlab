#ifndef _MDTCARWHEEL_H
#define _MDTCARWHEEL_H
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

/** @file MdtCarWheel.h 

    MdtCarWheel API functions.
  
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


  MdtCarWheelID     MEAPI MdtCarWheelCreate(const MdtWorldID w);
  void              MEAPI MdtCarWheelReset(MdtCarWheelID j);
  MdtCarWheelID     MEAPI MdtCarWheelCreateInPool(MePool *pool);





/*
  Car wheel joint accessors.
*/

#define MdtCarWheelGetPosition(j, v)  MdtConstraintGetPosition((j), (v)) 


  MeReal            MEAPI MdtCarWheelGetSteeringAngle(const MdtCarWheelID j);
  MeReal            MEAPI MdtCarWheelGetDesiredSteeringAngle(const MdtCarWheelID j);
  MeReal            MEAPI MdtCarWheelGetSteeringAngleRate(const MdtCarWheelID j);
  void              MEAPI MdtCarWheelGetSteeringAxis(const MdtCarWheelID j, MeVector3 v);
  
  MeReal            MEAPI MdtCarWheelGetHingeAngle(const MdtCarWheelID j);
  MeReal            MEAPI MdtCarWheelGetHingeAngleRate(const MdtCarWheelID j);
  void              MEAPI MdtCarWheelGetHingeAxis(const MdtCarWheelID j, MeVector3 v);
  
  MeReal            MEAPI MdtCarWheelGetSuspensionHeight(const MdtCarWheelID j);
  MeReal            MEAPI MdtCarWheelGetSuspensionRate(const MdtCarWheelID j);
  MeReal            MEAPI MdtCarWheelGetSteeringMotorDesiredVelocity( const MdtCarWheelID j);
  MeReal            MEAPI MdtCarWheelGetSteeringMotorMaxForce( const MdtCarWheelID j);
  MeBool            MEAPI MdtCarWheelIsSteeringLocked(MdtCarWheelID j);
  MeReal            MEAPI MdtCarWheelGetHingeMotorDesiredVelocity( const MdtCarWheelID j);
  MeReal            MEAPI MdtCarWheelGetHingeMotorMaxForce( const MdtCarWheelID j);
  MeReal            MEAPI MdtCarWheelGetSuspensionHighLimit( const MdtCarWheelID j);
  MeReal            MEAPI MdtCarWheelGetSuspensionLowLimit( const MdtCarWheelID j);
  MeReal            MEAPI MdtCarWheelGetSuspensionLimitSoftness( const MdtCarWheelID j);
  MeReal            MEAPI MdtCarWheelGetSuspensionReference( const MdtCarWheelID j);
  MeReal            MEAPI MdtCarWheelGetSuspensionKp(const MdtCarWheelID j);
  MeReal            MEAPI MdtCarWheelGetSuspensionKd(const MdtCarWheelID j);
  




  /*
    Car wheel joint mutators.
  */
  
  void MEAPI MdtCarWheelSetBodies(MdtConstraintID c, const MdtBodyID b1, const MdtBodyID b2);
  void MEAPI MdtCarWheelSetPosition(MdtCarWheelID j, const MeReal x, const MeReal y, const MeReal z);
  void MEAPI MdtCarWheelSetSteeringAxis(MdtCarWheelID j, const MeReal x, const MeReal y, const MeReal z);
  void MEAPI MdtCarWheelSetSteeringLimitedForceMotor( MdtCarWheelID j, const MeReal desiredVelocity, const MeReal forceLimit);
  void MEAPI MdtCarWheelSetSteeringLock(MdtCarWheelID j, const MeBool lock);
  void MEAPI MdtCarWheelSetSteeringAngle(MdtCarWheelID j, MeReal angle);
  void MEAPI MdtCarWheelSetSteeringMaxForce(MdtCarWheelID j, MeReal max_force);
  void MEAPI MdtCarWheelSetHingeAxis(MdtCarWheelID j, const MeReal x, const MeReal y, const MeReal z);
  void MEAPI MdtCarWheelSetHingeLimitedForceMotor(MdtCarWheelID j, const MeReal desiredVelocity, const MeReal forceLimit);

  void MEAPI MdtCarWheelSetSuspension(MdtCarWheelID j, const MeReal Kp, const MeReal Kd,
                            const MeReal limit_softness, const  MeReal lolimit,
                            const MeReal hilimit, const MeReal reference);

  
#define  MdtCarWheelDestroy(j)  MdtConstraintDestroy(j)
#define  MdtCarWheelEnable(j)  MdtConstraintEnable(j)
#define  MdtCarWheelDisable(j)  MdtConstraintDisable(j)
#define  MdtCarWheelIsEnabled(j) MdtConstraintIsEnabled(j)
#define  MdtCarWheelSetUserData(j, d)  MdtConstraintSetUserData(j, d)
#define  MdtCarWheelGetBody(j, bodyindex)  MdtConstraintGetBody(j, bodyindex)
#define  MdtCarWheelGetUserData(j)  MdtConstraintGetUserData(j)
#define  MdtCarWheelGetWorld(j)  MdtConstraintGetWorld(j)
#define  MdtCarWheelGetForce(j, bodyindex, f)  MdtConstraintGetForce(j, bodyindex, f)
#define  MdtCarWheelGetTorque(j, bodyindex, t)  MdtConstraintGetTorque(j, bodyindex, t)


#ifdef __cplusplus
}
#endif


#endif
