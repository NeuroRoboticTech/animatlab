#ifndef _MDTSAVE_H
#define _MDTSAVE_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/09/27 21:31:01 $
   
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
    Mdt API functions for saving in XML. Note that for all the serialization
    functions if you can pass in 0 for the stream, the data will be sent to
    MeInfo. This is very useful for debugging. You cannot say
    MdtBodySerialize(body,"BODY",stdout) because stdout does not exist on
    all platforms.
*/


#include "MdtTypes.h"
#include "MeSaveContext.h"

#ifdef __cplusplus
extern "C" {
#endif


MeBool              MEAPI MdtSave(MeSaveContext *sc,MeStream stream);

/* hash insert and lookup */

MeBool              MEAPI MdtBodyHashInsert(MdtBodyID b,char *ID,MeHash *h);
char *              MEAPI MdtBodyLookupStringID(MdtBodyID b,MeHash *h);

MeBool              MEAPI MdtConstraintHashInsert(MdtConstraintID c,char *ID,
                                                MeHash *h);
char *              MEAPI MdtConstraintLookupStringID(MdtConstraintID c,
                                                    MeHash *h);

MeBool              MEAPI MdtWorldHashInsert(const MdtWorldID w,char *ID,
                                           MeHash *h);
void                MEAPI MdtWorldHashInsertAll(const MdtWorldID w,
                                              MeHash *h);

#define                   MdtBSJointHashInsert(j, id, hash) \
                            MdtConstraintHashInsert(j,id,hash)

#define                   MdtHingeHashInsert(j, id, hash) \
                            MdtConstraintHashInsert(j,id,hash)

#define                   MdtPrismaticHashInsert(j, id, hash) \
                            MdtConstraintHashInsert(j,id,hash)

#define                   MdtCarWheelHashInsert(j, id, hash) \
                            MdtConstraintHashInsert(j,id,hash)

#define                   MdtFixedPathHashInsert(j, id, hash) \
                            MdtConstraintHashInsert(j,id,hash)


#define                   MdtLinear1HashInsert(j, id, hash) \
                            MdtConstraintHashInsert(j,id,hash)

#define                   MdtLinear2HashInsert(j, id, hash) \
                            MdtConstraintHashInsert(j,id,hash)

#define                   MdtUniversalHashInsert(j, id, hash) \
                            MdtConstraintHashInsert(j,id,hash)

#define                   MdtContactHashInsert(j, id, hash) \
                            MdtConstraintHashInsert(j,id,hash)

#define                   MdtSpringHashInsert(j, id, hash) \
                            MdtConstraintHashInsert(j,id,hash)

#define                   MdtAngular3HashInsert(j, id, hash) \
                            MdtConstraintHashInsert(j,id,hash)

#define                   MdtRPROJointHashInsert(j, id, hash) \
                            MdtConstraintHashInsert(j,id,hash)

/* Serialization functions */
void                MEAPI MdtWorldSerialize(const MdtWorldID w, char *ID,MeStream s);

void                MEAPI MdtBodySerialize(const MdtBodyID b, char *ID,MeStream s);

void                MEAPI MdtConstraintSerialize(const MdtConstraintID c, char *ID,
                                    char *body1, char *body2, MeStream s);

void                MEAPI MdtLimitSerialize(const MdtLimitID l, const int limitindex, const int savePos,
                                         MeStream s);

void                MEAPI MdtBSJointSerialize(const MdtBSJointID j,char *ID,
                                              char * body1, char * body2, MeStream s);


void                MEAPI MdtCarWheel2Serialize(const MdtCarWheel2ID j,char *ID,
                                            char * body1, char * body2, MeStream s);

void                MEAPI MdtFixedPathSerialize(const MdtFixedPathID j,char *ID,
                                             char * body1, char * body2, MeStream s);


void                MEAPI MdtHingeSerialize(const MdtHingeID j,char *ID,char * body1,
                                         char * body2, MeStream s);

void                MEAPI MdtLinear1Serialize(const MdtLinear1ID j,char *ID,
                                           char * body1, char * body2, MeStream s);

void                MEAPI MdtLinear2Serialize(const MdtLinear2ID j,char *ID,
                                           char * body1, char * body2, MeStream s);

void                MEAPI MdtPrismaticSerialize(const MdtPrismaticID j,char *ID,
                                             char * body1, char * body2, MeStream s);

void                MEAPI MdtUniversalSerialize(const MdtUniversalID j,char *ID,
                                                char * body1, char * body2, MeStream s);

void                MEAPI MdtContactSerialize(const MdtContactID c,char *ID,
                                        char *body1, char *body2, MeStream s);

void                MEAPI MdtContactParamsSerialize(const MdtContactParamsID p, MeStream s);

void                MEAPI MdtSpringSerialize(const MdtSpringID j,char *ID,
                                                char * body1, char * body2, MeStream s);

void                MEAPI MdtAngular3Serialize(const MdtAngular3ID j,char *ID,
                                                char * body1, char * body2, MeStream s);

void                MEAPI MdtRPROJointSerialize(const MdtRPROJointID j,char *ID,
                                 char *body1, char *body2, MeStream s);

/* Internal */
MeBool              MEAPI MdtWriteXML(MeSaveContext *sc,MeStream stream);

#ifdef __cplusplus
}
#endif



#endif

