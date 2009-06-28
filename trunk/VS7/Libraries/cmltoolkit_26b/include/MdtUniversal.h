#ifndef _MDTUNIVERSAL_H
#define _MDTUNIVERSAL_H
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
 * MdtUniversal API functions.

 The geometry of this joint is that a point p0 fixed in body b0 is
 coincident with a point p1 fixed in body b1 and that an axis u0 fixed
 in body b0 is kept perpendicular to an axis v1 fixed in body b1.  This
 is often used in transmissions as it allows a driving rod to transfer
 rotation to a secondary rod which is not necessarily aligned with it.

 The world coordinates of the coincident points p0 and p1 are called
 the joint position.  The world coordinates of u0  and v1 are called
 axis1 and axis2 respectively.  


 */

#include "MePrecision.h"
#include "MdtTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif


MdtUniversalID    MEAPI MdtUniversalCreate(const MdtWorldID w);
void              MEAPI MdtUniversalReset(MdtUniversalID j);
MdtUniversalID    MEAPI MdtUniversalCreateInPool(MePool *pool);




/*
  Universal joint accessors.
*/

void     MEAPI MdtUniversalGetAxis(const MdtUniversalID j, const unsigned int bodyindex, MeVector3 axis);


/*
  Universal joint mutators.
*/


#define  MdtUniversalDestroy(j)  MdtConstraintDestroy(j)
#define  MdtUniversalEnable(j)  MdtConstraintEnable(j)
#define  MdtUniversalDisable(j)  MdtConstraintDisable(j)
#define  MdtUniversalIsEnabled(j)  MdtConstraintIsEnabled(j)
#define  MdtUniversalGetBody(j, bodyindex)  MdtConstraintGetBody(j, bodyindex)
#define  MdtUniversalGetUserData(j)  MdtConstraintGetUserData(j)
#define  MdtUniversalGetWorld(j)  MdtConstraintGetWorld(j)
#define  MdtUniversalGetForce(j, bodyindex, f)  MdtConstraintGetForce(j, bodyindex, f)
#define  MdtUniversalGetTorque(j, bodyindex, t)  MdtConstraintGetTorque(j, bodyindex, t)
#define  MdtUniversalSetBodies(j, b1, b2)  MdtConstraintSetBodies(j, b1, b2)
#define  MdtUniversalSetUserData(j, d)  MdtConstraintSetUserData(j, d)


#ifdef __cplusplus
}
#endif


#endif
