#ifndef MDTKEACONSTRAINTSINTERNAL_H
#define MDTKEACONSTRAINTSINTERNAL_H

/* -*- mode: C; -*- */

/*
  Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
  http://www.cm-labs.com

  $Name: t-pre-multithread-050318 $
   
  Date: $Date: 2005/03/16 22:03:22 $
   
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

#include "MePrecision.h"
#include "MeMemory.h"


/*
 * Macros to manipulate the Jacobian blocks
 */
#define MDTKEACONSTRAINTSMAXCOLS 6
typedef MeReal  MdtKeaGRow[MDTKEACONSTRAINTSMAXCOLS]; 
typedef MdtKeaGRow      *MdtKeaGBlock;


/**
 * @if ADVANCED
 * Kea Constraints structure.
 *
 * To find the total number of @a rows_to_allocate, find the maximum
 * number of rows the constraints can add, but allow up to an extra
 * three rows padding per partition.
 *
 * @endif
 */
typedef struct MdtKeaConstraints
{

    /**
     * Constraint value vector.
     *
     * One MeReal for each rows_to_allocate initialised to 0.
     */
    MeReal *phi;

    /**
     * Forcing velocity term: m{J*v=c}.
     *
     * One MeReal for each rows_to_allocate initialised to 0.
     */
    MeReal *v0;
  
  
    /**
     * Low limit on Lagrange multiplier.
     *
     * One  MeReal for ebach rows_to_allocate initialised to 0.
     */
    MeReal *lower;

    /**
     * High limit on Lagrange multiplier.
     *
     * One MeReal for each rows_to_allocate initialised to 0.
     */
    MeReal *upper;

    /**
     *  Collision indicator.
     */

    int isColliding; 

    /**
     * Calculated Lagrange multiplier READ ONLY.
     *
     * One MeReal for each rows_to_allocate initialised to 0.
     */
    MeReal *lambda;


    /**
     * First order constraint slipping vector.
     *
     * One MeReal for each rows_to_allocate initialised to 0.
     */
    MeReal *xepsilon;

    /**
     * Projection constants.
     *
     * One MeReal for each rows_to_allocate initialised to 0.
     */
    MeReal *xgamma;

    /**
     * Jtype[i] is the constraint type id for constraint i
     */
    int *Jtype; 

    /**
     *  Special flags for the constraint to handle the perturbation parameters.
     */
    int *flags;

    /**
       result is the number of LCP iterations
    */
  
    int lcp_iteration_count;

    /**
       resulting index set returned by lcp solver.
    */
    int  *index_set;

    /**
       Selector for friction model.
    */
    int use_scaled_box_friction;
    unsigned int *friction_model;

    /** 
        For friction type constraint, this is an index which points to the
        corresponding normal constraint.
    */

    unsigned int *reference_normal;
    /**
       for friction constraints, this is the friction coefficient used to
       scale bounds with normal force.
    */
    MeReal *friction_coefficient;
    MeReal *soil_cohesion_coefficient;

    /** new data for new format */
    
    int n_constraint_rows;
    int n_constraints;   /* number of constraints and number of block rows */
    int n_g_blocks;               /* number of Jacobian blocks */
    int n_g_rows;                 /* number of Jacobian rows */
    int *n_g_blocks_row;          /* number of J blocks in each row */
    int *g_block_size;            /* size for each block: same as constraint size */
    int *g_block_first_row;        /* first  row for each block */
    int *constraint_first_block;  /* first block for each constraint */
    int *constraint_n_blocks;     /* number of blocks for given constraint */
    int *g_blocks_heads;        /* heads of linked list, one for each body */
    int *g_blocks_next;         /* index of next J block related to current:one for each block */
    int *g_blocks_tails;           /* tails of linked list, one for each body */
    void **g_bodies;              /* pointers to bodies, arranged as the blocks */
    int *g_bodies_idx;              /* bodies indices, arranged as the blocks */
    MeReal (*g_forces)[8];       /* points to constraint force on each body; uses padded format*/
    MdtKeaGBlock *G;               /* points to real data, indexed by blocks */
    MdtKeaGBlock *GM;               /* points to transformed data */
    MdtKeaGRow   *g_rows;         /* raw  data */
    MdtKeaGRow   *gm_rows;         /* transformed raw  data */
    int max_g_blocks; 
    int max_g_rows;
    int max_constraints;
    int max_bodies;
    MeReal *rhs;                  /* where the right hand side gets built */
  
    int scaled_box_iterations;
    MeReal scaled_box_error;
    
    struct MdtKeaConstraints *next; 
}

MdtKeaConstraints;




/**
   Structure containing the data relevant to one constraint.
*/

typedef struct  MdtKeaInputRowFactors
{
    /* Constraint (position) error. */
    MeReal *phi;
    /* RHS (velocity) value in the constraint equation J*v=c. */
    MeReal *v0;
    /* Low (force) limit on Lagrange multiplier. */
    MeReal *lower;
    /* High (force) limit on Lagrange multiplier. */
    MeReal *upper;
    /* First order constraint slipping vector. */
    MeReal *xepsilon;
    /* Projection constant. */
    MeReal *xgamma;
    /* array of G blocks: one per body */
    MdtKeaGBlock *G;

    /* points to the constraint flags, one set for each row */
    int * flags;
}
MdtKeaInputRowFactors;




#ifdef __cplusplus
extern "C"
{
#endif

    void MEAPI MdtKeaConstraintsPushConstraint(MdtKeaConstraints * c, 
                                               MdtKeaInputRowFactors *f,
                                               MdtKeaBody *b[], int n_bodies,
                                               int n_rows, int tag, int group);
#ifdef __cplusplus
}
#endif

#endif /* MDTKEACONSTRAINTS_H */

