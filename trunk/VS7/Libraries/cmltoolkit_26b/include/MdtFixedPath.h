#ifndef _MDTFIXEDPATH_H
#define _MDTFIXEDPATH_H
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

/** @file
 * MdtFixedPath API functions.

 This joint is used to constrain a point fixed on one of the bodies to follow a
 given trajectory.  For the the typical use case, one body is fixed
 in the world.  At each time step, the user can set the position of the
 and the velocity of the attachment point relative to the second body in
 the following way: 
 @code
 MdtConstraintBodySetPosition(ct, idx, x, y, z);
 MdtFixedPathSetVelocity(ct, idx, vx, vy, vz);
 @endcode
 where idx is the index of the body which is fixed in the world frame.
 */

#include "MePrecision.h"
#include "MdtTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif




MdtFixedPathID    MEAPI MdtFixedPathCreate(const MdtWorldID w);
void              MEAPI MdtFixedPathReset(MdtFixedPathID j);



/*
  Fixed path joint accessors.
*/

void              MEAPI MdtFixedPathGetVelocity(const MdtFixedPathID j,
                            const unsigned int bodyindex, MeVector3 velocity);



/*
  Fixed path joint mutators.
*/

void              MEAPI MdtFixedPathSetVelocity(MdtFixedPathID j, const unsigned int bodyindex,
                            const MeReal dx, const MeReal dy, const MeReal dz);



#define                 MdtFixedPathDestroy(j)  MdtConstraintDestroy(j)
#define                 MdtFixedPathEnable(j)  MdtConstraintEnable(j)
#define                 MdtFixedPathDisable(j)  MdtConstraintDisable(j)
#define                 MdtFixedPathIsEnabled(j)  MdtConstraintIsEnabled(j)
#define                 MdtFixedPathGetBody(j, bodyindex)  MdtConstraintGetBody(j, bodyindex)
#define                 MdtFixedPathGetUserData(j)  MdtConstraintGetUserData(j)
#define                 MdtFixedPathGetWorld(j)  MdtConstraintGetWorld(j)
#define                 MdtFixedPathGetForce(j, bodyindex, f)  MdtConstraintGetForce(j, bodyindex, f)
#define                 MdtFixedPathGetTorque(j, bodyindex, t)  MdtConstraintGetTorque(j, bodyindex, t)
#define                 MdtFixedPathSetBodies(j, b1, b2)  MdtConstraintSetBodies(j, b1, b2)
#define                 MdtFixedPathSetUserData(j, d)  MdtConstraintSetUserData(j, d)

#ifdef __cplusplus
}
#endif


#endif
