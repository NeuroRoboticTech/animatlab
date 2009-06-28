#ifndef _MSTBRIDGE_H
#define _MSTBRIDGE_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:51:51 $
   
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
 * Collision <-> Dynamics Toolkit Bridge API.
 *
 * Mst Bridge provides the data required to transfer contact geometry
 * information from the Vortex Collision Toolkit to the Vortex
 * Dynamics Toolkit.  This consists of a 'material table', used to set
 * the dynamics properties of contacts (friction, restitution etc.),
 * and buffers used during contact.

 * This is used by MstUniverseStep to set the physical parameters of
 * dynamics contacts (such as friction, restitution etc.) based on the
 * materials of the two geometries in contact. It also holds an optional
 * callback for each MstMaterialID pair.
 */


#ifdef __cplusplus
extern "C" {
#endif

#include "MstTypes.h"

#define MstBridgeGetDefaultMaterial() (0)


MstBridgeID           MEAPI MstBridgeCreate(const unsigned int maxMaterials);
void                  MEAPI MstBridgeDestroy(const MstBridgeID b);


void                  MEAPI MstBridgeStepAll(const MstBridgeID bridge,
                                const MeReal stepSize);
void                  MEAPI MstBridgeUpdateContacts(const MstBridgeID b,
                                                    const McdSpaceID s,
                                                    const MdtWorldID w);
void                  MEAPI MstBridgeUpdateTransitions(const MstBridgeID b,
                                                       const McdSpaceID s,
                                                       const MdtWorldID w);

MdtContactParamsID    MEAPI MstBridgeGetContactParams(const MstBridgeID b,
                                const MstMaterialID m1,
                                const MstMaterialID m2);
MstPerPairCB          MEAPI MstBridgeGetPerPairCB(const MstBridgeID b,
                                const MstMaterialID m1,
                                const MstMaterialID m2, void** userData);
MstPerContactCB       MEAPI MstBridgeGetPerContactCB(const MstBridgeID b,
                                const MstMaterialID m1,
                                const MstMaterialID m2, void** userData);
MstMaterialID         MEAPI MstBridgeGetNewMaterial(const MstBridgeID b);
MstIntersectCB        MEAPI MstBridgeGetIntersectCB(const MstBridgeID b,
                                const MstMaterialID m1,
                                const MstMaterialID m2, void** userData);

void                  MEAPI MstBridgeSetPerPairCB(const MstBridgeID b,
                                const MstMaterialID m1,
                                const MstMaterialID m2,
                                const MstPerPairCB cb, void* userData);
void                  MEAPI MstBridgeSetPerContactCB(const MstBridgeID b,
                                const MstMaterialID m1,
                                const MstMaterialID m2,
                                const MstPerContactCB cb, void* userData);
void                  MEAPI MstBridgeSetIntersectCB(const MstBridgeID b,
                                const MstMaterialID m1,
                                const MstMaterialID m2,
                                const MstIntersectCB cb, void* userData);

void                  MEAPI MstBridgeSetIntersectNewCB(const MstBridgeID b, 
                                const MstResponseID r1, const MstResponseID r2,
                                const MstIntersectActiveCB cb, void* userData);
void                  MEAPI MstBridgeSetIntersectActiveCB(const MstBridgeID b, 
                                const MstResponseID r1, const MstResponseID r2,
                                const MstIntersectActiveCB cb, void* userData);
void                  MEAPI MstBridgeSetIntersectInactiveCB(const MstBridgeID b, 
                                const MstResponseID r1, const MstResponseID r2,
                                const MstIntersectInactiveCB cb, void* userData);
void                  MEAPI MstBridgeSetDisjointCB(const MstBridgeID b, 
                                const MstResponseID r1, const MstResponseID r2,
                                const MstDisjointCB cb, void* userData);

void                  MEAPI MstBridgeGetIntersectNewCB(const MstBridgeID b, 
                                const MstResponseID r1, const MstResponseID r2,
                                MstIntersectActiveCB *cb, void** userData);
void                  MEAPI MstBridgeGetIntersectActiveCB(const MstBridgeID b, 
                                const MstResponseID r1, const MstResponseID r2,
                                MstIntersectActiveCB *cb, void** userData);
void                  MEAPI MstBridgeGetIntersectInactiveCB(const MstBridgeID b, 
                                const MstResponseID r1, const MstResponseID r2,
                                MstIntersectInactiveCB *cb, void** userData);
void                  MEAPI MstBridgeGetDisjointCB(const MstBridgeID b, 
                                const MstResponseID r1, const MstResponseID r2,
                                MstDisjointCB *cb, void** userData);

void                  MEAPI MstBridgeSetModelPairBufferSize(const MstBridgeID b,
                                const unsigned int s);
void                  MEAPI MstBridgeSetContactBufferSize(const MstBridgeID b,
                                const unsigned int s);

unsigned int          MEAPI MstBridgeGetNewResponseID(const MstBridgeID b);
unsigned int          MEAPI MstBridgeGetNewResponseIDFromTemplateID(const MstBridgeID b, const MstResponseID templateID);
MeBool                MEAPI MstBridgeResponseIDOfSameType(const MstBridgeID b, const MstResponseID ida, const MstResponseID idb);

MeBool                MEAPI MstBridgeRegisterResponse(MstBridgeID b, unsigned int id1, unsigned int id2, MstResponseHelloFn fh, MstResponseFn fr, MstResponseGoodbyeFn fg, McdRequest*request, void*data);

MstResponsePair*      MEAPI MstBridgeGetResponse(MstBridgeID b, unsigned int id1, unsigned int id2);

void                  MEAPI MstBridgeSetMaterialMaxCount(const MstBridgeID b, const unsigned int count);

#ifdef __cplusplus
}
#endif

#endif
