#ifndef _MCDUREQUESTTABLE_H
#define _MCDUREQUESTTABLE_H
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

#include "McdModelPair.h"
#include "McdInteractions.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
  @file McduRequestTable.h
   This symmetric table maps an unordered pair of Request IDs
   to a request. This is typically used to set the maximum
   number of contacts to be generated
   by specific pairs of classes of colliding models.
   These classes are specified by request IDs set by the user, starting 
   at 0.
   For each pair of IDs, the McduRequestTableSetRequest function is called
   to specify the number of contacts to be generated when the system
   encounters a pair of models with that pair of request IDs.
   Use McdModelSetRequestID to set request ID for that model (0 be default).
   The default McdRequest object is the one that each McdModelPair object
   points to upon creation ( via McdModelPairCreate() ) and is 
   accessible via McdGetDefaultRequestPtr().
*/

typedef struct McduRequestTable
{
  int idCount;
  int size;

  McdRequest *ididToElement;
} McduRequestTable ;

extern McduRequestTable *gMcduRequestTable;

/*----------------------------------------------------------------
 *  McduRequestTable
 *----------------------------------------------------------------
 */

McduRequestTable * MEAPI McduRequestTableInit( int idMaxCount, McdRequest* );

void              MEAPI McduRequestTableTerm();

McduRequestTable *MEAPI McduRequestTableGet();

void              MEAPI McduRequestTableEnable();
void              MEAPI McduRequestTableDisable();

void              MEAPI McduRequestTableSetRequest( int requestId1,
                            int requestId2,
                            McdRequest* );

void              MEAPI McduRequestTableGetRequest( McdModelPair*, McdRequest* );

void              MEAPI McduHelloRequestTableCB( McdModelPair* p, void* data );

int               MEAPI McduRequestTableGetSize( );

McdRequest *      MEAPI McduRequestTableGetRequestPtr( int id1, int id2 );

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* _MCDUREQUESTTABLE_H */
