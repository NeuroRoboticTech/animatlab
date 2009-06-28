#ifndef _MDTCYLINDRICAL_H
#define _MDTCYLINDRICAL_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/01/13 22:13:09 $
   
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
 *  MdtCylindrical  API functions.
 *  
 *  Standard Cylindrical joint 
 *
 *
 */

#include "MePrecision.h"
#include "MdtTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif


MdtCylindricalID    MEAPI MdtCylindricalCreate(const MdtWorldID w);
MdtCylindricalID    MEAPI MdtCylindricalCreateInPool(MePool *pool);
void              MEAPI MdtCylindricalReset(MdtCylindricalID j);

MdtLimitID MEAPI MdtCylindricalGetLinearLimit(MdtCylindricalID J);
MdtLimitID MEAPI MdtCylindricalGetRotationalLimit(MdtCylindricalID J);


/**
   The following macros are deprecated and provided only for backwards
   compatibility.  Use MdtConstraintXXX functions.  This will be nixed in
   vortex revision 3.0.  
*/

#define  MdtCylindricalDestroy(j)                  MdtConstraintDestroy(j)
#define  MdtCylindricalEnable(j)                   MdtConstraintEnable(j)
#define  MdtCylindricalDisable(j)                  MdtConstraintDisable(j)
#define  MdtCylindricalIsEnabled(j)                MdtConstraintIsEnabled(j)
              

/*
  Cylindrical  joint accessors.
*/

#define  MdtCylindricalGetLimit(j)                 MdtConstraintGetLimit(j)
#define  MdtCylindricalGetPosition(j, v)           MdtConstraintGetPosition(j, v)
#define  MdtCylindricalGetAxis(j, v)               MdtConstraintGetAxis(j, v)
#define  MdtCylindricalGetBody(j, bodyindex)       MdtConstraintGetBody(j, bodyindex)
#define  MdtCylindricalGetUserData(j)              MdtConstraintGetUserData(j)
#define  MdtCylindricalGetWorld(j)                 MdtConstraintGetWorld(j)
#define  MdtCylindricalGetForce(j, bodyindex, f)   MdtConstraintGetForce(j, bodyindex, f)
#define  MdtCylindricalGetTorque(j, bodyindex, t)  MdtConstraintGetTorque(j, bodyindex, t)


/*
  Cylindrical  joint mutators.
*/

#define  MdtCylindricalSetLimit(j, v)              MdtConstraintSetLimit(j, v)
#define  MdtCylindricalSetAxis(j, x, y, z)         MdtConstraintSetAxis(j, x, y, z);
#define  MdtCylindricalSetPosition(j, x, y, z)     MdtConstraintSetPosition(j, x, y, z);
#define  MdtCylindricalSetBodies(j, b1, b2)        MdtConstraintSetBodies(j, b1, b2)
#define  MdtCylindricalSetUserData(j, d)           MdtConstraintSetUserData(j, d)






#ifdef __cplusplus
}
#endif


#endif
