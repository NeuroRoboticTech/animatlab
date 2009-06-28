#ifndef _MCDMODELPAIRCONTAINER_H
#define _MCDMODELPAIRCONTAINER_H
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

/** @file McdModelPairContainer.h

    A container for McdModelPair objects.

    Intended to indicate pairs of McdModel objects that are in close
    proximity to each other.

    Three distinct states are identified: "hello", "staying" and "goodbye".
    When a pair of models are first detected to be in close proximity, the
    pair is in the "hello" state. When the pair continues to be in close
    proximity over successive time steps, the pair is in the "staying"
    state, and when the pair is first detected to be no longer in close
    proximity, it is a "goodbye" pair, After this point, the pair is no longer
    valid.


    The McdSpace module will respect these semantics and manage the state
    transitions for you. McdSpaceGetPairs() will fill in a
    McdModelPairContainer appropriately.

    This container is a compact way of passing on the results of McdSpace
    to various response modules, such as the Vortex Simulation Toolkit.

    Whether it is McdSpace or a module that you have written yourself that
    fills in a McdModelPairContainer, the categorization of the three
    states is essential for efficient use of the interaction functions such
    as McdIntersect or McdSafeTime:
    The "hello" and "goodbye" states match the McdHello, McdGoodbye
    calls that prepare a McdModelPair object for use.

    @see McdModelPair, McdSpaceGetPairs, McdHello, McdGoodbye, McdIntersect

 */

#include "McdModelPair.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------
 * McdModelPairContainer
 *---------------------------------------------------------------
 */

  /**
     An array of references to McdModelPair structures, in which three
     "nearby states" are distinguished:
     "hello", "staying" and "goodbye".

     The array is partioned into three index ranges, one for each of the
     distinct states:

     goodbye pairs:
     i = goodbyeFirst .. goodbyeEnd-1

     hello pairs:
     i = helloFirst .. helloEndStayingFirst - 1

     staying pairs:
     i = helloEndStayingFirst .. stayingEnd - 1

     frozen-frozen staying pairs (between frozen models):
     i = frozenFirst .. frozenEnd - 1

     This index scheme allows all "current pairs" ( hello +
     staying) to be iterated through in single loop using the index range
     i = helloFirst .. stayingEnd -1

   */

typedef struct
{
  McdModelPair **array;  /**< array of McdModel pairs */
  int size;              /**< number of elements in @a array */

  /** index range for "goodbye" pairs is:
      for( i = goodbyeFirst ; i < goodbyeEnd; ++i )
  */

  int goodbyeFirst;

  /** index range for "goodbye" pairs is:
      for( i = goodbyeFirst ; i < goodbyeEnd; ++i )
  */

  int goodbyeEnd;

  /** index range for "hello" pairs is:
      for( i = helloFirstIndex; i < helloEndStayingFirst ; ++i )
  */

  int helloFirst;

  /**  End of hello pairs, first of staying pairs.
       This index scheme allows all "current pairs" ( hello +
       staying) to be iterated through in single loop using the index range
       i = helloFirst .. stayingEnd -1

  */

  int helloEndStayingFirst;

  /** index range for "staying" pairs is:
      for( i = helloEndStayingFirst; i < stayingEnd ; ++i )
  */

  int stayingEnd;

  /** index range for "frozen-frozen staying" pairs is:
      for( i = frozenFirst; i < frozenEnd ; ++i )
  */

  int frozenFirst;

  int frozenEnd;

} McdModelPairContainer;

void              MEAPI McdModelPairContainerInit(
                            McdModelPairContainer*,
                            McdModelPair **array, int size );

McdModelPairContainer* MEAPI McdModelPairContainerCreate( int size );
void              MEAPI McdModelPairContainerDestroy(
                            McdModelPairContainer* a );

void              MEAPI McdModelPairContainerReset( McdModelPairContainer* );
int               MEAPI McdModelPairContainerGetGoodbyeCount(
                            McdModelPairContainer* );

int               MEAPI McdModelPairContainerGetHelloCount(
                            McdModelPairContainer* );

int               MEAPI McdModelPairContainerGetStayingCount(
                            McdModelPairContainer* );

int               MEAPI McdModelPairContainerGetFrozenCount(
                            McdModelPairContainer* );

McdModelPair**    MEAPI McdModelPairContainerGetGoodbyeArray(
                            McdModelPairContainer*, int *count );
McdModelPair**    MEAPI McdModelPairContainerGetHelloArray(
                            McdModelPairContainer*, int *count );
McdModelPair**    MEAPI McdModelPairContainerGetStayingArray(
                            McdModelPairContainer*, int *count );
McdModelPair**    MEAPI McdModelPairContainerGetFrozenArray(
                            McdModelPairContainer*, int *count );

#if 0
/* returns overflow: if non-0 pair was not added */
int               MEAPI McdModelPairContainerAddHelloPair
                            McdModelPairContainer*. McdModelPair *p);
int               MEAPI McdModelPairContainerAddGoodbyePair
                            McdModelPairContainer*. McdModelPair *p);
int               MEAPI McdModelPairContainerAddStayingPair
                            McdModelPairContainer*. McdModelPair *p);
int               MEAPI McdModelPairContainerAddFrozenPair
                            McdModelPairContainer*. McdModelPair *p);
#endif


#ifdef MCDCHECK
void              MEAPI McdModelPairContainerPrintStats(
                            McdModelPairContainer* );
#endif


/*----------------------------------------------------------------
 * McdModelPairContainerIterator
 *---------------------------------------------------------------
 */

  /** An iterator for the contents of a McdModelPairContainer.
      Currently used only in McdIntersectEach().

      @see McdIntersectEach
  */

typedef struct
{
  /** @internal */
  McdModelPairContainer *container;
  /* contacts generated beginning with the last staying index,
     down until first hello index */
  /** @internal */
  int unreadCurrentPairsLessThanIndex;

} McdModelPairContainerIterator;

void              MEAPI McdModelPairContainerIteratorInit(
                  McdModelPairContainerIterator*,
                            McdModelPairContainer* );

#if 0
/*----------------------------------------------------------------
 * McdPairContainerIterator
 *---------------------------------------------------------------
 */

  /** @internal */
typedef McdModelPair McdPair;

typedef struct
  /** @internal */
{
  McdPair *hello;
  McdPair *staying;
  McdPair *goodbye;

  int helloCount;
  int stayingCount;
  int goodbyeCount;

} McdPairContainer;

#ifdef MCDCHECK
  /** @internal */
void              MEAPI McdPairContainerPrintStats( McdPairContainer* );
#endif
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MCDMODELPAIRCONTAINER_H */
