#ifndef MDTKEACONSTRAINTS_H
#define MDTKEACONSTRAINTS_H

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


/* all we need here is a declaration */
#ifndef MDTKEACONSTRAINTS_DECLARED
#define MDTKEACONSTRAINTS_DECLARED
typedef struct MdtKeaConstraints MdtKeaConstraints;      /* forward declaration */
typedef struct MdtKeaInputRowFactors MdtKeaInputRowFactors;      
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    void MEAPI MdtKeaConstraintsResize(MdtKeaConstraints *c, int
                                       maxConstraints, int maxBodies, MeMemoryAPIStruct *mem);

    void MEAPI MdtKeaConstraintsDestroy(MdtKeaConstraints *c,
                                        MeMemoryAPIStruct *mem);

    void MEAPI MdtKeaConstraintsReset(MdtKeaConstraints* c, 
                                      int nrows_ex_padding, int nrows_inc_padding, 
                                      int nConstraints, int nBodies);

    MdtKeaConstraints * MEAPI MdtKeaConstraintsCreate(MeMemoryAPIStruct *mem ); 

    void MEAPI MdtKeaConstraintsGetForces(MdtKeaConstraints* c, 
                                          int i, MdtKeaBody *b[], int n_bodies, 
                                            MeReal F[2][4], MeReal
                                          T[2][4], MeReal lambda[7], MeReal phi[6]);

    int MEAPI  MdtKeaConstraintsGetNumRows(MdtKeaConstraints *c);

  void    MEAPI MdtKeaConstraintsInitRowList(struct MdtKeaConstraints *const clist);
  void    MEAPI MdtKeaConstraintsEndPartition(struct MdtKeaConstraints  *const clist);
  void    MEAPI MdtKeaConstraintsStartPartition(struct MdtKeaConstraints *const clist);
  MeReal  MEAPI MdtKeaConstraintsGetScaledBoxError(struct MdtKeaConstraints *const clist);
  int  MEAPI MdtKeaConstraintsGetScaledBoxIterations(struct MdtKeaConstraints *const clist);


  /**
   * @internal 
   * Utility function for detecting the CPU Resources
   *
   */
  MeCPUResources MEAPI MdtKeaQueryCPUResources(void);

  /** @internal 
   * Utility function for calculating the size of the workspace needed by
   * Kea.
   *
   * This memory is passed into Kea in the MdtKeaParameters structure.
   */
  int MEAPI MdtKeaMemoryRequired(MdtKeaParameters *k, int max_rows,int max_bodies);

  /** @internal 
   * Stepper functions.  These take the current list of constraints and a
   * pointer to the first body in the partition and step the system forward
   * according to the given parameters.  
   *
   */
  void MEAPI MdtKeaStep(MdtKeaConstraints * constraints, MdtKeaBody *blist, 
			int num_bodies, MdtKeaParameters * parameters);

  void MEAPI MdtKeaAddConstraintForces(MdtKeaConstraints * constraints,
				       MdtKeaBody * blist, int num_bodies,
				       MdtKeaParameters *parameters);


  void MEAPI MdtKeaIntegrateSystem(MdtKeaConstraints * constraints, MdtKeaBody *blist,
				   int num_bodies, MdtKeaParameters *parameters);

  /** @internal Creates the frame class that hold the function pointers required for stepping */
  void MEAPI MdtKeaFrameCreate(MdtKeaParameters* parameters);
  
  /** @internal Delete the frame class that hold the function pointers required for stepping */
  void MEAPI MdtKeaFrameDestroy(MdtKeaParameters* parameters);
  
  /** @internal Returns false if MdtKeaParameters not valid */
  MeBool MEAPI MdtKeaValidateKeaParameters(MdtKeaParameters* parameters);

  /** 
   * Allow addition of external stepper model. The returned int is the index 
   * associated to this stepper.
   */
  int MEAPI MdtKeaFrameAddStepperModel(MdtKeaParameters* parameters, void* stepper);

  /** 
   * Allow addition of external linear solver. The returned int is the index 
   * associated to the new solver.
   */
  int MEAPI MdtKeaFrameAddLinearSolver(MdtKeaParameters* parameters, void* mat, void *funcb);

  const char* MEAPI MdtKeaGetLinearSolverDescription(const MdtKeaParameters* parameters);
  const char* MEAPI MdtKeaGetStepperModelDescription(const MdtKeaParameters* parameters);



    


#ifdef __cplusplus
}
#endif

#endif /* MDTKEACONSTRAINTS_H */

