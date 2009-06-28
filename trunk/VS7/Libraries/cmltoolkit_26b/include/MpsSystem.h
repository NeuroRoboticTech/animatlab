#ifndef _MPSSYSTEM_H
#define _MPSSYSTEM_H
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

#include "MePrecision.h"
#include "MpsEnums.h"
#include "MpsEmitter.h"
#include "MpsLinkedLists.h"
#include "MpsObjects.h"

/**
   \file
   These are the particle systems.
   MpsSystem controls storage and settings for Particle Systems.
*/

/**
   Data for an Emitted Particle.
   The particles emitted have several parameters.  Above is a list of attributes.
*/

typedef struct _MpsEmissionData {
  /** the emitter this system belongs to */
    MpsEmitter *m_PE;
  /** average speed of the emitted particle */
    MeReal    m_SpeedMean;
  /** ~variance of the emittee's speed */
    MeReal    m_SpeedStDev;
  /** random noise [angle off normal] for emitted particles direction */
    MeReal    m_VelocityAngularSpread;
  /** rate at which to generate new particles from this emitter */
    MeReal    m_Rate;
  /** number of sub-particles an emitter will create */
    int       m_NumToGenerate;
} MpsEmissionData;

extern MpsEmissionData *NewEmissionData(MpsEmitter *);
extern void DeleteEmissionData(MpsEmissionData *);

/**
   Particle System properties and parameters.
   These are the controls for particles and their spawn.  Allows for complete control over 
   properties inherent to all particles in a given system.
*/
struct _MpsSystem {
    /** number of active particles */
    int       m_NumberOfActiveParticles;
    /** maximum number of particles */
    int       m_Size;
    /** accumulated number of particles created from emitters */
    int       m_TotalNumberCreated;
    /** Particle property data */
    MeVector3 *m_ParticlePosition;
    MeVector3 *m_ParticlePrevPosition;
    /** Particle property data */
    MeVector3 *m_ParticleVelocity;
    /** Particle property data */
    MeVector3 *m_ParticleForce;
    /** Particle property data */
    MeVector3 *m_ParticleOrientation;
    /** Particle property data */
    MeReal   *m_ParticleMass;
    /** Particle property data */
    MeReal   *m_ParticleCharge;
    /** Particle property data */
    MeReal   *m_ParticleAge;
    /** Particle property data */
    MeReal   *m_ParticleDecayCriterion;
    MeU8     *m_ParticleDecayFlag;
    /** The number of times that spawning rounds occur */
    int      *m_ParticleTimesSpawned;
    /** (void *)ptr for optional user storage  */
    void    **m_UserData;
    /** Particle Decay methods and criteria */
    MpsParticleDecayMethod m_DecayMethod;
    /** Particle Decay methods and criteria */
    MeReal    m_DecayCriterionMean;
    /** Particle Decay methods and criteria */
    MeReal    m_DecayCriterionStDev;
    /** Average particle mass and charge + standard deviations */
    MeReal    m_MassMean;
    /** Average particle mass and charge + standard deviations */
    MeReal    m_MassStDev;
    /** Average particle mass and charge + standard deviations */
    MeReal    m_ChargeMean;
    /** Average particle mass and charge + standard deviations */
    MeReal    m_ChargeStDev;
    /** Emitter data linked-list */
    MpsLinkedListNode *m_EmitterList;
    /** Current data linked-list */
    MpsLinkedListNode *m_CurrentData;
    /** Particle destruction data */
    int      *m_ParticleIndicesToDecay;
    /** decay this many particles from the m_ParticleIndicesToDecay list */
    int       m_NumberOfParticlesToDecay;
    /** Spawning (on decay) parameters */
    MpsEmitter *m_SpawnEmitter;
    /** Fraction of particles that will become spawners */
    MeReal    m_FractionToSpawn;
    /** Number of new particles created when a  spawn occurs */
    int       m_NumberToSpawn;
    /** direction for the velocities of spawned particles */
    MeVector3 m_SpawnNormal;
    /** Angular Velocity spread of particles created by spawners */
    MeReal    m_SpawnSpread;
    /** Speed [average] of new particles from a spawner */
    MeReal    m_SpawnSpeedMean;
    /** Distribution variance from the average speed for each spawned particle */
    MeReal    m_SpawnSpeedStDev;
    /** boolean to inherit the velocity from the spawing particle */
    int       m_SpawnInheritVelocity;
    /** Number of times a particle spawns.
	NumberOfSpawns = 3, a particle from a generator
	spawns new particles, which then spawn more new
	particles which then spawn more new particles
	which then decay.
    */
    int       m_NumberOfSpawns;
    /** particles spawn into this system */
    struct _MpsSystem *m_SpawnIntoSystem;
    /** spawning (while alive) parameters - each particle acts as a point emitter */
    MpsEmitter *m_ParticleEmitter;
    /** normal direction for emitter(s) bound to this system */
    MeVector3 m_EmitterNormal;
    /** angular [radians] offset from normal for emitted particle velocity */
    MeReal    m_EmitterSpread;
    /** Speed [average] of new particles from emitter */
    MeReal    m_EmitterSpeedMean;
    /** Distribution variance from the average speed for each emitted particle */
    MeReal    m_EmitterSpeedStDev;
    /** Rate of particles created by the emitter for this system */
    MeReal    m_EmitterRate;
    /** boolean for emitter to inherit the velocity */
    int       m_EmitterInheritVelocity;
    /** System to emit particles into */
    struct _MpsSystem *m_EmitIntoSystem;
    /** Callback Functions */
    MpsSystemBirthEventCallback m_BirthFunction;
    /** Callback Functions */
    MpsSystemLifeEventCallback m_LifeFunction;
    /** Callback Functions */
    MpsSystemDecayEventCallback m_DecayFunction;
    /** CollisionResponse Function and data */
    MpsClosure m_BodyCollisionResponse;
    MpsClosure m_CollisionResponse;
    
};

/* new a particle system */
extern MpsSystem *NewSystem(const int);
/* delete a particle system */
extern void DeleteSystem(MpsSystem *);

/*
   External interfaces to Particle System
*/
/*
   generate new particles from the emitters added to the system
*/
extern void GenerateNewParticles(MpsSystem *, MeReal);

/*
   Test all particles for decay criterion and destroy if true
*/
extern void DestroyParticlesInSystem(MpsSystem *, MeReal);

/*
   set all forces = 0
*/
extern void MeVectorSetZeroAllForcesInSystem(MpsSystem *);

/*
   (re-)initializes a particle
*/
extern void InitializeParticle(MpsSystem *, const int);

/*
   Integrate system
*/
extern void IntegrateOneTimeStep(MpsSystem *, MeReal);

#endif
