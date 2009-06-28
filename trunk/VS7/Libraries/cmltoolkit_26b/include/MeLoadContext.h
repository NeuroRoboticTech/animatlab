#ifndef _MELOADCONTEXT_H
#define _MELOADCONTEXT_H
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
    Load context API.
*/

#include "MeXMLTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Create and destroy */
MeLoadContext *     MEAPI MeLoadContextCreate(void);
void                MEAPI MeLoadContextDestroy(MeLoadContext *lc);

/* Accessors */
char *              MEAPI MeLoadContextGetWorkingDirectory(MeLoadContext *lc);
MeHash *            MEAPI MeLoadContextGetHash(MeLoadContext *lc);
MeReal              MEAPI MeLoadContextGetScaleFactor(MeLoadContext *lc);
void *              MEAPI MeLoadContextGetUserdata(MeLoadContext *lc);

/* Mutators */
void                MEAPI MeLoadContextSetWorkingDirectory(MeLoadContext *lc,char *d);
void                MEAPI MeLoadContextSetTransform(MeLoadContext *lc,MeMatrix4Ptr tm);
void                MEAPI MeLoadContextSetScaleFactor(MeLoadContext *lc,MeReal scale);
void                MEAPI MeLoadContextSetOptions(MeLoadContext *lc,int options);
void                MEAPI MeLoadContextSetUserdataHandler(MeLoadContext *lc,
                                         Userdata_Handler h,void *userdata);

/* Others */
void                MEAPI MeLoadContextMakeFullID(char *IDpath,char *ID,char *fullID);
void                MEAPI MeLoadContextHashDisplay(MeHash *h);
void                MEAPI MeLoadContextHashConvert(MeHash *hash1,MeHash *hash2);

/* option flags */
#define             MDT_DISABLE_BODIES                 0x00000001
#define             MDT_DISABLE_CONSTRAINTS            0x00000002
#define             MDT_DONT_LOAD_FORCES               0x00000004
#define             MDT_DONT_LOAD_VELOCITIES           0x00000008


/* Internal functions */

MeBool              MEAPI MeLoad(MeLoadContext *lc,MeStream stream,char *IDprefix);

MeLoadDatum *       MEAPI MeLoadHashInsert(char *key,void *ptr,MeDatumType_enum type,
                                     char *associatedKey,MeLoadAction_enum action,
                                     char *me_file,MeHash *h);

void                MEAPI MakeGraphicForAllModels(MeLoadContext *lc);

void                MEAPI addToCleanupList(MeLoadContext *lc, void *ptr);


#ifdef __cplusplus
}
#endif


#endif /* _MELOADCONTEXT_H */
