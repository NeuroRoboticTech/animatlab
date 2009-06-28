#ifndef _MDT_H
#define _MDT_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/03/16 22:26:52 $
   
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
 * Main include file.
 */


#include "MePrecision.h"
#include "MeAssert.h"

#include "MeMessage.h"
#include "MeMemory.h"

#include "MdtKea.h"
#include "MdtBcl.h"

#include "MdtTypes.h"

#include "MdtWorld.h"
#include "MdtConstraint.h"
#include "MdtContact.h"
#include "MdtContactParams.h"
#include "MdtBSJoint.h"
#include "MdtHinge.h"
#include "MdtPrismatic.h"
#include "MdtCylindrical.h"
#include "MdtDistance.h"
#include "MdtCarWheel2.h"
#include "MdtFixedPath.h"
#include "MdtRPROJoint.h"
#include "MdtUniversal.h"
#include "MdtLinear1.h"
#include "MdtLinear2.h"
#include "MdtLinear3.h"
#include "MdtAngular3.h"
#include "MdtUserConstraint.h"
#include "MdtSpring.h"
#include "MdtConeLimit.h"
#include "MdtGearRatio.h"
#include "MdtScrewJoint.h"
#include "MdtWinchJoint.h"
#include "MdtDoubleHinge.h"
#include "MdtDifferentialJoint.h"
#include "MdtCoupledDistance.h"
#include "MdtLimit.h"
#include "MdtUtilities.h"
#include "MdtBody.h"
#include "MdtGrouping.h"

/* Vortex Dynamics Toolkit 'Main Loop' API */


void          MEAPI
MdtPartitionParametersSetGroupingMode(MdtPartitionParameters *p, MdtPartitionGroupingMode mode);


MdtPartitionGroupingMode           MEAPI
MdtPartitionParametersGetGroupingMode(MdtPartitionParameters *p);

void          MEAPI 
MdtPartitionParametersSetDefault(MdtPartitionParameters *p); 

void          MEAPI 
MdtPartitionOutputResize(MdtPartitionOutput * po, 
                         int maxBodies, int maxCosntraints, 
                         MeMemoryAPIStruct *mem);

void          MEAPI 
MdtPartitionOutputDestroy(MdtPartitionOutput * po, MeMemoryAPIStruct *mem);

void          MEAPI 
MdtUpdatePartitions(const MdtBodyID enabledBodyList,
                    MdtPartitionOutput* po, const MdtPartitionEndCB pcb,
                    MdtPartitionParameters *p,
                    void* pcbdata);

void          MEAPI 
MdtAutoDisableLastPartition(MdtPartitionOutput* po, void* cbdata);


unsigned int  MEAPI 
MdtPackPartition(const MdtPartitionOutput* po,
                 const unsigned int partitionindex,
                 const MeReal stepSize, 
                 MdtWorldParams* params,
                 MdtBclConstraintParameters* bcl_params,
                 MdtKeaConstraints* constraints);

void          MEAPI 
MdtUnpackPartition(struct MdtKeaConstraints* constraints, 
                   const unsigned int partitionindex, MdtPartitionOutput* po);

MeReal        MEAPI 
MdtPartitionGetSafeTime(MdtPartitionOutput* po, int i);


#endif
