#ifndef _MDTBCLMATH_H
#define _MDTBCLMATH_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2002 CMLabs Simulations Inc. 
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/02/08 13:53:05 $
   
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
  Functions usefull when creating custom constraint.
*/

#include "MePrecision.h"
#include "MdtKeaConstraintsInternals.h"



/*
  Set a = b x c, where a is a pointer to a row in jstore.

  Note that adjacent row elements are 4 elements apart in jstore.
*/
static inline void MdtBclMathCrossToGRow(const MeReal * b, const MeReal * c, MeReal * g)
{
    g[0] = b[1] * c[2] - b[2] * c[1];
    g[1] = b[2] * c[0] - b[0] * c[2];
    g[2] = b[0] * c[1] - b[1] * c[0];
}

/*
  Set a = b x c, where a is a pointer to a row in jstore.

  Note that adjacent row elements are 4 elements apart in jstore.
*/
static inline void MdtBclMathCrossToJstore(const MeReal * b,
    const MeReal * c, MeReal a[12])
{
    a[0 * 4] = b[1] * c[2] - b[2] * c[1];
    a[1 * 4] = b[2] * c[0] - b[0] * c[2];
    a[2 * 4] = b[0] * c[1] - b[1] * c[0];
}


/*
  Set +{A} to a 3x3 matrix corresponding to the 3x1 vector +{a}, such
  that m{A*b = a x b} (x is the cross product operator).

  The result is multiplied by +{factor}.  There are +{colskip} elements
  between the adjacent columns of +{A}.  If +{A} is stored by rows then
  simply multiply +{factor} by -1.
  
  Should be called MdtBclMathTildeVectorToJstore or
                       MdtBclMathVectorAsCrossTransform... or something.

  Also, this is yet another hard-wired function those 0,4,8 strides
  should be #-def'ed or available via an inline function or something
  decent. - RJW
*/
static inline void MdtBclMathCrossMatrixToJstore(const MeVector3 a,
                                                 MeVector3 A0, 
                                                 MeVector3 A1, 
                                                 MeVector3 A2)
{
    A0[0] = 0;
    A0[4] = a[2];
    A0[8] = -a[1];

    A1[0] = -a[2];
    A1[4] = 0;
    A1[8] = a[0];

    A2[0] = a[1];
    A2[4] = -a[0];
    A2[8] = 0;
}

static inline void MdtBclMathCrossMatrixToGRows(const MeVector3 a,
                                                 MeReal * g0, 
                                                 MeReal * g1, 
                                                 MeReal * g2)
{
    g0[0] = 0;
    g0[1] = a[2];
    g0[2] = -a[1];

    g1[0] = -a[2];
    g1[1] = 0;
    g1[2] = a[0];

    g2[0] = a[1];
    g2[1] = -a[0];
    g2[2] = 0;
}



/** 
  * @internal
  * Convert the relative constraint transformation in the constraint header
  * into the world reference frame. 
  */
static inline void 
MdtBclConvertRefFramesToWorld(const MdtBclConstraintHeader * const head, MeMatrix4 * ref)
{
    for (int i=0; i<head->n_bodies; ++i) 
    {
        if ( head->r[i] ) 
        {
            MeMatrix4TMCompound( ref[i], head->ref[i], head->r[i]) ; 
        }
        else
        {
            MeMatrix4Copy( ref[i], head->ref[i]);
        }
    }
    
}


/** 
  * @internal
  * Convert the relative constraint transformation in the constraint header
  * into the world reference frame. This version also returns the 
  * vectors between the attachment and the center of mass in world space.
  */
static inline void 
MdtBclConvertRefFramesToWorld2(const MdtBclConstraintHeader * const head, 
                               MeMatrix4 * ref, MeVector3 * p)
{
    for (int j = 0; j<head->n_bodies; ++j)
    {
        if ( head->r[j] ) 
        {
            MeMatrix4TMCompound( ref[j], head->ref[j], head->r[j]) ; 
            /* vector between attach point and center of mass */
            MeVector3Subtract(p[j], ref[j][3], head->r[j][3]); 
        }
        else
        {
            MeMatrix4Copy( ref[j], head->ref[j]);
            MeVector3Copy(p[j], ref[j][3]);
        }
    }
}





static inline  void
MyMultiplyT2(int p, int q, int r, const MeReal *const A, const MeReal *const B, MeReal *C)
{
  int i,j,k;
  for (i = 0; i < p; ++i) {
    for (j = 0; j < r; ++j) {
      MeReal e = 0;
      for (k = 0; k < q; ++k) {
	e += A[i*q+k]*B[j*q+k];
      }
      C[i*r+j] = e;
    }
  }
}


/*
  Internal constraint math function.
*/
static inline void GetQMatrix(MeReal Q[4][4], const MeVector4 q)
{
  unsigned int i;
  MeMatrix3CrossFromVector(&Q[1][1], &Q[2][1], &Q[3][1], q, (MeReal)-1);

  Q[0][0] = q[0];
  for (i = 1; i < 4; ++i)
    {
      Q[i][0] = q[i];
      Q[0][i] = -q[i];
      Q[i][i] = q[0];
    }
}

/*
  Internal constraint math function.
*/
static inline void GetPMatrix(MeReal P[4][4], const MeVector4 q)
{
  unsigned int i;
  MeMatrix3CrossFromVector(&P[1][1], &P[2][1], &P[3][1], q, (MeReal) 1);

  P[0][0] = q[0];
  for (i = 1; i < 4; ++i)
    {
      P[i][0] = q[i];
      P[0][i] = -q[i];
      P[i][i] = q[0];
    }
}

/*
  Internal constraint math function.
  Constructs the 3x4 matrix E specified in section 6.10 of "Kinematic
  Constraints for Rigid Body Dynamics" by C. Lacoursière.
*/
static inline void GetEMatrix(MeReal E[3][4], const MeVector4 q)
{
  // first column
  E[0][0] = -q[1];
  E[1][0] = -q[2];
  E[2][0] = -q[3];
  // Upper diagonal
  E[0][1] = E[1][2] = E[2][3] = q[0];
  // Cross matrix
  E[1][3] = -(E[2][2] = q[1]);
  E[2][1] = -(E[0][3] = q[2]);
  E[0][2] = -(E[1][1] = q[3]);
}



/*
  Internal constraint math function.

  Constructs the 3x4 matrix G specified in section 6.10 of "Kinematic
  Constraints for Rigid Body Dynamics" by C. Lacoursière.
*/
static inline void GetGMatrix(MeReal G[3][4], const MeVector4 q)
{
  // first column
  G[0][0] = -q[1];
  G[1][0] = -q[2];
  G[2][0] = -q[3];
  // Upper diagonal
  G[0][1] = G[1][2] = G[2][3] = q[0];
  // Cross matrix
  G[1][3] = -(G[2][2] = -q[1]);
  G[2][1] = -(G[0][3] = -q[2]);
  G[0][2] = -(G[1][1] = -q[3]);
}

/**
 * Macro for adding a constraints bodies to the 'keaConstraints' 'struct'.
 */

/**
 * Macro for finding the row to start adding this constraint at.
 */
#define MDTBCLFINDSTARTROW(c)                                           \
int n_bodies = ((MdtBclConstraintHeader *)c)->n_bodies;                 \
int max_rows = ((MdtBclConstraintHeader *)c)->max_rows;                 \
int r = clist->n_constraint_rows;                                       \
  MeMatrix4 w[MdtKeaMAXBODYCONSTRAINT];                                 \
  MeVector3 p[MdtKeaMAXBODYCONSTRAINT];                                 \
   MdtBclConvertRefFramesToWorld2((MdtBclConstraintHeader *)c, w, p);   \
    MeReal *v0          = (clist->v0) + r;                              \
    MeReal *phi         = (clist->phi) + r;                             \
    MeReal *lower         = (clist->lower) + r;                         \
    MeReal *upper         = (clist->upper) + r;                         \
    MeReal *xepsilon = (clist->xepsilon) + r;                           \
    MeReal *xgamma     = (clist->xgamma) + r;                           \
    int *flags     = (clist->flags) + r;                                \
    MdtKeaGRow _G[MdtKeaMAXBODYCONSTRAINT][6];                          \
    memset(_G, 0, sizeof(_G));                                          \
    MdtKeaGBlock G[MdtKeaMAXBODYCONSTRAINT];                            \
    { for (int i=0; i<n_bodies; ++i) G[i] = _G[i]; }                    \
    MdtKeaInputRowFactors factors;                                      \
    factors.v0          = v0;                                           \
    factors.phi         = phi;                                          \
    factors.lower         = lower;                                      \
    factors.upper         = upper;                                      \
    factors.xepsilon = xepsilon;                                        \
    factors.xgamma     = xgamma;                                        \
    factors.G = G;                                                      \
    factors.flags = flags;


#define MDTBCL_SET_ANGULAR_MODE(x)   do { (x) = ((x)&~kMdtBclLinear   ) |kMdtBclAngular;   } while(0)
#define MDTBCL_SET_LINEAR_MODE(x)    do { (x) = ((x)&~kMdtBclAngular  ) |kMdtBclLinear;    } while(0)
#define MDTBCL_SET_POSITION_MODE(x)  do { (x) = ((x)&~kMdtBclVelocity ) |kMdtBclPosition;  } while(0)
#define MDTBCL_SET_VELOCITY_MODE(x)  do { (x) = ((x)&~kMdtBclPosition ) |kMdtBclVelocity;  } while(0)
#define MDTBCL_SET_CONTROLLED_MODE(x) do { (x) =((x)&~kMdtBclControlled)|kMdtBclControlled;} while(0)
#define MDTBCL_IS_LINEAR_MODE(x)     (((x)&kMdtBclLinear)  >>1)
#define MDTBCL_IS_ANGULAR_MODE(x)    (((x)&kMdtBclAngular) >>2)
#define MDTBCL_IS_POSITION_MODE(x)   (((x)&kMdtBclPosition)>>3)
#define MDTBCL_IS_VELOCITY_MODE(x)   (((x)&kMdtBclVelocity)>>4)
#define MDTBCL_IS_CONTROLLED_MODE(x) (((x)&kMdtBclControlled)>>5)

static inline void  MEAPI
MdtBclSetPerturbationParameters(MeReal compliance, MeReal damping, 
                                MeReal loss, int mode, MeReal h_inv, 
                                MeReal *xepsilon, MeReal *xgamma)
{
    if ( MDTBCL_IS_POSITION_MODE(mode) ) 
    {
        MeReal t    = (damping==MEINFINITY) ? 0 : MeRecip(1 + h_inv*compliance*damping) ;
        xepsilon[0] = h_inv*h_inv*compliance*t; 
        xgamma  [0] = t; 
    }
    
    else
    {
        xepsilon[0] = h_inv*loss; 
        xgamma  [0] = 0;
    }
}

static inline void MEAPI
MdtBclSetDefaultPerturbationParameters(const MdtBclConstraintParameters * const p, 
                                       int mode, 
                                       MeReal *xepsilon, 
                                       MeReal *xgamma)
{
    switch(mode)
    {
    case kMdtBclLinear|kMdtBclPosition:
        xepsilon[0] = p->diag[0];
        xgamma  [0] = p->rhs[0];
        break;
    case kMdtBclAngular|kMdtBclPosition:
        xepsilon[0] = p->diag[1];
        xgamma  [0] = p->rhs[1];
        break;
    case kMdtBclLinear |kMdtBclVelocity:
        xepsilon[0] = p->diag[2]; 
        break;
    case kMdtBclAngular|kMdtBclVelocity:
        xepsilon[0] = p->diag[3];
        break;
    default:
        // either user has set appropriate values or this is caught by a
        // global which comes later. 
        break;
    }
}
                                       

#endif
