#ifndef _MDTLINEAR1_H
#define _MDTLINEAR1_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/09/27 21:30:57 $
   
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
 * MdtLinear1 API functions.
 *
 * A point fixed in the reference frame of primary body is constrained to a plane fixed in the
 * reference frame of the second body or the inertial frame.  The
 * plane is specified by the MdtConstraintSetPosition() and
 * MdtConstraintSetAxis() functions.
 */

/**
 * 
 * @example Linear1.c
 *
 * This example shows a small box attached to a large box with a linear1
 * joint. 
 */


#include "MePrecision.h"
#include "MdtTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif


MdtLinear1ID      MEAPI MdtLinear1Create(const MdtWorldID w);
MdtLinear1ID      MEAPI MdtLinear1CreateInPool(MePool* pool);
void              MEAPI MdtLinear1Reset(MdtLinear1ID j);


/*
  Linear1 joint accessors.
*/



/*
  Linear1 joint mutators.
*/

void  MEAPI MdtLinear1SetPosition(MdtLinear1ID j, const MeReal x, const MeReal y, const MeReal z);

#define  MdtLinear1GetPosition(j, x) MdtConstraintGetPosition((j), (x))
#define  MdtLinear1GetBody(j, bodyindex)  MdtConstraintGetBody(j, bodyindex)
#define  MdtLinear1GetUserData(j)  MdtConstraintGetUserData(j)
#define  MdtLinear1GetWorld(j)  MdtConstraintGetWorld(j)
#define  MdtLinear1GetForce(j, bodyindex, f)  MdtConstraintGetForce(j, bodyindex, f)
#define  MdtLinear1GetTorque(j, bodyindex, t)  MdtConstraintGetTorque(j, bodyindex, t)
#define  MdtLinear1Destroy(j)  MdtConstraintDestroy(j)
#define  MdtLinear1Enable(j)        MdtConstraintEnable(j)
#define  MdtLinear1Disable(j)  MdtConstraintDisable(j)
#define  MdtLinear1IsEnabled(j)  MdtConstraintIsEnabled(j)
#define  MdtLinear1SetBodies(j, b1, b2)  MdtConstraintSetBodies(j, b1, b2)
#define  MdtLinear1SetUserData(j, d)  MdtConstraintSetUserData(j, d)


#ifdef __cplusplus
}
#endif


#endif
