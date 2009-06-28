#ifndef _MCDPRIMITIVES_H
#define _MCDPRIMITIVES_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:50:41 $
   
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

/**
  @file McdPrimitives.h
  Geometrical primitive types and interactions
*/

#include "McdSphere.h"
#include "McdBox.h"
#include "McdPlane.h"
#include "McdCone.h"
#include "McdCylinder.h"
#include "McdInteractions.h"

#ifdef __cplusplus
extern "C" {
#endif

  /** Returns the number of primitive geometry types. This is the number of
      types involved in the registration functions
      McdPrimitivesRegisterTypes() and McdPrimitivesRegisterInteractions().
      The parameter to McdInit() must be at least as large as this number
      if you are going to register all the primitive types.  */

unsigned int          MEAPI McdPrimitivesGetTypeCount();

  /** Register all the primitive geometry types with the Mcd system */

void                  MEAPI McdPrimitivesRegisterTypes();

  /** Register all the available interactions between primitive geometry types.
   The geometry types must be registered before calling this function.
   Return value indicates success: resources may be allocated inside.
*/

MeBool            MEAPI McdPrimitivesRegisterInteractions();

  /** Register the sphere, box and plane geometry types */

void              MEAPI McdSphereBoxPlaneRegisterTypes();

  /** Register all available interactions between sphere, box and plane geometry types.
   The geometry types must be registered before calling this function. */

void              MEAPI McdSphereBoxPlaneRegisterInteractions();



  /* Individual primitive-primitive interaction registration */

MeBool            MEAPI McdSphereSphereRegisterInteraction();
MeBool            MEAPI McdBoxPlaneRegisterInteraction();
MeBool            MEAPI McdBoxBoxRegisterInteraction();
MeBool            MEAPI McdBoxSphereRegisterInteraction();
MeBool            MEAPI McdSpherePlaneRegisterInteraction();
MeBool            MEAPI McdBoxCylinderRegisterInteraction();
MeBool            MEAPI McdCylinderPlaneRegisterInteraction();
MeBool            MEAPI McdCylinderSphereRegisterInteraction();
MeBool            MEAPI McdCylinderCylinderRegisterInteraction();

MeBool            MEAPI McdConePlaneRegisterInteraction();
MeBool            MEAPI McdConeSphereRegisterInteraction();
MeBool            MEAPI McdConeBoxRegisterInteraction();
MeBool            MEAPI McdConeConeRegisterInteraction();
MeBool            MEAPI McdConeCylinderRegisterInteraction();

MeBool            MEAPI McdSphereTriangleListRegisterInteraction();
MeBool            MEAPI McdBoxTriangleListRegisterInteraction();
MeBool            MEAPI McdCylinderTriangleListRegisterInteraction();

  /* Individual primitive-primitive intersection and safetime functions */

int MEAPI
McdSphereSphereSafeTime( McdModelPair* p, MeReal maxTime, McdSafeTimeResult *result);

int MEAPI
McdSpherePlaneSafeTime( McdModelPair* p, MeReal maxTime, McdSafeTimeResult *result);

int MEAPI
McdBoxSphereSafeTime( McdModelPair* p, MeReal maxTime, McdSafeTimeResult *result);


int MEAPI
McdBoxBoxSafeTime( McdModelPair* p, MeReal maxTime, McdSafeTimeResult *result);

int MEAPI
McdBoxPlaneSafeTime( McdModelPair* p, MeReal maxTime, McdSafeTimeResult *result);


int MEAPI
McdCylinderSphereSafeTime( McdModelPair* p, MeReal maxTime, McdSafeTimeResult *result);

int MEAPI
McdCylinderCylinderSafeTime( McdModelPair* p, MeReal maxTime, McdSafeTimeResult *result);

int MEAPI
McdBoxCylinderSafeTime( McdModelPair* p, MeReal maxTime, McdSafeTimeResult *result);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MCDPRIMITIVES_H */
