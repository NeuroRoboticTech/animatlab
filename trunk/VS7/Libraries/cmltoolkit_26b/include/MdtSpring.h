#ifndef _MDTSPRING_H
#define _MDTSPRING_H
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
 * MdtSpring API functions.


 By default, this constraint maintains a fixed distance between a point p0
 fixed in body b0 and a point p1 fixed in body b1.  It can be relaxed to
 serve as a model for a spring and this is done with the
 MdtSpringSetStiffness and MdtSpringSetDamping API.  

 The constraint contains a distance MdtLimit which locks the distance between
 the bodies attachments points. For this type of limit, position should be interpret as 
 distance in MdtConstraintLimit api. The distance limit cannot be motorized. Lower and upper stops
 can be set and motorized on the limit.
     
 There is no good notion of axis so calls to MdtConstraintGetAxis functions will returned
 arbitrary values.

 */

#include "MePrecision.h"
#include "MdtTypes.h"


#ifdef __cplusplus
extern "C"
{
#endif


  MdtSpringID       MEAPI MdtSpringCreate(const MdtWorldID w);
  MdtSpringID       MEAPI MdtSpringCreateInPool(MePool *pool);
  void              MEAPI MdtSpringReset(MdtSpringID j);

  
  
  /*
    Spring joint mutators
  */
  
  void  MEAPI MdtSpringSetNaturalLength(MdtSpringID j, const MeReal NewNaturalLength);
  void  MEAPI MdtSpringSetStiffness(MdtSpringID j, const MeReal NewStiffness);
  void  MEAPI MdtSpringSetDamping(MdtSpringID j, const MeReal NewDamping);
  void  MEAPI MdtSpringSetMaxSpringForce( MdtSpringID J,  const MeReal maxForce );

  MeReal  MEAPI MdtSpringGetLength(MdtSpringID j);
  MeReal  MEAPI MdtSpringGetNaturalLength(MdtSpringID j);
  MeReal  MEAPI MdtSpringGetStiffness(MdtSpringID j);
  MeReal  MEAPI MdtSpringGetDamping(MdtSpringID j);
  MeReal  MEAPI MdtSpringGetMaxSpringForce( MdtSpringID J,  const MeReal maxForce );
  


#define  MdtSpringSetPosition(j, bodyindex, x, y, z ) MdtConstraintBodySetPosition(j, bodyindex, x, y, z )
#define  MdtSpringGetLimit(j) MdtConstraintGetLimit(j)
#define  MdtSpringSetLimit(j, v) MdtConstraintSetLimit(j, v)
#define  MdtSpringDestroy(j)  MdtConstraintDestroy(j)
#define  MdtSpringEnable(j)  MdtConstraintEnable(j)
#define  MdtSpringDisable(j)  MdtConstraintDisable(j)
#define  MdtSpringIsEnabled(j)  MdtConstraintIsEnabled(j)
#define  MdtSpringGetBody(j, bodyindex)  MdtConstraintGetBody(j, bodyindex)
#define  MdtSpringGetUserData(j)  MdtConstraintGetUserData(j)
#define  MdtSpringGetWorld(j)  MdtConstraintGetWorld(j)
#define  MdtSpringGetForce(j, bodyindex, f)  MdtConstraintGetForce(j, bodyindex, f)
#define  MdtSpringGetTorque(j, bodyindex, t)  MdtConstraintGetTorque(j, bodyindex, t)
#define  MdtSpringSetBodies(j, b1, b2)  MdtConstraintSetBodies(j, b1, b2)
#define  MdtSpringSetUserData(j, d)  MdtConstraintSetUserData(j, d)



#ifdef __cplusplus
}
#endif


#endif
