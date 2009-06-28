#ifndef _MDTPRISMATIC_H
#define _MDTPRISMATIC_H
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
 *  MdtPrismatic API functions.

 For this joint, a line l0 fixed in body b0 is coincident with a line l1
 fixed on body 1 and the rotation of b1 about l0 is fixed.  The origin
 of l1 is however not fixed on l0 which allows sliding along the common
 axis but no rotation about it.  For convenience, the 
 the origin of l0 is called the position of the joint and the common
 axis of the two lines is the axis of the joint.  


 */

#include "MePrecision.h"
#include "MdtTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif


MdtPrismaticID    MEAPI MdtPrismaticCreate(const MdtWorldID w);
void              MEAPI MdtPrismaticReset(MdtPrismaticID j);
MdtPrismaticID    MEAPI MdtPrismaticCreateInPool(MePool *pool);



#define MDT_PRISMATIC_LINEAR1 (0)
#define MDT_PRISMATIC_LINEAR2 (1)
#define MDT_PRISMATIC_ROTATIONAL1 (2)
#define MDT_PRISMATIC_ROTATIONAL2 (3)
#define MDT_PRISMATIC_ROTATIONAL3 (4)



/**
   The following macros are deprecated and provided only for backwards
   compatibility.  Use MdtConstraintXXX functions.  This will be nixed in
   vortex revision 3.0.  
*/

#define  MdtPrismaticDestroy(j)                  MdtConstraintDestroy(j)
#define  MdtPrismaticEnable(j)                   MdtConstraintEnable(j)
#define  MdtPrismaticDisable(j)                  MdtConstraintDisable(j)
#define  MdtPrismaticIsEnabled(j)                MdtConstraintIsEnabled(j)
              

/*
  Prismatic joint accessors.
*/

#define  MdtPrismaticGetLimit(j)                 MdtConstraintGetLimit(j)
#define  MdtPrismaticGetPosition(j, v)           MdtConstraintGetPosition(j, v)
#define  MdtPrismaticGetAxis(j, v)               MdtConstraintGetAxis(j, v)
#define  MdtPrismaticGetBody(j, bodyindex)       MdtConstraintGetBody(j, bodyindex)
#define  MdtPrismaticGetUserData(j)              MdtConstraintGetUserData(j)
#define  MdtPrismaticGetWorld(j)                 MdtConstraintGetWorld(j)
#define  MdtPrismaticGetForce(j, bodyindex, f)   MdtConstraintGetForce(j, bodyindex, f)
#define  MdtPrismaticGetTorque(j, bodyindex, t)  MdtConstraintGetTorque(j, bodyindex, t)

/*
  Prismatic joint mutators.
*/

#define  MdtPrismaticSetLimit(j, v)              MdtConstraintSetLimit(j, v)
#define  MdtPrismaticSetAxis(j, x, y, z)         MdtConstraintSetAxis(j, x, y, z);
#define  MdtPrismaticSetPosition(j, x, y, z)     MdtConstraintSetPosition(j, x, y, z);
#define  MdtPrismaticSetBodies(j, b1, b2)        MdtConstraintSetBodies(j, b1, b2)
#define  MdtPrismaticSetUserData(j, d)           MdtConstraintSetUserData(j, d)


#ifdef __cplusplus
}
#endif


#endif
