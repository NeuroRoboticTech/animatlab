#ifndef _MCDLOAD_H
#define _MCDLOAD_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:51:03 $
   
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
    Mcd API for loading and lookup.
*/

#include "McdSpace.h"
#include "McdTriangleMesh.h"
#include "McdConvexMesh.h"
#include "MeLoadContext.h"

#ifdef __cplusplus
extern "C" {
#endif

MeBool            MEAPI McdSpaceLoad(McdSpaceID space,
                                     MeStream stream,
                                     MeLoadContext *lc);

McdGeometryID     MEAPI McdGeometryLookup(char *ID,MeHash *h);
McdModelID        MEAPI McdModelLookup(char *ID,MeHash *h);
void              MEAPI McdTriangleMeshRegisterLoader(MeLoadContext *lc);
void              MEAPI McdConvexMeshRegisterLoader(MeLoadContext *lc);
void              MEAPI McdRGHeightFieldRegisterLoader(MeLoadContext *lc);

/* Internal */
MeXMLError        MEAPI Handle_Mcd(MeXMLElement * elem);

#ifdef __cplusplus
}
#endif


#endif
