#ifndef _MDTUSERCONSTRAINT_H
#define _MDTUSERCONSTRAINT_H
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
 * User defined constraint type.

 Users who need special constraints can hook their computations  in this
 structure.  See the example supplied in MeTutorials.
 
*/

#include "MePrecision.h"
#include "MdtTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif


MdtUserConstraintID MEAPI MdtUserConstraintCreate(const MdtWorldID w);


/*
  Universal joint accessors.
*/

MdtBclAddConstraintFn MEAPI MdtUserConstraintGetFunction(const MdtUserConstraintID j);
void             *MEAPI MdtUserConstraintGetConstraintData(const MdtUserConstraintID j);


/*
  Universal joint mutators.
*/

void              MEAPI MdtUserConstraintSetFunction(MdtUserConstraintID j, MdtBclAddConstraintFn f);
void              MEAPI MdtUserConstraintSetConstraintData(MdtUserConstraintID j, void* d);

#define  MdtUserConstraintDestroy(j)  MdtConstraintDestroy(j)
#define  MdtUserConstraintEnable(j)  MdtConstraintEnable(j)
#define  MdtUserConstraintDisable(j)  MdtConstraintDisable(j)
#define  MdtUserConstraintIsEnabled(j)  MdtConstraintIsEnabled(j)
#define  MdtUserConstraintGetBody(j, bodyindex)  MdtConstraintGetBody(j, bodyindex)
#define  MdtUserConstraintGetUserData(j)  MdtConstraintGetUserData(j)
#define  MdtUserConstraintGetWorld(j)  MdtConstraintGetWorld(j)
#define  MdtUserConstraintGetForce(j, bodyindex, f)  MdtConstraintGetForce(j, bodyindex, f)
#define  MdtUserConstraintGetTorque(j, bodyindex, t)  MdtConstraintGetTorque(j, bodyindex, t)
#define  MdtUserConstraintSetBodies(j, b1, b2)  MdtConstraintSetBodies(j, b1, b2)
#define  MdtUserConstraintSetUserData(j, d)  MdtConstraintSetUserData(j, d)

#ifdef __cplusplus
}
#endif

#endif
