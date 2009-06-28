#ifndef _MCDRGHEIGHTFIELD_H
#define _MCDRGHEIGHTFIELD_H
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
  @file McdRGHeightField.h
  The regular-grid height field geometry type
*/

#include "McdGeometry.h"

#ifdef __cplusplus
extern "C" {
#endif

  /** McdRGHeightField is a concrete McdGeometry type. */

typedef McdGeometryID McdRGHeightFieldID;

/** Register the height field geometry type */
void              MEAPI McdRGHeightFieldRegisterType();

int               MEAPI McdRGHeightFieldGetTypeId();
void              MEAPI McdRGHeightFieldPrimitivesRegisterInteractions();
McdRGHeightFieldID MEAPI McdRGHeightFieldCreate( MeReal* heightArray,
                            int xVertexCount, int yVertexCount,
                            MeReal xIncrement, MeReal yIncrement,
                            MeReal x0, MeReal y0 );
void MEAPI McdRGHeightFieldGetParameters(McdGeometryID g,
                            MeReal** heightArray,
                            int *xVertexCount, int* yVertexCount,
                            MeReal *xIncrement, MeReal *yIncrement,
                            MeReal *x0, MeReal *y0);
void MEAPI McdRGHeightFieldSetHeightArray(McdGeometryID g, MeReal* heightArray);
MeReal* MEAPI McdRGHeightFieldGetHeightArray(McdGeometryID g);
MeBool MEAPI McdRGHeightFieldGetHeightAtLocal(McdGeometryID g, MeReal xLocal, MeReal yLocal, MeReal *outHeight);

void MEAPI McdRGHeightFieldSetForceTriangleNormalTolerance(McdGeometryID g, MeReal forceTriangleNormals);

void MEAPI McdRGHeightFieldComputeMinMaxHeight(McdGeometryID g);
void MEAPI McdRGHeightFieldGetIndexRangeMinMaxHeight(McdGeometryID g, int x0, int x1, int y0, int y1, 
                                                                    MeReal* minh, MeReal* maxh);
void MEAPI McdRGHeightFieldSetMinMaxHeight(McdGeometryID g, MeReal min, MeReal max);
void MEAPI McdRGHeightFieldGetMinMaxHeight(McdGeometryID g, MeReal *min, MeReal *max);
void MEAPI McdRGHeightFieldDestroy( McdGeometry* g);

  /* individual interactions */
MeBool            MEAPI McdBoxRGHeightFieldRegisterInteraction();
MeBool            MEAPI McdCylinderRGHeightFieldRegisterInteraction();
MeBool            MEAPI McdSphereRGHeightFieldRegisterInteraction();

/** Register the collision interaction between the height field geometry type and all primitives. */
void              MEAPI McdRGHeightFieldPrimitivesRegisterInteractions();

/** Register the interaction between the height field geometry type and line segments. */
void              MEAPI McdRGHeightFieldLineSegmentRegisterInteraction();

void              MEAPI McdBoxRGHeightFieldRegisterDistance();
void              MEAPI McdSphereRGHeightFieldRegisterDistance();

void              MEAPI McdPrimitiveRGHeightFieldApproximateDistance( struct McdModelPair *p, 
                                                                      struct McdDistanceRequest *dr,
                                                                      struct McdDistanceResult* r );

void              MEAPI McdPrimitiveRGHeightFieldRegisterDistance();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MCDRGHEIGHTFIELD_H */
