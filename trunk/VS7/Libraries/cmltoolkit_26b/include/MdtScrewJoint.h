#ifndef _MDTSREWJOINT_H
#define _MDTSREWJOINT_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/02/16 21:54:57 $
   
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
 * MdtScrewJoint API 
 * This constraint enforces that the rotation of body 0 about its primary
 * axis is proportional, in the given ratio but with the opposite sign, to
 * the relative translation of body 1 and body 2 about their primary axis.  
 * This is useful to
 * simulate gears but be aware that this constraint does not include the
 * hinge joint necessary to attach a gear to a part.
 */

#include "MePrecision.h"
#include "MdtTypes.h"


#ifdef __cplusplus
extern "C"
{
#endif


MdtScrewJointID     MEAPI MdtScrewJointCreate(const MdtWorldID w);
MdtScrewJointID     MEAPI MdtScrewJointCreateInPool(MePool *pool);
void                MEAPI MdtScrewJointReset(MdtScrewJointID j);



/*
  Gear Ratio joint accessors.
*/

MeReal            MEAPI MdtScrewJointGetRatio( const MdtScrewJointID j );
MeReal            MEAPI MdtScrewJointGetMaxTorque( const MdtScrewJointID j );
MeReal            MEAPI MdtScrewJointGetMinTorque( const MdtScrewJointID j );

/*
  Gear Ratio joint mutators
*/
    MeReal            MEAPI MdtScrewJointSetDifferential( MdtScrewJointID j, MeReal ratio );
    MeReal            MEAPI MdtScrewJointGetDifferential( MdtScrewJointID j);
    MeReal            MEAPI MdtScrewJointSetRatio    ( MdtScrewJointID j, MeReal ratio );
    MeReal            MEAPI MdtScrewJointSetMaxTorque( MdtScrewJointID j, MeReal max );
    MeReal            MEAPI MdtScrewJointSetMinTorque( MdtScrewJointID j, MeReal min );
    



#ifdef __cplusplus
}
#endif


#endif
