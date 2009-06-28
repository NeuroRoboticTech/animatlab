#ifndef _MDTWORLD_H
#define _MDTWORLD_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name: t-pre-multithread-050318 $
   
   Date: $Date: 2005/03/16 22:03:19 $
   
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
 * MdtWorld API functions.

 This provides interfaces to set a variety of parameters for the internal
 solvers and integrators.
 */

#include "MePrecision.h"
#include "MdtTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*
  World functions.
*/

    MdtWorldID        MEAPI MdtWorldCreate(const unsigned int maxBodies,
                                           const unsigned int maxConstraints);

    void MEAPI MdtWorldResize(MdtWorldID w, int maxBodies, int maxConstraints);
    void              MEAPI MdtWorldReset(const MdtWorldID w);
    void              MEAPI MdtWorldResetDynamics(const MdtWorldID w);
    void              MEAPI MdtWorldDestroy(const MdtWorldID w);
    void              MEAPI MdtWorldStep(const MdtWorldID w, MeReal stepSize);
    void              MEAPI MdtWorldStepOne(const MdtWorldID w);

/*
  World accessors
*/

    int               MEAPI MdtWorldGetTotalBodies(const MdtWorldID w);
    int               MEAPI MdtWorldGetEnabledBodies(const MdtWorldID w);
    int               MEAPI MdtWorldGetTotalConstraints(const MdtWorldID w);
    int               MEAPI MdtWorldGetEnabledConstraints(const MdtWorldID w);
    int               MEAPI MdtWorldGetMaxBodies(const MdtWorldID w);
    int               MEAPI MdtWorldGetMaxConstraints(const MdtWorldID w);
    MeReal            MEAPI MdtWorldGetTimeStep(const MdtWorldID w);
    void              MEAPI MdtWorldGetGravity(const MdtWorldID w, MeVector3 g);
    int               MEAPI MdtWorldGetAutoDisable(const MdtWorldID w);
    MeReal            MEAPI MdtWorldGetAutoDisableVelocityThreshold( const MdtWorldID w);
    MeReal            MEAPI MdtWorldGetAutoDisableAngularVelocityThreshold( const MdtWorldID w);
    MeReal            MEAPI MdtWorldGetAutoDisableAccelerationThreshold( const MdtWorldID w);
    MeReal            MEAPI MdtWorldGetAutoDisableAngularAccelerationThreshold( const MdtWorldID w);
    int               MEAPI MdtWorldGetAutoDisableAliveWindow(const MdtWorldID w);
    int               MEAPI MdtWorldGetMaxMemoryPoolUsed(const MdtWorldID w);
    MeChunkOverflowCB MEAPI MdtWorldGetMemoryPoolOverflowCB(const MdtWorldID w);


/*
  World mutators
*/

    void  MEAPI MdtWorldSetTimeStep(const MdtWorldID w, MeReal e);
    void  MEAPI MdtWorldSetGravity(const MdtWorldID w, MeReal gx, MeReal gy, MeReal gz);
    void  MEAPI MdtWorldResetForces(const MdtWorldID w);
    void  MEAPI MdtWorldSetAutoDisable(const MdtWorldID w, const MeBool d);
    void  MEAPI MdtWorldSetAutoDisableVelocityThreshold( const MdtWorldID w, const MeReal vt);
    void  MEAPI MdtWorldSetAutoDisableAngularVelocityThreshold( const MdtWorldID w, MeReal avt);
    void  MEAPI MdtWorldSetAutoDisableAccelerationThreshold( const MdtWorldID w, MeReal at);
    void  MEAPI MdtWorldSetAutoDisableAngularAccelerationThreshold( const MdtWorldID w, 
                                                                    MeReal aat);
    void  MEAPI MdtWorldSetAutoDisableAliveWindow( const MdtWorldID w, int aw);
    void  MEAPI MdtWorldSetMemoryPool(const MdtWorldID w, void *const m, int size);
    void  MEAPI MdtWorldSetMemoryPoolOverflowCB(const MdtWorldID w, const MeChunkOverflowCB cb);

    void  MEAPI MdtWorldSetLinearStiffness     (const MdtWorldID w, MeReal x);
    void  MEAPI MdtWorldSetLinearCompliance    (const MdtWorldID w, MeReal x);
    void  MEAPI MdtWorldSetLinearDamping       (const MdtWorldID w, MeReal x);
    void  MEAPI MdtWorldSetLinearKineticGain   (const MdtWorldID w, MeReal x);
    void  MEAPI MdtWorldSetLinearKineticLoss   (const MdtWorldID w, MeReal x);
    void  MEAPI MdtWorldSetAngularStiffness     (const MdtWorldID w, MeReal x);
    void  MEAPI MdtWorldSetAngularCompliance    (const MdtWorldID w, MeReal x);
    void  MEAPI MdtWorldSetAngularDamping       (const MdtWorldID w, MeReal x);
    void  MEAPI MdtWorldSetAngularKineticLoss   (const MdtWorldID w, MeReal x);
    void  MEAPI MdtWorldSetAngularKineticGain   (const MdtWorldID w, MeReal x);

    void  MEAPI MdtWorldSetComplianceParameters (const MdtWorldID w, 
        MeReal linearCompliance, MeReal angularCompliance, MeReal linearDamping, 
        MeReal angularDamping, MeReal linearLoss, MeReal angularLoss);

    MeReal  MEAPI MdtWorldGetLinearStiffness   (const MdtWorldID w);
    MeReal  MEAPI MdtWorldGetLinearCompliance  (const MdtWorldID w);
    MeReal  MEAPI MdtWorldGetLinearDamping     (const MdtWorldID w);
    MeReal  MEAPI MdtWorldGetLinearKineticGain (const MdtWorldID w);
    MeReal  MEAPI MdtWorldGetLinearKineticLoss (const MdtWorldID w);
    MeReal  MEAPI MdtWorldGetAngularStiffness   (const MdtWorldID w);
    MeReal  MEAPI MdtWorldGetAngularCompliance  (const MdtWorldID w);
    MeReal  MEAPI MdtWorldGetAngularDamping     (const MdtWorldID w);
    MeReal  MEAPI MdtWorldGetAngularKineticGain (const MdtWorldID w);
    MeReal  MEAPI MdtWorldGetAngularKineticLoss (const MdtWorldID w);

    void MEAPI MdtWorldSetComplianceParametersFromEG(const MdtWorldID world, MeReal epsilon,MeReal gamma,MeReal h);


    MdtStepperModel   MEAPI MdtWorldSetIntegrationModel(MdtWorldID w, MdtStepperModel m);
    MdtStepperModel   MEAPI MdtWorldGetIntegrationModel(MdtWorldID w);

    MdtLinearSolver    MEAPI MdtWorldSetLinearSolver(MdtWorldID w, MdtLinearSolver m);
    MdtLinearSolver    MEAPI MdtWorldGetLinearSolver(MdtWorldID w);

    MdtStepperOptions  MEAPI MdtWorldSetStepperOptions(MdtWorldID w, MdtStepperOptions m);
    MdtStepperOptions  MEAPI MdtWorldGetStepperOptions(MdtWorldID w);


    int          MEAPI MdtWorldLCPSetMaxIterations(MdtWorldID w, int m);
    int          MEAPI MdtWorldLCPSetMaxBlockIterations(MdtWorldID w, int m);
    int          MEAPI MdtWorldLCPSetMaxScaledBoxIterations(MdtWorldID w, int m);
    MeReal       MEAPI MdtWorldLCPSetScaledBoxTolerance(MdtWorldID w, MeReal m);
    MeReal       MEAPI MdtWorldLCPSetDegeneracyFilter(MdtWorldID w, const MeReal m);
    MeReal       MEAPI MdtWorldLCPSetTolerance(MdtWorldID w, const MeReal m);
    MdtLCPSolver MEAPI MdtWorldLCPSetSolverFunction(MdtWorldID w, MdtLCPSolver m);
    int          MEAPI MdtWorldLCPSetH5Dumping(MdtWorldID w, int m);
    void         MEAPI MdtWorldLCPSetH5DumpingDesc(MdtWorldID w, const char *t);
    MdtConstraintSolver MEAPI MdtWorldSetConstraintSolver(MdtWorldID w, MdtConstraintSolver m);
    int MEAPI     MdtWorldSetCtIterativeSolverMaxIt(const MdtWorldID w, int m);
    MeReal MEAPI     MdtWorldSetCtIterativeSolverTolerance(const MdtWorldID w, MeReal t);



    int          MEAPI MdtWorldLCPGetMaxBlockIterations(const MdtWorldID w);
    int          MEAPI MdtWorldLCPGetMaxIterations(const MdtWorldID w);
    MeReal       MEAPI MdtWorldLCPGetScaledBoxTolerance(MdtWorldID w);
    int          MEAPI MdtWorldLCPGetScaledBoxIterations(MdtWorldID w);
    MeReal       MEAPI MdtWorldLCPGetScaledBoxError(MdtWorldID w);
    int          MEAPI MdtWorldLCPGetMaxScaledBoxIterations(const MdtWorldID w);
    MeReal       MEAPI MdtWorldLCPGetDegeneracyFilter(const MdtWorldID w);
    MeReal       MEAPI MdtWorldLCPGetTolerance(const MdtWorldID w);
    MdtLCPSolver MEAPI     MdtWorldLCPGetSolverFunction(const MdtWorldID w);
    MdtConstraintSolver MEAPI     MdtWorldGetConstraintSolver(const MdtWorldID w);
    int MEAPI     MdtWorldGetCtIterativeSolverMaxIt(const MdtWorldID w);
    MeReal MEAPI     MdtWorldGetCtIterativeSolverTolerance(const MdtWorldID w);





/*
  World others.
*/
    void          MEAPI MdtWorldForAllConstraints(const MdtWorldID w, MdtConstraintIteratorCB cb, void* ccbdata);
    void          MEAPI MdtWorldInsertBody(MdtWorldID w, MdtBodyID b);
    void          MEAPI MdtWorldRemoveBody(MdtBodyID b);

    void          MEAPI MdtWorldInsertConstraint(MdtWorldID w, MdtConstraintID c);
    void          MEAPI MdtWorldRemoveConstraint(MdtConstraintID c);

    void MEAPI MdtWorldSetPartitionGroupingMode(MdtWorldID w, MdtPartitionGroupingMode m); 
    void MEAPI MdtWorldSetDefaultPartitionGroupingMode(MdtWorldID w);
    MdtPartitionGroupingMode MEAPI MdtWorldGetPartitionGroupingMode(MdtWorldID w);


#ifdef __cplusplus
}
#endif


#endif
