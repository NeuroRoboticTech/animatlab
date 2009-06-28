#ifndef _MDTBCLLIMIT_H
#define _MDTBCLLIMIT_H
/* -*- mode: C; -*- */

/*
  Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
  http://www.cm-labs.com

  $Name:  $
   
  Date: $Date: 2005/03/09 16:26:22 $
   
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
 * Constraint library limits.
 *
 * This implements hard and soft limits for the remaining degrees of freedom
 * of constraints.  All angles are specified in radians.
 */

/**
 * MdtBcl Single Limit structure.
 *
 * Specifies behaviour of the constraint at a specific individual limit
 * (either an upper or lower limit) of a motion which is otherwise
 * unconstrained by the joint's hard constraints.
 */
typedef struct MdtBclSingleLimit
{
    /**
     * Minimum (for lower limit) or maximum (for upper limit) linear or
     * angular separation of the attached bodies, projected onto the
     * relevant axis.
     *
     * For a soft limit, the stop is a boundary rather than an absolute
     * limit.
     */
    MeReal stop; 

    /**
     * The compliance constant used for restitution force when a limited
     * joint reaches or goes past the limit stop.
     *
     * This limit property must be zero or positive: the default value is 0
     * which corresponds to infinite stiffness.  If the compliance is set
     * to MEINFINITY and damping is set to 0 for a given single limit it is
     * effectively deactivated.
     */
    MeReal compliance;

    /**
     * The damping term for this limit.
     *
     * This must not be negative: the default value is zero.  
     */
    MeReal damping;

    /**
     * The ratio of rebound velocity to impact velocity
     * when the joint reaches the low or high stop.
     *
     * This is used only if the limit hardness exceeds the damping
     * threshold.  Restitution must be in the range zero to one inclusive:
     * the default value is 0.
     */
    MeReal restitution;

    /**
     * The velocity may be set for an upper or lower limit. If the 
     * two limits cross, the constraint will stop integrating the limits
     * forward, leaving them in their last good position.
     *
     */
    MeReal velocity;
}
MdtBclSingleLimit;


/**
 *  @internal Not ready for primetime yet.
 */
typedef enum
{
    kMdtConstraintControlFree,
    kMdtConstraintControlLockActive,
    kMdtConstraintControlMotorActive,
    kMdtConstraintControlControllerActive,
} MdtConstraintControlStateEnum;

typedef enum
{
    kMdtLimitInactive,
    kMdtLimitActive,     //meaning, no lock or motor on, but limits respected.
    kMdtLimitExceeded,
} MdtLimitStateEnum;

/**
 * Indicates the type of the limit.
 */
typedef enum
{
    kMdtLimitTypeLinear = 0,   /*!< linear limit */
    kMdtLimitTypeAngular,      /*!< angular limit */
    kMdtLimitTypeDistance,    /*!< angular limit */
    kMdtLimitTypeCount
} MdtLimitTypeEnum;




/**
 * MdtBcl Limit structure.
 *
 * Specifies behaviour of the constraint for a motion which is otherwise
 * unconstrained by the joint's hard constraints.  This motion may be
 * constrained at either an upper limit or a lower limit, or both, or
 * actuated (powered) by a force-limited motor between such limits or in the
 * absence of limits.
 */
typedef struct MdtBclLimit
{
    /**
     * The overshoot distance.
     *
     * This is set to zero between the limits; otherwise it is the amount by
     * which the joint has overshot its limit.  This is positive beyond the
     * upper limit, negative beyond the lower.
     */
    MeReal overshoot;

    /**
     * Relative position of the attached bodies, projected onto the
     * relevant axis.
     */
    MeReal position;

    /**
     *  user specified position lock: when set and current state is
     *  kLockActive, the  constraint will maintain the joint coordinate 
     * specified in position_lock.
     */
    MeReal lock_position;

    /** Velocity of the lock_position. 
        The lock_position will be updated each step with this velocity
        until it reaches one of the limit. 
    */
    MeReal lock_velocity;

    /**
     *  The following may be used to make a lock on a limit act as a spring
     */
    MeReal lock_compliance; 
    MeReal lock_damping; 
    MeReal lock_fmax;
    MeReal lock_fmin;

    /**
     * Relative position of the attached bodies in the previous time-step.
     */
    MeReal previous_position;

    /**
     * An offset used to transform the measured relative position
     * coordinate ("position") into the user's coordinate system.
     * This member variable (and not "position") is set by a call
     * to MdtLimitSetPosition.
     */
    MeReal m_offset;

    /**
     * Relative linear or angular velocity of the attached
     * bodies, projected onto the relevant axis.
     */
    MeReal velocity;

    /**
     * The specific properties of the lower and upper limits.
     */
    MdtBclSingleLimit limit[2];

    /**
     * Powered joint, desired velocity.
     *
     * A lower limiting velocity may be achieved if the attached bodies are
     * subject to velocity or angular velocity damping.
     */
    MeReal motor_desired_vel;

    /**
     * Powered joint, maximum force that should be
     * applied to reach the desired velocity.  only for motor
     */
    MeReal motor_fmax;
    MeReal motor_fmin;
    MeReal motor_loss; 

    MdtConstraintControlStateEnum    currentControlState;
    MdtLimitStateEnum                currentLimitState;
    MdtLimitTypeEnum                 type;
    int                              coord;
    int                              bodyRef;
    int                              lastStepRow;

    MeBool bForceCalculatePosition;
    MeBool bMotorStopAtLock;

}
MdtBclLimit;

#endif
