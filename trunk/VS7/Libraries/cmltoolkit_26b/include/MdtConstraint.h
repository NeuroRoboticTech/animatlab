#ifndef _MDTCONSTRAINT_H
#define _MDTCONSTRAINT_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/11/10 22:39:49 $
   
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

/** 
 * @file
 * MdtConstraint API functions.
 *
 * These functions setup the basic geometry of all constraints.  Thought the
 * specific meaning of the position and axis variable differs for each
 * constraint, the data is accessed in a uniform way through this API.  Most
 * of the data we work with here are the attachement points, one for each
 * body constrained by the specific joint or constraint.  These attachement
 * points are specified with on 4x4 transformation matrix for each body
 * involved in the constraint. 
 *
 * The MdtConstraint API specification also allows reading of the actual
 * force and torque which were applied to maintain this constraint during
 * the last time step.
 *
 * Please note: The MdtConstraintDOF* functions are experimental and subject to change
 *
 */

/**
 * @example PrismaticCable.c
 * 
 * This example demonstrates how a cable may be created using prismatic joint with 
 * relaxed constraints.
 *
 */


#include "MePrecision.h"
#include "MdtTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*
 *  Constraint functions are common to any type of constraint.
 */
void              MEAPI MdtConstraintDestroy(const MdtConstraintID c);
void              MEAPI MdtConstraintDestroyInPool(const MdtConstraintID c, MePool *pool);
void              MEAPI MdtConstraintEnable(const MdtConstraintID c);
void              MEAPI MdtConstraintEnableBodies(MdtConstraintID c);
void              MEAPI MdtConstraintDisable(const MdtConstraintID c);
MeBool            MEAPI MdtConstraintIsEnabled(const MdtConstraintID c);
int               MEAPI MdtConstraintGetTypeId(const MdtConstraintID c);
void              MEAPI MdtConstraintResetDynamics(const MdtConstraintID c);

/*
 * Constraint accessors.
 */

MdtBodyID         MEAPI MdtConstraintGetBody(const MdtConstraintID c,
                            const unsigned int bodyindex);
void             *MEAPI MdtConstraintGetUserData(const MdtConstraintID c);
MdtWorldID        MEAPI MdtConstraintGetWorld(const MdtConstraintID c);
void              MEAPI MdtConstraintGetForce(const MdtConstraintID c,
                            const unsigned int bodyindex, MeVector3 f);
void              MEAPI MdtConstraintGetTorque(const MdtConstraintID c,
                            const unsigned int bodyindex, MeVector3 t);
void              MEAPI MdtConstraintGetPosition(const MdtConstraintID c,
                            MeVector3 p);
void              MEAPI MdtConstraintBodyGetPosition(const MdtConstraintID c,
                            const unsigned int bodyindex, MeVector3 p);
void              MEAPI MdtConstraintBodyCOMGetPositionRel(const MdtConstraintID c,
                                       const unsigned int bodyindex, MeVector3 a);
void              MEAPI MdtConstraintBodyGetPositionRel(const MdtConstraintID c,
                            const unsigned int bodyindex, MeVector3 p);
void              MEAPI MdtConstraintGetAxis(const MdtConstraintID c,
                            MeVector3 a);
void              MEAPI MdtConstraintGetAxes(const MdtConstraintID c,
                            MeVector3 p,  MeVector3 o );

void              MEAPI MdtConstraintBodyGetAxis(const MdtConstraintID c,
                                    const unsigned int bodyindex,
                                    MeVector3 p);
/* old name for above API call, deprecated; */
#define           MdtConstraintGetBothAxes(c, p, o) \
                  MdtConstraintGetAxes((c), (p), (o))
void              MEAPI MdtConstraintBodyGetAxes(const MdtConstraintID c,
                                        const unsigned int bodyindex,
                                        MeVector3 p,  MeVector3 o );
/* old name for above API call, deprecated; */
#define           MdtConstraintBodyGetBothAxes(c, b, p, o) \
                  MdtConstraintBodyGetAxes((c), (b), (p), (o))
void              MEAPI MdtConstraintBodyGetAxesRel(const MdtConstraintID c,
                                        const unsigned int bodyindex,
                                        MeVector3 p,  MeVector3 o );

/*
  Constraint mutators.
*/

void              MEAPI MdtConstraintSetBody(MdtConstraintID c, int i, 
                                                 const MdtBodyID b);
void              MEAPI MdtConstraintSetBodies(MdtConstraintID c,
                            const MdtBodyID b1, const MdtBodyID b2);
void              MEAPI MdtConstraintSetUserData(MdtConstraintID c,
                            void *d);
void              MEAPI MdtConstraintBodySetPosition(MdtConstraintID c,
                            const unsigned int bodyindex,
                            const MeReal x, const MeReal y, const MeReal z);
void              MEAPI MdtConstraintBodySetPositionRel(MdtConstraintID c,
                            const unsigned int bodyindex,
                            const MeReal x, const MeReal y, const MeReal z);
void              MEAPI MdtConstraintBodyCOMSetPositionRel(MdtConstraintID c,
                            const unsigned int bodyindex,
                            const MeReal x, const MeReal y, const MeReal z);
void              MEAPI MdtConstraintSetPosition(MdtConstraintID c,
                            const MeReal x, const MeReal y, const MeReal z);
void              MEAPI MdtConstraintSetAxis(MdtConstraintID c,
                            const MeReal px, const MeReal py, const MeReal pz);
void              MEAPI MdtConstraintSetAxes(MdtConstraintID c,
                            const MeReal px, const MeReal py, const MeReal pz,
                            const MeReal ox, const MeReal oy, const MeReal oz);

void              MEAPI MdtConstraintBodySetAxes(MdtConstraintID c,
                            const unsigned int bodyindex,
                            const MeReal px, const MeReal py, const MeReal pz,
                            const MeReal ox, const MeReal oy, const MeReal oz);

void MEAPI        MdtConstraintBodySetAxis(MdtConstraintID c,
                                    const unsigned int bodyindex,
                                    const MeReal nx, const MeReal ny, 
                                    const MeReal nz);

void              MEAPI MdtConstraintBodySetAxesRel(MdtConstraintID c,
                            const unsigned int bodyindex,
                            const MeReal px, const MeReal py, const MeReal pz,
                            const MeReal ox, const MeReal oy, const MeReal oz);

int               MEAPI MdtConstraintGetLimitCount(const MdtConstraintID c);
MdtLimitID        MEAPI MdtConstraintGetLimit(const MdtConstraintID c);
MdtLimitID        MEAPI MdtConstraintGetIndexedLimit(const MdtConstraintID c, int i);
MeReal            MEAPI MdtConstraintGetPhi(const MdtConstraintID c, int i);
MeReal            MEAPI MdtConstraintGetLambda(const MdtConstraintID c, int i);
MeReal            MEAPI MdtConstraintLimitGetForce(const MdtConstraintID c, int i);

MeReal            MEAPI MdtConstraintLimitGetPosition(const MdtConstraintID c, int i);
MeReal            MEAPI MdtConstraintLimitGetCurrentPosition(const MdtConstraintID c, int i);

void              MEAPI MdtConstraintLimitSetCurrentPosition(const MdtConstraintID c, int i,  const MeReal NewPosition );




void              MEAPI MdtConstraintRelaxConstraint(const MdtConstraintID c,
                                                     int iConstraint,MeReal
                                                     stiffness, MeReal
                                                     damping, MeReal loss);
void              MEAPI MdtConstraintGetRelaxConstraint(const MdtConstraintID c, 
                                                     int iConstraint, 
                                                     MeReal *stiffness, 
                                                     MeReal *damping, 
                                                     MeReal *loss);

int MEAPI MdtConstraintGetModes(const MdtConstraintID c, int iConstraint);
void              MEAPI MdtConstraintDisableRelaxation(const MdtConstraintID c,int iConstraint);
void              MEAPI MdtConstraintEnableRelaxation(const MdtConstraintID c,int iConstraint);
MeBool            MEAPI MdtConstraintIsConstraintRelaxed(const MdtConstraintID c,int iConstraint);


MeBool            MEAPI MdtConstraintLockConstraint (const MdtConstraintID c);
void              MEAPI MdtConstraintUnlockConstraint (const MdtConstraintID c);
MeBool            MEAPI MdtConstraintIsLockedConstraint (const MdtConstraintID c);
MeBool            MEAPI MdtConstraintIsLockableConstraint(const MdtConstraintID c);



/*
  Iterators.
*/

MdtConstraintID   MEAPI MdtConstraintGetFirst(const MdtWorldID w);
MdtConstraintID   MEAPI MdtConstraintGetNext(const MdtConstraintID c);


#ifdef __cplusplus
}
#endif


#endif
