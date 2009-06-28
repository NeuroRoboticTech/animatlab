#ifndef _MSTUTILS_H
#define _MSTUTILS_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/06/08 22:40:45 $
   
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
 * Mst stand-alone utility API.
 */

#include "Mst.h"

#ifdef __cplusplus
extern "C" {
#endif

void          MEAPI MstAutoSetMassProperties(const MdtBodyID body,
                        const McdModelID model, const MeReal density);

void          MEAPI MstAutoSetMassProperties(const MdtBodyID body,
                        const McdModelID model, const MeReal density);

void          MEAPI MstHandleCollisions(McdModelPairContainer* pairs,
                        const McdSpaceID s,
                        const MdtWorldID w,
                        const MstBridgeID b);
void          MEAPI MstHandleTransitions(McdModelPairContainer* pairs, 
                        const McdSpaceID s,
                        const MdtWorldID w,
                        const MstBridgeID b);

void          MEAPI MstSetWorldHandlers(const MdtWorldID world);

McdModelID    MEAPI MstModelAndBodyCreate(const MstUniverseID u,
                              const McdGeometryID g, const MeReal density);

void          MEAPI MstModelAndBodyDestroy(const MstUniverseID u, const McdModelID m);

McdModelID    MEAPI MstFixedModelCreate(const MstUniverseID u,
                                        const McdGeometryID g,
                                        MeMatrix4Ptr transformation);

void          MEAPI MstFixedModelDestroy(const MstUniverseID u, const McdModelID m);

McdModelID    MEAPI MstCompositeModelAndBodyCreate(const MstUniverseID u,
                                                   int modelMaxCount, const MeReal density);

void          MEAPI MstCompositeModelAndBodyBuild(const MstUniverseID u, McdModelID model);

McdModelID    MEAPI MstKinematicModelAndBodyCreate(const MstUniverseID u,
                                            const McdGeometryID g);

McdModelID    MEAPI MstSensorModelCreate(const MstUniverseID u,
                                            const McdGeometryID g,
                                            MeMatrix4Ptr transformation);

McdModelID    MEAPI MstSensorCompositeModelCreate(const MstUniverseID u,
                                            int childMaxCount,
                                            MeMatrix4Ptr transformation);
/*
McdModelID    MEAPI MstParticleSystemModelCreate(const MstUniverseID u,
                                            const McdGeometryID g);
*/

void          MEAPI MstModelDestroy(const MstUniverseID u, const McdModelID m);
void          MEAPI MstModelSetTransform(const McdModelID m, MeMatrix4 tm);
void          MEAPI MstModelFreeze(const McdModelID m);
void          MEAPI MstModelDynamicsDisable(const McdModelID m);
void          MEAPI MstModelUnFreeze(const McdModelID m);
MeBool        MEAPI MstModelIsFrozen(const McdModelID m);

void          MEAPI MstUniverseRemoveModel(MstUniverseID u, McdModelID m);
void          MEAPI MstUniverseInsertModel(MstUniverseID u, McdModelID m, MdtBodyID b);
void          MEAPI MstUniverseRemoveAllModels(MstUniverseID u, MdtBodyID b);

struct MeCommandLineOptions;
void          MEAPI MstUniverseParseOptions(const MstUniverseID universe, struct MeCommandLineOptions *options);


#ifdef __cplusplus
}
#endif


#endif
