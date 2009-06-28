#ifndef _MDTDISTANCE_H
#define _MDTDISTANCE_H
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

/** 
 * @file
 *  MdtDistance  API functions.
 *  
 *  Standard Distance joint 
 *
 *
 */

#include "MePrecision.h"
#include "MdtTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif


MdtDistanceID     MEAPI MdtDistanceCreate(const MdtWorldID w);
MdtDistanceID     MEAPI MdtDistanceCreateInPool(MePool *pool);
void              MEAPI MdtDistanceReset(MdtDistanceID j);

void              MEAPI MdtDistanceSetDistance(MdtDistanceID j, MeReal distance);
MeReal            MEAPI MdtDistanceGetDistance(MdtDistanceID j);

void MEAPI MdtDistanceSetTwistDampingParameter(MdtDistanceID j,MeReal twistDamping);
void MEAPI MdtDistanceSetSwingDampingParameter(MdtDistanceID j,MeReal swingDamping);
void MEAPI MdtDistanceSetLinearDampingParameter(MdtDistanceID j,MeReal linearDamping);
void MEAPI MdtDistanceSetTwistDamping(MdtDistanceID j,MeReal twistDamping);
void MEAPI MdtDistanceSetSwingDamping(MdtDistanceID j,MeReal swingDamping);
void MEAPI MdtDistanceSetLinearDamping(MdtDistanceID j,MeReal linearDamping);

MeReal MEAPI MdtDistanceGetTwistDampingParameter(MdtDistanceID j);
MeReal MEAPI MdtDistanceGetSwingDampingParameter(MdtDistanceID j);
MeReal MEAPI MdtDistanceGetLinearDampingParameter(MdtDistanceID j);

void MEAPI MdtDistanceEnableTwistDamping(MdtDistanceID j);
void MEAPI MdtDistanceDisableTwistDamping(MdtDistanceID j);

void MEAPI MdtDistanceEnableSwingDamping(MdtDistanceID j);
void MEAPI MdtDistanceDisableSwingDamping(MdtDistanceID j);

void MEAPI MdtDistanceEnableLinearDamping(MdtDistanceID j);
void MEAPI MdtDistanceDisableLinearDamping(MdtDistanceID j);




#define MDT_DISTANCE_LINEAR      (0)





#ifdef __cplusplus
}
#endif


#endif
