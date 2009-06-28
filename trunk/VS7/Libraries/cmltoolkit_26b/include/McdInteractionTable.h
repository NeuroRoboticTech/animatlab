#ifndef _MCDINTERACTIONTABLE_H
#define _MCDINTERACTIONTABLE_H
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

#include "McdIntersectResult.h"
#include "McdSafeTimeResult.h"
#include "McdDistanceResult.h"
#include "McdLineSegIntersect.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------
 * struct McdInteractions
 *---------------------------------------------------------------
 */

  /* return value indicates success: possible failure,
     in cases when hello allocates coherence data, eg from a pool
     of limited resources */

typedef MeBool  (MEAPI *McdHelloFn)(McdModelPair*);

typedef void (MEAPI *McdGoodbyeFn)(McdModelPair*);

typedef struct
{
  McdHelloFn helloFn;
  McdGoodbyeFn goodbyeFn;

  McdIntersectFn intersectFn;
  McdSafeTimeFn safetimeFn;

  McdDistanceFn distanceFn;
  McdApproximateDistanceFn approxDistanceFn;

} McdInteractions;

void MEAPI McdInteractionsInit( McdInteractions* );

void MEAPI McdInteractionsCopy( McdInteractions* src,
                McdInteractions* dest );

typedef McdInteractions* (MEAPI *McdRequestBrokerFn)( McdModelPair* p );

/*----------------------------------------------------------------
 * McdInteractionTable
 *---------------------------------------------------------------
 */

typedef struct
{
  MeBool swap;
  McdInteractions interactions;
  McdRequestBrokerFn brokerFn;

} McdInteractionTableElement;

typedef struct
{
  int size;
  McdInteractionTableElement *array;

} McdInteractionTable;

extern McdInteractionTable gMcdInteractionTable;

void              MEAPI McdInteractionTableInit( int geoTypeMaxCount );

void              MEAPI McdInteractionTableTerm();

/* get ptr to table element, used by interaction registration
   functions to set the values */

McdInteractionTableElement* MEAPI
McdInteractionTableGetElementPtr( int geoType1, int geoType2);

/* convenience: set elementPtr for (i,j);swap=0 and for (j,i);swap=1
 */
void MEAPI
McdInteractionTableSetElement( int geoType1, int geoType2,
                   McdInteractions* interactions,
                   McdRequestBrokerFn brokerFn );


/* called at hello time to resolve correct function for a given pair */
/* a convenience wrapper to GetElementPtr() */
McdInteractions* MEAPI
McdInteractionTableGetInteractions( McdModelPair*, MeBool *swap );

#ifdef MCDCHECK
void MEAPI McdInteractionTablePrint();
#endif

#define MCDINTERACTIONTABLE_REGISTERINTERSECTONLY(_A_,_B_)\
extern "C"\
MeBool            MEAPI Mcd##_A_##_B_##RegisterInteraction() \
{ \
  McdInteractions interactions;\
  McdRequestBrokerFn brokerFn;\
\
  brokerFn = 0;\
  interactions.helloFn = 0;\
  interactions.goodbyeFn = 0;\
  interactions.intersectFn = Mcd##_A_##_B_##Intersect;\
  interactions.safetimeFn = 0;\
  interactions.distanceFn = 0;\
  interactions.approxDistanceFn = 0;\
\
  McdInteractionTableSetElement( Mcd##_A_##GetTypeId(), \
                 Mcd##_B_##GetTypeId(), \
                 &interactions, \
                 brokerFn );\
  return 1;\
}
#define MCDINTERACTIONTABLE_REGISTERINTERSECTANDSAFETIMEONLY(_A_,_B_)\
extern "C"\
MeBool            MEAPI Mcd##_A_##_B_##RegisterInteraction() \
{ \
  McdInteractions interactions;\
  McdRequestBrokerFn brokerFn;\
\
  brokerFn = 0;\
  interactions.helloFn = 0;\
  interactions.goodbyeFn = 0;\
  interactions.intersectFn = Mcd##_A_##_B_##Intersect;\
  interactions.safetimeFn =  Mcd##_A_##_B_##SafeTime;\
  interactions.distanceFn = 0;\
  interactions.approxDistanceFn = 0;\
\
  McdInteractionTableSetElement( Mcd##_A_##GetTypeId(), \
                 Mcd##_B_##GetTypeId(), \
                 &interactions, \
                 brokerFn );\
  return 1;\
}


void MEAPI
McdInteractionTableRegisterDistanceFn( int geoType1, int geoType2, McdDistanceFn distanceFn);

void MEAPI
McdInteractionTableRegisterApproximateDistanceFn( int geoType1, int geoType2, McdApproximateDistanceFn adistanceFn);

/* line segment */
void MEAPI
McdLineSegRegisterInteraction( int geoType1, McdLineSegIntersectFn isectfn );

/* line segment */
McdLineSegIntersectFn MEAPI
McdLineSegGetIntersectFnPtr( int geoType );


/* Notes

3 options for optimization:
i) pair points to an McdInteractionTableElement;
( potential data locality problems;
but no more data non-locality than looking up at ix time )

ii) pair holds an McdInteractionTableElement by-value,
    and data is copied into it at hello time.
( footprint of pair increases )

iii) pairs holds only those data elements it wants,
     copying them in at hello time.
( larger footprint than i), but ensures data locality;
  smaller footprint than ii), but ( a little) more code complexity
  to maintain )

Initial implementation is i) ( simplest ).

 */


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* _MCDINTERACTIONTABLE_H */
