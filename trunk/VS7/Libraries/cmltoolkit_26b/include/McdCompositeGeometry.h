#ifndef _MCDCOMPOSITEGEOMETRY_H
#define _MCDCOMPOSITEGEOMETRY_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:50:40 $
   
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
  @file McdCompositeGeometry.h
  The Composite hybrid geometry type
*/

#include "McdGeometry.h"
#include "McdInteractions.h"
#include "McdLineSegIntersect.h"

#ifdef __cplusplus
extern "C" {
#endif

/* @internal */
MCD_DECLARE_GEOMETRY_TYPE( McdComposite );

/* McdComposite is a concrete McdGeometry type. All functions taking type
    McdGeometryID as argument can be called with a McdCompositeID argument.
    @see McdGeometry.h
*/
typedef McdGeometryID McdCompositeID;

/*----------------------------------------------------------------
 * McdCompositeGeometry layout
 *----------------------------------------------------------------
 */

/* @internal Composite geometry, for internal use only */
typedef struct {
  McdGeometry m_g;
  int m_elemMaxCount;
  int m_elemCount;
  McdModelID *m_models;
  MeMatrix4Ptr *m_transforms;
    /* computed in McdCompsiteBuild() */
  MeReal m_radius;
  MeReal m_center[3];
} McdComposite;

McdCompositeID MEAPI
McdCompositeGeometryCreate( int elementMaxCount );

void MEAPI
McdCompositeGeometryBuild( McdCompositeID );

void MEAPI
McdCompositeGeometryRebuild( McdCompositeID );

void MEAPI
McdCompositeGeometryUpdateTransform( McdCompositeID g, MeMatrix4Ptr refTM);

int MEAPI
McdCompositeGeometryAddElement( McdCompositeID,
                McdModelID elemModel,
                MeMatrix4Ptr elemRelTransform );

int MEAPI
McdCompositeGeometryRemoveElement( McdCompositeID, int elemIndex);

void MEAPI
McdCompositeGeometryGetElement( McdCompositeID g, int elemIndex,
                McdGeometryID* elemGeometry,
                MeMatrix4Ptr* elemRelTransform );

McdModelID MEAPI
McdCompositeGeometryGetModel( McdCompositeID g, int elemIndex );

int MEAPI
McdCompositeGeometryGetElementCount( McdCompositeID );

int MEAPI
McdCompositeGeometryGetElementMaxCount( McdCompositeID );

McdModelID* MEAPI
McdCompositeGeometryDecompose( McdCompositeID );

/* interactions */

int MEAPI
McdCompositeGenericIntersect( McdModelPair* p, McdIntersectResult* result );

int MEAPI 
McdCompositeLineSegmentFirstEnabledIntersect( const McdModelID model,
          MeReal* const inOrig, MeReal* const inDest,
          McdLineSegIntersectEnableCallback filterCB,
          void * filterData,
          McdLineSegIntersectResult* outResult );

MeBool MEAPI
McdCompositeGenericRegisterInteractions();

void MEAPI
McdCompositeLineSegmentRegisterInteraction();

MeBool MEAPI
McdCompositeGenericRegisterDistance();


MeI16 MEAPI
McdCompositeGetMassPropertiesAndMass( McdGeometry *g, 
                        MeMatrix4 outRelTM, 
                        MeMatrix3 outInertia, 
                        MeReal *outVolume, MeReal *outMass);


MeI16 MEAPI
McdGetMassPropertiesAndMassFromModelList( McdModelID *models, int modelCount,
                                            MeMatrix4 outRelTM, 
                                            MeMatrix3 outInertia, 
                                            MeReal *outVolume, MeReal *outMass);


#define McdCompositeContactGetModelPtr(contact) McdCompositeContactGetModel1Ptr(contact)

/** Return the composite submodel corresponding to the pair.model1 involved in the given McdContact. @see McdIntersectResult. */
#define McdCompositeContactGetModel1Ptr(contact) ((McdModelID)((contact)->element1.ptr))

/** Return the composite submodel corresponding to the pair.model2 involved in the given McdContact. Valid only if the second models is also a composite. */
#define McdCompositeContactGetModel2Ptr(contact) ((McdModelID)((contact)->element2.ptr))

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MCDCOMPOSITEGEOMETRY_H */
