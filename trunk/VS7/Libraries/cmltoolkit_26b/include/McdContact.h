#ifndef _MCDCONTACT_H
#define _MCDCONTACT_H
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
  @file McdContact.h
  Geometric contact point information
*/

#include "MePrecision.h"

struct McdIntersectResult;
struct McdRequest;


#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------
 *  McdContact
 *----------------------------------------------------------------
 */

/** Geometric contact point information, typically between two McdModel
    objects. An array of these structs is associated with a given
    McdModelPair via the McdIntersectResult struct, and computed via
    McdIntersect();

    Some of the information in the McdContact struct depends on the order of the @a
    model1 and @a model2 fields of the associated McdModelPair.
    @see McdModelPair, McdIntersectResult, McdIntersect
*/

typedef struct McdContact
{
  /** Position of the point of contact, in world coordinates. */
    MeVector3 position;
  /** Surface normal at the contact position. Sign chosen so as to point
      from @a model1 to @a model2 in the associated McdModelPair. */
    MeVector3 normal;
  /** Separation between objects, negative if there is penetration */
    MeReal separation;
#ifdef MCD_CURVATURE
  /** Local curvature of @a model1's surface at the contact position */
    MeReal curvature1;
  /** Local curvature of @a model2's surface at the contact position */
    MeReal curvature2;
#endif
  /** Dimensional characterisation of the contact. Possible values are  0,
      1 and 2, corresponding to  point, line and surface, respectively,
      for each byte in dims. This is the dimension of the feature (face/edge/vertex)
      that the contact is coming from for each model.
      The low byte corresponds to the local 
      surface of @a model1, the high byte to @a model2. */ 
  short dims;
  /** Auxiliary data used by some intersection functions. */
  union {void *ptr; int tag;} element1;
  /** Auxiliary data used by some intersection functions. */
  union {void *ptr; int tag;} element2;
} McdContact;

/** Utility function for reducing a list of contacts to
    a smaller list. Useful mostly when several models compose one object.
*/
int MEAPI McdContactSimplify( MeReal* avgNormal,
                  McdContact* contacts_src, int nContacts_src,
                  McdContact* contacts_dst, int nContacts_dest,
                  int faceNormalsFirst);

void MEAPI McdContactSimplifyBufferToResult( MeVector3 inNormal, McdContact* contactBuffer, 
                                             int contactCount, struct McdIntersectResult* result, 
                                             struct McdRequest* defaultRequest, const char* caller);

void MEAPI McdSetContactDegeneracyThreshold(MeReal minDistance, MeReal minCosAngle);

#define McdContactIsFaceContact(c) (((c)->dims & 0xFF) == 2 || (c)->dims >> 8)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MCDCONTACT_H */
