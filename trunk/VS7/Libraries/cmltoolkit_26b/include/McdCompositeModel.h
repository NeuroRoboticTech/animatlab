#ifndef _MCDCOMPOSITEMODEL_H
#define _MCDCOMPOSITEMODEL_H
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
  @file McdCompositeModel.h
  Composite model type.
  This type holds a set of submodels forming a composite object.
  This facilitates manipulation of 'rigid bodies' composed of groupings
  of collections of primitive types.
  Collision is performed for a composite model by collecting and
  simplifying the collisions found for each of its submodels.
  Only the Composite Model itself need be added to a collision space,
  although the user of the composite model is responsible for the creation
  of each of the submodels.  These submodels should not be added to the
  collision space independently.  These submodels should be added to the
  composite model explicitly with a call to McdCompositeModelAdd() and these
  submodels should, of course, be destroyed by the user when no longer needed.
  All McdModel functions can also be applied
  to McdCompositeModel objects.

  Composites must be registered with Mcd by calling
  McdCompositeRegisterType() and McdCompositeGenericRegisterInteractions()
  after all other geometries have been registered.
*/

#include "McdModel.h"
#include "McdGeometry.h"
#include "McdCompositeGeometry.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef McdModelID McdCompositeModelID;
struct MePool;

McdModelID MEAPI
McdCompositeModelCreate( int childMaxCount );

McdModelID MEAPI
McdCompositeModelCreateInPool( int childMaxCount, struct MePool* pool );

McdModelID MEAPI
McdCompositeModelCreateFromCompositeGeometry( McdGeometryID g );

int MEAPI
McdCompositeModelAddElement( McdCompositeModelID composite,
             McdModelID child, MeMatrix4Ptr relTM );

int MEAPI
McdCompositeModelRemoveElement( McdCompositeModelID composite, int index);


void MEAPI
McdCompositeModelUpdateTransform( McdCompositeModelID composite );

McdModelID MEAPI
McdCompositeModelGetElement( McdCompositeModelID composite, int index );

MeMatrix4Ptr MEAPI
McdCompositeModelGetElementRelativeTransformPtr(McdCompositeModelID composite, int index );

void MEAPI
McdCompositeModelBuild( McdCompositeModelID composite );

int MEAPI
McdCompositeModelGetElementCount( McdCompositeModelID composite );

int MEAPI
McdCompositeModelGetElementMaxCount( McdCompositeModelID composite );

McdModelID* MEAPI
McdCompositeModelDecompose( McdCompositeModelID composite );

MeBool MEAPI
McdModelIsComposite( McdModelID );

McdModelID MEAPI
McdModelGetParent( McdModelID );

void MEAPI
McdCompositeModelUpdateTransform( McdCompositeModelID composite );

void MEAPI
McdCompositeModelDestroy( McdCompositeModelID composite );

void MEAPI
McdCompositeModelDestroyInPool( McdCompositeModelID composite, struct MePool* pool );

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MCDCOMPOSITEMODEL_H */
