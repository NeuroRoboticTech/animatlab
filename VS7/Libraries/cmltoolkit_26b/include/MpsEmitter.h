#ifndef _MPSEMITTER_H
#define _MPSEMITTER_H
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
#include "MeMath.h"
#include "MpsEnums.h"
#include "MpsObjects.h"

/**
   \file
   An MpsEmitter contains the geometry of the emitter.
*/

/**
   Particle Emitter properties + geometry.
*/
struct _MpsEmitter {
    /** the geometry of the emitter = {disc, sphere, polygon} */
    MpsEmitterGeometry m_Geometry;
    /** normal vector for emitter  */
    MeVector3 m_Normal;
    /** origin position for the emitter */
    MeVector3 m_Origin;
    /** radius of the emitter */
    MeReal m_Radius;
    /** geometry for polygon  */
    int m_NumVertices;
    MeVector4 m_RotationQuaternion;
    MeVector3 *m_Vertex;
    MeReal *m_TriangleProbs;
};

/*
   new particle emitter
*/
extern MpsEmitter *NewEmitter(const MpsEmitterGeometry);

/*
   delete particle emitter
*/
extern void DeleteEmitter(MpsEmitter *);

/*
   initialize particles from emitter.
   Assign positions, velocities and orientations to all new particles in one shot.
*/
extern void AssignInitialConditions(MpsEmitter * PE, MpsSystem * PS,
    int FirstIndex, MeReal AngSpread, MeReal SpeedMean, MeReal SpeedStDev);

#endif
