#ifndef _VXMATH_H
#define _VXMATH_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2003 CMLabs Simulations Inc. 
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/12/03 01:21:29 $
   
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
 * Dynamics Toolkit math helper functions.
 *
 * These are mostly provided as a convenience to users and writers of demos. Consideration has been given
 * to clarity and generality of implementation. Therefore the implementation of some functions may not
 * be appropriate for performance critical code.
 *
 * Math library conventions:
 * \li Matrices are stored row-major; that is, the memory layout is <em> a[0][0], a[0][1],... a[1][0], a[1][1],...</em> etc
 * \li Transformation matrices are stored with translation component in the bottom row, that is, <em>a[3][0]...a[3][2]</em>
 * \li Transformation and Rotation matrices are applied by <b>post-multiplication</b>, that is, @a v'=vM, where @a M
 * is a transformation or rotation matrix. This implies that transforming by the matrix @a A=B*C corresponds
 * to transforming by @a B, then transforming by @a C.
 * \li Quaternions are stored <em>[w,x,y,z]</em>, where @a w is the real part. This may change to <em>[x,y,z,w]</em> in a future release.
 * in order to support accelerated quaternion operations on PS/2
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Vx/VxPrecision.h"
#include "MeInline.h"

/*ANSI C code from the article "Euler Angle Conversion" by Ken Shoemake, 
  shoemake@graphics.cis.upenn.edu in "Graphics Gems IV", Academic Press, 1994 
  http://vered.rose.utoronto.ca/people/david_dir/GEMS/GEMS.html
*/

/*** Definitions ***/
namespace Vx
{

typedef struct {VxReal x, y, z, w;} VxQuat; /* Quaternion */
enum eQuatPart {X, Y, Z, W};
typedef VxQuat VxEulerAngles;    /* (x,y,z)=ang 1,2,3, w=order code  */

/*** Order type constants, constructors, extractors ***/

    /* There are 24 possible conventions, designated by:    */
    /*	  o EulAxI = axis used initially		    */
    /*	  o EulPar = parity of axis permutation		    */
    /*	  o EulRep = repetition of initial axis as last	    */
    /*	  o EulFrm = frame from which axes are taken	    */
    /* Axes I,J,K will be a permutation of X,Y,Z.	    */
    /* Axis H will be either I or K, depending on EulRep.   */
    /* Frame S takes axes from initial static frame.	    */
    /* If ord = (AxI=X, Par=Even, Rep=No, Frm=S), then	    */
    /* {a,b,c,ord} means Rz(c)Ry(b)Rx(a), where Rz(c)v	    */
    /* rotates v around Z by c radians.			    */

#define EulFrmS	     0
#define EulFrmR	     1
#define EulFrm(ord)  ((unsigned)(ord)&1)
#define EulRepNo     0
#define EulRepYes    1
#define EulRep(ord)  (((unsigned)(ord)>>1)&1)
#define EulParEven   0
#define EulParOdd    1
#define EulPar(ord)  (((unsigned)(ord)>>2)&1)
#define EulSafe	     "\000\001\002\000"
#define EulNext	     "\001\002\000\001"
#define EulAxI(ord)  ((int)(EulSafe[(((unsigned)(ord)>>3)&3)]))
#define EulAxJ(ord)  ((int)(EulNext[EulAxI(ord)+(EulPar(ord)==EulParOdd)]))
#define EulAxK(ord)  ((int)(EulNext[EulAxI(ord)+(EulPar(ord)!=EulParOdd)]))
#define EulAxH(ord)  ((EulRep(ord)==EulRepNo)?EulAxK(ord):EulAxI(ord))
   
/* Static axes */
#define EulOrdXYZs    EulOrd(X,EulParEven,EulRepNo,EulFrmS)  /* 0 */
#define EulOrdXYXs    EulOrd(X,EulParEven,EulRepYes,EulFrmS) /* 2 */
#define EulOrdXZYs    EulOrd(X,EulParOdd,EulRepNo,EulFrmS)   /* 4 */
#define EulOrdXZXs    EulOrd(X,EulParOdd,EulRepYes,EulFrmS)  /* 6 */
#define EulOrdYZXs    EulOrd(Y,EulParEven,EulRepNo,EulFrmS)  /* 8 */
#define EulOrdYZYs    EulOrd(Y,EulParEven,EulRepYes,EulFrmS) /* 10 */
#define EulOrdYXZs    EulOrd(Y,EulParOdd,EulRepNo,EulFrmS)   /* 12 */
#define EulOrdYXYs    EulOrd(Y,EulParOdd,EulRepYes,EulFrmS)  /* 14 */
#define EulOrdZXYs    EulOrd(Z,EulParEven,EulRepNo,EulFrmS)  /* 16 */
#define EulOrdZXZs    EulOrd(Z,EulParEven,EulRepYes,EulFrmS) /* 18 */
#define EulOrdZYXs    EulOrd(Z,EulParOdd,EulRepNo,EulFrmS)   /* 20 */
#define EulOrdZYZs    EulOrd(Z,EulParOdd,EulRepYes,EulFrmS)  /* 22 */
  /* Rotating axes */
#define EulOrdZYXr    EulOrd(X,EulParEven,EulRepNo,EulFrmR)  /* 1 */
#define EulOrdXYXr    EulOrd(X,EulParEven,EulRepYes,EulFrmR) /* 3 */
#define EulOrdYZXr    EulOrd(X,EulParOdd,EulRepNo,EulFrmR)   /* 5 */
#define EulOrdXZXr    EulOrd(X,EulParOdd,EulRepYes,EulFrmR)  /* 7 */
#define EulOrdXZYr    EulOrd(Y,EulParEven,EulRepNo,EulFrmR)  /* 9 */
#define EulOrdYZYr    EulOrd(Y,EulParEven,EulRepYes,EulFrmR) /* 11 */
#define EulOrdZXYr    EulOrd(Y,EulParOdd,EulRepNo,EulFrmR)   /* 13 */
#define EulOrdYXYr    EulOrd(Y,EulParOdd,EulRepYes,EulFrmR)  /* 15 */
#define EulOrdYXZr    EulOrd(Z,EulParEven,EulRepNo,EulFrmR)  /* 17 */
#define EulOrdZXZr    EulOrd(Z,EulParEven,EulRepYes,EulFrmR) /* 19 */
#define EulOrdXYZr    EulOrd(Z,EulParOdd,EulRepNo,EulFrmR)   /* 21 */
#define EulOrdZYZr    EulOrd(Z,EulParOdd,EulRepYes,EulFrmR)  /* 23 */

    /* EulGetOrd unpacks all useful information about order simultaneously. */
#define EulGetOrd(ord,i,j,k,h,n,s,f) {unsigned o=ord;f=o&1;o>>=1;s=o&1;o>>=1;\
    n=o&1;o>>=1;i=EulSafe[o&3];j=EulNext[i+n];k=EulNext[i+1-n];h=s?k:i;}
    /* EulOrd creates an order value between 0 and 23 from 4-tuple choices. */
#define EulOrd(i,p,r,f)	   (((((((i)<<1)+(p))<<1)+(r))<<1)+(f))


//#ifdef __cplusplus
//extern    "C" {
//#endif

VxEulerAngles MEAPI VxEuler(VxReal ai, VxReal aj, VxReal ah, int order);

VxEulerAngles MEAPI VxEuler2(VxReal3 a, int order);

VxQuat MEAPI VxEulerToVxQuat(VxEulerAngles ea);

void MEAPI VxEulerToVxMatrix44(VxEulerAngles ea, VxReal44 M);

VxEulerAngles MEAPI VxEulerFromVxMatrix44(VxReal44 M, int order);

VxEulerAngles MEAPI VxEulerFromVxQuat(VxQuat q, int order);


//#ifdef __cplusplus
//}
//#endif

} // namespace Vx

#endif              /* _VXMATH_H */
