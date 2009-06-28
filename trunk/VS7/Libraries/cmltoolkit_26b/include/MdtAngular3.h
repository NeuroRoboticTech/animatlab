#ifndef _MDTANGULAR3_H
#define _MDTANGULAR3_H
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
 * MdtAngular3 API functions.
 *
 * This joint constrains one body to have a fixed orientation with
 * respect to another, and does not constrain either boies' position.
 * This constraint may, at the user's choice, become an Angular 2
 * joint by freeing rotation about a specified axis.
 * The default orientation that is maintained by this constraint
 * is that of the bodies when MdtAngular3SetBodies is called.





 */

#include "MePrecision.h"
#include "MdtTypes.h"


#ifdef __cplusplus
extern "C"
{
#endif


MdtAngular3ID     MEAPI MdtAngular3Create(const MdtWorldID w);
void              MEAPI MdtAngular3Reset(MdtAngular3ID j);



/*
  Angular3 joint accessors.
*/

MeBool            MEAPI MdtAngular3RotationIsEnabled( const MdtAngular3ID j );

/*
  Angular3 joint mutators
*/

void              MEAPI MdtAngular3EnableRotation( const MdtAngular3ID j, const MeBool NewRotationState );



#define           MdtAngular3Destroy(j)    MdtConstraintDestroy(j)
#define           MdtAngular3Enable(j)     MdtConstraintEnable(j)
#define           MdtAngular3Disable(j)    MdtConstraintDisable(j)
#define           MdtAngular3IsEnabled(j)  MdtConstraintIsEnabled(j)

#define           MdtAngular3GetAxis(j, axis)           MdtConstraintGetAxis(j, axis)
#define           MdtAngular3GetBody(j, bodyindex)      MdtConstraintGetBody(j, bodyindex)
#define           MdtAngular3GetUserData(j)             MdtConstraintGetUserData(j)
#define           MdtAngular3GetWorld(j)                MdtConstraintGetWorld(j)
#define           MdtAngular3GetForce(j, bodyindex, f)  MdtConstraintGetForce(j, bodyindex, f)
#define           MdtAngular3GetTorque(j, bodyindex, t) MdtConstraintGetTorque(j, bodyindex, t)


#define           MdtAngular3SetAxis(j, x, y, z)        MdtConstraintSetAxis(j, x, y, z)
#define           MdtAngular3SetBodies(j, b1, b2)       MdtConstraintSetBodies(j, b1, b2)
#define           MdtAngular3SetUserData(j, d)          MdtConstraintSetUserData(j, d)



#ifdef __cplusplus
}
#endif


#endif
