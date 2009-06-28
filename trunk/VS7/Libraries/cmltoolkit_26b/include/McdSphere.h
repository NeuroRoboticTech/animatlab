#ifndef _MCDSPHERE_H
#define _MCDSPHERE_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:50:41 $
   
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
  @file McdSphere.h
  The sphere primitive geometry type
*/

#include "McdGeometry.h"

#ifdef __cplusplus
extern "C" {
#endif

MCD_DECLARE_GEOMETRY_TYPE( McdSphere );

/** McdSphere is a concrete McdGeometry type. */
typedef McdGeometryID McdSphereID;

typedef struct
{
  McdGeometry m_g;

  MeReal mRadius;

} McdSphere;

void              MEAPI McdSphereRegisterType();
McdSphereID       MEAPI McdSphereCreate( MeReal r );
void              MEAPI McdSphereSetRadius( McdSphereID g, MeReal r );
MeReal            MEAPI McdSphereGetRadius( McdSphereID g );

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MCDSPHERE_H */
