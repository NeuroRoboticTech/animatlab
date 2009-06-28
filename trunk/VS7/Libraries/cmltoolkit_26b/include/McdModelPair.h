#ifndef _MCDMODELPAIR_H
#define _MCDMODELPAIR_H
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
  @file McdModelPair.h
  Objects for tracking pairs of McdModel constituent objects

  McdModelPair objects identify and keep track of a particular pair of
  McdModel objects over multiple time steps, and to hold a variety of data
  associated with that pair.

  It is used by the farfield module to
  identify pairs of models that are nearby to each other and
  potentially colliding.

  It is used as the first argument to all interactions, such as
  McdIntersect and McdSafeTime. To prepare a McdModelPair object
  for use by these functions, McdHello must first be called. The pair is
  then ready to be used over multiple time steps. When the pair is no
  longer needed, this must be indicated by calling McdGoodbye.

*/

#include "McdModel.h"

#ifdef __cplusplus
extern "C" {
#endif


/*----------------------------------------------------------------
 * McdRequest
 *---------------------------------------------------------------
 */

  /** A McdRequest structure provides parameters that are used to characterise
      how various interaction algorithms are carried out.
      Each McdModelPair object points to a McdRequest structure,
      and all interactions take a McdModelPair object as first argument.
  */

typedef struct McdRequest
{
  /** The maximum number of McdContact objects to be produced when using
      McdIntersect(). This number represents only a target: the algorithm may
      fail to acheive this target, or may ignore it altogether. See the
      individual documentation for each geotype-geotype intersect function.
      To set a limit on the number of contacts that is guaranteed to be
      respected, use the contactMaxCount field of McdIntersectResult
      instead.
  */

  int contactMaxCount;

  /** When computing the normal for a given McdContact object,
      many algorithms simply use the normal from the surface at the point
      of contact. There are two surfaces to choose from, the first model or
      the second model. This parameter tells the algorithm to prefer using
      the surface that corresponds to a face, if there is one.
   */

  int faceNormalsFirst;

} McdRequest;

McdRequest*       MEAPI McdGetDefaultRequestPtr();
void              MEAPI McdSetDefaultRequestPtr(McdRequest *r);


void              MEAPI McdRequestDefaultInit();
void              MEAPI McdRequestInit(McdRequest* r);

void              MEAPI McdRequestCopy( McdRequest *read, McdRequest *write );

/*----------------------------------------------------------------
 *  McdModelPair
 *----------------------------------------------------------------
 */

  /** Hold a pair of McdModels for performing parwise collision tests. */

typedef struct McdModelPair
{
  /** @internal */
  McdModelID model1;
  /** @internal */
  McdModelID model2;

  /** @internal */
  McdRequest *request;

  /** @internal */
  void *userData;

  /** @internal */
  void *m_cachedData;

  /** @internal */
  void *m_intersectFn;
  /** @internal */
  void *m_safetimeFn;
  /** @internal */
  void *m_goodbyeFn;

  /** @internal */
  void *responseData;

  /** @internal */
  int m_flags;

} McdModelPair;

enum {McdModelPairFlagsWasIntersecting = 1};

  /** @internal */
typedef McdModelPair* McdModelPairID;

McdModelPairID    MEAPI McdModelPairCreate( McdModelID, McdModelID );

void              MEAPI McdModelPairDestroy( McdModelPairID );

void             MEAPI McdModelPairGetModels( McdModelPairID,
                          McdModelID *, McdModelID* );

void              MEAPI McdModelPairReset( McdModelPairID,
                       McdModelID, McdModelID );

McdRequest *      MEAPI McdModelPairGetRequestPtr( const McdModelPairID );

void             MEAPI McdModelPairSetRequestPtr( McdModelPairID,
                          McdRequest* );

void *            MEAPI McdModelPairGetUserData( McdModelPairID );

void             MEAPI McdModelPairSetUserData( McdModelPairID, void* );

void             MEAPI McdModelPairSetWasIntersecting( McdModelPairID p, MeBool wasIntersecting );

MeBool           MEAPI McdModelPairGetWasIntersecting( McdModelPairID p );

#ifdef MCDCHECK
  /** @internal */
/* convenience function for debug messages output */
void              MEAPI McdModelPairGetGeometryNames( McdModelPairID,
                            char** stringPtr1, char** stringPtr2 );
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#ifdef MCDCHECK

  /** @internal */
#define MCD_CHECK_MODEL_PAIR(p) {}

#else

  /** @internal */
#define MCD_CHECK_MODEL_PAIR(p)

#endif /* MCDCHECK */

#endif /* _MCDMODELPAIR_H */
