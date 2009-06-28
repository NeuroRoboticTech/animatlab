#ifndef _MDTUTILITIES_H
#define _MDTUTILITIES_H
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
 * Utility API functions.

 This is a collection of utilities to convert position and axial vectors
 between variety of reference frames and to setup inertia tensors.
 
 */

#include "MePrecision.h"
#include "MdtTypes.h"


#ifdef __cplusplus
extern "C"
{
#endif


/*
  Public utility functions.
*/

void  MEAPI MdtConvertVector(const MdtBodyID from_body,
                            const MeVector3 f, const MdtBodyID to_body,
                            MeVector3 t);
void  MEAPI MdtConvertPositionVector(const MdtBodyID from_body,
                            const MeVector3 f, const MdtBodyID to_body,
                            MeVector3 t);


void  MEAPI MdtConvertCOMVector(const MdtBodyID from_body,
                            const MeVector3 f, const MdtBodyID to_body,
                            MeVector3 t);
void  MEAPI MdtConvertCOMPositionVector(const MdtBodyID from_body,
                            const MeVector3 f, const MdtBodyID to_body,
                            MeVector3 t);
void  MEAPI MdtMakeInertiaTensorSphere(const MeReal mass,
                            const MeReal radius, MeMatrix3 i);

void  MEAPI MdtMakeInertiaTensorBox(const MeReal mass,
                            const MeReal lx, const MeReal ly, const MeReal lz,
                            MeMatrix3 i);

void  MEAPI MdtLimitController(const MdtLimitID limit,
                            const MeReal desiredPosition, const MeReal gap,
                            const MeReal maxSpeed, 
                               const MeReal minForce,
                               const MeReal maxForce);

void  MEAPI MdtConvertSoftnessDampingToEpsilonGamma(MeReal softness, MeReal damping,
                                                  MeReal *epsilon, MeReal *gamma, MeReal h);

void  MEAPI MdtConvertStiffnessDampingToEpsilonGamma(MeReal stiffness, MeReal damping,
                                                  MeReal *epsilon, MeReal *gamma, MeReal h);

#ifdef __cplusplus
}
#endif


#endif
