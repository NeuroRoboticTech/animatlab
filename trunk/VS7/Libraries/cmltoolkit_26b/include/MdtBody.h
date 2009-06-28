#ifndef _MDTBODY_H
#define _MDTBODY_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/03/16 22:03:18 $
   
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
 * MdtBody API functions.
 * The MdtBody represents dynamic behaviour for an McdModel.
 * Normally, it is created using MstModelAndBodyCreate for objects
 * with collision, but can also
 * be created separately using MdtBodyCreate for dynamics-only objects.
 *
 * An MdtBody can have several states. It can be frozen, in which case
 * it will never be moved or processed by dynamics.
 * It can be disabled, in which case it is not moved until a collision
 * occurs with it. Bodies are automatically disabled by default when
 * they are detected not to be moving for efficiency.
 */

#include "MePrecision.h"
#include "MdtTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif


    MdtBodyID         MEAPI MdtBodyCreate(const MdtWorldID w);
    MdtBodyID         MEAPI MdtBodyCreateInPool(MePool *pool);
    void              MEAPI MdtBodyReset(const MdtBodyID b);
    void              MEAPI MdtBodyDestroy(const MdtBodyID b);
    void              MEAPI MdtBodyDestroyInPool(const MdtBodyID b, MePool*);
    MdtBodyID         MEAPI MdtBodyCopy(const MdtBodyID b, const MdtWorldID w);
    void              MEAPI MdtBodyEnable(const MdtBodyID b);
    void              MEAPI MdtBodyDisable(const MdtBodyID b);
    MeBool            MEAPI MdtBodyIsEnabled(const MdtBodyID b);
    void              MEAPI MdtBodyEnableConnectedBodies(MdtBodyID body);
    void              MEAPI MdtBodyLog(const MdtBodyID b);
    void              MEAPI MdtBodyDisableConstraints(const MdtBodyID b);
    void              MEAPI MdtBodyDestroyConstraints(const MdtBodyID b);

    void              MEAPI MdtBodyMakeKinematic(const MdtBodyID b);
    void              MEAPI MdtBodyMakeDynamic(const MdtBodyID b);
    void              MEAPI MdtBodyUnFreeze(const MdtBodyID b);
    void              MEAPI MdtBodyFreeze(const MdtBodyID b);
    MeBool            MEAPI MdtBodyIsFrozen(const MdtBodyID b);
    MeBool            MEAPI MdtBodyIsKinematic(const MdtBodyID b);
    MeBool            MEAPI MdtBodyIsDynamic(const MdtBodyID b);

/* Body accessors */

    MdtWorldID        MEAPI MdtBodyGetWorld(const MdtBodyID b);
    int               MEAPI MdtBodyGetPartition(const MdtBodyID b);
    MeMatrix4Ptr      MEAPI MdtBodyGetTransformPtr(const MdtBodyID b);
    void              MEAPI MdtBodyGetTransform(const MdtBodyID b,MeMatrix4 m);
    void             *MEAPI MdtBodyGetUserData(const MdtBodyID b);
    MeReal            MEAPI MdtBodyGetMass(const MdtBodyID b);
    void              MEAPI MdtBodyGetInertiaTensor(const MdtBodyID b, MeMatrix3 i);
    void              MEAPI MdtBodyGetPosition(const MdtBodyID b, MeVector3 p);
    void              MEAPI MdtBodyGetQuaternion(const MdtBodyID b, MeVector4 q);
    void              MEAPI MdtBodyGetOrientation(const MdtBodyID b, MeMatrix3 R);
    void              MEAPI MdtBodyGetLinearVelocity(const MdtBodyID b,
                                                     MeVector3 v);
    MeReal           *MEAPI MdtBodyGetLinearVelocityPtr(const MdtBodyID b);
    void              MEAPI MdtBodyGetAngularVelocity(const MdtBodyID b,
                                                      MeVector3 v);
    MeReal           *MEAPI MdtBodyGetAngularVelocityPtr(const MdtBodyID b);
    void              MEAPI MdtBodyGetVelocityAtPoint(const MdtBodyID body,
                                                      MeVector3 p, MeVector3 v);
    void              MEAPI MdtBodyGetLinearAcceleration(const MdtBodyID b,
                                                         MeVector3 v);
    void              MEAPI MdtBodyGetAngularAcceleration(const MdtBodyID b,
                                                          MeVector3 v);
    void              MEAPI MdtBodyGetFastSpinAxis(const MdtBodyID b, MeVector3 v);
    MeReal            MEAPI MdtBodyGetLinearVelocityDamping(const MdtBodyID b);
    MeReal            MEAPI MdtBodyGetAngularVelocityDamping(const MdtBodyID b);
    void              MEAPI MdtBodyGetForce(const MdtBodyID b, MeVector3 v);
    void              MEAPI MdtBodyGetTorque(const MdtBodyID b, MeVector3 v);
    MeReal            MEAPI MdtBodyGetKineticEnergy(const MdtBodyID b);
    int               MEAPI MdtBodyGetEnabledTime(const MdtBodyID b);
    void              MEAPI MdtBodyGetCenterOfMassRelativeTransform(
        const MdtBodyID b, MeMatrix4 t);
    void              MEAPI MdtBodyGetCenterOfMassPosition(const MdtBodyID b,
                                                           MeVector3 pos);
    MeMatrix4Ptr      MEAPI MdtBodyGetCenterOfMassTransformPtr(const MdtBodyID b);
    MeReal            MEAPI MdtBodyGetSafeTime(const MdtBodyID b);

/*
  Body mutators
*/

    void              MEAPI MdtBodySetUserData(const MdtBodyID b, void *d);
    void              MEAPI MdtBodySetTransform(const MdtBodyID b,
                                                const MeMatrix4 tm);
    void              MEAPI MdtBodySetTransformAndUpdateConstraintAttachments(const MdtBodyID b, const MeMatrix4 tm);

    void              MEAPI MdtBodySetMassAndScaleInertiaTensor(const MdtBodyID b, const MeReal mass);
    void              MEAPI MdtBodySetMass(const MdtBodyID b, const MeReal mass);
    void              MEAPI MdtBodySetInertiaTensor(const MdtBodyID b,
                                                    const MeMatrix3 i);
    void              MEAPI MdtBodySetSphericalInertiaTensor(const MdtBodyID b,
                                                             const MeReal i);
    void              MEAPI MdtBodySetPosition(const MdtBodyID b,
                                               const MeReal x, const MeReal y, const MeReal z);
    void              MEAPI MdtBodySetOrientation(const MdtBodyID b,
                                                  const MeMatrix3 R);
    void              MEAPI MdtBodySetQuaternion(const MdtBodyID b,
                                                 const MeReal qw, const MeReal qx, const MeReal qy,
                                                 const MeReal qz);
    void              MEAPI MdtBodySetLinearVelocity(const MdtBodyID b,
                                                     const MeReal dx, const MeReal dy, const MeReal dz);
    void              MEAPI MdtBodySetAngularVelocity(const MdtBodyID b,
                                                      const MeReal wx, const MeReal wy, const MeReal wz);
    void              MEAPI MdtBodySetLinearVelocityDamping(const MdtBodyID b,
                                                            const MeReal d);
    void              MEAPI MdtBodySetAngularVelocityDamping(const MdtBodyID b,
                                                             const MeReal d);
    void              MEAPI MdtBodyMoveTo(MdtBodyID b, const MeMatrix4 newTm, MeReal dt);

    void              MEAPI MdtBodySetFastSpinAxis(const MdtBodyID b,
                                                   const MeReal x, const MeReal y, const MeReal z);
    void              MEAPI MdtBodySetNoFastSpinAxis(const MdtBodyID b);
    void              MEAPI MdtBodySetCenterOfMassRelativeTransform(
        const MdtBodyID b, const MeMatrix4 t);
    void              MEAPI MdtBodySetSafeTime(const MdtBodyID b, const MeReal t);
/*
  Iterators.
*/

    MdtBodyID         MEAPI MdtBodyGetFirst(const MdtWorldID w);
    MdtBodyID         MEAPI MdtBodyGetNext(const MdtBodyID b);


/*
  Body others.
*/
    void              MEAPI MdtBodyForAllConstraints(const MdtBodyID b,
                                                     MdtConstraintIteratorCB cb, void* ccbdata);
    void              MEAPI MdtBodyResetForces(const MdtBodyID b);
    void              MEAPI MdtBodyGetLastForces(const MdtBodyID b, MeVector3Ptr lastForce, 
                                                 MeVector3Ptr lastTorque);

    void              MEAPI MdtBodyResetDynamics(const MdtBodyID b);
    void              MEAPI MdtBodyAddForce(const MdtBodyID b,
                                            const MeReal fx, const MeReal fy, const MeReal fz);
    void              MEAPI MdtBodyAddForceAtPosition(const MdtBodyID b,
                                                      const MeReal fx, const MeReal fy, const MeReal fz,
                                                      const MeReal px, const MeReal py, const MeReal pz);
    void              MEAPI MdtBodyAddTorque(const MdtBodyID b,
                                             const MeReal tx, const MeReal ty, const MeReal tz);
    void              MEAPI MdtBodyAddImpulse(const MdtBodyID b,
                                              const MeReal ix, const MeReal iy, const MeReal iz);
    void              MEAPI MdtBodyAddImpulseAtPosition(const MdtBodyID b,
                                                        const MeReal ix, const MeReal iy, const MeReal iz,
                                                        const MeReal px, const MeReal py, const MeReal pz);
    MeBool            MEAPI MdtBodyIsMovingTest(const MdtBodyID b,
                                                const MdtAutoDisableParams* adparams);

    void              MEAPI MdtBodyFrictionDirectionPerpendicularToAxisCB(
        const MdtBodyID body, MdtContactID contacts);
    void              MEAPI MdtBodyFrictionDirectionParallelToAxisCB(
        const MdtBodyID body, MdtContactID contacts);

    void              MEAPI MdtBodySetFrictionDirectionCB(
        MdtBodyID body, MdtFrictionDirectionCB cb, void* userData);

    void              MEAPI MdtBodySetDecimation(MdtBodyID body,MeReal decimation);
    void              MEAPI MdtBodyDecimate(MdtBodyID body,MeBool bSetDecimation);

    void              MEAPI MdtBodySetFrictionDirectionCB(
        MdtBodyID body, MdtFrictionDirectionCB cb, void* userData);


    void MEAPI MdtBodySetGroup(const MdtBodyID b, int g); 
    int MEAPI MdtBodyGetGroup(const MdtBodyID b);
    void MEAPI MdtBodySetUserGroup(const MdtBodyID b, int g); 
    int MEAPI MdtBodyGetUserGroup(const MdtBodyID b);


#ifdef __cplusplus
}
#endif


#endif
