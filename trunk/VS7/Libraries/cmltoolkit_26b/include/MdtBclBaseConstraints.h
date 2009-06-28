#ifndef _MDTBASECONSTRAINTS_H
#define _MDTBASECONSTRAINTS_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/09/27 21:30:53 $
   
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



int MEAPI MdtBclSetDot1AngleConstraint( MdtKeaInputRowFactors *const factors, 
                                                 MdtKeaGBlock *G, 
                                                int iRow,
                                                int ref_body, 
                                                MeReal * u, MeReal * v,
                                                MeVector3 p[2], MeReal *
                                                 d01, int set_factors = 1);   




void MEAPI MdtBclSetRowFactorsToLock(MdtKeaInputRowFactors * const factors,
                                     const MdtBclConstraintParameters *const params,
                                     int iRow,
                                     MdtBclLimit * limit);

void MEAPI MdtBclSetRowFactorsToMotor(MdtKeaInputRowFactors * const factors,
                                      const MdtBclConstraintParameters *const params,
                                      int iRow,
                                      MdtBclLimit * limit);

void MEAPI MdtBclSetRowFactorsToLimitBounce(MdtKeaInputRowFactors * const factors, 
                                            const MdtBclConstraintParameters *const params,
                                            const unsigned int RowIndex,
                                            MdtBclLimit * const limit);

void MEAPI MdtBclSetRowFactorsToSpring(MdtKeaInputRowFactors * const factors,
                                       const MdtBclConstraintParameters *const params,
                                       int iRow,
                                       MeReal stiffness,
                                       MeReal damping,
                                       MeReal stretch, 
                                       MeReal minForce,
                                       MeReal maxForce
                                      );

MeBool MEAPI MdtBclGetAxisOvershoot    ( MdtBclLimit * const limit);

MeBool MEAPI MdtBclUpdateLinearLimit(MdtBclConstraintHeader *c,
                                     const  MdtBclConstraintParameters *const params,
                                     MdtBclLimit * limit,  
                                     MeMatrix4 *w, MeVector3 p[2], MeVector3 d01);

void MEAPI MdtBclUpdateRotationalLimit(MdtBclConstraintHeader *c,
                                       const  MdtBclConstraintParameters *const params,                                
                                       MdtBclLimit * limit, 
                                       MeMatrix4 *w);

MeReal MEAPI MdtBclGetBodiesRelAngularVelocityAbout(MdtBclConstraintHeader *c, MeVector3 ang_vel);

MeReal MEAPI MdtBclGetBodiesRelLinearVelocityAlong(MdtBclConstraintHeader *c, 
                                                   MeVector3 along_vector, 
                                                   MeVector3 p[2]);

void MdtBclUpdateRotationalPosition(MdtBclConstraintHeader *c, MdtBclLimit * limit, MeMatrix4 *w);

void MdtBclUpdateRotationalPosition(MdtBclConstraintHeader *c, MdtBclLimit * limit, 
                                    MeReal *x, MeReal *u, MeReal *v);


/**
 * Brand new constraint interface.  
 * Basic constraint types are: 
 *   01) sperical
 *   02) dot1
 *   03) dot2
 */

int MdtBclSetSphericalConstraint(MdtKeaInputRowFactors *const factors, 
                                 MdtKeaGBlock *G, 
                                 int start_row, 
                                 MeMatrix4 *w, 
                                 MeVector3 *p, int set_factors = 1 ); 

int MdtBclSetDot1Constraint( MdtKeaInputRowFactors *const factors, 
                             MdtKeaGBlock *G, 
                             int start_row, 
                             MeVector3 r1, 
                             MeVector3 r2, int set_factors = 1 ); 

int MdtBclSetAxialConstraint( MdtKeaInputRowFactors *const factors, 
                             MdtKeaGBlock *G, 
                             int start_row, 
                             MeVector3 n, 
                             int set_factors = 1 ); 

int MdtBclSetDot2Constraint( MdtKeaInputRowFactors *const factors, 
                             MdtKeaGBlock *G, 
                             int start_row, 
                             MeVector3 d01, 
                             MeVector3 n, 
                             MeVector3 *p, 
                             int ref_body, int set_factors = 1 ); 

int MdtBclSetDistanceConstraint( MdtKeaInputRowFactors *const factors, 
                                 MdtKeaGBlock *G, 
                                 int start_row, 
                                 MeVector3 d01, 
                                 MeVector3 *p, 
                                 MeReal d0, int set_factors = 1 );
int MdtBclSetDriver(MdtKeaInputRowFactors *const factors, 
                    int row,  MeReal lower, MeReal upper, MeReal v0);



int MEAPI MdtBclLimitUpdateControlState( MdtBclLimit *limit );

int MEAPI MdtBclLimitSetDriver(
    MdtKeaInputRowFactors * const factors,  
    const  MdtBclConstraintParameters *const params,
    int row, MdtBclLimit *limit );

#endif
