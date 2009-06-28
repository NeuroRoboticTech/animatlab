#ifndef _MDTGEARRATIO_H
#define _MDTGEARRATIO_H
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
 * MdtGearRatio API 
 * This constraint enforces that the rotation of body 0 about body 0 primary
 * axis is proportional, in the given ratio but with the opposite sign, to
 * the rotation of body 1 about body 1 primary axis.  This is useful to
 * simulate gears but be aware that this constraint does not include the
 * hinge joint necessary to attach a gear to a part.
 */

#include "MePrecision.h"
#include "MdtTypes.h"


#ifdef __cplusplus
extern "C"
{
#endif


MdtGearRatioID     MEAPI MdtGearRatioCreate(const MdtWorldID w);
MdtGearRatioID     MEAPI MdtGearRatioCreateInPool(MePool *pool);
void               MEAPI MdtGearRatioReset(MdtGearRatioID j);



/*
  Gear Ratio joint accessors.
*/

MeReal            MEAPI MdtGearRatioGetRatio( const MdtGearRatioID j );
MeReal            MEAPI MdtGearRatioGetMaxTorque( const MdtGearRatioID j );
MeReal            MEAPI MdtGearRatioGetMinTorque( const MdtGearRatioID j );

/*
  Gear Ratio joint mutators
*/
    MeReal            MEAPI MdtGearRatioSetDifferential( MdtGearRatioID j, MeReal ratio );
    MeReal            MEAPI MdtGearRatioGetDifferential( MdtGearRatioID j);
    MeReal            MEAPI MdtGearRatioSetRatio    ( MdtGearRatioID j, MeReal ratio );
    MeReal            MEAPI MdtGearRatioSetMaxTorque( MdtGearRatioID j, MeReal max );
    MeReal            MEAPI MdtGearRatioSetMinTorque( MdtGearRatioID j, MeReal min );
    



#ifdef __cplusplus
}
#endif


#endif
