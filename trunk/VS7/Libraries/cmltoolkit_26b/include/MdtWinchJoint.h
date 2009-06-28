#ifndef _MDTWINCHJOINT_H
#define _MDTWINCHJOINT_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/02/17 17:00:47 $
   
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
 * MdtWinchJoint API 
 * This constraint enforces that the rotation of body 0 about its primary
 * axis is proportional, in the given ratio but with the opposite sign, to
 * the translation of body 1 about its primary axis.  This is useful to
 * simulate gears but be aware that this constraint does not include the
 * hinge joint necessary to attach a gear to a part.
 */

#include "MePrecision.h"
#include "MdtTypes.h"


#ifdef __cplusplus
extern "C"
{
#endif


MdtWinchJointID     MEAPI MdtWinchJointCreate(const MdtWorldID w);
MdtWinchJointID     MEAPI MdtWinchJointCreateInPool(MePool *pool);
void                MEAPI MdtWinchJointReset(MdtWinchJointID j);



/*
  Gear Ratio joint accessors.
*/

MeReal            MEAPI MdtWinchJointGetRatio( const MdtWinchJointID j );
MeReal            MEAPI MdtWinchJointGetMaxTorque( const MdtWinchJointID j );
MeReal            MEAPI MdtWinchJointGetMinTorque( const MdtWinchJointID j );

/*
  Gear Ratio joint mutators
*/
    MeReal            MEAPI MdtWinchJointSetDifferential( MdtWinchJointID j, MeReal ratio );
    MeReal            MEAPI MdtWinchJointGetDifferential( MdtWinchJointID j);
    MeReal            MEAPI MdtWinchJointSetDistance( MdtWinchJointID j, MeReal ratio );
    MeReal            MEAPI MdtWinchJointGetDistance( MdtWinchJointID j);
    MeReal            MEAPI MdtWinchJointSetMaxDistance( MdtWinchJointID j, MeReal ratio );
    MeReal            MEAPI MdtWinchJointGetMaxDistance( MdtWinchJointID j);
    MeReal            MEAPI MdtWinchJointSetMinDistance( MdtWinchJointID j, MeReal ratio );
    MeReal            MEAPI MdtWinchJointGetMinDistance( MdtWinchJointID j);
    MeReal            MEAPI MdtWinchJointSetRatio    ( MdtWinchJointID j, MeReal ratio );
    MeReal            MEAPI MdtWinchJointSetMaxTorque( MdtWinchJointID j, MeReal max );
    MeReal            MEAPI MdtWinchJointSetMinTorque( MdtWinchJointID j, MeReal min );
    



#ifdef __cplusplus
}
#endif


#endif
