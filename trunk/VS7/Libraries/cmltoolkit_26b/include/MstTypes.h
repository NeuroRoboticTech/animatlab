#ifndef _MSTTYPES_H
#define _MSTTYPES_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/01/28 15:52:47 $
   
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
 * Mst Type Definitions.
 */

#include "MeMemory.h"
#include "MePool.h"
#include "McdFrame.h"
#include "McdModel.h"
#include "Mdt.h"

typedef struct  MstUniverse         MstUniverse;
typedef struct  MstUniverseSizes    MstUniverseSizes;
typedef struct  MstBridge           MstBridge;

typedef struct  MstGrouping         MstGrouping;
typedef struct  MdtBodyNode         MdtBodyNode;
typedef struct  MdtConstraintNode   MdtConstraintNode;
typedef struct  McdModelNode        McdModelNode;



typedef struct  MstMaterialPair     MstMaterialPair;
typedef struct  MstResponsePair     MstResponsePair;

typedef         MstUniverse*        MstUniverseID;
typedef         MstBridge*          MstBridgeID;
typedef         MstGrouping*        MstGroupingID;

typedef         unsigned int        MstMaterialID;
typedef         unsigned int        MstResponseID;

/**
 * Mst all contacts callback.
 * This is the prototype for the user-defined, optional pairwise-material
 * callback. The callback will be called once per pair of models. The
 * MdtContactID @a c is the first in a linked list of dynamic contacts. If the
 * callback returns 0, @b all contacts created for this pair will be removed.
 * Note that it can happen that this callback gets executed even when there are 0
 * contacts generated due to behaviour of some of the intersection functions.
 */
typedef MeBool
(MEAPI *MstPerPairCB)(McdIntersectResult* result, MdtContactID* c, void* userData);

/**
 * Mst per contact callback.
 * This is the prototype for the user-defined, optional pairwise-material
 * callback. The callback will be called once per contact between models.
 * If the callback returns 0, this will be removed.
 */
typedef MeBool
(MEAPI *MstPerContactCB)(McdIntersectResult* result, McdContact* colC,
                         MdtContactID dynC, void* userData);

/**
 * Mst 'intersect' model pair callback.
 * This is the prototype for the user-defined, optional per-intersection
 * callback, which is executed once for each successful intersection between
 * a pair models, before any dynamics contacts are generated, allowing the
 * user to (for example) perform further contact culling at this stage.
 */
typedef void
(MEAPI *MstIntersectCB)(McdIntersectResult* result, void* userData);

/**
 * Mst pair callback for pairs that are colliding. Similar to MstPerPairCB
 * but guarantees that the objects are colliding, and is
 * not dependent of the material types.
 * This is the prototype for the user-defined, optional pairwise
 * callback. The callback will be called once per pair of models. The
 * @a c parameter can be cast to an (MdtContactID *) for pairs arising from
 * rigid body collision. In this case,
 * MdtContactID @a firstContact = @a *c is the first in a linked list of dynamic contacts.
 * Note that @a c can not be 0, but @a *c can if there are no contacts.
 */
typedef void
(MEAPI *MstIntersectActiveCB)(McdIntersectResult* result, void** c, void *userData);

/**
 * Mst pair callback for frozen-frozen (inactive) pairs.
 * This is the prototype for the user-defined, optional pairwise
 * callback. The callback will be called once per pair of models. The
 * @a 'c' parameter can be cast to an MdtContactID * for pairs arising from
 * rigid body collision. In this case,
 * MdtContactID 'firstContact = *c' is the first in a linked list of dynamic contacts
 * which were created the last time this pair was active (at least one model non-frozen). 
 * Note that @a c can not be 0, but @a *c can if there are no contacts.
 */
typedef void
(MEAPI *MstIntersectInactiveCB)(McdModelPair* modelpair, void** c, void *userData);

/**
 * Mst pair callback for newly disjoint pairs.
 * This is the prototype for the user-defined, optional pairwise
 * callback. The callback will be called once per pair of models when they
 * become first non-intersecting.
 */
typedef void
(MEAPI *MstDisjointCB)(McdModelPair* modelpair, void *userData);


/**
 * @internal
 * Function to be called for each pair. Subject to change.
 */
typedef void (MEAPI *MstResponseHelloFn)(MstBridgeID b, McdModelPairID pair, void* module1, void* module2, MstResponsePair* response);
typedef void (MEAPI *MstResponseFn)(MstBridgeID b, McdModelPairID pair, McdIntersectResult* result, void* module1, void* module2, MstResponsePair* response);
typedef void (MEAPI *MstResponseGoodbyeFn)(MstBridgeID b, McdModelPairID pair, void* module1, void* module2, MstResponsePair* response);


/**
 * Parameters for a pair of reponseIDs, containing functions
 * to initialise a response when a new pair of objects with a
 * specific responseID pair comes into contact (this might store
 * data into the responseData field of the McdModelPair, to perform
 * the response and to cleanup any response info associated
 * with the pair. All functions are optional. Subject to change.
 */
struct MstResponsePair
{
    MstResponseHelloFn   startResponseFn;
    MstResponseFn        responseFn;
    MstResponseGoodbyeFn endResponseFn;
    void *               responseData;

    McdRequest           request;
    
    /** 
     * Optional callback that is independent of materials and gets called
     * whenever a pair of models intersects when
     * (intersectActiveCB) either model is unfrozen,
     * (intersectInactiveCB) both models are frozen,
     */

    MstIntersectActiveCB     intersectNewCB;
    MstIntersectActiveCB     intersectActiveCB;
    MstIntersectInactiveCB   intersectInactiveCB;
    void*   intersectNewUserData;
    void*   intersectActiveUserData;
    void*   intersectInactiveUserData;

    /**
     * Optional callback that gets called whenever a pair of models 
     * stops intersecting. This callback is executed only once.
     */
    MstDisjointCB           disjointCB;
    void*    disjointUserData;

    /* int priority; */      
};

/** Standard default response ID. Used by Mst to indicate what to do with
    the results of collision detection. 
    More can be created by the user. 
*/
typedef enum {
    kMstResponseBody = McdModelGetDefaultResponseID(), /*!< the default, rigid body, response ID. Is equal to 0. */
    kMstResponseSensor, /*!< the sensor response ID. */
    kMstResponseParticleSystem, /*!< the particle system response ID. */
    kMstResponseUser, /*!< user ids start here */
    kMstUsedResponseCount = kMstResponseUser, /* number actually used initially in Mst */
    kMstResponseCount /* total count of default ids. must be last one */
} MstResponseIDEnum;

/**
 Method used to step dynamics. The default is kMstStepMethodFixed.
*/
typedef enum {
    kMstStepMethodFixed, /*!< use fixed time step */
    kMstStepMethodToFirstCollision /*!< use time of impact calculations */
} MstStepMethodEnum;

/**
 * Parameters for a pair of materials, containing dynamic contact
 * properties, and an optional user contact callback.
 */
struct MstMaterialPair
{
    /**
     * Dynamics contact parameters (friction, restitution etc.) used for a
     * particular pair of materials.
     */
    MdtBclContactParams cp;

    /**
     * Optionals user-defined callbacks executed when models of particular
     * materials come into contact.
     */

    MstPerContactCB     contactCB;
    void*               contactCBUserData;

    MstPerPairCB        pairCB;
    void*               pairCBUserData;

    MstIntersectCB      intersectCB;
    void*               intersectCBUserData;
};

/**
 * Structure defining the maximum number of dynamic/collision bodies,
 * materials etc. in the 'universe'. This allows most memory allocation
 * to be done once at the start.
 */
struct MstUniverseSizes
{
    /** Maximum number of dynamic bodies allowed. */
    unsigned int dynamicBodiesMaxCount;

    /** Maximum number of dynamic constraints (joints & contacts) allowed. */
    unsigned int dynamicConstraintsMaxCount;

    /** Number of collision geometry types allowed. */
    unsigned int collisionGeometryTypesMaxCount;

    /** Maximum number of collision models allowed. */
    unsigned int collisionModelsMaxCount;

    /** Maximum number of simultaneously overlapping models allowed. */
    unsigned int collisionPairsMaxCount;

    /** Maximum number materials allowed. */
    unsigned int materialsMaxCount;
};

/**
 * Information needed to move contact information from the Collision Toolkit
 * to the Dynamics Toolkit. This includes a material table, used for finding
 * dynamic contact properties (friction, restitution etc.) based on the two
 * materials involved.
 */
struct MstBridge
{
    /* Material Table */

    /** Number of MstMaterialPairs in table. */
    unsigned int            maxMaterials;

    /** Number of materials currently used from the table. */
    unsigned int            usedMaterials;

    /**
     * Array of MstMaterialPairs used for storing dynamics contact
     * paramters.
     */
    MstMaterialPair*        materialPairArray;

    unsigned int            usedResponseIDs;
    unsigned int            maxResponseIDs;

    MstResponsePair*        responsePairArray;

    /* Buffers */

    /**
     * McdModelPair container only used during ..Step for getting ModelPairs
     * out of the McdSpace farfield.
     */
    McdModelPairContainer*  pairs;

    /**
     * Temporary store for collision detection contact information. Used
     * during ..Step.
     */
    McdContact*             contacts;

    /** Number of contacts in 'contacts' above. */
    unsigned int            contactsMaxCount;

#if 0
    /** 
     * Optional callback that is independent of materials and gets called
     * whenever a pair of models intersects when
     * (intersectActiveCB) either model is unfrozen,
     * (intersectInactiveCB) both models are frozen,
     */

    MstIntersectActiveCB     intersectNewCB;
    MstIntersectActiveCB     intersectActiveCB;
    MstIntersectInactiveCB   intersectInactiveCB;
    void*   intersectNewUserData;
    void*   intersectActiveUserData;
    void*   intersectInactiveUserData;

    /**
     * Optional callback that gets called whenever a pair of models 
     * stops intersecting. This callback is executed only once.
     */
    MstDisjointCB           disjointCB;
    void*    disjointUserData;
#endif
    int intersectInactiveCBWasSet;

};

/**
 * Convenient Simulation 'container', with an MdtWorld (for dynamics), McdSpace
 * (for collision farfield) and an MstBridge for move contact information
 * between them.
 * Dynamic objects are created in an MstUniverse using MstModelAndBodyCreate
 * and static objects using MstFixedModelCreate.
 */
struct MstUniverse
{
    /** Record of sizes specified when universe was created. */
    MstUniverseSizes    sizes;

    /** Vortex Dynamics Toolkit world container. */
    MdtWorldID          world;

    /** Vortex Collision Toolkit farfield. */
    McdSpaceID          space;

    /** Collision -> Dynamics bridge data. */
    MstBridgeID         bridge;

    MstStepMethodEnum   stepMethod;

    MePool              mcdModelPool;

    /** if profiler is enabled, stores the time spent on collision. */
    MeReal collisionTime;
    /** if profiler is enabled, stores the time spent on dynamics. */
    MeReal dynamicsTime;
};

/* Used by an MstGrouping */
struct MdtBodyNode
{
    MdtBodyID body;
    MdtBodyNode *next;
};

/* Used by an MstGrouping */
struct MdtConstraintNode
{
    MdtConstraintID constraint;
    MdtConstraintNode *next;
};

/* Used by an MstGrouping */
struct McdModelNode
{
    McdModelID model;
    McdModelNode *next;
};

/* Grouping of bodies, constraints and geomtries for high level manipulation. */
struct MstGrouping
{
    MdtBodyNode *bodyHead;
    MdtConstraintNode *constraintHead;
    McdModelNode *modelHead;
};

#endif
