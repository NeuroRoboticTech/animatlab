#ifndef _MDTKEA_H
#define _MDTKEA_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name: t-pre-multithread-050318 $
   
   Date: $Date: 2005/03/16 22:56:11 $
   
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
 * @internal 
 * 
 * MdtKeaMAXBODYCONSTRAINT
 *
 * The maximum number of bodies that can be constrained by each constraint
 */
#define MdtKeaMAXBODYCONSTRAINT 4


/** @file
 *  @internal 
 *
 * Kea solver header file.
 *
 */

#include "MePrecision.h"
#include <stddef.h>
#define MdtKeaBODYVER1           100

/**

@internal
   Picks  the alternatives for integration of angular degrees of freedom.
   Euler stepping is standard explicit stepping followed by normalization.
   Finite stepping uses a finite rotation formula.  Semi implicit is a
   stabilized integration model much like a secant rule.

@endif
 */
typedef enum   { kMdtKeaRotationExplicit               = 1,  
                 kMdtKeaRotationFinite                 = 2,  
                 kMdtKeaRotationSemiImplicit = 3 } MdtKeaRotationModel;

/**
   Picks the alternative for the stepper.  
   kMdtKeaStepper is the old kea model which neglects
   all non-inertial forces and approximates the inertia tensor with a
   homogeneous tensor i.e., one that corresponds to a dynamically symmetric
   body.  The stable stepper includes non-inertial forces and
   stabilization.
 */
typedef enum { kMdtStableStepper, kMdtKeaStepper } MdtStepperModel;

/** This further controls the behavior of the stepper.  Only the
    kMdtStableStepper has any options so far.  This stepper can resolve the
    collisions in a first stage and then step the system.  This can be more
    stable but can be much slower in some cases.  Use with caution.
*/
typedef enum { kMdtStepperDefaultOptions=0, kMdtStepperOneStageCollisions = 2 }  MdtStepperOptions;

/**
    Flags used to select appropriate optimized linear algebra module.  The
    CPU type is auto detected and by default, the appropriate version will
    be chosen.  
    kCPUMeUnknown is the default and it selects the plain C version of the
    linear algebra module.  On Win32 platform, kCPUMeX86 selects an assembler
    version of the linear algebra module which uses MMX instructions, and
    kCPUMeSSE uses the Intel SSE1 extensions.  When available, kCPUMeSSE is the
    fastest but is only available in single precision.   

 */
typedef enum 
{
  kCPUMeUnknown,    /* uses plain C: no assembler */
  kCPUMeX86,        /* uses MMX on win32 PII if available */
  kCPUMeSSE,         /* uses SSE1 on win32 PIII and PIV if available */
  kCPUMeSSE2,
  kCPUMe3DNow, 
  kCPUMe3DNow2
} MeCPUResourceID ;



typedef struct _MeCPUResources* MeCPUResources;

/**
   Selection for the lcp solver routine.   Only one choice provided at this
   time. 
 */

typedef enum  { kMdtLCPSolverBM = 0 } MdtLCPSolver  ; 

/**
 * Selection of the linear algebra solver module to be used by the stepper
 * *and the lcp solver.  The kMdtBlockCholeskySolver is the one which has
 * been published in previous version and provides SSE optimisations on
 * win32 platforms. The kMdtVBCholeskySolver is a new solver which takes
 * better advantage of sparsity, especially in the cases of long chains.
 * The default is kMdtBlockCholeskySolver.
 */
typedef enum  {  kMdtVBCholeskySolver =0,
		kMdtBVBRRTCholeskySolver, kMdtDenseSolver,
		kMdtBVBCholeskySolver
} MdtLinearSolver  ; 

/**
 * Selection of the constraint solver strategy.  
 */
typedef enum  {  kMdtCTSolverSTD =0,
                 kMdtCTSolverITER, 
                 kMdtCTSolverKELLER
} MdtConstraintSolver  ; 



/**
 * MdtKeaBody Flag enum.
 *
 * Any bitwise combination of these options is valid for use in
 * 'MdtKeaBody.flags'.
 */
typedef enum
{
  /**
   * Use the 'fast spin' integrator for this body.
     */
  kMdtKeaBodyFlagUseFastSpin = 0x01,

  /**
     * Indicates that this body has at least one constraint affecting it.
     */
  kMdtKeaBodyFlagIsConstrained = 0x02,

  /**
        Indicates that this body has a spherical inertia
    */
  kMdtKeaBodyFlagIsSphericalInertia = 0x04,

  kMdtKeaBodyIsFrozen = 0x10,
  kMdtKeaBodyIsKinematic = 0x20
} MdtKeaBodyFlag;


/** 
    Kea LCP solver parameter structure
*/

typedef struct 
{
  /** which lcp solver to use: see MdtLCPSolver enumeration*/
  MdtLCPSolver solver_function; 

  /** Maximum allowed number of LCP iterations. */
  int max_iterations; 

  /** Maximum allowed number of block LCP iterations. */
  int max_block_iterations;
  MeReal tolerance ; 
  MeReal filter_scale; 
  int cycle_detection;		/** flag to turn cycle detection on or off */
  /** internal flag */
  int warm_start; 
  
  int max_scaled_box_iterations;
  MeReal scaled_box_tolerance;
 
  int dump_problem;
  char * description;
  int    it_ct_solver_max_it; 
  MeReal it_ct_solver_tol; 

} MdtKeaLCPSolverParameters; 

/**
 * Kea Parameters structure.
 */
typedef struct
{
    /** Amount of time to evolve system by. */
    MeReal stepsize;

    /** Integration and stability methods */
    MdtStepperModel stepper_model;
    
    /** extra options to pass to the steppers.  @see MdtStepperOptions */
    MdtStepperOptions stepper_options; 
    
    MdtKeaLCPSolverParameters lcp_parameters; 
    
    /**
     * Pointer to some memory that Kea can use.
     */
    void *memory_pool;
    void *memory_pool_handler;
    
    /** Size of this area in bytes. */
    size_t memory_pool_size;
    size_t memory_pool_needed; 
    size_t memory_pool_max_used;
    /** CPU resources */
    MeCPUResources cpu_resources;

    /** Matrix Solver */
    MdtLinearSolver linear_solver;

    /** Overall constraint solver */
    MdtConstraintSolver ct_solver;
    
    void* kea_frame;


  /** For Critical Mass Labs' internal use only */
  void *debug;
}
MdtKeaParameters;




#define CAST_KEA_TM  *(MeMatrix4 *)& 


/**
 * @internal 
 * Kea Body structure.
 *
 * After a time step, the contents of 'force' and 'torque' will be the
 * total applied forces plus any forces and torques due to constraints.
 * These MUST BE RE-ZEROED before accumulating values for the next time
 * step.
*/
typedef struct _MdtKeaBody
{
    /** Data stucture tag. Should be MdtKeaBODYVER1. */
    int tag;                    /* 0x00*/
    
    /** Total length of data structure. */
    int len;
    
    /** 1/Mass of the body. */
    MeReal invmass;
    
    /** Linear viscous drag. */
    MeReal              linear_damping;
    /** Angular viscous drag. */
    MeReal              angular_damping;
    
    /**
   * Bitwise combination of MdtKeaBodyFlag s.
   * @see MdtKeaBodyFlag.
   */
    int flags;
    
    /** Applied (to centre of mass) force. */
    MeVector4 force;            
    
    /** Applied torque. */
    MeVector4 torque;          
    
    MeReal    _I[3][4];		// current value of of inertia tensor
    MeReal _invI[3][4];		// current value of inverse of inertia tensor
    MeVector4 _i0[6];		// initial values of both inertia tensor and inverse
    
    
    /** Linear velocity of body. */
    MeVector4 vel;           
    /** Angular velocity of body. */
    MeVector4 velrot;       
    /** Body orientation quaternion. */
    MeVector4 qrot;        
    
    /** Linear Acceleration of body (READ ONLY). */
    MeVector4 accel;      
    /** Angular acceleration of body (READ ONLY). */
    MeVector4 accelrot;  
    
    /** Position correction from post stabilization */
    MeVector4 xpos;      
    /** Rotational correction from post stabilization */
    MeVector4 xrot;  
    
    /**
     * Axis of assumed fast rotation (unit length).
     * Only used if MdtKeaBodyFlagUseFastSpin is set in 'MdtKeaBody.flags'.
     */
    MeVector4 fastSpinAxis;     
    
    /** Linear Momentum */   
    MeVector4 p;
    
    /** Angular Momentum */   
    MeVector4 L;
    
    
    MeMatrix4 tm;
    
    
    struct _MdtKeaBody * next ;
    
    int idx ;			/* temporary index in a given partition */
    int group;                  /* index of group this body belongs to */
    int user_group;             /* group set by the user: this is never modified */
    
} MdtKeaBody;


/* Forward declaration */
#ifndef MDTKEACONSTRAINTS_DECLARED
#define MDTKEACONSTRAINTS_DECLARED
typedef struct MdtKeaConstraints MdtKeaConstraints;      /* forward declaration */
typedef struct MdtKeaInputRowFactors MdtKeaInputRowFactors;      
#endif


#endif
