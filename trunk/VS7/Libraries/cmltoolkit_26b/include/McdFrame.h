#ifndef _MCDFRAME_H
#define _MCDFRAME_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:50:40 $
   
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

/**
  @file McdFrame.h
  Collision Toolkit main header file.
*/

#include "MePrecision.h"
#include "MePool.h"
#include "MeMemory.h"
#include "McdGeometry.h"
#include "McdModel.h"
#include "McdModelPair.h"
#include "McdSpace.h"
#include "McdLineSegIntersect.h"
#include "McdContact.h"
#include "McdInteractions.h"
#include "McdEach.h"
#include <stddef.h> /* for size_t */


#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------
 * memory management
 *---------------------------------------------------------------
 */

#ifdef MCD_NO_MEMEMORY

  /** @internal */
  void              MEAPI McdMemorySetFunctions(void *(*create)(size_t bytes),
                            void *(*resize)(void * block, size_t bytes),
                            void (*destroy)(void * block) );

#endif

/*----------------------------------------------------------------
 * Framework
 *---------------------------------------------------------------
 */

  /** @internal */
typedef
void (*McdTermAction)(void);

void              MEAPI McdInit( int geoTypeMaxCount, int modelCount);
void              MEAPI McdTerm();

void              MEAPI McdRegisterTermAction( McdTermAction );

int               MEAPI McdFrameRef();
int               MEAPI McdFrameUnref();
int               MEAPI McdFrameGetRef();

  /* Collision state struct */

typedef struct
{
  McdTermAction action;
  void *next;

} McdTermActionLink;


/* global collision framework structure */
typedef struct
{
  int refCount;
  McdTermActionLink *termActions;
  MePool modelPool;
  MeReal mDefaultPadding;
  const char *toolkitVersionString;
} McdFramework;

  /** @internal */
extern McdFramework *gMcdFramework;

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* _MCDFRAME_H */
