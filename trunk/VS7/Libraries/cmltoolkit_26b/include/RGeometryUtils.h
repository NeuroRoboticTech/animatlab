#ifndef _RGEOMETRYUTILS_H
#define _RGEOMETRYUTILS_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/05/03 17:36:51 $
   
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

#include "MeViewer.h"
#include "MeASELoad.h"


#ifdef __cplusplus
extern "C"
{
#endif

void MEAPI RConvertTriStripToTriList(RGraphic* rg, RObjectVertex* strips,
                                     int* stripSize, int* stripStart, int numStrips);

void MEAPI RSetVertex(RObjectVertex* vertex,
                      MeVector3 vert, MeVector3 norm,
                      MeReal u, MeReal v);


int RCalculateTorusVertexCount(int sides, int rings);
void RCalculateTorusGeometry(RGraphic* rg, AcmeReal outerRadius,
                             AcmeReal innerRadius, int sides, int rings);

int RCalculateFrustumVertexCount(int sides);
void RCalculateFrustumGeometry(RGraphic* rg, AcmeReal bottomRadius,
                               AcmeReal topRadius, AcmeReal bottom,
                               AcmeReal top, int sides);

int RCalculateConeVertexCount(int sides);
void RCalculateConeGeometry(RGraphic* rg, AcmeReal radius,
                               AcmeReal bottom, AcmeReal top, int sides);

int RCalculateSphereVertexCount(int sides, int rings);
void RCalculateSphereGeometry(RGraphic* rg, AcmeReal radius,
                              int sides, int rings);

int RCalculateBoxVertexCount();
void RCalculateBoxGeometry(RGraphic* rg, AcmeReal lx,
                           AcmeReal ly, AcmeReal lz);

int RCalculateASEVertexCount(MeASEObject* object);
void RLoadASEGeometry(RGraphic* rg, MeASEObject* object);

int RCalculateCapsuleVertexCount(int sides, int rings);
void RCalculateCapsuleGeometry(RGraphic* rg, AcmeReal radius, AcmeReal height,
                              int sides, int rings);

int RCalculateDomeVertexCount(int sides, int rings);
void RCalculateDomeGeometry(RGraphic* rg, AcmeReal radius,
                            int sides, int rings,
                            int tileU, int tileV);

#ifdef __cplusplus
}
#endif
#endif
