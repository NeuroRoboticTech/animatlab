#ifndef MDT_LINEAR3_H
#define MDT_LINEAR3_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/09/27 21:30:58 $
   
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
 * MdtLinear3 API definition.

 The geometry of this joint is such that during the simulation, a point p1
 fixed on body 1 will be on a line l0 fixed in body 0.  The line l0 is
 defined by a point and a normal.  In addition, a direction vector v1,
 fixed in body 1, has a constant orientation in the plane perpendicular to
 line l0.  

 Another way to think about this joint is that the bodies are 
 hinged about an axis which is perpendicular to both the direction of the
 line and the given direction vector.  The difference from a hinge is that
 the location is not fixed.

 */

#include "MePrecision.h"
#include "MdtTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif



MdtLinear3ID      MEAPI MdtLinear3Create(const MdtWorldID w);
MdtLinear3ID      MEAPI MdtLinear3CreateInPool(MePool* pool);
void              MEAPI MdtLinear3Reset(MdtLinear3ID j);


#ifdef __cplusplus
}
#endif


#endif
