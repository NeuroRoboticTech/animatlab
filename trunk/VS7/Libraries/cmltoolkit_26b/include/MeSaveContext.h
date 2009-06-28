#ifndef _MESAVECONTEXT_H
#define _MESAVECONTEXT_H
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
    Save context API.
*/


#include "MeXMLTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* create and destroy */
MeSaveContext *     MEAPI MeSaveContextCreate(void);
void                MEAPI MeSaveContextDestroy(MeSaveContext *sc);

/* accessors */
MeHash *            MEAPI MeSaveContextGetHash(MeSaveContext *sc);

/* mutators */
void                MEAPI MeSaveContextSetUserdataHandler(MeSaveContext *sc,
                                      Save_UserdataCB cb,void *userdata);
void                MEAPI MeSaveContextSetOptions(MeSaveContext *sc,int options);


/* option flags */
#define             MDT_SAVE_CONTACTS                  0x00000001
#define             MCD_DONT_SAVE_OBJS                 0x00000002


/* Internal functions */

MeBool              MEAPI MeSave(MeSaveContext *sc,MeStream stream);
MeSaveDatum *       MEAPI MeSaveHashInsert(void *ptr,char *ID,
                                           MeDatumType_enum type,MeHash *hash);

#ifdef __cplusplus
}
#endif


#endif /* _MESAVECONTEXT_H */


