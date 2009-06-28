#ifndef _MCDSPACE_H
#define _MCDSPACE_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:50:41 $
   
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
    @file McdSpace.h

    Manages the large-scale spatial properties of a region of 3D space
    populated by McdModel objects.
    Its principal role is to keep track of proximities between many McdModel objects.

    There are two types of operations on McdSpace objects: state-query and
    state-modification. State-query functions can only be used when the
    state is well-defined, i.e. not in the process of being modified.
    Once modifications to the state begin to be applied (signalled by a call to
    McdSpaceBeginChanges() ) , the original state is
    no longer available for query. When the set of modifications have been
    completed, ( indicated by McdSpaceEndChanges() ) the new state is
    properly defined and ready to be queried again. The current mode is
    indicated by McdSpaceIsChanging().

    Pairwise proximities are reported using a retained-mode mechanism that
    maintains identity of a given pair over successive states:
    When pairs of McdModel objects are first detected to be in close
    proximity, a McdModelPair object is assigned to them.
    In subsequent steps, the same McdModelPair
    object will be re-used to refer to the same pair.
    When the pair is no longer in proximity, the McdModelPair object is
    recycled as needed for use as a reference to a differnt pair.

    The continuity of McdModelPair identity is exploited by many of the Mcd
    interaction algorithms, and also by response modules such as
    the Vortex Dynamics Toolkit and the Vortex Simulation Toolkit.

*/

#include "McdModelPairContainer.h"
#include "McdLineSegIntersect.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------
 *  McdSpace
 *----------------------------------------------------------------
 */


  /** @internal */
typedef struct _McdSpace McdSpace;
  /** @internal */
typedef McdSpace *McdSpaceID;


extern int McdXAxis;    /**< X-axis to be or'ed with McdYAxis and McdZAxis */
extern int McdYAxis;    /**< Y-axis to be or'ed with McdXAxis and McdZAxis */
extern int McdZAxis;    /**< Z-axis to be or'ed with McdXAxis and McdYAxis */
extern int McdAllAxes;  /**< A constant equal to McdXAxis+McdYAxis+McdZAxis.*/

typedef void (MEAPI *McdSpaceGetAABBFn)(McdModelID m, MeVector3 minCorner, MeVector3 maxCorner, MeReal);

McdSpaceID        MEAPI McdSpaceAxisSortCreate( int axes, int objectCount, int pairCount );
void              MEAPI McdSpaceDestroy( McdSpaceID s );
void              MEAPI McdSpaceBuild( McdSpaceID s );
MeBool            MEAPI McdSpaceInsertModel( McdSpaceID s, McdModelID cm );
MeBool            MEAPI McdSpaceRemoveModel( McdModelID );
void              MEAPI McdSpaceUpdateModel( McdModelID );
MeBool            MEAPI McdSpaceFreezeModel( McdModelID );
MeBool            MEAPI McdSpaceUnfreezeModel( McdModelID );
MeBool            MEAPI McdSpaceModelIsFrozen( McdModelID );
void              MEAPI McdSpaceUpdateAll( McdSpaceID s );
void              MEAPI McdSpacePathUpdateAll( McdSpaceID s, MeReal duration );

void              MEAPI McdSpaceBeginChanges( McdSpaceID space );
void              MEAPI McdSpaceEndChanges( McdSpaceID space );

void              MEAPI McdSpaceSetAABBFn( McdSpaceID s, McdSpaceGetAABBFn getAABBFn);
int               MEAPI McdSpaceGetLineSegIntersections( McdSpaceID space,
                                  MeReal* inOrig, MeReal* inDest,
                                  McdLineSegIntersectResult *outList,
                                  int inMaxListSize);
int               MEAPI McdSpaceGetLineSegFirstIntersection( McdSpaceID space,
                          MeReal* inOrig, MeReal* inDest,
                                  McdLineSegIntersectResult *outResult );

int               MEAPI McdSpaceGetLineSegFirstEnabledIntersection(
                                McdSpaceID space,
                                MeReal* inOrig, MeReal* inDest,
                                McdLineSegIntersectEnableCallback filterCB,
                                void * filterData,
                                McdLineSegIntersectResult *outResult );

MeBool            MEAPI McdSpaceEnablePair( McdModelID cm1, McdModelID cm2 );
MeBool            MEAPI McdSpaceDisablePair( McdModelID cm1, McdModelID cm2 );
MeBool            MEAPI McdSpacePairIsEnabled( McdModelID cm1, McdModelID cm2 );
MeBool            MEAPI McdSpacePairIsNearby( McdModelID m1, McdModelID m2 );
McdModelPairID    MEAPI McdSpaceGetModelPair( McdModelID m1, McdModelID m2 );

void              MEAPI McdSpaceSetUserData( McdSpaceID s, void *data );
void *            MEAPI McdSpaceGetUserData( McdSpaceID s );

/** Structure holding iteration information for McdSpaceGetPairs */
typedef struct
{
  void *m_h; /**< @internal */
  void *m_s; /**< @internal */
  void *m_g; /**< @internal */

} McdSpacePairIterator;

/** @internal */
void              MEAPI McdSpacePairIteratorBegin( McdSpaceID,
                            McdSpacePairIterator* );

/** @internal */
int               MEAPI McdSpaceGetPairs( McdSpaceID, McdSpacePairIterator*,
                            McdModelPairContainer* );

int               MEAPI McdSpaceGetPairsIncludingFrozen( McdSpaceID, McdSpacePairIterator*,
                            McdModelPairContainer* );

int               MEAPI McdSpaceGetTransitions( McdSpaceID s, McdSpacePairIterator* iter,
                                McdModelPairContainer* a);

/** @internal */
/*
void               MEAPI McdSpaceGetPairs_list( McdSpaceID,
                           McdPairContainer* );
*/

/** Structure holding iteration information for McdSpaceGetModel */
typedef struct
{
  int it;
  int count;
} McdSpaceModelIterator;

void              MEAPI McdSpaceModelIteratorBegin( McdSpaceID,
                            McdSpaceModelIterator *it);

MeBool            MEAPI McdSpaceGetModel( McdSpaceID,
                            McdSpaceModelIterator *it, McdModelID* );


int               MEAPI McdSpaceGetModelCount( McdSpaceID space );


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* _MCDSPACE_H */
