#ifndef _MEMEMORY_H
#define _MEMEMORY_H
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

#include "MeCall.h"
#include "MeAssert.h"
#include <stdlib.h>


/**
  @file MeMemory.h
  Memory allocation and deallocation. 
  All such operations in Vortex go through the MeMemoryAPI functions
  which can be overridden by the user.
*/

/* Stack allocation macro */
#if (defined PS2 && defined __GNUC__)
#  define MeMemoryALLOCA(n) (alloca(n))
#  define MeMemoryFREEA(b)  ((void) 0)
#elif defined __GNUC__
#  define MeMemoryALLOCA(n) (__builtin_alloca(n))
#  define MeMemoryFREEA(b)  ((void) 0)
#elif defined IRIX
#  include <alloca.h>
#  define MeMemoryALLOCA(n) (alloca(n))
#  define MeMemoryFREEA(b)  ((void) 0)
#elif defined _MSC_VER
#  include <malloc.h>
#  define MeMemoryALLOCA(n) (_alloca(n))
#  define MeMemoryFREEA(b)  ((void) 0)
#else
#  define MeMemoryALLOCA(n) (MeMemoryAPI.create(n))
#  define MeMemoryFREEA(b)  (MeMemoryAPI.destroy(b))
#endif


#define MeMemoryQUADALIGNED(n) \
    (MEASSERT((((long unsigned) (n)) % 16) == 0))

#define MeMemoryQUADALIGN(a) \
    ((void *) ((((long unsigned) (void *) (a)) + 15) &~ 15))


#ifdef MeMemoryALLOCA
# define MeMemoryQUADALIGNEDALLOCA(name, type, n) \
        void *name##Addr = \
            MeMemoryALLOCA(((n) * sizeof (type)) + 16); \
        type *name = (type *) MeMemoryQUADALIGN(name##Addr);
# define MeMemoryQUADALIGNEDFREEA(name) \
        MeMemoryFREEA(name##Addr)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef void *(MEAPI *MeMemoryFuncPtrCreate)(size_t bytes);
typedef void *(MEAPI *MeMemoryFuncPtrCreateAligned)(size_t bytes, unsigned int alignment);
typedef void  (MEAPI *MeMemoryFuncPtrDestroy)(void *const block);
typedef void *(MEAPI *MeMemoryFuncPtrResize)(void *const block, size_t bytes);

/** Structure holding all memory functions. */
typedef struct MeMemoryAPIStruct
{
    MeMemoryFuncPtrCreate create;               /**< Allocate memory and returns pointer to allocated memory block. */
    MeMemoryFuncPtrCreate createZeroed;         /**< Allocate memory and clear it to zero. */
    MeMemoryFuncPtrCreateAligned createAligned; /**< Allocate memory aligned to given byte boundary. */

    MeMemoryFuncPtrDestroy destroy;             /**< Free memory pointed to by block pointer. */
    MeMemoryFuncPtrDestroy destroyAligned;      /**< Free memory that was allocated using createAligned. */

    MeMemoryFuncPtrResize resize;               /**< Resize memory that was allocated using create. */
} MeMemoryAPIStruct;

/** Global pointer to memory API. */
extern MeMemoryAPIStruct MeMemoryAPI;


int     MEAPI MeMemoryCheckGetAllocatedBlockCount();
size_t  MEAPI MeMemoryCheckGetAllocatedByteCount();
int     MEAPI MeMemoryCheckMemory(void (MEAPI* cb)(void*, void*, int ));

#ifdef __cplusplus
}
#endif

#endif /* _MEMEMORY_H */
