#ifndef _MPSENUMS_H
#define _MPSENUMS_H
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

/**
   \file
   Collection of tags for the internal Mps type definitions.
   These are all the enums for use with the Mps types.
*/

/** Particle decay enums  */
typedef enum _MpsParticleDecayMethod
{
    /** decay based on time */
    MpsLifetime,
    /** decay based on minimum speed tolerance */
    MpsMinSpeed,
    /** decay based on maximum speed tolerance */
    MpsMaxSpeed,
    /** decay when a collision is registered from an interaction */
    MpsOnCollision
}
MpsParticleDecayMethod;


/** Emitter geometry enums  */
typedef enum _MpsEmitterGeometry
{
    /** flat disc emitter */
    MpsDisc,
    /** spherical emitter (from surface) */
    MpsSphere,
    /** define a polygonal shape for emission from surface */
    MpsPolygon
}
MpsEmitterGeometry;


/** Global force enums  */
typedef enum _MpsGlobalForceType
{
    /** gravity force. Gravity force provides constant acceleration downwards (against normal) 
	based on particle mass */
    MpsGravity,
    /** universal drag. Universal drag for all particles deceleration along normal direction 
	based on particle mass. */
    MpsWindDrag,
    /** electric field. Electric field acceleration in normal direction based on particle charge.*/
    MpsElectricField,
    /** magentic [rotational] field. Magnetic field provides a rotational effect based on 
	particle charge and mass.*/
    MpsMagneticField,
    /** one of the global forces, randomly chosen.  This provides an interesting way to add a 
     chaotic effect to a particle system. */
    MpsRandom
}
MpsGlobalForceType;


/** Local force enums  */
typedef enum _MpsLocalForceType
{
    /** local force for a linear acceleration */
    MpsElectric,
    /** local force for a rotational acceleration */
    MpsMagnetic
}
MpsLocalForceType;


/** Possible types of particle interactions  */
typedef enum _MpsInteractionType
{
    /** particles attract */
    MpsAttractive,
    /** particles attract */
    MpsRepulsive,
    /** mutual push/pull */
    MpsAttractiveRepulsive,
    /** users may customize an interaction and use this storage type*/
    MpsUserDefinedInteraction,
    /** linear acceleration for interacting particles */
    MpsElectricLocalForce,
    /** rotational acceleration for interacting particles */
    MpsMagneticLocalForce
}
MpsInteractionType;

#endif
