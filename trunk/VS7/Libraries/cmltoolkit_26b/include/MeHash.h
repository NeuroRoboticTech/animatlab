#ifndef _MEHASH_H
#define _MEHASH_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:51:07 $
   
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
 *  Generic hash table implementation.
 */

#include "MeCall.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef int  (MEAPI *MeHashFunc)(const void *key,int size);
typedef int  (MEAPI *MeHashCompareFunc)(const void *key1,const void *key2);
typedef void (MEAPI *MeHashFreeFunc)(void *);

typedef struct MeHash           MeHash;
typedef struct MeHashBucket     MeHashBucket;
typedef struct MeHashIterator   MeHashIterator;

struct MeHashBucket
{
    void *key;
    void *datum;
    MeHashBucket *next;
};

struct MeHash
{
    int size;
    int population;
    MeHashFunc hash;
    MeHashCompareFunc compare;
    MeHashFreeFunc freeKey;
    MeHashFreeFunc freeDatum;
    MeHashBucket **table;
    unsigned long collisions;
};

struct MeHashIterator
{
    MeHash *table;
    MeHashBucket *bucket;
    int index;
};

MeHash *          MEAPI MeHashCreate(int size);
void              MEAPI MeHashDestroy(MeHash *table);
void              MEAPI MeHashReset(MeHash *table);

void *            MEAPI MeHashInsert(const void *key,const void *datum,MeHash *table);
void *            MEAPI MeHashDelete(const void *key,MeHash *table);

void *            MEAPI MeHashLookup(const void *key,const MeHash *table);

void              MEAPI MeHashSetHashFunc(MeHash *table,MeHashFunc hash);
void              MEAPI MeHashSetKeyCompareFunc(MeHash *table,
                                                MeHashCompareFunc compare);

void              MEAPI MeHashSetKeyFreeFunc(MeHash *table,MeHashFreeFunc keyFree);
void              MEAPI MeHashSetDatumFreeFunc(MeHash *table,MeHashFreeFunc datumFree);

int               MEAPI MeHashPopulation(const MeHash *table);

MeHashIterator *  MEAPI MeHashInitIterator(MeHashIterator *i,
                            const MeHash *table);

void *            MEAPI MeHashGetDatum(MeHashIterator *i);


#ifdef __cplusplus
}
#endif

#endif /* _MEHASH_H */
