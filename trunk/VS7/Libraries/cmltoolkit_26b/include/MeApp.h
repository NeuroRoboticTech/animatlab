#ifndef _MEAPP_H
#define _MEAPP_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/12/08 14:31:43 $
   
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
  Demo support functions.
*/

#include "Mst.h"
#include "MeViewer.h"
#include "MeAppTypes.h"


#ifdef __cplusplus
extern "C" {
#endif

MeApp          *MEAPI MeAppCreateFromUniverse(const MstUniverseID u, RRender *rc);

MstUniverseID   MEAPI MeAppGetMstUniverse(MeApp *app);
void            MEAPI MeAppSetMstUniverse(MeApp *app, const MstUniverseID u);

void *          MEAPI MeAppGetUserData(MeApp *app);
void            MEAPI MeAppSetUserData(MeApp *app, void* userData);

MeApp          *MEAPI MeAppCreate(const MdtWorldID world,
                                  const McdSpaceID space,RRender *rc);

void            MEAPI MeAppDestroy(MeApp* const app);

/* Graphics-related functions */

void            MEAPI MeAppDrawContactsInit(MeApp* const app, float color[4],int max);

void            MEAPI MeAppDrawContacts(const MeApp *app);
void			MEAPI MeAppSetContactDrawLength(MeApp *app, MeReal length);
void			MEAPI MeAppSetContactDrawType(MeApp *app, enum MeAppDrawContactEnum type);
void			MEAPI MeAppToggleDrawContacts(MeApp *app, MeBool d);
int 			MEAPI MeAppDrawContactsEnabled(MeApp *app);

void            MEAPI MeAppFindClickDir(const MeApp *const app, int x, int y,
                          MeVector3 normClickDir);

McdModelID      MEAPI MeAppPickMcdModel(const MeApp *const app,
                                         int x, int y,
                                         MeVector3 normClickDir,
                                         MeVector3 pos);

void            MEAPI MeAppUpdateMouseSpring(MeApp *app);

void            MEAPI MeAppMousePickCB(RRender *rc, int x, int y,
                        int modifiers, RMouseButtonWhich which,
                        RMouseButtonEvent event,void *userdata);

void            MEAPI MeAppStep(MeApp *app);

RGraphic *      MEAPI MeAppRGraphicFromModelCreate(RRender *rc,McdModelID model,float color[4]);

void            MEAPI MeAppSetModelPickCallback(MeApp *app, McdLineSegIntersectEnableCallback cb);


#ifdef __cplusplus
}
#endif

#endif
