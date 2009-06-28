#ifndef _MDTCONTACT_H
#define _MDTCONTACT_H
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
 * MdtContact API functions.

 This API is mostly used by the collision detection module or during
 contact specific callback functions.  The contact constraint differs from
 other constraints in that the position and normal direction of the contact
 are specified at each step in world coordinates.

 */

#include "MePrecision.h"
#include "MdtTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif



/*
  Contact functions.
*/

MdtContactID      MEAPI MdtContactCreate(const MdtWorldID w);
void              MEAPI MdtContactReset(MdtContactID c);



/*
  Contact accessors.
*/

void               MEAPI MdtContactGetPosition(const MdtContactID c, MeVector3 v);
void               MEAPI MdtContactGetNormal(const MdtContactID c, MeVector3 v);
MeReal             MEAPI MdtContactGetPenetration(const MdtContactID c);
void               MEAPI MdtContactGetDirection(const MdtContactID c, MeVector3 v);
void               MEAPI MdtContactGetSlipVelocity(const MdtContactID c, MeVector3 v);
MdtContactParamsID MEAPI MdtContactGetParams(const MdtContactID c);
MdtContactID       MEAPI MdtContactGetNext(const MdtContactID c);

/** Invalid ContactID indicating end of MdtContact linked list. */
extern const MdtContactID MdtContactInvalidID;



/*
  Contact mutators.
*/

void              MEAPI MdtContactSetPosition(MdtContactID c, const MeReal x, const MeReal y, const MeReal z);
//void              MEAPI MdtContactSetSense(MdtContactID c,int iSense);
//void              MEAPI MdtContactSetPrimarySense(MdtContactID c,int iSense);
//void              MEAPI MdtContactSetSecondarySense(MdtContactID c,int iSense);

void              MEAPI MdtContactSetNormal(MdtContactID c, const MeReal x, const MeReal y, const MeReal z);
void              MEAPI MdtContactSetPenetration(MdtContactID c, const MeReal p);
void              MEAPI MdtContactSetDirection(MdtContactID c, const MeReal x, const MeReal y, const MeReal z);
void              MEAPI MdtContactSetParams(MdtContactID c, const MdtContactParamsID p);
void              MEAPI MdtContactSetNext(MdtContactID c, const MdtContactID nc);


#define   MdtContactDestroy(c)   MdtConstraintDestroy(c)
#define   MdtContactEnable(c)    MdtConstraintEnable(c)
#define   MdtContactDisable(c)   MdtConstraintDisable(c)
#define   MdtContactIsEnabled(c) MdtConstraintIsEnabled(c)
#define   MdtContactGetBody(c, bodyindex)  MdtConstraintGetBody(c, bodyindex)
#define   MdtContactGetUserData(c)  MdtConstraintGetUserData(c)
#define   MdtContactGetWorld(c)  MdtConstraintGetWorld(c)
#define   MdtContactGetForce(c, bodyindex, f)  MdtConstraintGetForce(c, bodyindex, f)
#define   MdtContactGetTorque(c, bodyindex, t) MdtConstraintGetTorque(c, bodyindex, t)
#define   MdtContactSetBodies(c, b1, b2) MdtConstraintSetBodies((c), b1, b2)
#define   MdtContactSetUserData(c, d) MdtConstraintSetUserData((c), d)


#ifdef __cplusplus
}
#endif


#endif
