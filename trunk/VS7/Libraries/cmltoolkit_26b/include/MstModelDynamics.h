#ifndef _MSTMODELDYNAMICS_H
#define _MSTMODELDYNAMICS_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:51:51 $
   
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
 * McdModel Dynamics API.
 *
 * When using the Vortex Collision toolkit with the Vortex Dynamics
 * Toolkit, an McdModel may have an associated MdtBody. These functions 
 * are used for setting and modifying a model's associated dynamics
 * information. You can use McdModelGetBody, and then use the Mdt 
 * interface to, for example, set the bodies of a constraint.
 */

#ifdef __cplusplus
extern "C" {
#endif


MdtBodyID         MEAPI McdModelGetBody(const McdModelID m);
void              MEAPI McdModelSetBody(const McdModelID m, const MdtBodyID b);

void              MEAPI McdModelDynamicsReset(const McdModelID m);



/* Dynamic Shortcut Macros */

/**
 * Enable the collision model's dynamics body
 * @deprecated
 */

#define McdModelDynamicsEnable(m) \
    (MdtBodyEnable(McdModelGetBody(m)))

/**
 * Disable the collision model's dynamics body
 * @deprecated
 */

#define McdModelDynamicsDisable(m) \
    (MdtBodyDisable(McdModelGetBody(m)))

/**
 * Test whether the collision model's dynamics body is enabled
 * @deprecated
 */

#define McdModelDynamicsIsEnabled(m) \
    (MdtBodyIsEnabled(McdModelGetBody(m)))


/**
 * Set the transformation matrix of the collision model's dynamics body
 * @deprecated
 */


#define McdModelDynamicsSetTransform(m, t) \
    (MdtBodySetTransform(McdModelGetBody(m), (t)))

/**
 * Set the position of the collision model's dynamics body
 * @deprecated
 */

#define McdModelDynamicsSetPosition(m, x, y, z) \
    (MdtBodySetPosition(McdModelGetBody(m), (x), (y), (z)))

/**
 * Set the quaternion of the collision model's dynamics body
 * @deprecated
 */

#define McdModelDynamicsSetQuaternion(m, qw, qx, qy, qz) \
    (MdtBodySetQuaternion(McdModelGetBody(m), (qw), (qx), (qy), (qz)))

/**
 * Set the linear velocity of the collision model's dynamics body
 * @deprecated
 */

#define McdModelDynamicsSetLinearVelocity(m, vx, vy, vz) \
    (MdtBodySetLinearVelocity(McdModelGetBody(m), (vx), (vy), (vz)))

/**
 * Set the angular velocity of the collision model's dynamics body
 * @deprecated
 */

#define McdModelDynamicsSetAngularVelocity(m, vx, vy, vz) \
    (MdtBodySetAngularVelocity(McdModelGetBody(m), (vx), (vy), (vz)))

/**
 * Set the linear and angular velocity damping of the collision model's dynamics body
 * @see MdtBodySetLinearVelocityDamping(), MdtBodySetAngularVelocityDamping()
 * @deprecated
 */

#define McdModelDynamicsSetDamping(m, lvd, avd) \
    do { \
        MdtBodySetLinearVelocityDamping(McdModelGetBody(m), (lvd)); \
        MdtBodySetAngularVelocityDamping(McdModelGetBody(m), (avd)); \
    } while(0)



/**
 * Get the position of the collision model's dynamics body
 * @deprecated
 */

#define McdModelDynamicsGetPosition(m, p) \
    (MdtBodyGetPosition(McdModelGetBody(m), (p)))

/**
 * Get the quaternion of the collision model's dynamics body
 * @deprecated
 */

#define McdModelDynamicsGetQuaternion(m, q) \
    (MdtBodyGetQuaternion(McdModelGetBody(m), (q)))

/**
 * Get the linear velocity of the collision model's dynamics body
 * @deprecated
 */

#define McdModelDynamicsGetLinearVelocity(m, v) \
    (MdtBodyGetLinearVelocity(McdModelGetBody(m), (v)))

/**
 * Get the angular velocity of the collision model's dynamics body
 * @deprecated
 */
#define McdModelDynamicsGetAngularVelocity(m, v) \
    (MdtBodyGetAngularVelocity(McdModelGetBody(m), (v)))



/**
 * apply a force to the centre of mass of the collision model's dynamics body
 * during the coming time step. Several calls for the same time step result in the forces
 * being accumulated.
 * @deprecated
 */

#define McdModelDynamicsAddForce(m, fx, fy, fz) \
    (MdtBodyAddForce(McdModelGetBody(m), (fx), (fy), (fz)))

/**
 * get the currently accumulated force to be applied to the centre of mass of the
 * collision model's dynamics body during the next time step.
 * @deprecated
 */

#define McdModelDynamicsGetForce(m, f) \
    (MdtBodyGetForce(McdModelGetBody(m), (f)))



/**
 * apply a torque to the collision model's dynamics body
 * during the next time step. Several calls for the same time step result in the torques
 * being accumulated.
 * @deprecated
 */

#define McdModelDynamicsAddTorque(m, tx, ty, tz) \
    (MdtBodyAddTorque(McdModelGetBody(m), (tx), (ty), (tz)))

/**
 * get the torque to be applied to the collision model's dynamics body
 * during the next time step.
 * @deprecated
 */

#define McdModelDynamicsGetTorque(m, t) \
    (MdtBodyGetTorque(McdModelGetBody(m), (t)))


#ifdef __cplusplus
}
#endif

#endif
