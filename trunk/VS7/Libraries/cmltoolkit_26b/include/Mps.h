#ifndef _MPS_H
#define _MPS_H
/* -*- mode: C; -*- */

 /*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:51:50 $
   
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

#ifdef __cplusplus
extern    "C"
{
#endif
#include "MpsObjects.h"
#include "MpsEnums.h"
#include "MePrecision.h"
     /**
	\file
	Vesuvius Particle System Main Header.
	This provides the main interface to the creation and control of the
	various functions for particle system properties.
     */
     /*
	MpsManager functions.
	An MpsManager must be created to hold MpsSystems.
     */
extern MpsManager *MpsManagerCreate(void);
extern void MpsManagerDestroy(MpsManager * PM);
     /*
	Create and destroy MpsSystem.
     */
extern MpsSystem *MpsSystemCreate(MpsManager * PM, int maxnum);
extern void MpsSystemDestroy(MpsManager * PM, MpsSystem * PS);
     /*
      * Create and destroy MpsEmitter
      */
extern MpsEmitter *MpsEmitterCreate(MpsManager * PM, MpsEmitterGeometry geom);
extern void MpsEmitterDestroy(MpsManager * PM, MpsEmitter * PE);
     /*
      * Create and destroy MpsGlobalForce
      */
extern MpsGlobalForce *MpsGlobalForceCreate(MpsManager * PM, MpsGlobalForceType type);
extern void MpsGlobalForceDestroy(MpsManager * PM, MpsGlobalForce * GF);
     /*
      * Create and destroy MpsLocalForce
      */
/*
extern MpsLocalForce *MpsLocalForceCreate(MpsManager * PM, MpsLocalForceType type, int maxnum);
extern void MpsLocalForceDestroy(MpsManager * PM, MpsLocalForce * LF);
*/
     /*
      * Create and destroy MpsInteraction
      */
extern MpsInteraction *MpsInteractionCreate(MpsManager * PM, MpsInteractionType type);
extern void MpsInteractionDestroy(MpsManager * PM, MpsInteraction * IN);

     /*
	Bind global and local forces
     */
/*
extern void MpsBindGlobalForceAndLocalForce(MpsManager * PM,
					    MpsGlobalForce * GF, MpsLocalForce * LF);
*/
     /**
	Bind global force to a system
     */
extern void MpsBindGlobalForceAndSystem(MpsManager * PM, MpsGlobalForce * GF, MpsSystem * PS);
     /**
	Bind local force to a system
     */
extern void MpsBindLocalForceAndSystem(MpsManager * PM, MpsLocalForce * LF, MpsSystem * PS);
     /**
	Bind an interaction to a single system
     */
extern void MpsBindInteractionAndOneSystem(MpsManager * PM, MpsInteraction * IN, MpsSystem * PS);
     /**
	Bind an interaction to 2 separate systems
     */
extern void MpsBindInteractionAndTwoSystems(MpsManager * PM, MpsInteraction * IN,
					    MpsSystem * Sys1, MpsSystem * Sys2);

     /*
      * Unbind various objects
      */
extern void MpsUnbindGlobalForceAndLocalForce(MpsManager * PM,
					      MpsGlobalForce * GF, MpsLocalForce * LF);
extern void MpsUnbindGlobalForceAndSystem(MpsManager * PM, MpsGlobalForce * GF, MpsSystem * PS);
extern void MpsUnbindLocalForceAndSystem(MpsManager * PM, MpsLocalForce * LF, MpsSystem * PS);
extern void MpsUnbindInteractionAndOneSystem(MpsManager * PM, MpsInteraction * IN, MpsSystem * PS);
extern void MpsUnbindInteractionAndTwoSystems(MpsManager * PM, MpsInteraction * IN,
					      MpsSystem * Sys1, MpsSystem * Sys2);
     /*
      * user interface functions for evolving the particle systems
      */
extern void MpsUpdateAllSystems(MpsManager * PM, MeReal TimeStep);
     /*
      * End of MpsManager functions
      */

     /*
      * MpsSystem functions
      */
extern int MpsSystemAddParticles(MpsSystem * PS, int num);
extern int MpsSystemAddParticlesFromEmitter(MpsSystem * PS, int num, MpsEmitter * PE,
					    MeReal AngSpread, MeReal SpeedMean, MeReal SpeedStDev);
/* CHANGE: 
extern int MpsSystemAddParticlesFromEmitter(MpsSystem * PS, int num, MpsEmitter * PE);
and rename above ...WithSpread()
*/


extern int MpsSystemGetParticleMaxCount(const MpsSystem * PS);
extern int MpsSystemGetParticleCount(const MpsSystem * PS);

extern int MpsSystemGetTotalParticlesCreated(const MpsSystem * PS);
extern void MpsSystemSetTotalParticlesCreated(MpsSystem * PS, int i);
     /*
      * functions for decay, mass and charge
      */
extern void MpsSystemSetDecayMethod(MpsSystem * PS, MpsParticleDecayMethod method);
extern void MpsSystemSetDecayMean(MpsSystem * PS, MeReal mean);
extern void MpsSystemSetDecayStDev(MpsSystem * PS, MeReal stdev);
extern void MpsSystemSetConstantMass(MpsSystem * PS, MeReal mass);
extern void MpsSystemSetMassMean(MpsSystem * PS, MeReal mean);
extern void MpsSystemSetMassStDev(MpsSystem * PS, MeReal stdev);
extern void MpsSystemSetConstantCharge(MpsSystem * PS, MeReal charge);
extern void MpsSystemSetChargeMean(MpsSystem * PS, MeReal mean);
extern void MpsSystemSetChargeStDev(MpsSystem * PS, MeReal stdev);
     /*
      * emitter functions
      */
extern void MpsSystemAddEmitter(MpsSystem * PS, MpsEmitter * PE);
extern void MpsSystemRemoveEmitter(MpsSystem * PS, MpsEmitter * PE);
extern void MpsSystemSetCurrentEmitter(MpsSystem * PS, MpsEmitter * PE);

/* CHANGE: MpsEmitter functions:*/ 
extern void MpsSystemSetSpeedMean(MpsSystem * PS, MeReal mean);
extern void MpsSystemSetSpeedStDev(MpsSystem * PS, MeReal stdev);
extern void MpsSystemSetVelocityAngularSpread(MpsSystem * PS, MeReal ang);
extern void MpsSystemSetRate(MpsSystem * PS, MeReal rate);
extern void MpsSystemSetNumberToGenerate(MpsSystem * PS, int num);
/*
 * functions for spawning
 */
extern void MpsSystemSetFractionToSpawn(MpsSystem * PS, MeReal frac);
extern void MpsSystemSetNumberToSpawn(MpsSystem * PS, int num);
extern void MpsSystemSetSpawnNormal(MpsSystem * PS, MeReal nx, MeReal ny, MeReal nz);
extern void MpsSystemSetSpawnSpread(MpsSystem * PS, MeReal spr);
extern void MpsSystemSetSpawnSpeedMean(MpsSystem * PS, MeReal mean);
extern void MpsSystemSetSpawnSpeedStDev(MpsSystem * PS, MeReal stdev);
extern void MpsSystemSetNumberOfSpawns(MpsSystem * PS, int num);
extern void MpsSystemSetSpawnIntoSystem(MpsSystem * PS, MpsSystem * SpawnPS);
extern void MpsSystemEnableInheritVelocity(MpsSystem * PS);
extern void MpsSystemDisableInheritVelocity(MpsSystem * PS);
/*
 * functions to set particles as point emitters
 */
extern void MpsSystemSetEmitToSystem(MpsSystem * PS, MpsSystem * PS1);
extern void MpsSystemSetEmitterNormal(MpsSystem * PS, MeReal nx, MeReal ny, MeReal nz);
extern void MpsSystemSetEmitterSpread(MpsSystem * PS, MeReal spr);
extern void MpsSystemSetEmitterSpeedMean(MpsSystem * PS, MeReal mean);
extern void MpsSystemSetEmitterSpeedStDev(MpsSystem * PS, MeReal stdev);
extern void MpsSystemSetEmitterRate(MpsSystem * PS, MeReal rate);
extern void MpsSystemEnableEmitterInheritVelocity(MpsSystem * PS);
extern void MpsSystemDisableEmitterInheritVelocity(MpsSystem * PS);
/*
 * set event callbacks
 */
extern void MpsSystemSetBirthEventCallback(MpsSystem * PS, MpsSystemBirthEventCallback func);
extern void MpsSystemSetLifeEventCallback(MpsSystem * PS, MpsSystemLifeEventCallback func);
extern void MpsSystemSetDecayEventCallback(MpsSystem * PS, MpsSystemDecayEventCallback func);
/*
 * set particle data
 */
extern void MpsSystemSetParticlePosition(MpsSystem * PS, int i, MeReal *p);
extern void MpsSystemSetParticleVelocity(MpsSystem * PS, int i, MeReal *v);
extern void MpsSystemSetParticleForce(MpsSystem * PS, int i, MeReal *f);
extern void MpsSystemSetParticleMass(MpsSystem * PS, int i, MeReal m);
extern void MpsSystemSetParticleCharge(MpsSystem * PS, int i, MeReal q);
extern void MpsSystemSetParticleToDecay(MpsSystem * PS, int i);
/*
 * get particle data
 */
extern void MpsSystemGetParticlePosition(MpsSystem * PS, int i, MeVector3 p);
extern void MpsSystemGetParticlePrevPosition(MpsSystem * PS, int i, MeVector3 p);
extern void MpsSystemGetParticleVelocity(MpsSystem * PS, int i, MeReal *v);
extern void MpsSystemGetParticleForce(MpsSystem * PS, int i, MeReal *f);
extern MeReal MpsSystemGetParticleMass(const MpsSystem * PS, int i);
extern MeReal MpsSystemGetParticleCharge(const MpsSystem * PS, int i);
extern MeReal MpsSystemGetParticleAge(const MpsSystem * PS, int i);
extern MeReal MpsSystemGetParticleRelativeAge(const MpsSystem * PS, int i);
extern MeVector3 * MpsSystemGetPositionBase(MpsSystem * PS);
extern MeVector3 * MpsSystemGetPrevPositionBase(MpsSystem * PS);

void MEAPI MpsSystemOnCollisionDecay(MpsSystem *PS, int idx);

void MpsSystemAllocateParticleUserData(MpsSystem * PS);
void MpsSystemSetParticleUserData(MpsSystem * PS, int i, void* value);
void* MpsSystemGetParticleUserData(MpsSystem * PS,int i);

/*
 * End of MpsSystem functions
 */

/*
 * MpsEmitter functions
 */
extern void MpsEmitterSetNormal(MpsEmitter * PE, MeReal nx, MeReal ny, MeReal nz);
extern void MpsEmitterSetPosition(MpsEmitter * PE, MeReal ox, MeReal oy, MeReal oz);
extern void MpsEmitterGetPosition(MpsEmitter * PE, MeVector3 origin);
extern void MpsEmitterSetRadius(MpsEmitter * PE, MeReal rad);
extern void MpsEmitterSetVertexCount(MpsEmitter * PE, int num);

extern void MpsEmitterSetVertices(MpsEmitter * PE, const MeReal *verts);
/*
 * End of MpsEmitter functions
 */

/*
 * MpsGlobalForce functions
 */
/*
 * set and get
 */
extern void MpsGlobalForceSetScalar(MpsGlobalForce * GF, MeReal p);
extern void MpsGlobalForceSetVector(MpsGlobalForce * GF, MeReal px, MeReal py, MeReal pz);
/*
 * End of MpsGlobalForce functions
 */

#if 0
/*
 * MpsLocalForce functions
 * Set functions for decay, mass and charge of local force
 * particles
 */
extern void MpsLocalForceSetDecayMethod(MpsLocalForce * LF, MpsParticleDecayMethod method);
extern void MpsLocalForceSetDecayMean(MpsLocalForce * LF, MeReal mean);
extern void MpsLocalForceSetDecayStDev(MpsLocalForce * LF, MeReal stdev);
extern void MpsLocalForceSetMassMean(MpsLocalForce * LF, MeReal mean);
extern void MpsLocalForceSetMassStDev(MpsLocalForce * LF, MeReal stdev);
extern void MpsLocalForceSetChargeMean(MpsLocalForce * LF, MeReal mean);
extern void MpsLocalForceSetChargeStDev(MpsLocalForce * LF, MeReal stdev);
/*
 * Functions for generation of local force particles
 */
extern void MpsLocalForceAddEmitter(MpsLocalForce * LF, MpsEmitter * PE);
extern void MpsLocalForceRemoveEmitter(MpsLocalForce * LF, MpsEmitter * PE);
extern void MpsLocalForceSetCurrentEmitter(MpsLocalForce * LF, MpsEmitter * PE);

extern void MpsLocalForceSetSpeedMean(MpsLocalForce * LF, MeReal mean);
extern void MpsLocalForceSetSpeedStDev(MpsLocalForce * LF, MeReal stdev);
extern void MpsLocalForceSetVelocityAngularSpread(MpsLocalForce * LF, MeReal ang);
extern void MpsLocalForceSetRate(MpsLocalForce * LF, MeReal rate);
extern void MpsLocalForceSetNumberToGenerate(MpsLocalForce * LF, int num);
/*
 * Set functions for the interaction parameters
 */
extern void MpsLocalForceSetStrength(MpsLocalForce * LF, MeReal str);
extern void MpsLocalForceSetRange(MpsLocalForce * LF, MeReal ran);
/*
 * Set functions for the interaction box
 */
extern void MpsLocalForceEnableInteractionBox(MpsLocalForce * LF);
extern void MpsLocalForceDisableInteractionBox(MpsLocalForce * LF);
extern void MpsLocalForceSetBoxCenter(MpsLocalForce * LF, MeReal bx, MeReal by, MeReal bz);
extern void MpsLocalForceSetBoxDimensions(MpsLocalForce * LF, MeReal dx, MeReal dy, MeReal dz);
extern void MpsLocalForceSetBoxCutOff(MpsLocalForce * LF, MeReal cutoff);
extern void MpsLocalForceSetMaxNumOfPairs(MpsLocalForce * LF, int max);
extern void MpsLocalForceSetMaxNumInBin(MpsLocalForce * LF, int max);
/*
 * End of MpsLocalForce functions
 */

#endif

/*
 * MpsInteraction functions
 * Set functions for interaction parameters
 */
extern void MpsInteractionSetStrength(MpsInteraction * IN, MeReal str);
extern void MpsInteractionSetRange(MpsInteraction * IN, MeReal ran);
extern void MpsInteractionSetDamping(MpsInteraction * IN, MeReal dam);
extern void MpsInteractionSetUserDefinedFunction(MpsInteraction * IN, void (*func)
						 (MpsSystem *, int, MpsSystem *, int));
/*
 * Set functions for the interaction box
 */
extern void MpsInteractionEnableInteractionBox(MpsInteraction * IN);
extern void MpsInteractionDisableInteractionBox(MpsInteraction * IN);
extern void MpsInteractionSetBoxCenter(MpsInteraction * IN, MeReal bx, MeReal by, MeReal bz);
extern void MpsInteractionSetBoxDimensions(MpsInteraction * IN, MeReal dx, MeReal dy, MeReal dz);
extern void MpsInteractionSetBoxCutOff(MpsInteraction * IN, MeReal cutoff);
extern void MpsInteractionSetMaxNumOfPairs(MpsInteraction * IN, int max);
extern void MpsInteractionSetMaxNumInBin(MpsInteraction * IN, int max);
/*
 * End of MpsInteraction functions
 */

/*
 * Graphical utilities 
 */
unsigned char * MEAPI
MpsBlobTextureCreate(int size, int r, int g, int b);

MpsGraphicsInfo * MEAPI
MpsGraphicsInfoCreate(MpsSystem * mpsSystem, int colorCount, float *colorArray, MeReal *sizes, MeReal *ageLimits);
void MEAPI MpsGraphicsSetParticleSystemInfo(MpsGraphicsInfo * PSG, int n, float *newcolors, MeReal *sizes, MeReal* newages);
void MEAPI MpsGraphicsGetParticleColorAndSize(MpsGraphicsInfo * PSG, int i, float* Col, MeReal *size);

#ifdef __cplusplus
}
#endif

#endif
