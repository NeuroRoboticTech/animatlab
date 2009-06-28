#ifndef _MPSOBJECTS_H
#define _MPSOBJECTS_H
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
   Data types for use with Particle Systems.
   These should be in roughly hierarchical order, except for back-edges.
   At least one Manager is needed.
   Many systems can be associated with a Manager.
   Systems can be emitted by [bound to] an emitter.
   Global, Local and Interactions can be bound to [multiple] systems.
*/
#include "MePrecision.h"

/** Mps object typedefs  */
typedef struct _MpsManager MpsManager;
/** Mps object typedefs  */
typedef struct _MpsSystem MpsSystem;
/** Mps object typedefs  */
typedef struct _MpsEmitter MpsEmitter;
/** Mps object typedefs  */
typedef struct _MpsGlobalForce MpsGlobalForce;
/** Mps object typedefs  */
typedef struct _MpsLocalForce MpsLocalForce;
/** Mps object typedefs  */
typedef struct _MpsInteraction MpsInteraction;


/** Birth event callback functions  */
typedef void (*MpsSystemBirthEventCallback) (MpsSystem *, int);
/** While in life -- event callback functions  */
typedef void (*MpsSystemLifeEventCallback) (MpsSystem *);
/** Decay event callback functions  */
typedef void (*MpsSystemDecayEventCallback) (MpsSystem *, int *, int);

/* Destroy function for MpsClosure, called automatically on the closure user data when the
   closure is destroyed. */
typedef void (MEAPI *MpsClosureDataDestroyFn)(void* data);

typedef struct {
    void *fn;
    void *data;
    MpsClosureDataDestroyFn destroyFn;
} MpsClosure;


/** 
  Utility structure to hold data for color interpolation for systems
  that vary their color or alpha values during the life of a particle.
*/
typedef struct MpsGraphicsInfo
{
  MpsSystem*       m_System;  /**< Particle system this data refers to */

  /*
  int              m_MaximumParticleCount;

  MeReal*          m_ParticlePositions;
  MeReal*          m_ParticleVelocities;
  */

  int              m_NumberOfColors;  /**< Number of colors to interpolate between */
  float*           m_Colors;          /**< Pointer to array containing colors, 4 values (r,g,b,a) for each */
  MeReal*          m_AgeLimits;       /**< Relative particle ages (0..1) to which these colors correspond. 
                                          Initially, particle has color @a m_Colors[0], after reaching relative
                                          age @a m_AgeLimits[1] it will get color @a m_Colors[4], etc. */
  MeReal*          m_Sizes;           /**< Sizes for particles also interpolated using same @a m_ageLimits as colors */
}
MpsGraphicsInfo;

#endif

