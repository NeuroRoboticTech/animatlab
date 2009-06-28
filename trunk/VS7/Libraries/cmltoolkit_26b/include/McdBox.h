#ifndef _MCDBOX_H
#define _MCDBOX_H
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
  @file McdBox.h
  The box primitive geometry type
*/

#include "McdGeometry.h"

#ifdef __cplusplus
extern "C" {
#endif


  /** @internal */
MCD_DECLARE_GEOMETRY_TYPE( McdBox );

  /** McdBox is a concrete McdGeometry type. All functions taking type
      McdGeometryID as argument can be called with a McdBoxID argument.
  @see McdGeometry.h */
typedef McdGeometryID McdBoxID;

typedef struct
{
  McdGeometry m_g;

  MeReal mR[3];
  MeReal mRadius;

} McdBox;

/**  */
void              MEAPI McdBoxRegisterType();

McdBoxID          MEAPI McdBoxCreate( MeReal dx, MeReal dy, MeReal dz );
void              MEAPI McdBoxSetDimensions( McdGeometryID g,
                            MeReal dx, MeReal dy, MeReal dz );
void              MEAPI McdBoxGetDimensions( McdGeometryID g,
                            MeReal *dx, MeReal *dy, MeReal *dz );
MeReal            MEAPI McdBoxGetBSphereRadius( McdGeometryID g );

  /** @internal */
  /* carry-over from Cx version: used by Ix functions
     can probably get rid of by modifying Ix usage */

MeReal*           MEAPI McdBoxGetRadii( McdBoxID );

  /** @internal */

void              MEAPI McdBoxGetXYAABB( McdBoxID, MeMatrix4 tm,
                            MeReal bounds[4]);

  /** @internal */
void              MEAPI McdBoxMaximumPointLocal( McdBoxID, MeReal* inDir,
                            MeReal* outPoint);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MCDBOX_H */
