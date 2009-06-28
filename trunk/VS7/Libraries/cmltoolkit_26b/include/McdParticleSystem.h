#ifndef _MCDPARTICLE_H
#define _MCDPARTICLE_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:50:40 $
   
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
  @file McdParticleSystem.h
  The particle system geometry type
*/

#include "McdFrame.h"

#ifdef __cplusplus
extern    "C" {
#endif

MCD_DECLARE_GEOMETRY_TYPE(McdParticleSystem);

/** McdParticleSystem is a concrete McdGeometry type. */
typedef McdGeometryID McdParticleSystemID;

typedef struct {
    McdGeometry m_g;
    int      *mVertexCount_Ptr;
    int       mMaxVertices;
    MeVector3 mPSCenter;
    MeVector3 *mVertexBase;
    MeVector3 *mVertexPrevPos;
    int       mVertexStride;
    MeReal    *mParticleRadius;
    int       mRadiusStride;
    MeReal    mMaxParticleRadius;
    void     *mSpacePartition;  /* This will the Hash Table for IxOptimization */
    /*  Keep these up our sleeve for a little later */
    MeReal    mPSMass;
    MeMatrix4 mCenterMass;
    MeReal   *mMassBase;
    int       mMassStride;
    MeMatrix3 mInertiaMM;
} McdParticleSystem;

/** This is the prototype for a single common particle radius */
McdParticleSystemID MEAPI McdParticleSystemCreate(int *currentSizePtr,int maxParticles,
                          MeVector3 *vertexBasePtr, MeVector3 *prevVertexBasePtr, int vertexStride,
                          MeReal    particleRadius);

MeReal MEAPI McdParticleSystemGetRadius(McdParticleSystemID g);

void MEAPI McdParticleSystemSetParticleRadii(McdGeometry * g,
                         MeReal *particleRadius, int radiusStride);

void MEAPI McdParticleSystemSetParameters(McdGeometry * G, int *countPtr, int maxVerts,
                      MeVector3 *Verts, MeVector3 *prevVertexBasePtr, int strideVertex,
                      MeReal *particleRad, int radStride, MeReal maxRadius);

void MEAPI McdParticleSystemSetCellSize(McdGeometry * g, MeReal diam);

void MEAPI McdParticleSystemSetPositionArrays(McdGeometry * g, MeVector3 *newParticlePos,
                          MeVector3 *oldParticlePos);

void MEAPI McdParticleSystemGetCurrentAABB(McdGeometry * g, MeMatrix4Ptr tm, MeVector3 minCorner, MeVector3 maxCorner);

/** Register the Particle system geometry type */
void MEAPI McdParticleSystemRegisterType();

/** Register the collision interactions between particle system geometry and all primitives. */
MeBool MEAPI McdParticleSystemPrimitivesRegisterInteractions();

MeBool MEAPI McdSphereParticleSystemRegisterInteraction();
MeBool MEAPI McdPlaneParticleSystemRegisterInteraction();
MeBool MEAPI McdBoxParticleSystemRegisterInteraction();
MeBool MEAPI McdCylinderParticleSystemRegisterInteraction();
/* MeBool MEAPI McdConeParticleSystemRegisterInteraction(); */
MeBool MEAPI McdParticleSystemParticleSystemRegisterInteraction();

/** Register the collision interactions between particle system geometry and triangle meshes. */
MeBool MEAPI McdTriangleMeshParticleSystemRegisterInteraction();
MeBool MEAPI McdRGHeightFieldParticleSystemRegisterInteraction();

/** Get the current number of particles in the particle system @a PS */
#define McdParticleSystemGetParticleCount(PS) (*((McdParticleSystem *) PS)->mVertexCount_Ptr)

MeReal MEAPI
McdParticleSystemGetParticleRadius(McdParticleSystemID g, int idx);

/** Return index of particle involved in the given McdContact. @see McdIntersectResult. */
#define McdParticleSystemContactGetParticleIndex(contact) ((contact)->element2.tag)

#ifdef __cplusplus
inline MeVector3 *MEAPI getVertex(McdParticleSystem * PS, int index) {
    char     *offset = (char *) PS->mVertexBase;
    offset += PS->mVertexStride * index;
    return (MeVector3 *) offset;
}

inline MeReal *MEAPI getParticleRadius(McdParticleSystem * PS, int index) {
    char     *offset = (char *) PS->mParticleRadius;
    offset += PS->mRadiusStride * index;
    return (MeReal *) offset;
}

inline MeVector3 *MEAPI getVertexPrev(McdParticleSystem * PS, int index) {
    char     *offset = (char *) PS->mVertexPrevPos;
    offset += PS->mVertexStride * index;
    return (MeVector3 *) offset;
}

#endif

#ifdef __cplusplus
}               /* extern "C" */
#endif
#endif /* _MCDPARTICLE_H */
