#ifndef _MDTHINGE_H
#define _MDTHINGE_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/02/23 15:42:03 $
   
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
 * MdtHinge API functions.

 The geometry of this joint is such that during the simulation, a line
 l0 fixed in body b0 will be coincident to a line l1 fixed in body1,
 and the origin of l1 is fixed relative to the origin of l0.  This
 alows relative rotation of body b0 and b1 about the common axis
 defined by l0 or l1.  For convenience, the origin of l0 and l1 is
 chosen to coincide and this point is called the joint position.  This
 means that in world coordinates, the direction vector of line l0 and
 line l1 will be almost identical and so will be the world coordinates
 of the origin of line l0 and line l1.

 The remaining degree of freedom of the hinge, the joint angle, can be
 fully controlled using the MdtLimit API.  This allows to set joint limits,
 motors, locks etc.  @see MdtLimitID

 */

#include "MePrecision.h"
#include "MdtTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif



MdtConstraintID        MEAPI MdtHingeCreate(const MdtWorldID w);
void              MEAPI MdtHingeReset(MdtConstraintID j);
MdtConstraintID        MEAPI MdtHingeCreateInPool(MePool *pool);

/**
   The following macros are deprecated and provided only for backwards
   compatibility.  Use MdtConstraintXXX functions.  This will be nixed in
   vortex revision 3.0.  
*/

#define                 MdtHingeDestroy(j)    MdtConstraintDestroy(j)
#define                 MdtHingeEnable(j)     MdtConstraintEnable(j)
#define                 MdtHingeDisable(j)    MdtConstraintDisable(j)
#define                 MdtHingeIsEnabled(j)  MdtConstraintIsEnabled(j)



/*
  Hinge joint accessors 
*/


#define                 MdtHingeGetLimit(j)                MdtConstraintGetLimit(j) 
#define                 MdtHingeGetPosition(j, v)          MdtConstraintGetPosition(j, v)
#define                 MdtHingeGetAxis(j, v)              MdtConstraintGetAxis(j, v)
#define                 MdtHingeGetBody(j, bodyindex)      MdtConstraintGetBody(j, bodyindex)
#define                 MdtHingeGetUserData(j)             MdtConstraintGetUserData(j)
#define                 MdtHingeGetWorld(j)                MdtConstraintGetWorld(j)


/*
  Hinge joint mutators.
*/

#define                 MdtHingeSetLimit(j, v)              MdtConstraintSetLimit(j, v) 
#define                 MdtHingeSetPosition(j, x, y, z)     MdtConstraintSetPosition(j, x, y, z)
#define                 MdtHingeSetAxis(j, x, y, z)         MdtConstraintSetAxis(j, x, y, z)
#define                 MdtHingeSetBodies(j, b1, b2)        MdtConstraintSetBodies(j, b1, b2)
#define                 MdtHingeSetUserData(j, d)           MdtConstraintSetUserData(j, d)


#ifdef __cplusplus
}
#endif


#endif
