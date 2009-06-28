#ifndef _MDTCONTACTPARAMS_H
#define _MDTCONTACTPARAMS_H
/* -*- mode: C; -*- */

/*
  Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
  http://www.cm-labs.com

  $Name: t-pre-multithread-050318 $
   
  Date: $Date: 2004/10/06 09:42:29 $
   
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
 * MdtContactParams API functions.

 This controls the behavior of the forces generated at contact points to
 prevent interpenetration and simulate dry friction.  Use this API to
 specify which type of friction you want on given contact points.
 Management of contact parameters for pairs of material is provided in the
 Mst API.  


*/

#include "MePrecision.h"
#include "MdtTypes.h"


#ifdef __cplusplus
extern "C"
{
#endif


/*
  Contact Parameters functions
*/
    
    
    void              MEAPI MdtContactParamsReset(MdtContactParamsID p);
    
/*
  Contact Parameters accessors
*/

    MdtContactType    MEAPI MdtContactParamsGetType( const MdtContactParamsID p);
    MdtFrictionModel  MEAPI MdtContactParamsGetPrimaryFrictionModel( const MdtContactParamsID p);
    MdtFrictionModel  MEAPI MdtContactParamsGetSecondaryFrictionModel( const MdtContactParamsID p);
    MdtFrictionModel  MEAPI MdtContactParamsGetFrictionModel( const MdtContactParamsID p);
    MeReal            MEAPI MdtContactParamsGetRestitution( const MdtContactParamsID p);
    MeReal            MEAPI MdtContactParamsGetRestitutionThreshold( const MdtContactParamsID p);
    MeReal            MEAPI MdtContactParamsGetSoftness( const MdtContactParamsID p);
    MeReal            MEAPI MdtContactParamsGetDamping( const MdtContactParamsID p);
    MeReal            MEAPI MdtContactParamsGetMaxAdhesiveForce( const MdtContactParamsID p);
    MeReal            MEAPI MdtContactParamsGetPrimaryFriction( const MdtContactParamsID p);
    MeReal            MEAPI MdtContactParamsGetPrimarySlip( const MdtContactParamsID p);
    MeReal            MEAPI MdtContactParamsGetPrimarySlide( const MdtContactParamsID p);
    MeReal            MEAPI MdtContactParamsGetSecondaryFriction( const MdtContactParamsID p);
    MeReal            MEAPI MdtContactParamsGetSecondarySlip( const MdtContactParamsID p);
    MeReal            MEAPI MdtContactParamsGetSecondarySlide( const MdtContactParamsID p);

    MeReal            MEAPI MdtContactParamsGetPrimaryFrictionCoefficient(
        const MdtContactParamsID p);
    MeReal            MEAPI MdtContactParamsGetSecondaryFrictionCoefficient(
        const MdtContactParamsID p);


    MeReal            MEAPI MdtContactParamsGetFrictionCoefficient(const MdtContactParamsID p);
/*
  Contact Parameters mutators
*/

    void              MEAPI MdtContactParamsSetType(const MdtContactParamsID p,
                                                    const MdtContactType t);
    void              MEAPI MdtContactParamsSetFrictionModel( const MdtContactParamsID p, 
                                                              const MdtFrictionModel m);
    void              MEAPI MdtContactParamsSetPrimaryFrictionModel( const MdtContactParamsID p, 
                                                                     const MdtFrictionModel m);
    void              MEAPI MdtContactParamsSetSecondaryFrictionModel( const MdtContactParamsID p, 
                                                                       const MdtFrictionModel m);
    void              MEAPI MdtContactParamsSetRestitution( const MdtContactParamsID p, 
                                                            const MeReal r);
    void              MEAPI MdtContactParamsSetRestitutionThreshold( const MdtContactParamsID p, 
                                                                     const MeReal v);
    void              MEAPI MdtContactParamsSetDamping( const MdtContactParamsID p, 
                                                        const MeReal s);
    void              MEAPI MdtContactParamsSetSoftness( const MdtContactParamsID p, 
                                                         const MeReal s);
    void              MEAPI MdtContactParamsSetMaxAdhesiveForce( const MdtContactParamsID p, 
                                                                 const MeReal s);
    void              MEAPI MdtContactParamsSetPrimaryFriction( const MdtContactParamsID p, 
                                                                const MeReal f);
    void              MEAPI MdtContactParamsSetPrimarySlip( const MdtContactParamsID p, 
                                                            const MeReal s);
    void              MEAPI MdtContactParamsSetPrimarySlide( const MdtContactParamsID p, 
                                                             const MeReal s);
    void              MEAPI MdtContactParamsSetSecondaryFriction( const MdtContactParamsID p, 
                                                                  const MeReal f);

    void   MEAPI MdtContactParamsSetPrimaryFrictionSense(MdtContactParamsID c,int i);
    void   MEAPI MdtContactParamsSetSecondaryFrictionSense(MdtContactParamsID c,int i);
    void   MEAPI MdtContactParamsSetPrimaryFrictionLowerAndUpper(MdtContactParamsID c,
                                                                 MeReal lower,MeReal upper);
    void   MEAPI MdtContactParamsSetSecondaryFrictionLowerAndUpper(MdtContactParamsID c,
                                                                   MeReal lower,MeReal upper);

    void   MEAPI MdtContactParamsSetSecondarySlip( const MdtContactParamsID p, 
                                                              const MeReal s);
    void   MEAPI MdtContactParamsSetSecondarySlide( const MdtContactParamsID p, 
                                                               const MeReal s);
    void   MEAPI MdtContactParamsSetFriction( const MdtContactParamsID p, 
                                                         const MeReal f);

    void   MEAPI MdtContactParamsSetFrictionCoefficient( const MdtContactParamsID p, 
                                                                    const MeReal f);
    void   MEAPI MdtContactParamsSetPrimaryFrictionCoefficient( const MdtContactParamsID p, 
                                                                           const MeReal f);
    void   MEAPI MdtContactParamsSetSecondaryFrictionCoefficient( const MdtContactParamsID p, 
                                                                             const MeReal f);

    void MEAPI MdtContactParamsSetCohesion( const MdtContactParamsID p, MeReal C);
    void MEAPI MdtContactParamsSetInternalShearResistance( const MdtContactParamsID p, MeReal phi);
    void MEAPI MdtContactParamsSetNormalExponnent( const MdtContactParamsID p, MeReal N);
    void MEAPI MdtContactParamsSetNormalCoefficient( const MdtContactParamsID p, MeReal N);
    void MEAPI MdtContactParamsSetNormalDamping( const MdtContactParamsID p, MeReal K);
    void MEAPI MdtContactParamsSetShearModulus( const MdtContactParamsID p, MeReal K);
    void MEAPI MdtContactParamsSetShearDamping( const MdtContactParamsID p, MeReal K);
    void MEAPI MdtContactParamsSetArea(const MdtContactParamsID p, MeReal A);
    void MEAPI MdtContactParamsSetLoad(const MdtContactParamsID p, MeReal W);
    void MEAPI MdtContactParamsSetReferenceVelocity(const MdtContactParamsID p, MeReal v);


    MeReal MEAPI MdtContactParamsGetCohesion( const MdtContactParamsID p);
    MeReal MEAPI MdtContactParamsGetInternalShearResistance( const MdtContactParamsID p);
    MeReal MEAPI MdtContactParamsGetNormalExponnent( const MdtContactParamsID p);
    MeReal MEAPI MdtContactParamsGetNormalCoefficient( const MdtContactParamsID p);
    MeReal MEAPI MdtContactParamsGetNormalDamping( const MdtContactParamsID p);
    MeReal MEAPI MdtContactParamsGetShearModulus( const MdtContactParamsID p);
    MeReal MEAPI MdtContactParamsGetShearDamping( const MdtContactParamsID p);
    MeReal MEAPI MdtContactParamsGetArea(const MdtContactParamsID p);
    MeReal MEAPI MdtContactParamsGetLoad(const MdtContactParamsID p);
    MeReal MEAPI MdtContactParamsGetReferenceVelocity(const MdtContactParamsID p);

#ifdef __cplusplus
}
#endif


#endif
