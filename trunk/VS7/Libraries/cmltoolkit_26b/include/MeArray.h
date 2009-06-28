#ifndef _MEARRAY_H_
#define  _MEARRAY_H_

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

#include "MeAssert.h"
#include "MeMemory.h"



typedef struct MeArray {
  char* v;      /* data */
  int size;    /* actual memory requested */
  int capacity; /* memory allocated */
  int elementSize; /* byte size of each array element */
} MeArray;

#define MeArrayDECLARE(type) \
/* enum { MeArraySizeEnum##type = sizeof(type) }; */ \
typedef struct MeArray##type { \
  type * v;      /* data */ \
  int size;    /* actual memory requested */ \
  int capacity; /* memory allocated */ \
  int elementSize; /* byte size of each array element */ \
}

#define MeArrayDECLARE_PTR(type) \
/* enum { MeArraySizeEnum##type = sizeof(type*) }; */ \
typedef struct MeArray##type##Ptr { \
  type ** v;      /* data */ \
  int size;    /* actual memory requested */ \
  int capacity; /* memory allocated */ \
  int elementSize; /* byte size of each array element */ \
} 

#ifdef __cplusplus
extern    "C" {
#endif

/* generic functions */
void* MEAPI MeArrayInit_internal(MeArray *array, int size, int elementSize);
void* MEAPI MeArrayGrow_internal(MeArray *array, int growsize);
void* MEAPI MeArrayEnsureSize_internal(MeArray *array, int growsize);
int MEAPI MeArrayGetSize_internal(MeArray *array);
int MEAPI MeArrayGetCapacity_internal(MeArray *array);
void MEAPI MeArrayTerm_internal(MeArray *array);
void MEAPI MeArrayClear_internal(MeArray *array);

void* MEAPI MeSymUpperDiagonalResize(int newSize, int oldSize, int elementSize, void*v);

#ifdef __cplusplus
}
#endif

#define MeArrayINIT(array, size) \
    (MeArrayInit_internal((MeArray*) (array), (size), sizeof((array)->v[0])),(array)->v)

#define MeArrayTERM(array) MeArrayTerm_internal( (MeArray*)(array) )

#define MeArrayCLEAR(array) (MeArrayClear_internal( (MeArray*)(array) ))

/* assert as a "C" expression */
#define MEASSERTEXP(test) ((test)? 1 : ( MeFatalError(0,"Assertion failure: %s, in file %s, line %s.", # test, __FILE__,__LINE__) , 0))

#ifdef _MECHECK
# define MeArrayACCESS(array,position) \
    (*(MEASSERTEXP( 0<=(position) && (position)<(array)->size) ,(array)->v+(position)))
#else
# define MeArrayACCESS(array,position) ((array)->v[position])
#endif


#ifdef _MECHECK
# define MeArrayACCESSP(array,position) \
   (MEASSERTEXP( 0<=(position) && (position)<(array)->size) ,(array)->v+(position))
#else
# define MeArrayACCESSP(array,position) ((array)->v + (position))
#endif

#define MeArrayGET_SIZE(array) (MeArrayGetSize_internal((MeArray*) (array)))
#define MeArrayGET_CAPACITY(array) (MeArrayGetCapacity_internal((MeArray*) (array)))

/* return new element if successful, 0 otherwise */
#define MeArrayPUSHBACK(array, element) \
     ( (MeArrayGrow_internal((MeArray*)array, 1)? \
        (MeArrayACCESS((array),(array)->size-1) = element, &(MeArrayACCESS((array),(array)->size - 1))) : 0 )  )


/* pop an element. Returns size remaining. */
#define MeArrayPOP_BACK(array) ( ((array)->size)? --((array)->size) : 0)

/* return new element if successful, 0 otherwise */
#define MeArrayGROW_BY(array, newElementCount) (MeArrayGrow_internal((MeArray*)array, newElementCount))

/* return new element if successful, 0 otherwise */
#define MeArrayENSURE_SIZE(array, newElementCount) (MeArrayEnsureSize_internal((MeArray*)array, newElementCount))


#define MeArrayACCESS_LAST(array) \
  MeArrayACCESS(array,(array)->size-1)


#define MeArrayACCESS_LASTP(array) \
  MeArrayACCESSP(array,(array)->size-1)


#endif


