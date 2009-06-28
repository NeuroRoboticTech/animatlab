#ifndef _MDTTYPES_H
#define _MDTTYPES_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name: t-pre-multithread-050318 $
   
   Date: $Date: 2005/03/16 22:03:18 $
   
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
 * Declares all the data structures used by the  Mdt library.
 */

#include "MePrecision.h"
#include "MdtBcl.h"
#include "MePool.h"
#include "MeChunk.h"


typedef struct MdtConstraintHeader MdtConstraintHeader;

typedef struct MdtGrouping       MdtGrouping; 
typedef struct MdtWorld          MdtWorld;
typedef struct MdtBody           MdtBody;
typedef struct MdtContact        MdtContact;
typedef struct MdtBSJoint        MdtBSJoint;
typedef struct MdtHinge          MdtHinge;
typedef struct MdtPrismatic      MdtPrismatic;
typedef struct MdtCylindrical    MdtCylindrical;
typedef struct MdtCarWheel2       MdtCarWheel2;
typedef struct MdtDistance       MdtDistance;
typedef struct MdtFixedPath      MdtFixedPath;
typedef struct MdtRPROJoint      MdtRPROJoint;
typedef struct MdtUniversal      MdtUniversal;
typedef struct MdtLinear1        MdtLinear1;
typedef struct MdtLinear2        MdtLinear2;
typedef struct MdtLinear3        MdtLinear3;
typedef struct MdtAngular3       MdtAngular3;
typedef struct MdtSpring         MdtSpring;
typedef struct MdtConeLimit      MdtConeLimit;
typedef struct MdtGearRatio      MdtGearRatio;
typedef struct MdtScrewJoint     MdtScrewJoint;
typedef struct MdtWinchJoint     MdtWinchJoint;
typedef struct MdtDoubleHinge    MdtDoubleHinge;
typedef struct MdtDifferentialJoint    MdtDifferentialJoint;
typedef struct MdtCoupledDistance      MdtCoupledDistance;
typedef struct MdtUserConstraint MdtUserConstraint;
typedef struct MdtBaseConstraint MdtBaseConstraint;

typedef struct MdtCListNode         MdtCListNode;
typedef struct MdtStructPool        MdtStructPool;
typedef struct MdtPartitionOutput   MdtPartitionOutput;
typedef struct MdtPartitionParameters   MdtPartitionParameters;
typedef struct MdtWorldParams       MdtWorldParams;
typedef struct MdtAutoDisableParams MdtAutoDisableParams;


/** MdtWorld identifier */
typedef MdtWorld                 *MdtWorldID;
typedef MdtGrouping                 *MdtGroupingID;
typedef MdtBody                  *MdtBodyID;


typedef MdtBaseConstraint               *MdtContactID;
typedef MdtBaseConstraint               *MdtBSJointID;
typedef MdtBaseConstraint                 *MdtHingeID;
typedef MdtBaseConstraint             *MdtPrismaticID;
typedef MdtBaseConstraint             *MdtCylindricalID;
typedef MdtBaseConstraint             *MdtDistanceID;
typedef MdtBaseConstraint              *MdtCarWheel2ID;
typedef MdtBaseConstraint             *MdtFixedPathID;
typedef MdtBaseConstraint             *MdtRPROJointID;
typedef MdtBaseConstraint             *MdtUniversalID;
typedef MdtBaseConstraint               *MdtLinear1ID;
typedef MdtBaseConstraint               *MdtLinear2ID;
typedef MdtBaseConstraint               *MdtLinear3ID;
typedef MdtBaseConstraint              *MdtAngular3ID;
typedef MdtBaseConstraint                *MdtSpringID;
typedef MdtBaseConstraint             *MdtConeLimitID;
typedef MdtBaseConstraint             *MdtGearRatioID;
typedef MdtBaseConstraint             *MdtScrewJointID;
typedef MdtBaseConstraint             *MdtWinchJointID;
typedef MdtBaseConstraint               *MdtDoubleHingeID;
typedef MdtBaseConstraint               *MdtDifferentialJointID;
typedef MdtBaseConstraint               *MdtCoupledDistanceID;
typedef MdtBaseConstraint        *MdtUserConstraintID;

typedef enum
{
    kMdtBodyIsDynamic   = 0x00,
    kMdtBodyIsFrozen    = 0x01,
    kMdtBodyIsKinematic = 0x02
} MdtBodyStateFlag;

typedef enum
{
    kMdtPartitionAutoGrouping = 0,
    kMdtPartitionUserGrouping 
} MdtPartitionGroupingMode;

/**
 * Genereal MdtConstraint identifier.
 * This type is used for the entire API, for all the joints.  Note that
 * this differs from previous releases of the Vortex toolkits where it was
 * necessary to dynamically cast the various constraint types to the basic
 * MdtConstraintID type.  This is no longer necessary.
 */
typedef MdtBaseConstraint        *MdtConstraintID;

/** MdtBclLimit joint limit identifier */
typedef MdtBclLimit              *MdtLimitID;
/** MdtBclSingleLimit joint stop identifier */
typedef MdtBclSingleLimit        *MdtSingleLimitID;
/** MdtBclContactParams identifier */
typedef MdtBclContactParams        *MdtContactParamsID;

typedef union MdtConstraintUnion MdtConstraintUnion;

/* data types for rigid body forces */
typedef struct _MdtBodyForce  MdtBodyForce; 
typedef struct _MdtBodyForce *MdtBodyForceID; 




#ifdef __cplusplus
extern "C"
{
#endif
    typedef void (MEAPI * MdtBodyForceFn)(MdtBodyForceID f);


/* Callbacks */
    typedef void (MEAPI *MdtPartitionEndCB)(MdtPartitionOutput* po, void* pcbdata);
    typedef void (MEAPI *MdtAutoEnableCB)(MdtBodyID b, void* ecbdata);
    typedef void (MEAPI *MdtConstraintIteratorCB)(const MdtConstraintID c, void* ccbdata);
    typedef void (MEAPI *MdtBodyCallbackCB)(const MdtBodyID b);
/**
 * model call back allowing to tune the frictional direction of the contact 
 * on that body. 
 * @see MdtBodyFrictionDirectionPerpendicularToAxisCB and MdtBodyFrictionDirectionParallelToAxisCB
 */
    typedef void (MEAPI *MdtFrictionDirectionCB)(const MdtBodyID body, MdtContactID contacts);


/* 'Virtual Function' prototypes */
    typedef void (MEAPI *MdtConstraintSetBodyFn)(MdtConstraintID c,
                                                 const MdtBodyID b1, const MdtBodyID b2);
    typedef void (MEAPI *MdtConstraintSetAxisFn)(MdtConstraintID c,
                                                 const MeReal px, const MeReal py, const MeReal pz);

#ifdef __cplusplus
}
#endif

/* Data Structures */

/**
 * Output from the partitioner.
 * Contains 'partitions' of bodies and constraints that can be simulated
 * independently.
 */
struct MdtPartitionOutput
{
    /** Constraint pointers to be processed, grouped into partitions. */
    MdtBaseConstraint   **constraints;
    /** The index of the first constraint in each partition. */
    int                 *constraintsStart;
    /** The number of constraints in each partition. */
    int                 *constraintsSize;

    /** Body pointers to be processed, grouped into partitions. */
    MdtBody             **bodies;
    /** The index of the first body in each partition. */
    int                 *bodiesStart;
    /** The number of bodies in each partition. */
    int                 *bodiesSize;

    /** Number of partitions. */
    int                 nPartitions;
    /** Total number of bodies. */
    int                 totalBodies;
    /** Total number of constraints. */
    int                 totalConstraints;

    /** Maximum number of bodies. */
    int                 maxBodies;
    /** Maximum number of constraints. */
    int                 maxConstraints;

};

struct MdtPartitionParameters
{
    int auto_grouping;          /* use this for grouping bodies automatically: this is the default */
}; 

/** Global parameters for a world (gravity, compliance, damping, kinetic * loss.) */
struct MdtWorldParams
{
    MeReal              timeStep;  /** current value of the time step to be
                                       used to be used with @see MdtWorldStepOne()*/

    /** Constant acceleration vector due to gravity. */
    MeVector4           gravity;

    MdtKeaParameters       k_params;
    MdtBclConstraintParameters bcl_params;
};

/**
 * Threshold values for consider a body as 'stopped'.
 */
struct MdtAutoDisableParams
{
    /** Sum-squared velocity threshold. */
    MeReal              vel_thresh;
    /** Sum-squared rotational velocity threshold. */
    MeReal              velrot_thresh;
    /** Sum-squared acceleration threshold. */
    MeReal              acc_thresh;
    /** Sum-squared rotation acceleration threshold. */
    MeReal              accrot_thresh;

    /**
     * Minimum number of steps to keep an object alive for.
     *
     * This is needed to give an object enough time to gain
     * a minimum velocity after it has been awakened.
     */
    int                 alive_window;
};

/**
 * Mdt Constraint Header.
 * Contains data common to all constraints.
 */
struct MdtConstraintHeader
{
    /** The world the constraint is in. */
    MdtWorldID              world;

    /** Pointer to next constraint. */
    MdtBaseConstraint       *enabledNext;
    /** Address of the 'next' pointer in the previous constraint. */
    MdtBaseConstraint       **enabledTome;


    /** The constrained bodies. */
    MdtBody                 *body[MdtKeaMAXBODYCONSTRAINT];

    /**
     * Pointers to this constraints entries in its bodies
     * lists. This is kept so these references can be removed
     * when the constraint is Disabled.
     */
    MdtCListNode*           cNode[MdtKeaMAXBODYCONSTRAINT];

    /* Output Applied Forces and Torques on each body - READ ONLY! */

    /**
     * Force applied to each of the constraints bodies by this
     * constraint.
     */
    MeVector4               resultForce[MdtKeaMAXBODYCONSTRAINT];

    /**
     * Torque applied to each of the constraints bodies by this constraint
     */
    MeVector4               resultTorque[MdtKeaMAXBODYCONSTRAINT];
 



    /** Used by the partitioner to mark constraint as ADDED.*/
    MeBool                  flag;


    /** Max number of rows this constraint can add to MdtKeaConstraints. */
    int                     maxRows;
 
    //output values from solver.
    MeReal                  lambda[7]; 
    MeReal                  phi[6]; 


    /* 'V-table' */
    MdtBclAddConstraintFn   bclFunction; /* computes dynamic data */
    MdtConstraintSetBodyFn  setBodyFunc;
    MdtConstraintSetAxisFn  setAxisFunc;
    int iNumLimits;

    /** User data. This will not be changed from with the toolkit*/
    void                    *userData;
};

/**
 * World Struct contains simulation-wide parameters.
 *
 * vel_thresh, velrot_thresh, acc_thresh, accrot_thresh and alive_window
 * are used for determining when bodies are at rest. A body at rest is
 * not processed by the solver till reenabled by a force or collision
 * event. A body is disabled if it falls below all of the 4 threshhold
 * values.

 * force_thresh and torque_thresh are threshholds for waking up resting
 * bodies.  If the force on a body is more than the force that was
 * applied to it when it was disabled plus the force threshold, then the
 * body is awakened.
 */

struct MdtWorld
{
    /** Mdt body list. */
    MdtBody             *bodyHead;

    /** Mdt Constraint list for all constraint types. */
    MdtBaseConstraint   *enabledConstraintHead;

    /** List of enabled bodies. */
    MdtBody             *enabledBodyHead;


    /** List of body forces */
    MdtBodyForce        *forceHead;
    MdtBodyForce        *enabledForceHead;

    /** The type and number of cpu's (PC only) */
    void                *cpu_resources;

    /** 
	Memory pool used by Kea when solving systems.  This pool is managed
	using a simple stack in the MdtKea module.  It is  used to store
	temporary system matrices and vectors used during the computation
	of constraint forces and integration of the system dyanmics.
	The user can provide her own pool and register a callback to handle
	overflow condition.
    */
    MeChunk             keaPool;

    /** Max number of bodies allowed in this world. */
    int                 maxBodies;
    /** Number of bodies in existence (enabled and disabled). */
    int                 nBodies;
    /** Total count of enabled bodies in the world*/
    int                 nEnabledBodies;

    /** Memory pool to get bodies from. */
    MePool              bodyPool;

    /** Maximum number of constraints allowed in this world. */
    int                 maxConstraints;
    /** Total count of enabled constraints in the world. */
    int                 nEnabledConstraints;
    /** Total count of constraints in the world. */
    int                 nConstraints;

    /** Memory pool to get constraints from. */
    MePool              constraintPool;
    /** Memory pool to get constraint list nodes. */
    MePool              cNodePool;

    /** Memory pool to get body forces from. */
    MePool              bodyForcePool;

    /**
     * Structure filled in with low-level constraint information each
     * time step by MdtBcl functions and passed as input to Kea.
     * This struct is opaque for MdtWorld.  No internal details are
     * needed.  For the interface, see MdtKeaConstraints.h
     */
    MdtKeaConstraints   * constraints;

    /**
     * Partitions of Bodies and Constraints to simulate together.
     * This is the outputted by the partitioner and used by the
     * keaConstraint maker (MdtPackPartition and MdtPackWorld).
     */
    MdtPartitionOutput  partitionOutput;

    /** World simulation parameters (gravity etc.) */
    MdtWorldParams      params;

    /** Flag to enable/disable AutoDisable option. */
    int               autoDisable;

    /**
     * Parameters for auto-disabling - disable body if it
     * falls below all of these.
     */
    MdtAutoDisableParams autoDisableParams;

    /** 
     * Parameters controling how the partitioning is done.
     */

    MdtPartitionParameters partition_parameters;

    /**
     * Callback called whenever a Body is Enabled.
     * This is used by the MstBridge to unfreeze collision etc.
     */
    MdtBodyCallbackCB   bodyEnableCallback;

    /**
     * Callback called whenever a Body is Enabled.
     * This is used by the MstBridge to unfreeze collision etc.
     */
    MdtBodyCallbackCB   bodyDisableCallback;

    /**
     * version string for the Vortex Toolkit.
     */

    const char *toolkitVersionString;

  
    /**
     *  This flag activates some collision checking during the step.  This is
     *  set by MstUniverseStep if the collision checking is enabled.
     */
    int safe_step; 

    /** 
     *  This flags a resize event: this will happen when MdtBodyCreate or
     *  MdtConstraintCreate is called.
     */
    int resize;
    
    MePool  groupings_pool;
    MdtGroupingID *groupings; 
    int n_groupings; 
    int max_groupings; 

};


struct MdtGrouping
{
    MdtWorldID                       world;
    int                                 id; 
    int                          partition; 
    MdtKeaParameters              k_params;
    MdtBclConstraintParameters  bcl_params;
};

/**
 * MdtBody represents a physical body in a world simulation.
 */
struct MdtBody
{

    /** Transform of the body (given by MdtBodyGetTransform etc.) */
    MeMatrix4            bodyTM; /* 0x040 */

    /** Position of body in center-of-mass reference frame. */
    MeMatrix4            comToBodyTM; /* 0x080 */

    /**
     * Linear Impulse accumulator.
     * Converted to force and added to force accumulator at step time
     */
    MeVector4           impulseLinear; /* 0x0C0 */

    /**
     * Angular Impulse accumulator.
     * Converted to torque and added to torque accumulator at step time
     */
    MeVector4           impulseAngular; /* 0x0D0 */

    /** Kea body data. */
    MdtKeaBody           keaBody;

    /** Update bodyTM using comToBodyTM. */
    MeBool               useBodyOffset;

    /**
     * Flag to indicate an impulse has been added to this body this
     * time-step.
     */
    MeBool              impulseAdded;

    /** The world the body is in. */
    MdtWorldID          world;

    /** Next MdtBody. */
    MdtBody             *next;
    /** Address of the next pointer in the previous MdtBody. */
    MdtBody             **tome;

    /** Next MdtBody. */
    MdtBody             *enabledNext;
    /** Address of the next pointer in the previous MdtBody. */
    MdtBody             **enabledTome;

    /** User data. This will not be changed from within the toolkit */
    void                *userData;

    /**
     * Integer indicating the partition to which this body belongs.
     * This is updated by (and is invalid after) re-partitioning the world.
     */
    int                 partitionIndex;

    /** Index to corresponding MdtKeaBody in whole world array passed into Kea. */
    int                 arrayIdWorld;

    /** Index to corresponding MdtKeaBody in just its partitions array passed into Kea. */
    int                 arrayIdPartition;

    /** Flag used in partitioner to indicate body has not yet been added. */
    MeBool              flag;


    /** Mass in kilogrammes. */
    MeReal              mass;


    /** A body's constraints may include joints or contacts. */
    MdtCListNode        *constraintList;

    /** callback used for setting the friction direction */ 
    MdtFrictionDirectionCB frictionDirectionCB;
    void* frictionDirectionCBData;

    /**
     * Number of steps that a body has been enabled; -1 if object is
     * disabled.
     */
    int                 enabledTime;

    /**
     *  Finer control over enabling/disabling.   A frozen body will not be
     *  put into the partition no matter what.
     */
    unsigned int        stateFlags ; 

    /** associated collision model */
    void                *model;

    /** safe time to evolve body. */
    MeReal              safeTime;

    MeVector3 lastForce;
    MeVector3 lastTorque;

};

/**
 * A 'null' empty constraint, containing only data common to all
 * constraints.
 */
struct MdtBaseConstraint
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL constraint header. */
    MdtBclConstraintHeader bclH;
};

/** Contact constraint. */
struct MdtContact
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL contact structure. */
    MdtBclContact       bclContact;

    /**
     * Used by collision to create a list of contacts associated with a
     * pair of bodies.
     */
    MdtContactID        nextContact;
};

/** Ball and Socket joint constraint */
struct MdtBSJoint
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL ball and socket structure. */
    MdtBclBSJoint       bclBSJoint;
};

/** Hinge joint constraint. */
struct MdtHinge
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL hinge structure. */
    MdtBclHinge         bclHinge;
};

/** Prismatic joint constraint. */
struct MdtPrismatic
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL prismatic structure. */
    MdtBclPrismatic     bclPrismatic;
};


/** Prismatic joint constraint. */
struct MdtCylindrical
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL prismatic structure. */
    MdtBclCylindrical     bclCylindrical;
};

/** Distance constraint. */
struct MdtDistance
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL prismatic structure. */
    MdtBclDistance     bclDistance;
};



/** Wheel constraint. */
struct MdtCarWheel2
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL car wheel joint struct. */
    MdtBclCarWheel2      bclCarWheel2;
};


/** Fixed Path Joint. */
struct MdtFixedPath
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL fixed path joint structure. */
    MdtBclFixedPath     bclFixedPath;
};


/** Relative Position Relative Orientation Joint. */
struct MdtRPROJoint
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL fixed path fixed orientation structure. */
    MdtBclRPROJoint     bclRPROJoint;
};



/** Universal Joint. */
struct MdtUniversal
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL universal joint structure. */
    MdtBclUniversal     bclUniversal;
};

/** Linear1 Joint. */
struct MdtLinear1
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL linear 1 structure. */
    MdtBclLinear1       bclLinear1;
};

/** Linear2 Joint. */
struct MdtLinear2
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL linear 2 structure. */
    MdtBclLinear2       bclLinear2;
};


/** Linear3 Joint. */
struct MdtLinear3
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL linear 3 structure. */
    MdtBclLinear3       bclLinear3;
};


/** Angular3 Joint. */
struct MdtAngular3
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL angular 3 structure. */
    MdtBclAngular3      bclAngular3;
};


/** Spring Joint. */
struct MdtSpring
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL spring structure. */
    MdtBclSpring      bclSpring;
};

/** Cone Limit Constraint. */
struct MdtConeLimit
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL cone limit struct. */
    MdtBclConeLimit     bclConeLimit;
};


/** Gear Ratio Constraint. */
struct MdtGearRatio
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL gear ratio struct. */
    MdtBclGearRatio     bclGearRatio;
};

/** Screw Joint Constraint. */
struct MdtScrewJoint
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL gear ratio struct. */
    MdtBclScrewJoint  bclScrewJoint;
};

/** Winch Joint Constraint. */
struct MdtWinchJoint
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL gear ratio struct. */
    MdtBclWinchJoint  bclWinchJoint;
};


/** DoubleHinge Joint. */
struct MdtDoubleHinge
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL double hinge  structure. */
    MdtBclDoubleHinge       bclDoubleHinge;
};


/** Differential Joint. */
struct MdtDifferentialJoint
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL differential structure. */
    MdtBclDifferential       bclDifferential;
};


/** Coupled Distance Joint. */
struct MdtCoupledDistance
{
    /** common to all constraints */
    MdtConstraintHeader head;

    /** BCL coupled distance  structure. */
    MdtBclCoupledDistance       bclCoupledDistance;
};

/** User Defined Constraint Type. */
struct MdtUserConstraint
{
    /** common to all Mdt constraints */
    MdtConstraintHeader     head;

    /** common to all Bcl constraints */
    MdtBclUserConstraint    bclUser;


};

/**
 * An element used for keep each bodies linked list of constraints.
 */
struct MdtCListNode
{
    /** Pointer to specific constraint */
    MdtBaseConstraint   *constraint;

    /** Next CListNode  in linked list */
    MdtCListNode        *next;

    /** Pointer to the next pointer in the previous node */
    MdtCListNode        **tome;
};



/** \struct  MdtBodyForce  MdtTypes.h MdtTypes.h
 *  This is a body force header: it contains all the overhead data
 *  necessary to hook a force computation into the system but no definition
 *  of force itself.   The array of MdtBody is allocated with the header
 *  and defaults to size 3 which allows for 2 body forces.  This memory is
 *  managed with the rest of the header.  Users may allocate a bigger array
 *  and overwrite the pointer b as they need.   They are then responsible
 *  to free this memory themselves when the force is deallocated.
 */

#ifdef __cplusplus
extern "C"
{
#endif
    struct _MdtBodyForce
    {
        int id;			/** this is used to keep type information about the force.   */
        MdtWorldID w;           /** back reference to world for quick lookup */
        MdtBodyID *b;           /** null terminated array of rigid bodies: -1 for all bodies in world */
        /* poniters for hooking into lists much like the bodies */
        struct _MdtBodyForce * next;  /** next in linked list of all MdtBodyForce in the world*/
        struct _MdtBodyForce ** tome; /** pointer for whom we are next in complete list*/
        struct _MdtBodyForce * enabledNext; /** next in the list  of active MdtBodyForce */
        struct _MdtBodyForce ** enabledTome; /** pointer form whom we are next in active list*/
        void *info ;          /** hook for force computational data: this is managed by the user.*/

        MdtBodyForceFn update ;	/** this callback is invoked during MdtWorldStep */
        MdtBodyForceFn destroy ;	/** this callback is invoked before destruction of this force */
        MdtBodyForceFn disable ;      /** this callback is invoked before disabling of this force */
        MdtBodyForceFn enable ;       /** this callback is invoked before enabling of this force */

        MdtBodyID defaultBodyList[3];   /** space for body list of 2 */

    };

#ifdef __cplusplus
}
#endif


/**
 * Union of all constraint types.
 * Used for determining maximum size of a constraint struct when
 * allocating pools on world creation.
 */
union MdtConstraintUnion
{
    MdtBaseConstraint   base;
    MdtContact          contact;
    MdtBSJoint          bsjoint;
    MdtHinge            hinge;
    MdtPrismatic        prism;
    MdtCylindrical      cylinder;
    MdtCarWheel2        carwheel2;
    MdtDistance         distance;
    MdtFixedPath        fpjoint;
    MdtRPROJoint        rprojoint;
    MdtUniversal        univjoint;
    MdtLinear1          linear1;
    MdtLinear2          linear2;
    MdtLinear3          linear3;
    MdtAngular3         angular3;
    MdtSpring           spring;
    MdtConeLimit        conelimit;
    MdtGearRatio        gearRatio;
    MdtDoubleHinge      doubleHinge;
    MdtDifferentialJoint differentialJoint;
    MdtBclCoupledDistance coupledDistance;
    MdtUserConstraint   user;
};




#endif
