#ifndef _MCDCYLINDER_H
#define _MCDCYLINDER_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/04/21 15:41:50 $
   
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
  @file McdCylinder.h
  The cylinder primitive geometry type
*/

#include "McdGeometry.h"

#ifdef __cplusplus
extern "C" {
#endif

MCD_DECLARE_GEOMETRY_TYPE( McdCylinder );

/** McdCylinder is a concrete McdGeometry type. */
typedef McdGeometryID McdCylinderID;

typedef struct
{
  McdGeometry m_g;

  MeReal mR;
  /* half-height */
  MeReal mRz;
  MeReal mSphereRadius;
  MeReal mMaxEdgeCosForEdgeContact;

} McdCylinder;

void              MEAPI McdCylinderRegisterType();
McdCylinderID     MEAPI McdCylinderCreate( MeReal r, MeReal h );
void              MEAPI McdCylinderSetRadius( McdCylinderID g, MeReal r );
void              MEAPI McdCylinderSetHeight( McdCylinderID g, MeReal h );
MeReal            MEAPI McdCylinderGetRadius( McdCylinderID g );
MeReal            MEAPI McdCylinderGetHeight( McdCylinderID g );
MeReal            MEAPI McdCylinderGetHalfHeight( McdCylinderID g );
MeReal            MEAPI McdCylinderGetBSphereRadius( McdGeometryID g );
void              MEAPI McdCylinderSetMaxEdgeCosForEdgeContact(McdCylinderID g, MeReal val);
MeReal            MEAPI McdCylinderGetMaxEdgeCosForEdgeContact(McdCylinderID g);

  /** @internal */
void              MEAPI McdCylinderGetXYAABB( McdGeometry* g, MeMatrix4 tm,
                            MeReal bounds[4]) ;
/** @internal */
void MEAPI
McdCylinderGetEndCenters( McdGeometry *g, MeMatrix4 _tm, MeVector3 end0, MeVector3 end1);

  /** @internal */
void              MEAPI McdCylinderMaximumPointLocal( McdGeometry *g,
                            MeReal *inDir, MeReal *outPoint);

  /** @internal */
void              MEAPI McdCylinderSetGeometricalParameters( McdGeometry* g,
                            MeReal inRadius, MeReal inHeight);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MCDCYLINDER_H */
