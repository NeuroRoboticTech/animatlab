#ifndef _MCDSAVE_H
#define _MCDSAVE_H
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
    Mcd API functions for saving.
*/

#include "MeSaveContext.h"
#include "McdGeometry.h"
#include "McdModel.h"
#include "McdSpace.h"
#include "McdTriangleMesh.h"
#include "McdConvexMesh.h"


#ifdef __cplusplus
extern "C" {
#endif

MeBool            MEAPI McdSave(MeSaveContext *sc,MeStream stream);

char            * MEAPI McdGeometryLookupStringID(McdGeometryID g,
                                                  MeHash *h);

char            * MEAPI McdModelLookupStringID(McdModelID m,
                                               MeHash *h);

MeBool            MEAPI McdGeometryHashInsert(McdGeometryID g,char *ID,
                                                 MeHash *h);

MeBool            MEAPI McdModelHashInsert(McdModelID m,char *ID,
                                           MeHash *h);

void              MEAPI McdSpaceHashInsertAll(McdSpaceID s,MeHash *h);

void              MEAPI McdModelSerialize( const McdModelID g,
                              char * id,char *geom, int inComposite, char *body,MeSaveContext *sc, MeStream s);

void              MEAPI McdTriangleMeshRegisterSaver(MeSaveContext *sc);

void              MEAPI McdTriangleMeshSerialize(const McdTriangleMeshID g,char *ID,MeSaveContext*,MeStream s);

void              MEAPI McdConvexMeshRegisterSaver(MeSaveContext *sc);

void              MEAPI McdConvexMeshSerialize(const McdConvexMeshID g,char *ID,MeSaveContext*,MeStream s);

void              MEAPI McdRGHeightFieldRegisterSaver(MeSaveContext *sc);

/* Internal */
MeBool            MEAPI McdWriteXML(MeSaveContext *sc,MeStream stream);


#ifdef __cplusplus
}
#endif


#endif



