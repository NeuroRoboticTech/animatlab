#ifndef MDTDIFFERENTIAL_H
#define MDTDIFFERENTIAL_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/09/27 21:32:20 $
   
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
 * MdtDifferentialJoint  API 
 * With this constraint, the rotation of 3 bodies about the given axes are
 * constrained  so that   ratio[0]*w[0] + ratio[1]*w[1] + ratio[2]*w[2] =  0;
 * Each of the axes can be in a different direction. 
 */

#include "MePrecision.h"
#include "MdtTypes.h"


#ifdef __cplusplus
extern "C"
{
#endif


    MdtDifferentialJointID     MEAPI MdtDifferentialJointCreate(const MdtWorldID w);
    MdtDifferentialJointID     MEAPI MdtDifferentialJointCreateInPool(MePool *pool);
    void              MEAPI MdtDifferentialJointReset(MdtDifferentialJointID j);
    


/*
  Differential Joint accessors.
*/

    MeReal MEAPI MdtDifferentialJointGetRatio( const MdtDifferentialJointID j, int i );
    MeReal MEAPI MdtDifferentialJointGetMaxTorque( const MdtDifferentialJointID j );
    MeReal MEAPI MdtDifferentialJointGetMinTorque( const MdtDifferentialJointID j );

/*
  Differential Joint mutators
*/
    void MEAPI MdtDifferentialJointSetRatios(MdtDifferentialJointID j, MeReal r0, MeReal r1, MeReal r2);
    MeReal MEAPI MdtDifferentialJointSetMaxTorque(MdtDifferentialJointID j, MeReal max );
    MeReal MEAPI MdtDifferentialJointSetMinTorque(MdtDifferentialJointID j, MeReal min );
    

#ifdef __cplusplus
}
#endif


#endif
