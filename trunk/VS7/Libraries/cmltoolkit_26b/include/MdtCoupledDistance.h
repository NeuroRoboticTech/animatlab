#ifndef MDTCOUPLEDDISTANCE_H
#define MDTCOUPLEDDISTANCE_H
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
 * MdtCoupledDistance  API 
 * With this constraint, the distance of body 1 to body 0 is the same as
 * that of body 2 with body 0, with respect to the same attachment point on
 * body 0, in the direction of the normal for the joint.   Namely, if
 * d(0,1) is the distance between attachment point p(1) on body 1 and p(0)
 * on body 0, and similarly for d(0, 2), then,  the projection  of 
 * d(0, 1)-d(0, 2) along the normal is 0. 
 */

#include "MePrecision.h"
#include "MdtTypes.h"


#ifdef __cplusplus
extern "C"
{
#endif


    MdtCoupledDistanceID     MEAPI MdtCoupledDistanceCreate(const MdtWorldID w);
    MdtCoupledDistanceID     MEAPI MdtCoupledDistanceCreateInPool(MePool *pool);
    void              MEAPI MdtCoupledDistanceReset(MdtCoupledDistanceID j);
    


/*
  Coupled distance Joint accessors.
*/

    MeReal MEAPI MdtCoupledDistanceGetMaxForce( const MdtCoupledDistanceID j );
    MeReal MEAPI MdtCoupledDistanceGetMinForce( const MdtCoupledDistanceID j );

/*
  Coupled distance Joint mutators.
*/

    void MEAPI MdtCoupledDistanceSetMaxForce(const MdtCoupledDistanceID j,
                                             MeReal m);
    void MEAPI MdtCoupledDistanceSetMinForce(const MdtCoupledDistanceID j,
                                             MeReal m);
    

#ifdef __cplusplus
}
#endif


#endif
