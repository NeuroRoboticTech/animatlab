#ifndef _MDTBCL_H
#define _MDTBCL_H
/* -*- mode: C; -*- */

/*
  Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
  http://www.cm-labs.com

  $Name: t-pre-multithread-050318 $
   
  Date: $Date: 2005/03/16 22:03:21 $
   
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
 * Mdt constraints header File.
 *
 * All angles are specified in radians.
 */

#include "MePrecision.h"
#include "MdtKea.h"
#include "MdtBclLimit.h"
#include "MdtBclContactParams.h"

/*
  Mdt Basic Constraint Library Data Structure Tags
*/

/*
  The first Constraint type MUST be defined as 0, and the others must follow
  in strict ascending sequence, with no gaps, otherwise there will be an
  access violation in MdtWorldStep
*/

/** The enumeration of joint types supported by the MdtBcl module. */
typedef enum
{
    /** @var A Ball and Socket Joint*/
    MdtBclBSJOINT = 0,
    MdtBclHINGE,
    MdtBclPRISMATIC,
    MdtBclCARWHEEL2,
    MdtBclCONTACT,
    MdtBclFIXEDPATH,
    MdtBclRPROJOINT,
    MdtBclUNIVERSAL,
    MdtBclLINEAR1,
    MdtBclLINEAR2,
    MdtBclLINEAR3,
    MdtBclANGULAR3,
    MdtBclSPRING,
    MdtBclCONELIMIT,
    MdtBclGEARRATIO,
    MdtBclSCREWJOINT,
    MdtBclWINCHJOINT,
    MdtBclCYLINDRICAL,
    MdtBclDISTANCE,
    MdtBclDOUBLEHINGE,
    MdtBclDIFFERENTIAL,
    MdtBclCOUPLEDDISTANCE,
    MdtBclUSER,
    MdtBclJointTypeCount /* must be last */
}
MdtBclJointType;


typedef enum { 
    kMdtBclLinear = 0x02, 
    kMdtBclAngular = 0x04, 
    kMdtBclPosition = 0x08, 
    kMdtBclVelocity = 0x10,
    kMdtBclControlled = 0x20
}  MdtBclConstraintRowFlags; 




/** Flag used to indicate the absence of an attached body or fact that one
    of the two bodies is unyielding */

enum { MdtBclNO_BODY=-1, MdtBclFROZEN_BODY = -2 } ; 

/**
 * Constraint perturbation parameters.
 */
typedef struct
{
    /**
     * Amount of time to evolve system by.
     */
    MeReal stepsize;


    /** Global default compliance of linear constraints: units are
     * (second*second)/mass i.e., the inverse of a spring constant.  
     * When compliance is zero, constraints are not compliant i.e., they
     * should keep to their target value within small errors due to the
     * discretization.   To prevent against singular matrices, this should
     * be non-zero in which case the constraint will oscillate around the
     * target value.  The oscillation frequency is w =
     * 1/sqrt(M*compliance) where M is the effective mass attached to the
     * constraint.  
     */
    MeReal              linear_compliance;
    
    /** Global default for compliance of angular constraints.  Here, the
        units are (second*second*lenght*length)/mass */
    MeReal              angular_compliance;

    /** Global default linear damping constant for constraint relaxation.  This has
     * units of mass/second i.e., same as the damping constant of a spring 
     * With zero damping, constraints relax very quickly back to their
     * target value but they might overshoot.  Non-zero damping helps
     * dissipate energy during constraint relaxation.  Note that the rate of damping
     * depends on the effective masses on a given constraint. 
     */
    MeReal              linear_damping;
    MeReal              angular_damping;

    /** Global default loss constant for velocity constraint relaxation.
        This has units of time/mass i.e., the inverse of the damping constant
        of a spring.  With 0 loss, the velocity constraint will achieve the
        target velocity almost instantaneously.  
    */
    MeReal              linear_kinetic_loss; 
    MeReal              angular_kinetic_loss; 
    int group;                  /* index of the constraint group */

    /** used internally */
  MeReal h_inv;
  MeReal h_inv_2;
  MeReal diag[4];               
  MeReal rhs[4];
}
MdtBclConstraintParameters;

/**
 * MdtBcl constraint header struct.
 *
 * Contains data common to all MdtBcl constraints.
 */
typedef struct
{
    /** Constraint type: see enumeration  @see MdtBclJointType. */
    int tag;
    int n_bodies;			/* max number of live bodies a constraint
                                           may contain: set by constructor*/
    int max_rows;                 /* max number of rows the constraint may
                                   * generate: set by constructor
                                   */
    /** 
        ref[0] and ref[1]: attachment points and coordinate systems in COM
        cooridnates for body 0 and 1 respectively
    */

    MeMatrix4 ref[MdtKeaMAXBODYCONSTRAINT];
    MdtKeaBody * b[MdtKeaMAXBODYCONSTRAINT] ;     /** constrained bodies: either may be null*/
    MeMatrix4Ptr r[MdtKeaMAXBODYCONSTRAINT];      /** rotation matrices for body 0 and body 1*/
    MeReal *velocity[MdtKeaMAXBODYCONSTRAINT];    /** points to velocity vector of constrained body */
    MeReal *q[MdtKeaMAXBODYCONSTRAINT];           /** points to quaternion vector of constrained body */


    /**
     *  Toggles the use of stiffness and damping on row i of the
     *  constraint.
     */
    MeBool bRelaxConstraint[6];

    /**
     * Overall 'compliance' of this constraint.
     * By default, constrained degrees of freedom are 'hard' ie.
     * Stiffness = INFINITY, but can be allowed some springy 'give'
     * using this parameter.
     */
    MeReal compliance[6];
    MeReal damping[6];
    MeReal loss[6];
    int    flags[6];

}
MdtBclConstraintHeader;

/**
 * MdtBcl ball and socket constraint structure..
 */
typedef struct
{
    /** Data stucture tag should be MdtBclBSJOINT. */
    MdtBclConstraintHeader head;

    MeBool bTwistDamp;
    MeBool bSwingDamp;
    MeReal twistDamp;
    MeReal swingDamp;


}
MdtBclBSJoint;

/**
 * MdtBcl Contact constraint struct.
 *
 * @see MdtBclContactParams
 */
typedef struct
{
    /** Data stucture tag should be MdtBclCONTACT. */
    MdtBclConstraintHeader head;

    /*
      Contact Geometry (different every time)
    */

    /** Position of contact (world reference frame). */
    MeVector3 cpos;
    /** Unit normal at contact (world reference frame). */
    MeVector3 normal;

    /** Penetration depth of bodies at contact. */
    MeReal penetration;

    /**
     * Principal friction direction.
     *
     * World reference frame, unit length and perpendicular to normal.
     */
    MeVector3 direction;

    /*int SenseOfPrimaryFriction;
      int SenseOfSecondaryFriction;*/

    /** Friction and restitution prarmeters. */
    MdtBclContactParams params;
    
}
MdtBclContact;

/**
 * MdtBcl Hinge constraint structure.
 *
 * A hinge constraint limits a body to rotate about one axis relative to
 * its parent.
 */ 
typedef struct
{
    /** Data stucture tag should be MdtBclHINGE. */
    MdtBclConstraintHeader head;

    /** Optional limits set to motion about the hinge axis. */
    MdtBclLimit limit;
}
MdtBclHinge;

/**
 * MdtBcl Prismatic constraint structure.
 *
 * A prismatic constraint fixes the orientation of two bodies, and limits
 * their position along a line.
 */
typedef struct
{
    /** Data stucture tag should be MdtBclPRISMATIC. */
    MdtBclConstraintHeader head;

    /** Optional limits set to motion along the sliding axis. */
    MdtBclLimit limit;
}
MdtBclPrismatic;

/**
 * MdtBclCylindrical constraint structure.
 *
 * A cylindrical constraint fixes the orientation of two bodies, and limits
 * their position along a line, and about an axis
 */
typedef struct
{
    /** Data stucture tag should be MdtBclCYLINDRICAL. */
    MdtBclConstraintHeader head;

    /** Optional limits set to motion along the sliding axis. */
    MdtBclLimit limit[2];
}
MdtBclCylindrical;


/**
 *  Data type for a differential constraint:  this will keep 3 bodies
 *  rotating in sync.
 */
typedef struct 
{
    MdtBclConstraintHeader head; 
    MeReal ratio[3];
    MeReal min_torque;
    MeReal max_torque;
} MdtBclDifferential; 


typedef struct 
{
    MdtBclConstraintHeader head; 
    MeReal min_force; 
    MeReal max_force; 
} MdtBclCoupledDistance ; 


enum
{
    kMdtCylindricalLimitLinear = 0,
    kMdtCylindricalLimitAngular = 1,
};


/**
 * MdtBclDistance constraint structure.
 *
 * A distance constraint locks two objects at a fixed distance.
 */
typedef struct
{
    /** Data stucture tag should be MdtBclDISTANCE. */
    MdtBclConstraintHeader head;

    /** Optional limits set to motion along the sliding axis. */
    MdtBclLimit limit;

    MeReal distance;
    MeBool bTwistDamp;
    MeBool bSwingDamp;
    MeBool bLinearDamp;
    MeReal twistDamp;
    MeReal swingDamp;
    MeReal linearDamp;


}
MdtBclDistance;


/**
 * MdtBcl Car Wheel 2 constraint structure.
 *
 * A hinge that can be steered along a steering axis, is powered along
 * the hinge and steering axes, and has suspension along the steering
 * axis. Body 1 is the chassis and body 2 is the wheel. The connection
 * point for the wheel body is its center of mass.
 */
typedef struct
{
    /** Data stucture tag should be MdtBclCARWHEEL2. */
    MdtBclConstraintHeader head;

    /* limit[0] will be suspension limit */
    /* limit[1] the  hinge limit */
    /* limit[2] the steering angle */
    MdtBclLimit limit[3];
}
MdtBclCarWheel2;

/** MdtLimit index for the MdtCarWheel2 constraint for use with MdtConstraintGetIndexedLimit(). */
enum
{
    kMdtBclCarWheel2LimitSuspension  = 0,
    kMdtBclCarWheel2LimitAxle        = 1,
    kMdtBclCarWheel2LimitSteering    = 2,
};

/**
 * Fixed Path constraint data structure.
 *
 * A fixed path constraint is used to kinematically control the movement
 * of a dynamic body.  This means a dynamic body may be forced to
 * animate (translation only for this constraint type) and the resulting
 * forces are passed on to other constrained bodies.
 */
typedef struct
{
    /** Data stucture tag should be MdtBclFIXEDPATH. */
    MdtBclConstraintHeader head;

    /** Kinematic velocity of 1st body (1st body reference frame). */
    MeVector3 vel1;
    /** Kinematic velocity of 2nd body (2nd body reference frame). */
    MeVector3 vel2;
}
MdtBclFixedPath;

/**
 * Relative Position, Relative Orientation constraint data structure.
 *
 */
typedef struct
{
    /** Data stucture tag should be MdtBclRPROJOINT.
        WARNING: This feature is experimental and may not have an API which
        is compatible with other joints.
    */
    MdtBclConstraintHeader head;

    /** Orientation of joint in body 'i' reference frame */
    MeVector4 q[2];
    /** Becomes true if q[i] is set */
    MeBool use_q[2];

    /** Relative linear velocity at the joint */
    MeVector3 v_rel;

    /** Desired relative orientation between reference frame attached at pos1
        and coordinate system attached at pos2 (this takes q1 and q2 into
        account)
    */
    MeVector4 q_rel;
    /** Relative angular velocity at the joint */
    MeVector3 omega;

    /** Lagrange multiplier limits */
    MeReal angular_fmax[3];
    MeReal linear_fmax[3];

}
MdtBclRPROJoint;

/**
 * Universal Joint constraint data structure.
 */
typedef struct
{
    /** Data stucture tag should be MdtBclUNIVERSAL. */
    MdtBclConstraintHeader head;

    /* two rotational limits */
    MdtBclLimit limit[2];

    /**
     * Dot product of the initial orientations of the two axes in the
     * inertial reference frame: invariant for this joint.
     */
    MeReal axis1_dot_axis2;

}
MdtBclUniversal;

enum
{
    kMdtUniversalLimitAngularPrimary = 0,
    kMdtUniversalLimitAngularSecondary = 1,
};

/**
 * Linear1 constraint data structure.
 *
 * Constrains one body to lie in plane relative to the other, and does
 * not constraint orientation.
 */
typedef struct
{
    /** Data stucture tag should be MdtBclLINEAR1. */
    MdtBclConstraintHeader head;

}
MdtBclLinear1;

/**
 * Linear2 constraint data structure.
 *
 * Constrains a point on a body to lie on a plane which is fixed in the
 * second body but does not constrain orientation.
 */
typedef struct
{
    /** Data stucture tag - should be MdtBclLINEAR2. */
    MdtBclConstraintHeader head;
}
MdtBclLinear2;


/**
 * Linear3 constraint data structure.
 *
 * Constrains a point on a body to lie on a line which is fixed in the
 * second body but does not constrain orientation.
 */
typedef struct
{
    /** Data stucture tag - should be MdtBclLINEAR3. */
    MdtBclConstraintHeader head;
}
MdtBclLinear3;


/**
 * Angular3 constraint data structure.
 *
 * Constrains one body to have a fixed orientation with respect to
 * the other, and does not constrain either boies' position.  This
 * constraint may, at the user's choice, become an Angular 2 joint
 * by freeing rotation about a specified axis.
 */
typedef struct
{
    /** Data stucture tag - should be MdtBclANGULAR3. */
    MdtBclConstraintHeader head;

    /*
      Angular3 data:
    */
    /**
     * Flag which specifies whether rotation is allowed about the
     * vector[0] axis.  When set, this is effectively an
     * Angular2 constraint.
     */
    MeBool bEnableRotation;

}
MdtBclAngular3;

/**
 * MdtBclSpring constraint data structure.
 *
 * This joint attaches one body to another, or to the inertial
 * reference frame, at a given separation.  This separation
 * is governed by two limits which may both be "hard" (which
 * simulates a rod or strut joint) or both soft (simulating a
 * spring) or hard on one limit but soft on the other (e.g. an
 * elastic attachment which may be stretched but not compressed).
 * The default behaviour is spring-like, with two soft, damped
 * limits, both initialised at the initial separation of the
 * bodies.
 */
typedef struct
{
    /** Data stucture tag - should be MdtBclSPRING. */
    MdtBclConstraintHeader head;
    /** Limits set to the displacement between the two bodies. */
    MdtBclLimit limit;
}
MdtBclSpring;

/**
 * Cone Limit constraint data structure.
 */
typedef struct
{
    /** Data stucture tag should be MdtBclCONELIMIT. */
    MdtBclConstraintHeader head;

    /** cone half-angle */
    MeReal cone_half_angle;

    /** cosine of cone half-angle */
    MeReal cos_cone_half_angle;

    /**
     * Overall 'stiffness' of the limit constraint.
     * By default, constrained degrees of freedom are 'hard' ie.
     * Stiffness = INFINITY, but can be allowed some springy 'give'
     * using this parameter.
     */
    MeReal compliance;

    /**
     * Overall spring 'damping' of the limit constraint.
     * By default, Damping = 0. Should be used in conjunction with
     * 'stiffness' above.
     */
    MeReal damping;
}
MdtBclConeLimit;


/**
 * Gear Ratio constraint
 */
typedef struct
{
    /** Data stucture tag should be MdtBclGEARRATIO. */
    MdtBclConstraintHeader head;

    MeReal ratio;
    MeReal max_torque;
    MeReal min_torque;
    MeReal differential;
} MdtBclGearRatio;

/**
 * ScrewJoint constraint
 */
typedef struct
{
    /** Data stucture tag should be MdtBclSCREWJOINT. */
    MdtBclConstraintHeader head;

    MeReal ratio;
    MeReal max_torque;
    MeReal min_torque;
    MeReal differential;
} MdtBclScrewJoint;

/**
 * WinchJoint constraint
 */
typedef struct
{
    /** Data stucture tag should be MdtBclWINCHJOINT. */
    MdtBclConstraintHeader head;

    MeReal ratio;
    MeReal max_torque;
    MeReal min_torque;
    MeReal differential;
    MeReal distance;
    MeReal mindistance;
    MeReal maxdistance;
    MeReal winchdir;
} MdtBclWinchJoint;


/**
 * MdtBcl Hinge constraint structure.
 *
 * A hinge constraint limits a body to rotate about one axis relative to
 * its parent.
 */ 
typedef struct
{
    /** Data stucture tag should be MdtBclDOUBLEHINGE. */
    MdtBclConstraintHeader head;
    /** Limits set to motion of the connection and about each hinge axis. */
    MdtBclLimit limit[3];
}
MdtBclDoubleHinge;

/*
 * Indices to access the DoubleHinge limits by name.
 */
enum {kMdtBclDoubleHingeDistance, kMdtBclDoubleHingeTorsion,
      kMdtBclDoubleHingeWheel};

typedef enum
{
    kConstraintLinear1,
    kConstraintRot1,
    kConstraintRot2,
    kConstraintDistance,
    kConstraintScrew,
    kConstraintMixed,
    kConstraintFree,

}MdtParameterTypeEnum;

typedef enum
{
    kConstrained,
    kLocked,
    kMotorized,
    kSpring,
    kLimitExceeded

}MdtConstraintTypeEnum;

/**
 * MdtBcl User Constraint.
 */
typedef struct
{
    /** Data stucture tag should be MdtBclUSERCONSTRAINT. */
    MdtBclConstraintHeader head;
    MdtBclLimit limit[6];

    int iNumConstraints;
    MeVector3 velBody1;
    MeVector3 angBody1;
    MeVector3 velBody2;
    MeVector3 angBody2;
    /* up to 6 of each of the following, but with caveats! */
    MdtParameterTypeEnum  enumParameterType[6];
    MdtConstraintTypeEnum constraintState[6];
    int iBodyIndex[6];  /*if only one body is used.*/
    int iAxis1Index[6]; /*axis of body 1.*/
    int iAxis2Index[6]; /*axis of body 2.*/


    /** User constraint data */
    void                   *userConstraint;
}
MdtBclUserConstraint;


#ifdef __cplusplus
extern "C"
{
#endif

    extern char *MdtBclJointNames[]; 

    /**
     *
     * Function type for converting data structure 'constraint' into constraint
     * equations and adding them to the MdtKeaConstraints struct.
     */
    typedef int
    (MEAPI *MdtBclAddConstraintFn)(struct MdtKeaConstraints *const keaConstraints,
                                   void *const constraint,
                                   const MdtBclConstraintParameters *const params);

    /*
      Basic Constraint Library Add... Functions

      Functions for adding a constraint to the MdtKeaConstraints
      structure. Converts constraints from the geometric data
      structures above, into matrix rows to be passed to the solver.
    */

    int    MEAPI MdtBclAddBSJoint(struct MdtKeaConstraints *const clist,
                                  void *const constraint,
                                  const MdtBclConstraintParameters *const params);

    int    MEAPI MdtBclAddHinge(struct MdtKeaConstraints *const clist,
                                void *const constraint,
                                const MdtBclConstraintParameters *const params);

    int    MEAPI MdtBclAddPrismatic(struct MdtKeaConstraints *const clist,
                                    void *const constraint,
                                    const MdtBclConstraintParameters *const params);

    int    MEAPI MdtBclAddCylindrical(struct MdtKeaConstraints *const clist,
                                      void *const constraint,
                                      const MdtBclConstraintParameters *const params);

    int    MEAPI MdtBclAddDistance(struct MdtKeaConstraints *const clist,
				   void *const constraint,
				   const MdtBclConstraintParameters *const params);


    int    MEAPI MdtBclAddCarWheel2(struct MdtKeaConstraints *const clist,
                                    void *const constraint,
                                    const MdtBclConstraintParameters *const params);



    int    MEAPI MdtBclAddContact(struct MdtKeaConstraints *const clist,
                                  void *const constraint,
                                  const MdtBclConstraintParameters *const params);

    int    MEAPI MdtBclAddFixedPath(struct MdtKeaConstraints *const clist,
                                    void *const constraint,
                                    const MdtBclConstraintParameters *const params);

    int    MEAPI MdtBclAddFPFOJoint(struct MdtKeaConstraints *const clist,
                                    void *const constraint,
                                    const MdtBclConstraintParameters *const params);


    int    MEAPI MdtBclAddRPROJoint(struct MdtKeaConstraints *const clist,
                                    void *const constraint,
                                    const MdtBclConstraintParameters *const params);

    int    MEAPI MdtBclAddUniversal(struct MdtKeaConstraints *const clist,
                                    void *const constraint,
                                    const MdtBclConstraintParameters *const params);

    int    MEAPI MdtBclAddLinear1(struct MdtKeaConstraints *const clist,
                                  void *const constraint,
                                  const MdtBclConstraintParameters *const params);

    int    MEAPI MdtBclAddLinear2(struct MdtKeaConstraints *const clist,
                                  void *const constraint,
                                  const MdtBclConstraintParameters *const params);

    int    MEAPI MdtBclAddLinear3(struct MdtKeaConstraints *const clist,
                                  void *const constraint,
                                  const MdtBclConstraintParameters *const params);

    int    MEAPI MdtBclAddAngular3(struct MdtKeaConstraints *const clist,
                                   void *const constraint,
                                   const MdtBclConstraintParameters *const params);

    int    MEAPI MdtBclAddSpring(struct MdtKeaConstraints *const clist,
                                 void *const constraint,
                                 const MdtBclConstraintParameters *const params);

    int    MEAPI MdtBclAddConeLimit(struct MdtKeaConstraints *const clist,
                                    void *const constraint,
                                    const MdtBclConstraintParameters *const params);

    int    MEAPI MdtBclAddGearRatio(struct MdtKeaConstraints *const clist,
                                    void *const constraint,
                                    const MdtBclConstraintParameters *const params);

    int    MEAPI MdtBclAddScrewJoint(struct MdtKeaConstraints *const clist,
                                    void *const constraint,
                                    const MdtBclConstraintParameters *const params);
    
    int    MEAPI MdtBclAddWinchJoint(struct MdtKeaConstraints *const clist,
                                    void *const constraint,
                                    const MdtBclConstraintParameters *const params);

    int    MEAPI MdtBclAddDoubleHinge(struct MdtKeaConstraints *const clist,
                                      void *const constraint,
                                      const MdtBclConstraintParameters *const params);


    int    MEAPI MdtBclAddLockJoint(struct MdtKeaConstraints *const clist,
                                    void *const constraint, 
                                    const MdtBclConstraintParameters *const params);

    int    MEAPI MdtBclAddDifferential(struct MdtKeaConstraints *const clist,
                                       void *const constraint, 
                                       const MdtBclConstraintParameters *const params);

    int    MEAPI MdtBclAddCoupledDistance(struct MdtKeaConstraints *const clist,
                                          void *const constraint, 
                                          const MdtBclConstraintParameters *const params);

    int    MEAPI MdtBclEndConstraint(struct MdtKeaConstraints *const clist, 
                                     const void * c, struct MdtKeaInputRowFactors * const f,
                                     const unsigned rows_added, 
                                     const MdtBclConstraintParameters *const params);

    MeReal  MEAPI MdtBclConstraintGetLimitLinearPosition(MdtBclConstraintHeader *const constraint, MdtBclLimit * limit);
    MeReal  MEAPI MdtBclConstraintGetLimitRotationalPosition(MdtBclConstraintHeader *const constraint, MdtBclLimit * limit);
    MeReal MEAPI MdtBclConstraintGetLimitDistancePosition(MdtBclConstraintHeader *const head, MdtBclLimit * limit);
    
    void  MEAPI MdtBclContactGetSlipVelocity(const MdtBclConstraintHeader *head, MeVector3 v);
    void  MEAPI MdtBclUpdateConstraintParameters(MdtBclConstraintParameters *p, 
                                                 int group, 
                                                 MeReal max_mass,
                                                 MeReal max_inertia, 
                                                 MeReal freq_max, 
                                                 MeReal h);
    void  MEAPI MdtBclSetDefaultConstraintParameters(MdtBclConstraintParameters *p);

    /*
      The following are useful macros for finding the maximum number of
      constraint rows that can be added by a type of constraints.

      This can be used to provide an upper bound on the amount of memory to
      allocate fo constraint input. Some rows may be actuation or limits.
    */

    /**
     * @internal 
     * Maximum rows added by a Ball And Socket Joint constraint.
     */
#define MdtBclGETMAXROWSBSJOINT         (6)
    /**
     * @internal 
     * Maximum rows added by a Hinge constraint.
     */
#define MdtBclGETMAXROWSHINGE           (6)
    /**
     * @internal 
     * Maximum rows added by a Prismatic constraint.
     */
#define MdtBclGETMAXROWSPRISMATIC       (6)

    /**
     * Maximum rows added by a Prismatic constraint.
     */
#define MdtBclGETMAXROWSCYLINDRICAL       (6)

    /**
     * Maximum rows added by a Distance constraint.
     */
#define MdtBclGETMAXROWSDISTANCE       (6)

   
    /**
     * @internal 
     * Maximum rows added by a Car Wheel constraint.
     */
#define MdtBclGETMAXROWSCARWHEEL2        (6)

    /**
     * @internal 
     * Maximum rows added by a Contact constraint.
     */
#define MdtBclGETMAXROWSCONTACT         (3)
    /**
     * @internal 
     * Maximum rows added by a Fixed Path constraint.
     */
#define MdtBclGETMAXROWSFIXEDPATH       (3)
    /**
     * @internal 
     * Maximum rows added by a Fixed Path Fixed Orientation constraint.
     */
#define MdtBclGETMAXROWSFPFOJOINT       (6)
    /**
     * @internal 
     * Maximum rows added by a Relative Position Relative Orientation constraint.
     */
#define MdtBclGETMAXROWSRPROJOINT       (6)
    /**
     * @internal 
     * Maximum rows added by a Universal Joint constraint.
     */
#define MdtBclGETMAXROWSUNIVERSAL       (6)
    /**
     * @internal 
     * Maximum rows added by a Linear1 constraint.
     */
#define MdtBclGETMAXROWSLINEAR1         (1)
    /**
     * @internal 
     * Maximum rows added by a Linear2 constraint.
     */
#define MdtBclGETMAXROWSLINEAR2         (2)
    /**
     * @internal 
     * Maximum rows added by a Linear3 constraint.
     */
#define MdtBclGETMAXROWSLINEAR3         (4)
    /**
     * @internal 
     * Maximum rows added by a Angular3 constraint.
     */
#define MdtBclGETMAXROWSANGULAR3        (3)
    /**
     * @internal 
     * Maximum rows added by a Spring constraint.
     */
#define MdtBclGETMAXROWSSPRING           (1)
    /**
     * @internal 
     * Maximum rows added by a ConeLimit constraint.
     */
#define MdtBclGETMAXROWSCONELIMIT       (1)

    /**
     * @internal 
     * Maximum rows added by a gear ratio constraint.
     */
#define MdtBclGETMAXROWSGEARRATIO       (1)

    /**
     * @internal 
     * Maximum rows added by a screw joint constraint.
     */
#define MdtBclGETMAXROWSSCREWJOINT       (1)

    /**
     * @internal 
     * Maximum rows added by a winch joint constraint.
     */
#define MdtBclGETMAXROWSWINCHJOINT       (2)

     /**
     * @internal 
     * Maximum rows added by a double hinge constraint.
     */
#define MdtBclGETMAXROWSDOUBLEHINGE       (6)

    /**
     * @internal 
     * Maximum rows added by a differential constraint
     */
#define MdtBclGETMAXROWSDIFFERENTIAL       (1)

    /**
     * @internal 
     * Maximum rows added by a coupled distance constraint
     */
#define MdtBclGETMAXROWSCOUPLEDDISTANCE       (1)

    /**
     * @internal 
     * Maximum rows added by a lock constraint.
     */
#define MdtBclGETMAXROWSLOCK            (6)



#ifdef __cplusplus
}
#endif

#endif
