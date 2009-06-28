#ifndef _MEPOOL_H
#define _MEPOOL_H
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
 *  Memory pool implementation.
 */

#include "MePrecision.h"

typedef struct MePool MePool;


/* #define ME_FIXED_SIZED_POOL */
#ifdef ME_FIXED_SIZED_POOL

/**
 * Simple fixed-size pool (used for bodies etc.)
 */
struct MePool
{
    /** Actual chunk of memory holding structs. */
    void                *structArray;

    /** Stack of pointers to free structs. */
    void                **freeStructStack;

    /** Index of next free pointer in stack. */
    int                 nextFreeStruct;

    /** Size of each struct in the pool. */
    int                 structSize;

    /** Total number of structs in pool. */
    int                 poolSize;

    /**
     * Indicates if the pool memory was allocated with
     * createAligned or create.
     */
    MeBool              createdAligned;
};

#else

/**
 * Resizable pool (used for bodies etc.)
 */
    
/* linked list node overlaid on top of freed user structures */

struct MePoolElement;
struct MePoolBlock;

typedef struct MePoolElement MePoolElement;
typedef struct MePoolBlock MePoolBlock;

struct MePool
{
    int structSize;         /** Size of struct */
    int elementSize;        /** Size of structs to store in bytes including alignement */
    int currElementCount;   /** number of structs per block in bytes */
    const char * name;      /** pool name for user */
    
    MePoolBlock* currBlock; /** current unfilled block */
    char* nextFreePtr;      /** next free unused element pointer in current block */
    
    MePoolElement *nextFreeListElement;    /* linked list of freed elements: */
 
    /* allocation schedule: 
    newblocksize = currBlockSize*blockSizeMultiplier + 
    blockSizeIncrement. 
    They could both be 0, indicating no resize operations allowed.
    This could also be in a callback or simplified
    */
    float blockSizeMultiplier;
    int blockSizeIncrement;

    int alignment;

    int totalElementCount;

    int freedElementCount; /* used only for debugging */

};

#endif

#ifdef __cplusplus
extern "C"
{
#endif

void    MEAPI MePoolInit      (MePool* pool, int poolSize, int structSize, int alignment);
/* void    MEAPI MePoolSetResizeParameters(MePool* pool, ...); */
void    MEAPI MePoolSetPoolName(MePool* pool, const char*name);
void    MEAPI MePoolTerm      (MePool* pool);
void    MEAPI MePoolReset     (MePool* pool);
void   *MEAPI MePoolGetStruct (MePool* pool);
void    MEAPI MePoolPutStruct (MePool* pool, void* s);
int     MEAPI MePoolGetUsed   (MePool* pool);
int     MEAPI MePoolGetStructSize(MePool* pool);
/* int     MEAPI MePoolGetUnused (MePool* pool); */

#ifdef __cplusplus
}
#endif

#endif /* _MEPOOL_H */
