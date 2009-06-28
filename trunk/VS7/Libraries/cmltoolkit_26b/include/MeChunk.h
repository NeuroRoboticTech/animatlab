#ifndef _MECHUNK_H
#define _MECHUNK_H
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
 *  Memory chunk implementation.
 */

#include "MePrecision.h"

typedef struct MeChunk MeChunk;


/**
 * Memory Chunk Overflow Callback
 * If the memory pool is found to be too small the user may specify a callback
 * that will be executed, allowing the user to change the size of the
 * chunk. The first parameter is the current MeChunk, the second is
 * the current MeChunk size and the third is the size of the memory required.
 * The callback should call MeChunkInitUser with a new piece of memory to
 * manually increase its size.
 */
typedef void
(MEAPI *MeChunkOverflowCB)(MeChunk* chunk, void *m, int current_size, int required_size);

/**
 * Memory Chunk.
 * Guarantees alignment, can auto-resize or otherwise inform user
 * with a callback if too small.
 */
struct MeChunk
{
    /** Original memory chunk. */
    void                *memBase;
    /** Aligned memory chunk. Above pointer shifted up to be aligned. */
    void                *memBaseAligned;

    /** Size of original memory chunk. */
    int                 memSize;
    /** Size of aligned memory chunk. */
    int                 memSizeAligned;

    /** Maximum size of chunk needed so far. */
    int                 maxUsed;

    /** Byte alignment of aligned chunk. */
    int                 alignment;
    /** If this chunk is auto-resized as needed. */
    MeBool              isAuto;
    /** User callback when chunk to small and not Auto. */
    MeChunkOverflowCB   overflowCallback;
};

#ifdef __cplusplus
extern "C"
{
#endif

void              MEAPI MeChunkInit             (MeChunk* chunk);

void              MEAPI MeChunkSetAuto          (MeChunk* chunk, int size,
                                                 int alignment);
void              MEAPI MeChunkSetUser          (MeChunk* chunk, void* m,
                                                 int size, int alignment);

void              MEAPI MeChunkDestroy          (MeChunk* chunk);

void              MEAPI MeChunkSetOverflowCB    (MeChunk* chunk, MeChunkOverflowCB cb);

void             *MEAPI MeChunkGetMem           (MeChunk* chunk, int size);
MeChunkOverflowCB MEAPI MeChunkGetOverflowCB    (MeChunk* chunk);
int               MEAPI MeChunkGetAlignment     (MeChunk* chunk);
int               MEAPI MeChunkGetMaxUsed       (MeChunk* chunk);

#ifdef __cplusplus
}
#endif

#endif /* _MECHUNK_H */
