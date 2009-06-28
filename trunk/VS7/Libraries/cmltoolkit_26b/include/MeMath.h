#ifndef _MEMATH_H
#define _MEMATH_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/02/22 22:41:10 $
   
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
#include "MePrecision.h"
#include "MeInline.h"
#include "MeAssert.h"

/** Return the greatest integer <= @a n divisible by 4 */
#define MeMathFLOOR4(n)         ((n) - (n)%4)
/** Return the greatest integer >= @a n divisible by 4 */
#define MeMathCEIL4(n)          (((n)%4) == 0 ? (n) : (n) - (n)%4 + 4)
/** Return the greatest integer >= @a n divisible by 12 */
#define MeMathCEIL12(n)         (((n)%12) == 0 ? (n) : (n) - (n)%12 + 12)
/** Return the greatest integer >= @a n divisible by 16 */
#define MeMathCEIL16(n)         (((n)%16) == 0 ? (n) : (n) - (n)%16 + 16)
/** Return the greatest integer >= @a n divisible by 64 */
#define MeMathCEIL64(n)         (((n)%64) == 0 ? (n) : (n) - (n)%64 + 64)
/** Return the greatest integer >= @a n divisible by c */
#define MeMathCEILN(c,n)        (((n)%(c)) == 0 ? (n) : (n) - (n)%(c) + (c))

/** Return the max of @a a and @a b */
#define MeMAX(a,b)              (((a)<(b)) ? b : a)

/** Return the min of @a a and @a b */
#define MeMIN(a,b)              (((a)<(b)) ? a : b)

/** Return the max of @a a, @a b and @a c */
#define MeMAX3(a,b,c)           (MeMAX(MeMAX(a,b),c))

/** Return @a a clamped to the range <em>[b,c]</em> */
#define MeCLAMP(a,b,c)          (MeMAX(MeMIN(a,c),b))

#ifndef ME_PI
#   define ME_PI                ((MeReal) 3.14159265358979323846)
#endif

#ifdef __cplusplus
extern    "C" {
#endif

/* **** MEREAL *****/
#if (!MeDEFINE)
    void MEAPI MeRealSwap(MeReal *const a, MeReal *const b);
    MeReal MEAPI MeRealRandomInRange(MeReal start, MeReal end);
    MeBool MEAPI MeRealIsFinite(MeReal a);
#endif

/* **** MEVECTOR3 *****/
#if (!MeDEFINE)
    void MEAPI MeVectorSetZero(MeReal * A, const int n);
    void MEAPI MeVector3Copy(MeVector3 c, const MeVector3 b);
    void MEAPI MeVector3Negate(MeVector3 c, const MeVector3 b);
    void MEAPI MeVector3Set(MeVector3 c, const MeReal x, const MeReal y, const MeReal z);
    void MEAPI MeVector3SetScalar(MeVector3 c, const MeReal x);
    void MEAPI MeVector3Subtract(MeVector3 a, const MeVector3 b, const MeVector3 c);
    void MEAPI MeVector3Add(MeVector3 a, const MeVector3 b, const MeVector3 c);
    MeReal MEAPI MeVector3Dot(const MeVector3 b, const MeVector3 c);
    void MEAPI MeVector3Cross(MeVector3 a, const MeVector3 b, const MeVector3 c);
    void MEAPI MeVector3OuterProduct(MeMatrix3 a, const MeVector3 b, const MeVector3 c);
    MeReal MEAPI MeVector3MagnitudeSqr(const MeVector3 v);
    MeReal MEAPI MeVector3Magnitude(const MeVector3 v);
    MeINLINE MeReal MEAPI MeVector3DistanceSqr(const MeVector3 v, const MeVector3 w);
    MeINLINE MeReal MEAPI MeVector3Distance(const MeVector3 v, const MeVector3 w);
    void MEAPI MeVector3Scale(MeVector3 v, const MeReal a);
    void MEAPI MeVector3MultiplyAdd(MeVector3 v, const MeReal a, const MeVector3 v1);
    void MEAPI MeVector3MultiplyAdd2(MeVector3 v, const MeReal a, const MeVector3 v1, const MeVector3 v2);
    void MEAPI MeVector3MultiplyScalar(MeVector3 a, const MeVector3 b, const MeReal c);
    MeReal MEAPI MeVector3Normalize(MeVector3 v);
    void MEAPI MeVector3MakeOrthogonal(MeVector3 v, const MeVector3 v1);
    void MEAPI MeVector3PlaneSpace(const MeVector3 n, MeVector3 a, MeVector3 b);

    MeBool MEAPI MeMatrix3IsIsotropic(const MeMatrix3 i);
    void MEAPI MeVector3MultiplyElements(MeVector3 a, const MeVector3 b, const MeVector3 c);
    void MeVector3Clamp(MeVector3 a, const MeReal min, const MeReal max);
#endif
    MeReal MEAPI MeVector3AreaOfTriangle(const MeVector3 v1, const MeVector3 v2);
    void MEAPI MeVector3Swap(MeVector3 a, MeVector3 b);

/* *** MEVECTOR4 *****/
#if (!MeDEFINE)
    void MEAPI MeVector4Copy(MeVector4 c, const MeVector4 b);
    void MEAPI MeVector4Set(MeVector4 c, const MeReal x, const MeReal y, const MeReal z, const MeReal t);
    void MEAPI MeVector4Subtract(MeVector4 a, const MeVector4 b, const MeVector4 c);

    void MEAPI MeVector4MultiplyElements(MeVector4 a, const MeVector4 b, const MeVector4 c);
    void MeVector4Clamp(MeVector4 a, const MeReal min, const MeReal max);
    void MEAPI MeVector4Scale(MeVector3 v, const MeReal a);
    void MEAPI MeVector4Add(MeVector4 a, const MeVector4 b, const MeVector4 c);

    MeReal MEAPI MeVector4MagnitudeSqr(const MeVector4 v);
    MeReal MEAPI MeVector4Dot(const MeVector4 b, const MeVector4 c);
#endif

/* *** MEQUATERNION *****/
#if (!MeDEFINE)
    void MEAPI MeQuaternionMake(MeVector4 q, const MeVector3 x, const MeReal a);
    void MEAPI MeQuaternionToR(MeMatrix3 R, const MeVector4 q);
    void MEAPI MeQuaternionToTM(MeMatrix4 tm, const MeVector4 q);
    void MEAPI MeQuaternionToD(MeReal D[12], const MeVector4 q);
    void MEAPI MeQuaternionWtoDQ(MeVector4 dq, const MeVector4 q, const MeVector3 w);
    void MEAPI MeQuaternionProduct(MeVector4 r, const MeVector4 p, const MeVector4 q);
    void MEAPI MeQuaternionRotateVector3(MeVector3 vout, const MeVector4 q, const MeVector3 v);
    void MEAPI MeQuaternionInvert(MeVector4 q);
#endif
    void MEAPI MeQuaternionFromTM(MeVector4 q, const MeMatrix4 tm);
    void MEAPI MeQuaternionFiniteRotation(MeVector4 q, const MeVector3 w, const MeReal h);
    void MEAPI MeQuaternionForRotation(MeVector4 q, const MeVector3 v1, const MeVector3 v2);
    void MEAPI MeQuaternionSlerp(MeVector4 q, const MeVector4 from, const MeVector4 to, const MeReal howFar);
  void MEAPI MeQuaternionGetForwardVelocity(MeVector3 w, const MeVector4 from, const MeVector4 to);
  /* void MEAPI MeQuaternionGetBackwardVelocity(MeVector3 w, const MeVector4 from, const MeVector4 to); */
  void MEAPI MeQuaternionGetEMatrix(MeReal E[3][4], const MeVector4 q);

/* *** MEMATRIX *****/
#if (!MeDEFINE)
    void MEAPI MeMatrixTranspose(MeReal *A, const int n, const int m, const MeReal *const B);
    void MEAPI MeMatrixMultiply(MeReal *A, const int p, const int q, const int r,
                const MeReal *const B, const MeReal *const C);
    void MEAPI MeMatrixMultiplyT1(MeReal *A, const int p, const int q, const int r,
                  const MeReal *const B, const MeReal *const C);
#endif
    void MEAPI MeMatrixFPrint(FILE * const file, const MeReal *const A,
                  const int n, const int m, const char *const format);
    void MEAPI MeMatrixPrint(const MeReal *const A,
                 const int n, const int m, const char *const format);

/* *** MEMATRIX3 *****/
#if (!MeDEFINE)
    void MEAPI MeMatrix3Copy(MeMatrix3 A, const MeMatrix3 B);
    void MEAPI MeMatrix3CopyVec(MeMatrix3 A,
                const MeVector3 B1, const MeVector3 B2, const MeVector3 B3);
    void MEAPI MeMatrix3Transpose(MeMatrix3 A);
    void MEAPI MeMatrix3CrossFromVector(const MeVector3 a, const MeReal factor, MeVector3 A0,
                    MeVector3 A1, MeVector3 A2);
    void MEAPI MeMatrix3MultiplyVector(MeVector3 A, const MeMatrix3 B, const MeVector3 C);
    void MEAPI MeMatrix3Add(MeMatrix3 A, const MeMatrix3 B, const MeMatrix3 C);
    void MEAPI MeMatrix3Subtract(MeMatrix3 A, const MeMatrix3 B, const MeMatrix3 C);

    /*  MeMatrix3Multiply is nuked, now replaced with MeMatrix3MultiplyMatrix,
        *** WHICH TAKES ITS SOURCE ARGUMENTS IN THE OTHER ORDER ***

        void MEAPI MeMatrix3Multiply(MeMatrix3 A, const MeMatrix3 B, const MeMatrix3 C);
    */

    void MEAPI MeMatrix3MultiplyMatrix(MeMatrix3 A, const MeMatrix3 B, const MeMatrix3 C);
    void MEAPI MeMatrix3MakeIdentity(MeMatrix3 tm);
    void MEAPI MeMatrix3Scale(MeMatrix3 A, const MeReal a);
    MeReal MEAPI MeMatrix3Trace(MeMatrix3 m);
#endif
    MeBool MEAPI MeMatrix3IsIdentity(const MeMatrix3 a, const MeReal tolerance);
    void MEAPI MeMatrix3LUDecompose(MeMatrix3 L, MeMatrix3 U, const MeMatrix3 a,
                    unsigned int *const SwappedRow, MeVector3 scale);
    void MEAPI MeMatrix3SwapColumns(MeMatrix3 a, const unsigned int col1, const unsigned int col2);
    MeBool MEAPI MeMatrix3Invert(MeMatrix3 a);
    MeBool MEAPI MeMatrix3SymmetricInvert(MeMatrix3 a);
    void MEAPI MeMatrix3InvertTo3x4(const MeMatrix3 a,
                    MeVector4 inv1, MeVector4 inv2, MeVector4 inv3);
    void MEAPI MeMatrix3MakeRotationX(MeMatrix3 m, const MeReal a);
    void MEAPI MeMatrix3MakeRotationY(MeMatrix3 m, const MeReal a);
    void MEAPI MeMatrix3MakeRotationZ(MeMatrix3 m, const MeReal a);
    MeBool MEAPI MeMatrix3IsValidOrientationMatrix(const MeMatrix3 rot, const MeReal tolerance);
    void MEAPI MeMatrix3FromEulerAngles(MeMatrix3 m,
                    const MeReal xangle, const MeReal yangle, const MeReal zangle);
    void MEAPI MeMatrix3GetEulerAngles(MeMatrix3 m, MeReal euler[3]);

/* *** MEMATRIX4 *****/
#if (!MeDEFINE)
    void MEAPI MeMatrix4SetZero(MeMatrix4 A);
    void MEAPI MeMatrix4Add(MeMatrix4 A, const MeMatrix4 B, const MeMatrix4 C);
    MeBool MEAPI MeMatrix4IsZero(const MeMatrix4 m);
    void MEAPI MeMatrix4Copy(MeMatrix4 A, const MeMatrix4 B);

    /*  MeMatrix4Multiply is nuked, now replaced with MeMatrix4MultiplMatrix,
        *** WHICH TAKES ITS SOURCE ARGUMENTS IN THE OTHER ORDER ***

        void MEAPI MeMatrix4Multiply(MeMatrix4 A, const MeMatrix4 B, const MeMatrix4 C);
    */

    void MEAPI MeMatrix4MultiplyMatrix(MeMatrix4 A, const MeMatrix4 B, const MeMatrix4 C);
    void MEAPI MeMatrix4MultiplyVector(MeVector4 A, const MeMatrix4 B, const MeVector4 C);
    void MEAPI MeMatrix4Transpose(MeMatrix4 A);
#endif
    MeBool MEAPI MeMatrix4IsIdentity(const MeMatrix4 a, const MeReal tolerance);

/* *** MEMATRIX4 TRANSFORMATION *****/
#if (!MeDEFINE)
    void MEAPI MeMatrix4TMGetRotation(MeMatrix3 R, const MeMatrix4 tm);
    void MEAPI MeMatrix4TMGetPosition(MeMatrix4 tm, MeVector3 position);
    void MEAPI MeMatrix4TMMakeFromRotationAndPosition(MeMatrix4 A,
                              const MeMatrix3 R,
                              const MeReal x,
                              const MeReal y, const MeReal z);
    void MEAPI MeMatrix4TMSetRotation(MeMatrix4 tm, const MeMatrix3 R);
    void MEAPI MeMatrix4TMSetRotationFromQuaternion(MeMatrix4 tm, const MeVector4 q);
    void MEAPI MeMatrix4TMMakeIdentity(MeMatrix4 tm);
    void MEAPI MeMatrix4TMSetPosition(MeMatrix4 tm, MeReal x, MeReal y, MeReal z);
    void MEAPI MeMatrix4TMRotateVector3(MeVector3 A, const MeMatrix4 B, const MeVector3 C);
    void MEAPI MeMatrix4TMTransformVector3(MeVector3 A, const MeMatrix4 B, const MeVector3 C);
    void MEAPI MeMatrix4TMInverseRotateVector3(MeVector3 A, const MeMatrix4 B, const MeVector3 C);
    void MEAPI MeMatrix4TMInverseTransformVector3(MeVector3 A, const MeMatrix4 B, const MeVector3 C);
#endif
    MeBool MEAPI MeMatrix4IsTM(const MeMatrix4 tm, const MeReal tolerance);
    void MEAPI MeMatrix4TMCompound(MeMatrix4 A, const MeMatrix4 B, const MeMatrix4 C);
    void MEAPI MeMatrix4TMCompoundRotation(MeMatrix4 A, const MeMatrix4 B, const MeMatrix4 C);
    void MEAPI MeMatrix4TMCompoundInverseSecond(MeMatrix4 A, const MeMatrix4 B, const MeMatrix4 C);
    void MEAPI MeMatrix4TMCompoundRotationInverseSecond(MeMatrix4 A, const MeMatrix4 B, const MeMatrix4 C);
    void MEAPI MeMatrix4TMInvert(MeMatrix4 tm);
    void MEAPI MeMatrix4TMUpdateFromVelocities(MeMatrix4 aTransformReturn,
                           MeReal aEpsilon,
                           MeReal aTimeStep,
                           const MeVector3 aVelocity,
                           const MeVector3 aAngularVelocity,
                           const MeMatrix4 aTransform);

    void MEAPI MeMatrix4TMUpdateFromVelocitiesAndAcceler(MeMatrix4 aTransformReturn,
                             MeReal aEpsilon,
                             MeReal aTimeStep,
                             const MeVector3 aVelocity,
                             const MeVector3 aAcceler,
                             const MeVector3 aAngularVelocity,
                             const MeVector3 aAngularAcceler,
                             const MeMatrix4 aTransform);

  void MEAPI MeMatrix4TMSetRotationFromEulerAngles(MeMatrix4 m,
                    MeReal xangle, MeReal yangle, MeReal zangle);

  void MEAPI MeMatrix4TMGetEulerAngles(MeMatrix4 tm, MeReal euler[3]);

/* *** OTHER *****/
#if (!MeDEFINE)
/* Useful indexing functions */
    int MEAPI MeUpperDiagonalIndex(const int inRow, const int inCol, const int inNCols);
    int MEAPI MeUpperDiagonalSize(const int inNCols);
    int MEAPI MeSymUpperDiagonalIndex(const int inRow, const int inCol, const int inNCols);
    int MEAPI MeSuperDiagonalIndex(const int inRow, const int inCol, const int inNCols);
    int MEAPI MeSuperDiagonalSize(const int inNCols);
    int MEAPI MeSymSuperDiagonalIndex(const int inRow, const int inCol, const int inNCols);
    MeBool MEAPI MeRealArrayIsZero(MeReal *v, int elements);

#endif

#ifdef __cplusplus
}
#endif
#if (MeDEFINE)
/** Swap the real values addressed by pointers @a a and @a b. */ 
MeINLINE void MEAPI
MeRealSwap(MeReal *const a, MeReal *const b)
{
    const MeReal temp = *a;
    MEASSERT(a != 0);
    MEASSERT(b != 0);
    *a = *b;
    *b = temp;
}

MeINLINE MeBool MEAPI MeRealIsFinite(MeReal a)
{
#ifdef WIN32
    return _finite(a);
#elif defined(IRIX)
    return 1;
#else
#if defined(isfinite)
    return isfinite(a);
#else
    return 1;
#endif
#endif
}

/**
 * Set the first @a n elements of @a A to zero.
 * There is such a thing as a standard library and we do use it.
 */
MeINLINE void MEAPI
MeVectorSetZero(MeReal * A, const int n)
{
   memset(A, 0, n*sizeof(MeReal));
}

/**
 * MeVector3 copy.
 */
MeINLINE void MEAPI
MeVector3Copy(MeVector3 c, const MeVector3 b)
{
    c[0] = b[0];
    c[1] = b[1];
    c[2] = b[2];
}

/**
 * MeVector3 copy.
 */
MeINLINE void MEAPI
MeVector3Set(MeVector3 c, const MeReal x, const MeReal y, const MeReal z)
{
    c[0] = x;
    c[1] = y;
    c[2] = z;
}


/**
 * MeVector3 set from scalar.
 */
MeINLINE void MEAPI
MeVector3SetScalar(MeVector3 c, const MeReal x)
{
    c[0] = x;
    c[1] = x;
    c[2] = x;
}
/**
 * MeVector3 copy.
 */
MeINLINE void MEAPI
MeVector3Negate(MeVector3 c, const MeVector3 b)
{
    c[0] = -b[0];
    c[1] = -b[1];
    c[2] = -b[2];
}

/**
 * MeVector3 subtraction: a = b - c ; 
 */
MeINLINE void MEAPI
MeVector3Subtract(MeVector3 a, const MeVector3 b, const MeVector3 c)
{
    a[0] = b[0] - c[0], a[1] = b[1] - c[1], a[2] = b[2] - c[2];
}

/**
 * MeVector3 addition.
 */
MeINLINE void MEAPI
MeVector3Add(MeVector3 a, const MeVector3 b, const MeVector3 c)
{
    a[0] = b[0] + c[0], a[1] = b[1] + c[1], a[2] = b[2] + c[2];
}

/**
 * MeVector3 dot product.
 */
MeINLINE MeReal MEAPI
MeVector3Dot(const MeVector3 b, const MeVector3 c)
{
    return b[0] * c[0] + b[1] * c[1] + b[2] * c[2];
}

/**
 * MeVector3 cross product: @a a = @a b @b x @a c.
 */
MeINLINE void MEAPI
MeVector3Cross(MeVector3 a, const MeVector3 b, const MeVector3 c)
{
    a[0] = b[1] * c[2] - b[2] * c[1];
    a[1] = b[2] * c[0] - b[0] * c[2];
    a[2] = b[0] * c[1] - b[1] * c[0];
}

/**
 * MeVector3 outer product.
 */
MeINLINE void MEAPI
MeVector3OuterProduct(MeMatrix3 a, const MeVector3 b, const MeVector3 c)
{
    a[0][0] = b[0] * c[0];
    a[0][1] = b[0] * c[1];
    a[0][2] = b[0] * c[2];
    a[1][0] = b[1] * c[0];
    a[1][1] = b[1] * c[1];
    a[1][2] = b[1] * c[2];
    a[2][0] = b[2] * c[0];
    a[2][1] = b[2] * c[1];
    a[2][2] = b[2] * c[2];
}

/**
 * Return the square of the magnitude of the vector.
 */
MeINLINE MeReal MEAPI
MeVector3MagnitudeSqr(const MeVector3 v)
{
    MeReal    m = (MeReal) 0;

    m += MeSqr(v[0]);
    m += MeSqr(v[1]);
    m += MeSqr(v[2]);

    return m;
}

/**
 * Return the index which has the largest element
 */
MeINLINE int MEAPI
MeVector3LargestElementIndex(const MeVector3 v)
{
  int i = 0;
  MeReal fmax = MeFabs(v[0]);
  MeReal a = MeFabs(v[1]);
  if ( a>fmax)  { fmax = a; i = 1;}
  if (MeFabs(v[2])>fmax) i=2;
  return i;     
}


/**
 * Return the square of the magnitude of the vector.
 */
MeINLINE MeReal MEAPI
MeVector3Magnitude(const MeVector3 v)
{
    return MeSqrt(MeVector3MagnitudeSqr(v));
}


/**
 * Return the square of the distance between two vectors representing points.
 */
MeINLINE MeReal MEAPI
MeVector3DistanceSqr(const MeVector3 v, const MeVector3 w)
{
    MeVector3 d;
    MeVector3Subtract(d,v,w);
    return MeVector3MagnitudeSqr(d);
}

/**
 * Return the distance between two vectors representing points.
 */
MeINLINE MeReal MEAPI
MeVector3Distance(const MeVector3 v, const MeVector3 w)
{
    return MeSqrt(MeVector3DistanceSqr(v,w));
}


/**
 * Multiply all elements of a 3 vector by a scalar.
 */
MeINLINE void MEAPI
MeVector3Scale(MeVector3 v, const MeReal a)
{
    v[0] *= a;
    v[1] *= a;
    v[2] *= a;
}

/**
 * Add to each element of a 3 vector the corresponding element of
 * another 3 vector multiplied by a scalar.
 */
MeINLINE void MEAPI
MeVector3MultiplyAdd(MeVector3 v, const MeReal a, const MeVector3 v1)
{
    v[0] += a * v1[0];
    v[1] += a * v1[1];
    v[2] += a * v1[2];
}

MeINLINE void MEAPI
MeVector3MultiplyAdd2(MeVector3 v, const MeReal a, const MeVector3 v1, const MeVector3 v2)
{
    v[0] = v2[0] + a * v1[0];
    v[1] = v2[1] + a * v1[1];
    v[2] = v2[2] + a * v1[2];
}

/**
 * Multiply each element of @a b by @a c and place resulting vector into @a a.
 */
MeINLINE void MEAPI
MeVector3MultiplyScalar(MeVector3 a, const MeVector3 b, const MeReal c)
{
    a[0] = b[0] * c;
    a[1] = b[1] * c;
    a[2] = b[2] * c;
}

/**
 * Make the given vector unit length and returns its original magnitude.
 */
MeINLINE MeReal MEAPI
MeVector3Normalize(MeVector3 v)
{
    MeReal    k;
    MeReal    mag = MeSqr(v[0]) + MeSqr(v[1]) + MeSqr(v[2]);
    
    if (mag > 0) {
        mag = MeSqrt(mag);
        k = MeRecip(mag);
        
        v[0] *= k;
        v[1] *= k;
        v[2] *= k;
    } else {
        v[0] = 1;
        v[1] = 0;
        v[2] = 0;
    }
    return mag;
}

/**
 * Compute a 3 vector orthogonal to another 3 vector.
 */
MeINLINE void MEAPI
MeVector3MakeOrthogonal(MeVector3 v, const MeVector3 v1)
{
    MeVector3 tmp;

    tmp[0] = v1[0];
    tmp[1] = v1[1];
    tmp[2] = v1[2];

    MeVector3Normalize(tmp);

    if (tmp[2] < (MeReal) 0.5) {
    v[0] = -tmp[1];
    v[1] = tmp[0];
    v[2] = (MeReal) 0;
    } else {
    v[0] = -tmp[2];
    v[1] = (MeReal) 0;
    v[2] = tmp[0];
    }

    MeVector3Normalize(v);
}

/**
 * Make normal 3x1 vectors @a a and @a b such that together with the
 * normal 3x1 vector @a n they form an orthonormal basis.
 *
 * @a a and @a b span the plane that is normal to @a n, and @a n = @a a x @a b.
 * Note that if @a n is not normalized then @a b will not be
 * normalized either.
 */

MeINLINE void MEAPI
MeVector3PlaneSpace(const MeVector3 n, MeVector3 a, MeVector3 b)
{
    if (MeFabs(n[0]) > MeFabs(n[1])) {
    /* |n.e1| > |n.e2|, so a = e2 x n */
    MeReal    scale = MeRecipSqrt(n[2] * n[2] + n[0] * n[0]);

    a[0] = n[2] * scale;
    a[1] = 0;
    a[2] = -n[0] * scale;

    /* b = n x a */
    b[0] = n[1] * a[2];
    b[1] = n[2] * a[0] - n[0] * a[2];
    b[2] = -n[1] * a[0];
    } else {
    /* |n.e1| <= |n.e2|, so a = e1 x n */
    MeReal    scale = MeRecipSqrt(n[2] * n[2] + n[1] * n[1]);

    a[0] = 0;
    a[1] = -n[2] * scale;
    a[2] = n[1] * scale;

    /* b = n x a */
    b[0] = n[1] * a[2] - n[2] * a[1];
    b[1] = -n[0] * a[2];
    b[2] = n[0] * a[1];
    }
}

/**
 * MeVector3 multiply element by element
 */

MeINLINE void MEAPI
MeVector3MultiplyElements(MeVector3 a, const MeVector3 b, const MeVector3 c) {
    a[0] = b[0] * c[0];
    a[1] = b[1] * c[1];
    a[2] = b[2] * c[2];
}

/**
 * Clamp all members of @a a between @a max and @a min
 */
MeINLINE void MeVector3Clamp(MeVector3 a, const MeReal min, const MeReal max) {
    a[0] = a[0] < min ? min : (a[0] > max ? max : a[0]),
        a[1] = a[1] < min ? min : (a[1] > max ? max : a[1]),
        a[2] = a[2] < min ? min : (a[2] > max ? max : a[2]);
}

/**
 * MeVector4 copy.
 */
MeINLINE void MEAPI
MeVector4Copy(MeVector4 c, const MeVector4 b)
{
    c[0] = b[0], c[1] = b[1], c[2] = b[2], c[3] = b[3];
}

/**
 * MeVector4 set.
 */
MeINLINE void MEAPI 
MeVector4Set(MeVector4 c, const MeReal x, const MeReal y, const MeReal z, const MeReal t)
{
    c[0] = x;
    c[1] = y;
    c[2] = z;
    c[3] = t;
}

/**
 * MeVector4 subtraction.
 */
MeINLINE void MEAPI
MeVector4Subtract(MeVector4 a, const MeVector4 b, const MeVector4 c)
{
    a[0] = b[0] - c[0], a[1] = b[1] - c[1], a[2] = b[2] - c[2], a[3] = b[3] - c[3];
}

/**
 * Multiply element by element
 */
MeINLINE void MEAPI MeVector4MultiplyElements(MeVector4 a, const MeVector4 b, const MeVector4 c) {
    a[0] = b[0] * c[0], a[1] = b[1] * c[1], a[2] = b[2] * c[2], a[3] = b[3] * c[3];
}

/**
 * Clamp all elements of @a a between @a max and @a min
 */
MeINLINE void MeVector4Clamp(MeVector4 a, const MeReal min, const MeReal max) {
    a[0] = a[0] < min ? min : (a[0] > max ? max : a[0]),
        a[1] = a[1] < min ? min : (a[1] > max ? max : a[1]),
        a[2] = a[2] < min ? min : (a[2] > max ? max : a[2]),
        a[3] = a[3] < min ? min : (a[3] > max ? max : a[3]);
}

/**
 * Scale all elements of @a v by @a a
*/
MeINLINE void MEAPI MeVector4Scale(MeVector3 v, const MeReal a) {
    v[0] *= a, v[1] *= a, v[2] *= a, v[3] *= a;
}

/**
 * Add elementwise
 */
MeINLINE void MEAPI MeVector4Add(MeVector4 a, const MeVector4 b, const MeVector4 c) {
    a[0] = b[0] + c[0], a[1] = b[1] + c[1], a[2] = b[2] + c[2], a[3] = b[3] + c[3];
}

/**
 * Return the square of the magnitude of the vector.
 */
MeINLINE MeReal MEAPI
MeVector4MagnitudeSqr(const MeVector4 v)
{
    MeReal    m = (MeReal) 0;

    m += MeSqr(v[0]);
    m += MeSqr(v[1]);
    m += MeSqr(v[2]);
    m += MeSqr(v[3]);

    return m;
}

/**
 * MeVector4 dot product.
 */
MeINLINE MeReal MEAPI
MeVector4Dot(const MeVector4 b, const MeVector4 c)
{
    return b[0] * c[0] + b[1] * c[1] + b[2] * c[2] + b[3] * c[3];
}



/**
 * Make a quaternion @a q given a normalised axis @a x and an angle @a a.
 */
MeINLINE void MEAPI
MeQuaternionMake(MeVector4 q, MeVector3 x, MeReal a)
{
    MeReal    half_a = a / 2;
    MeReal    s = MeSin(half_a);
    MeReal    c = MeCos(half_a);

    q[0] = c;
    q[1] = s * x[0];
    q[2] = s * x[1];
    q[3] = s * x[2];
}

/**
 * Compute a 3x3 rotation matrix @a R from a 4x1 normalized quaternion @a q.
 */
MeINLINE void MEAPI
MeQuaternionToR(MeMatrix3 R, const MeVector4 q)
{
    /* 13 multiplications + 12 local variables */
    MeReal    q0sq = q[0] * q[0];
    MeReal    q1sq = q[1] * q[1];
    MeReal    q2sq = q[2] * q[2];
    MeReal    q3sq = q[3] * q[3];
    MeReal    q0t2 = 2 * q[0];
    MeReal    q1t2 = 2 * q[1];
    MeReal    q0q1 = q0t2 * q[1];
    MeReal    q0q2 = q0t2 * q[2];
    MeReal    q0q3 = q0t2 * q[3];
    MeReal    q1q2 = q1t2 * q[2];
    MeReal    q1q3 = q1t2 * q[3];
    MeReal    q2q3 = 2 * q[2] * q[3];

    R[0][0] = q0sq + q1sq - q2sq - q3sq;
    R[1][0] = q1q2 - q0q3;
    R[2][0] = q0q2 + q1q3;

    R[0][1] = q1q2 + q0q3;
    R[1][1] = q0sq - q1sq + q2sq - q3sq;
    R[2][1] = -q0q1 + q2q3;

    R[0][2] = -q0q2 + q1q3;
    R[1][2] = q0q1 + q2q3;
    R[2][2] = q0sq - q1sq - q2sq + q3sq;

    /* 36 multiplications */
    /*
       R[0][0] = q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3];
       R[1][0] = 2.0f * q[1] * q[2] - 2.0f * q[0] * q[3];
       R[2][0] = 2.0f * q[0] * q[2] + 2.0f * q[1] * q[3];

       R[0][1] = 2.0f * q[1] * q[2] + 2.0f * q[0] * q[3];
       R[1][1] = q[0] * q[0] - q[1] * q[1] + q[2] * q[2] - q[3] * q[3];
       R[2][1] = -2.0f * q[0] * q[1] + 2.0f * q[2] * q[3];

       R[0][2] = -2.0f * q[0] * q[2] + 2.0f * q[1] * q[3];
       R[1][2] = 2.0f * q[0] * q[1] + 2.0f * q[2] * q[3];
       R[2][2] = q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3];
     */
}

/**
 * Convert a quaternion to a rotation matrix.
 *
 * \deprecated This function only sets the top left hand corner of the
 * matrix, and will be changed in a future release to properly generate a valid
 * transformation matrix. To Set a the rotation part of a transformation,
 * use MeMatrix4TMSetRotationFromQuaternion instead
 */

MeINLINE void MEAPI
MeQuaternionToTM(MeMatrix4 tm, const MeVector4 q)
{
    /* 13 multiplications + 12 local variables */
    MeReal    q0sq = q[0] * q[0];
    MeReal    q1sq = q[1] * q[1];
    MeReal    q2sq = q[2] * q[2];
    MeReal    q3sq = q[3] * q[3];
    MeReal    q0t2 = 2 * q[0];
    MeReal    q1t2 = 2 * q[1];
    MeReal    q0q1 = q0t2 * q[1];
    MeReal    q0q2 = q0t2 * q[2];
    MeReal    q0q3 = q0t2 * q[3];
    MeReal    q1q2 = q1t2 * q[2];
    MeReal    q1q3 = q1t2 * q[3];
    MeReal    q2q3 = 2 * q[2] * q[3];

    tm[0][0] = q0sq + q1sq - q2sq - q3sq;
    tm[1][0] = q1q2 - q0q3;
    tm[2][0] = q0q2 + q1q3;

    tm[0][1] = q1q2 + q0q3;
    tm[1][1] = q0sq - q1sq + q2sq - q3sq;
    tm[2][1] = -q0q1 + q2q3;

    tm[0][2] = -q0q2 + q1q3;
    tm[1][2] = q0q1 + q2q3;
    tm[2][2] = q0sq - q1sq - q2sq + q3sq;

    /* 36 multiplications */
    /*
       tm[0][0] = q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3];
       tm[1][0] = 2.0f * q[1] * q[2] - 2.0f * q[0] * q[3];
       tm[2][0] = 2.0f * q[0] * q[2] + 2.0f * q[1] * q[3];

       tm[0][1] = 2.0f * q[1] * q[2] + 2.0f * q[0] * q[3];
       tm[1][1] = q[0] * q[0] - q[1] * q[1] + q[2] * q[2] - q[3] * q[3];
       tm[2][1] = -2.0f * q[0] * q[1] + 2.0f * q[2] * q[3];

       tm[0][2] = -2.0f * q[0] * q[2] + 2.0f * q[1] * q[3];
       tm[1][2] = 2.0f * q[0] * q[1] + 2.0f * q[2] * q[3];
       tm[2][2] = q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3];
     */
}

/**
 * Set the rotation part of a transformation matrix from a quaternion.
 */

MeINLINE void MEAPI
MeMatrix4TMSetRotationFromQuaternion(MeMatrix4 tm, const MeVector4 q)
{
    /* 13 multiplications + 12 local variables */
    MeReal    q0sq = q[0] * q[0];
    MeReal    q1sq = q[1] * q[1];
    MeReal    q2sq = q[2] * q[2];
    MeReal    q3sq = q[3] * q[3];
    MeReal    q0t2 = 2 * q[0];
    MeReal    q1t2 = 2 * q[1];
    MeReal    q0q1 = q0t2 * q[1];
    MeReal    q0q2 = q0t2 * q[2];
    MeReal    q0q3 = q0t2 * q[3];
    MeReal    q1q2 = q1t2 * q[2];
    MeReal    q1q3 = q1t2 * q[3];
    MeReal    q2q3 = 2 * q[2] * q[3];

    tm[0][0] = q0sq + q1sq - q2sq - q3sq;
    tm[1][0] = q1q2 - q0q3;
    tm[2][0] = q0q2 + q1q3;

    tm[0][1] = q1q2 + q0q3;
    tm[1][1] = q0sq - q1sq + q2sq - q3sq;
    tm[2][1] = -q0q1 + q2q3;

    tm[0][2] = -q0q2 + q1q3;
    tm[1][2] = q0q1 + q2q3;
    tm[2][2] = q0sq - q1sq - q2sq + q3sq;
}

/**
 * Return a 4x3 matrix @a D corresponding to the 4x1 quaternion @a q
 * such that @a dq/dt = @a D * @a w, where @a w is the angular velocity.
 */
MeINLINE void MEAPI
MeQuaternionToD(MeReal D[12], const MeVector4 q)
{
    D[0] = -q[1] * (MeReal) (0.5);
    D[4] = -q[2] * (MeReal) (0.5);
    D[8] = -q[3] * (MeReal) (0.5);

    D[1] = +q[0] * (MeReal) (0.5);
    D[5] = +q[3] * (MeReal) (0.5);
    D[9] = -q[2] * (MeReal) (0.5);

    D[2] = -q[3] * (MeReal) (0.5);
    D[6] = +q[0] * (MeReal) (0.5);
    D[10] = +q[1] * (MeReal) (0.5);

    D[3] = +q[2] * (MeReal) (0.5);
    D[7] = -q[1] * (MeReal) (0.5);
    D[11] = +q[0] * (MeReal) (0.5);
}

/**
 * Given the angular velocity @a w and the orientation quaternion @a q,
 * return the quaternion derivative @a dq, such that @a dq/dt = @a D * @a w.
 */
MeINLINE void MEAPI
MeQuaternionWtoDQ(MeVector4 dq, const MeVector4 q, const MeVector3 w)
{
    dq[0] = (MeReal) (0.5) * (-q[1] * w[0] - q[2] * w[1] - q[3] * w[2]);
    dq[1] = (MeReal) (0.5) * (+q[0] * w[0] + q[3] * w[1] - q[2] * w[2]);
    dq[2] = (MeReal) (0.5) * (-q[3] * w[0] + q[0] * w[1] + q[1] * w[2]);
    dq[3] = (MeReal) (0.5) * (+q[2] * w[0] - q[1] * w[1] + q[0] * w[2]);
}

/**
 * Multiplication of quaternions: @a r = @a p * @a q
 */

MeINLINE void MEAPI
MeQuaternionProduct(MeVector4 r, const MeVector4 p, const MeVector4 q)
{
    r[0] = q[0] * p[0] - q[1] * p[1] - q[2] * p[2] - q[3] * p[3];

    r[1] = q[0] * p[1] + q[1] * p[0] + p[2] * q[3] - p[3] * q[2];
    r[2] = q[0] * p[2] + q[2] * p[0] + p[3] * q[1] - p[1] * q[3];
    r[3] = q[0] * p[3] + q[3] * p[0] + p[1] * q[2] - p[2] * q[1];
}


/**
 * Invert a quaternion by computing its conjugate.
 * Assumes @a q is a unit quaternion.
 */
MeINLINE void MEAPI MeQuaternionInvert(MeVector4 q)
{
    q[1] = -q[1];
    q[2] = -q[2];
    q[3] = -q[3];
}

/**
 * Copy conjugate of quaternion computing its conjugate: @a q = @a p*.
 */
MeINLINE void MEAPI MeQuaternionCopyConjugate(MeVector4 q, const MeVector4 p)
{
    q[0] = p[0]; q[1] = -p[1]; q[2] = -p[2]; q[3] = -p[3];
}

/**
 * Rotate a vector using a quaternion.
 *
 * <em>vout = (q0*q0 - qv.qv)v + 2(qv.v)qv + 2q0 (qv</em> @b x <em>v)</em>
 * where @b x is the cross product operator
 */
MeINLINE void MEAPI
MeQuaternionRotateVector3(MeVector3 vout, const MeVector4 q, const MeVector3 v)
{
    MeVector3 qv;

    qv[0] = q[1];
    qv[1] = q[2];
    qv[2] = q[3];

    MeVector3Cross(vout, qv, v);

    MeVector3Scale(vout, 2.0f * q[0]);

    MeVector3MultiplyAdd(vout, MeSqr(q[0]) - MeVector3MagnitudeSqr(qv), v);
    MeVector3MultiplyAdd(vout, 2.0f * MeVector3Dot(qv, v), qv);
}

/**
 * MeMatrix3 Copy
 */
MeINLINE void MEAPI
MeMatrix3Copy(MeMatrix3 A, const MeMatrix3 B)
{
  A[0][0] = B[0][0];
  A[0][1] = B[0][1];
  A[0][2] = B[0][2];
  A[1][0] = B[1][0];
  A[1][1] = B[1][1];
  A[1][2] = B[1][2]; 
  A[2][0] = B[2][0]; 
  A[2][1] = B[2][1]; 
  A[2][2] = B[2][2];
}

/**
 * Set the rows of an MeMatrix3 @a A from three row vectors.
 */
MeINLINE void MEAPI
MeMatrix3CopyVec(MeMatrix3 A, const MeVector3 B1, const MeVector3 B2, const MeVector3 B3)
{
    A[0][0] = B1[0],
    A[0][1] = B1[1],
    A[0][2] = B1[2],
    A[1][0] = B2[0],
    A[1][1] = B2[1], A[1][2] = B2[2], A[2][0] = B3[0], A[2][1] = B3[1], A[2][2] = B3[2];
}


/** Check if this 3x3 matrix is diagonal with identical values. */
MeINLINE MeBool MEAPI
MeMatrix3IsIsotropic(const MeMatrix3 i)
{
    if (!((i[0][0] == i[1][1]) && (i[1][1] == i[2][2])))
    return 0;

    if (!((i[0][1] == 0) && (i[0][2] == 0) && (i[1][2] == 0)))
    return 0;

    if (!((i[1][0] == 0) && (i[2][0] == 0) && (i[2][1] == 0)))
    return 0;

    return 1;
}

/**
 * Transpose of the @a n * @a m matrix @a B into @a A
 */
MeINLINE void MEAPI
MeMatrixTranspose(MeReal *A, const int n, const int m, const MeReal *const B)
{
    int       i, j;

    for (i = 0; i < n; i++)
    for (j = 0; j < m; j++)
        A[i + j * n] = B[i * m + j];
}

/**
 * Set @a A=B*C, where @a A has dimensions @a p*r, @a B has dimensions @a p*q, @a C has dimensions @a q*r.
 *
 * This is an inline function for use on SMALL matrices.
 */
MeINLINE void MEAPI
MeMatrixMultiply(MeReal *A, const int p, const int q, const int r, const MeReal *const B,
         const MeReal *const C)
{
    int       i, j, k;
    for (j = 0; j < r; j++)
        for (i = 0; i < p; i++) {
            const MeReal *c = C + j * q;
            const MeReal *b = B + i;
            MeReal    sum = 0;
            for (k = q; k > 0; k--) {
                sum += (*b) * (*c);
                b += p;
                c++;
            }
            *A++ = sum;
    }
}

/**
 * Set @a A to be the product of @a B transposed and @a C,
 * where @a A has dimensions @a p*r, @a B has dimensions @a q*p, @a C has dimensions @a q*r.
 * This is an inline function for use on SMALL matrices.
 */

MeINLINE void MEAPI
MeMatrixMultiplyT1(MeReal *A, const int p, const int q, const int r, const MeReal *const B,
           const MeReal *const C)
{
    int       i, j, k;
    for (j = 0; j < r; j++) {
        const MeReal *b = B;
        for (i = 0; i < p; i++) {
            const MeReal *c = C + j * q;
            MeReal    sum = 0;
            for (k = q; k > 0; --k)
                sum += (*b++) * (*c++);
            *A++ = sum;
        }
    }
}

/**
 * Return a 3x3 identity matrix.
 */
MeINLINE void MEAPI
MeMatrix3MakeIdentity(MeMatrix3 tm)
{
#if 0
    /*
       This won't work if the matrix is laid out in a funny way.
     */
    memset((void *) tm, 0, 9 * sizeof(MeReal));

    tm[0][0] = (MeReal) (1.0);
    tm[1][1] = (MeReal) (1.0);
    tm[2][2] = (MeReal) (1.0);
#else
    /*
       Why do we write this as a single expression with terms separated
       by the ',' operator? Well, we hope this suggests that all the
       assignments could be done in parallel.
     */
    tm[0][0] = (MeReal) (1.0),
    tm[0][1] = (MeReal) (0.0),
    tm[0][2] = (MeReal) (0.0),
    tm[1][0] = (MeReal) (0.0),
    tm[1][1] = (MeReal) (1.0),
    tm[1][2] = (MeReal) (0.0),
    tm[2][0] = (MeReal) (0.0), tm[2][1] = (MeReal) (0.0), tm[2][2] = (MeReal) (1.0);
#endif
}

/**
 * In place (via an internal copy) transpose of the 3x3 matrix @a A.
 */
MeINLINE void MEAPI
MeMatrix3Transpose(MeMatrix3 A)
{
    MeMatrix3 tmp;

    memcpy(tmp, A, sizeof(MeMatrix3));

    A[0][1] = tmp[1][0],
    A[0][2] = tmp[2][0],
    A[1][0] = tmp[0][1], A[1][2] = tmp[2][1], A[2][0] = tmp[0][2], A[2][1] = tmp[1][2];
}

/**
 * Set @a A to a 3x3 matrix corresponding to the 3x1 vector @a a, such
 * that @a A*b = @a a @b x @a b (@b x is the cross product operator).
 *
 * The result is scaled by @a factor.
 *
 * \deprecated This function will be changed in a future release to generate an MeMatrix3 and
 * remove the scaling factor.
 */
MeINLINE void MEAPI
MeMatrix3CrossFromVector(MeVector3 A0, MeVector3 A1, MeVector3 A2, const MeVector3 a,
             const MeReal factor)
{
    A0[0] = 0;
    A0[1] = a[2] * factor;
    A0[2] = -a[1] * factor;

    A1[0] = -a[2] * factor;
    A1[1] = 0;
    A1[2] = a[0] * factor;

    A2[0] = a[1] * factor;
    A2[1] = -a[0] * factor;
    A2[2] = 0;
}

/**
 * Postmultiply a 3x1 vector by 3x3 matrix.
 */
MeINLINE void MEAPI
MeMatrix3MultiplyVector(MeVector3 A, const MeMatrix3 B, const MeVector3 C)
{
    A[0] = B[0][0] * C[0] + B[1][0] * C[1] + B[2][0] * C[2];
    A[1] = B[0][1] * C[0] + B[1][1] * C[1] + B[2][1] * C[2];
    A[2] = B[0][2] * C[0] + B[1][2] * C[1] + B[2][2] * C[2];
}

/**
 * Subtract 3x3 matrices. @a A = @a B - @a C.
 */
MeINLINE void MEAPI
MeMatrix3Subtract(MeMatrix3 A, MeMatrix3 B, MeMatrix3 C)
{
    A[0][0] = B[0][0] - C[0][0];
    A[0][1] = B[0][1] - C[0][1];
    A[0][2] = B[0][2] - C[0][2];
    A[1][0] = B[1][0] - C[1][0];
    A[1][1] = B[1][1] - C[1][1];
    A[1][2] = B[1][2] - C[1][2];
    A[2][0] = B[2][0] - C[2][0];
    A[2][1] = B[2][1] - C[2][1];
    A[2][2] = B[2][2] - C[2][2];
}

/**
 * Add 3x3 matrices. @a A = @a B + @a C.
 */
MeINLINE void MEAPI
MeMatrix3Add(MeMatrix3 A, MeMatrix3 B, MeMatrix3 C)
{
    A[0][0] = B[0][0] + C[0][0];
    A[0][1] = B[0][1] + C[0][1];
    A[0][2] = B[0][2] + C[0][2];
    A[1][0] = B[1][0] + C[1][0];
    A[1][1] = B[1][1] + C[1][1];
    A[1][2] = B[1][2] + C[1][2];
    A[2][0] = B[2][0] + C[2][0];
    A[2][1] = B[2][1] + C[2][1];
    A[2][2] = B[2][2] + C[2][2];
}

/**
 * Multiply 3x3 matrices. @a A = @a B*C, where the matrices are stored in row major order.
 *
 * @internal
 * RJW - I believe the question of row major is irrelevant. 
 * What is meant is that Av= B(Cv) and not vA=(vB)C
 */
MeINLINE void MEAPI
MeMatrix3MultiplyMatrix(MeMatrix3 A, const MeMatrix3 B, const MeMatrix3 C)
{
    A[0][0] = B[0][0] * C[0][0] + B[0][1] * C[1][0] + B[0][2] * C[2][0];
    A[0][1] = B[0][0] * C[0][1] + B[0][1] * C[1][1] + B[0][2] * C[2][1];
    A[0][2] = B[0][0] * C[0][2] + B[0][1] * C[1][2] + B[0][2] * C[2][2];

    A[1][0] = B[1][0] * C[0][0] + B[1][1] * C[1][0] + B[1][2] * C[2][0];
    A[1][1] = B[1][0] * C[0][1] + B[1][1] * C[1][1] + B[1][2] * C[2][1];
    A[1][2] = B[1][0] * C[0][2] + B[1][1] * C[1][2] + B[1][2] * C[2][2];

    A[2][0] = B[2][0] * C[0][0] + B[2][1] * C[1][0] + B[2][2] * C[2][0];
    A[2][1] = B[2][0] * C[0][1] + B[2][1] * C[1][1] + B[2][2] * C[2][1];
    A[2][2] = B[2][0] * C[0][2] + B[2][1] * C[1][2] + B[2][2] * C[2][2];

}

/**
 * Multiply all elements of a 3x3 matrix by a scalar.
 */
MeINLINE void MEAPI
MeMatrix3Scale(MeMatrix3 A, const MeReal a)
{
    A[0][0] *= a;
    A[0][1] *= a;
    A[0][2] *= a;
    A[1][0] *= a;
    A[1][1] *= a;
    A[1][2] *= a;
    A[2][0] *= a;
    A[2][1] *= a;
    A[2][2] *= a;
}

/**
   Zero an MeMatrix4
*/
MeINLINE void MEAPI
MeMatrix4SetZero(MeMatrix4 m)
{
    m[0][0] = 0,
    m[0][1] = 0,
    m[0][2] = 0,
    m[0][3] = 0,
    m[1][0] = 0,
    m[1][1] = 0,
    m[1][2] = 0,
    m[1][3] = 0,
    m[2][0] = 0,
    m[2][1] = 0, m[2][2] = 0, m[2][3] = 0, m[3][0] = 0, m[3][1] = 0, m[3][2] = 0, m[3][3] = 0;
}

/**
 * Make a 4x4 identity matrix.
 */
MeINLINE void MEAPI
MeMatrix4TMMakeIdentity(MeMatrix4 tm)
{
#if 0
    /*
       This won't work if the matrix is laid out in a funny way.
     */
    memset((void *) tm, 0, 16 * sizeof(MeReal));

    tm[0][0] = (MeReal) (1.0);
    tm[1][1] = (MeReal) (1.0);
    tm[2][2] = (MeReal) (1.0);
    tm[3][3] = (MeReal) (1.0);
#else
    /*
       Why do we write this as a single expression with terms separated
       by the ',' operator? Well, we hope this suggests that all the
       assignments could be done in parallel.
     */
    tm[0][0] = (MeReal) (1.0),
    tm[0][1] = (MeReal) (0.0),
    tm[0][2] = (MeReal) (0.0),
    tm[0][3] = (MeReal) (0.0),
    tm[1][0] = (MeReal) (0.0),
    tm[1][1] = (MeReal) (1.0),
    tm[1][2] = (MeReal) (0.0),
    tm[1][3] = (MeReal) (0.0),
    tm[2][0] = (MeReal) (0.0),
    tm[2][1] = (MeReal) (0.0),
    tm[2][2] = (MeReal) (1.0),
    tm[2][3] = (MeReal) (0.0),
    tm[3][0] = (MeReal) (0.0),
    tm[3][1] = (MeReal) (0.0), tm[3][2] = (MeReal) (0.0), tm[3][3] = (MeReal) (1.0);
#endif
}

/**
 * Sets position for a MeMatrix4, when interpreted as a transformation
 * matrix.
 */
MeINLINE void MEAPI
MeMatrix4TMSetPosition(MeMatrix4 tm, MeReal x, MeReal y, MeReal z)
{
    tm[3][0] = x;
    tm[3][1] = y;
    tm[3][2] = z;
}

/**
 * Sets position for a MeMatrix4, when interpreted as a transformation
 * matrix. Alternative form taking an MeVector3 as the position.
 */
MeINLINE void MEAPI
MeMatrix4TMSetPositionVector(MeMatrix4 tm, MeVector3 position)
{
    tm[3][0] = position[0];
    tm[3][1] = position[1];
    tm[3][2] = position[2];
}


/**
 * Sets position for a MeMatrix4, when interpreted as a transformation
 * matrix. Alternative form taking an MeVector3 as the position.
 */
MeINLINE void MEAPI
MeMatrix4TMGetPosition(MeMatrix4 tm, MeVector3 position)
{
    position[0] = tm[3][0];
    position[1] = tm[3][1];
    position[2] = tm[3][2];
}

/** Updates only the rotation matrix part of the transformation. */
MeINLINE void MEAPI
MeMatrix4TMSetRotation(MeMatrix4 tm, const MeMatrix3 R)
{
    tm[0][0] = R[0][0];
    tm[0][1] = R[0][1];
    tm[0][2] = R[0][2];

    tm[1][0] = R[1][0];
    tm[1][1] = R[1][1];
    tm[1][2] = R[1][2];

    tm[2][0] = R[2][0];
    tm[2][1] = R[2][1];
    tm[2][2] = R[2][2];
}

/** Extract the rotation matrix part of transformation. */
MeINLINE void MEAPI
MeMatrix4TMGetRotation(MeMatrix3 R, const MeMatrix4 tm)
{
    R[0][0] = tm[0][0];
    R[0][1] = tm[0][1];
    R[0][2] = tm[0][2];

    R[1][0] = tm[1][0];
    R[1][1] = tm[1][1];
    R[1][2] = tm[1][2];

    R[2][0] = tm[2][0];
    R[2][1] = tm[2][1];
    R[2][2] = tm[2][2];
}

/**
 * MeMatrix4 Copy
 */
MeINLINE void MEAPI
MeMatrix4Copy(MeMatrix4 A, const MeMatrix4 B)
{
    A[0][0] = B[0][0],
    A[0][1] = B[0][1],
    A[0][2] = B[0][2],
    A[0][3] = B[0][3],
    A[1][0] = B[1][0],
    A[1][1] = B[1][1],
    A[1][2] = B[1][2],
    A[1][3] = B[1][3],
    A[2][0] = B[2][0],
    A[2][1] = B[2][1],
    A[2][2] = B[2][2],
    A[2][3] = B[2][3],
    A[3][0] = B[3][0],
    A[3][1] = B[3][1],
    A[3][2] = B[3][2],
    A[3][3] = B[3][3];
}

/**
    Copy an MeMatrix4. Input and output must be 16 Byte aligned.

    This currently works on platforms where alignment is not as important as
    PS2, even when the data is not aligned. This may change.
*/
MeINLINE void MEAPI
MeMatrix4CopyAligned(MeMatrix4 A, const MeMatrix4 B)
{
    MeMatrix4Copy(A, B);
}
/**
 * In place (via an internal copy) transpose of the 4x4 matrix @a A.
 */
MeINLINE void MEAPI
MeMatrix4Transpose(MeMatrix4 A) {
    MeMatrix4 tmp;
    memcpy(tmp, A, sizeof(MeMatrix4));
    A[0][1] = tmp[1][0],
    A[0][2] = tmp[2][0],
    A[0][3] = tmp[3][0],
    A[1][0] = tmp[0][1],
    A[1][2] = tmp[2][1],
    A[1][3] = tmp[3][1],
    A[2][0] = tmp[0][2],
    A[2][1] = tmp[1][2],
    A[2][3] = tmp[3][2],
    A[3][0] = tmp[0][3],
    A[3][1] = tmp[1][3],
    A[3][2] = tmp[2][3];
}

/**
 * MeMatrix4 Addition
 */
MeINLINE void MEAPI
MeMatrix4Add(MeMatrix4 A, const MeMatrix4 B, const MeMatrix4 C) {
    A[0][0] = B[0][0] + C[0][0],
    A[0][1] = B[0][1] + C[0][1],
    A[0][2] = B[0][2] + C[0][2],
    A[0][3] = B[0][3] + C[0][3],
    A[1][0] = B[1][0] + C[1][0],
    A[1][1] = B[1][1] + C[1][1],
    A[1][2] = B[1][2] + C[1][2],
    A[1][3] = B[1][3] + C[1][3],
    A[2][0] = B[2][0] + C[2][0],
    A[2][1] = B[2][1] + C[2][1],
    A[2][2] = B[2][2] + C[2][2],
    A[2][3] = B[2][3] + C[2][3],
    A[3][0] = B[3][0] + C[3][0],
    A[3][1] = B[3][1] + C[3][1],
    A[3][2] = B[3][2] + C[3][2],
    A[3][3] = B[3][3] + C[3][3];
}

/**
 * Postmultiply a 4x1 vector by a 4x4 matrix. If the matrix is a transformation with the
 * translation component in the last row, this operation applies the transformation to the
 * vector.
 */
MeINLINE void MEAPI
MeMatrix4MultiplyVector(MeVector4 A, const MeMatrix4 B, const MeVector4 C) {
#if 0
    MeMatrixMultiply((MeReal *) A, 4, 4, 1, (const MeReal *) B,
             (const MeReal *) C);
#else
    A[0] = B[0][0] * C[0] + B[1][0] * C[1] + B[2][0] * C[2] + B[3][0] * C[3],
    A[1] = B[0][1] * C[0] + B[1][1] * C[1] + B[2][1] * C[2] + B[3][1] * C[3],
    A[2] = B[0][2] * C[0] + B[1][2] * C[1] + B[2][2] * C[2] + B[3][2] * C[3],
    A[3] = B[0][3] * C[0] + B[1][3] * C[1] + B[2][3] * C[2] + B[3][3] * C[3];
#endif
}

/**
 * Postmultiply a 3x1 vector by a 4x4 matrix. If the matrix is a transformation with the
 * translation component in the last row and if "translate" is TRUE, this operation applies 
 * the transformation to the  vector.
 *
MeINLINE void MEAPI
MeMatrix4MultiplyVector3(MeVector3 A, const MeMatrix4 B, const MeVector3 C, MeBool translate) {
 
    A[0] = B[0][0] * C[0] + B[1][0] * C[1] + B[2][0] * C[2]; 
    A[1] = B[0][1] * C[0] + B[1][1] * C[1] + B[2][1] * C[2]; 
    A[2] = B[0][2] * C[0] + B[1][2] * C[1] + B[2][2] * C[2];
    if (translate) {
      A[0] += B[3][0];
      A[1] += B[3][1];
      A[2] += B[3][2];
    }
}
*/


/**
 * Postmultiply a 3x1 vector by the rotation part of a 4x4 transformation matrix. 
 * This operation applies the rotation to the vector, ignoring the translation.
 */
/* transformWithoutTranslate */
MeINLINE void MEAPI
MeMatrix4TMRotateVector3(MeVector3 A, const MeMatrix4 B, const MeVector3 C) {

    A[0] = B[0][0] * C[0] + B[1][0] * C[1] + B[2][0] * C[2]; 
    A[1] = B[0][1] * C[0] + B[1][1] * C[1] + B[2][1] * C[2]; 
    A[2] = B[0][2] * C[0] + B[1][2] * C[1] + B[2][2] * C[2];
}

/**
 * Postmultiply a 3x1 vector by a 4x4 transformation matrix. 
 * This operation applies the transformation to the vector.
 */
MeINLINE void MEAPI
MeMatrix4TMTransformVector3(MeVector3 A, const MeMatrix4 B, const MeVector3 C) {

    A[0] = B[0][0] * C[0] + B[1][0] * C[1] + B[2][0] * C[2]; 
    A[1] = B[0][1] * C[0] + B[1][1] * C[1] + B[2][1] * C[2]; 
    A[2] = B[0][2] * C[0] + B[1][2] * C[1] + B[2][2] * C[2];
    A[0] += B[3][0];
    A[1] += B[3][1];
    A[2] += B[3][2];
}


/**
 * Postmultiply a 3x1 vector by the inverse of the rotation part of a 4x4 transformation matrix. 
 * This operation applies the inverse rotation to the vector, ignoring the translation.
 * Equivalent to multiplying by the transpose of the rotation.
 */
/* inverseTransformWithoutTranslate */
MeINLINE void MEAPI
MeMatrix4TMInverseRotateVector3(MeVector3 A, const MeMatrix4 B, const MeVector3 C) {

    A[0] = B[0][0] * C[0] + B[0][1] * C[1] + B[0][2] * C[2]; 
    A[1] = B[1][0] * C[0] + B[1][1] * C[1] + B[1][2] * C[2]; 
    A[2] = B[2][0] * C[0] + B[2][1] * C[1] + B[2][2] * C[2];
}

/**
 * Postmultiply a 3x1 vector by the inverse of a 4x4 transformation matrix. 
 * This operation applies the transformation to the vector.
 * Equivalent to multiplying by the transpose of the matrix.
 */
MeINLINE void MEAPI
MeMatrix4TMInverseTransformVector3(MeVector3 A, const MeMatrix4 B, const MeVector3 C) {
    MeVector3 D;
    MeVector3Subtract(D,C,B[3]);

    A[0] = B[0][0] * D[0] + B[0][1] * D[1] + B[0][2] * D[2]; 
    A[1] = B[1][0] * D[0] + B[1][1] * D[1] + B[1][2] * D[2]; 
    A[2] = B[2][0] * D[0] + B[2][1] * D[1] + B[2][2] * D[2];
}



/**
 * Multiply 4x4 matrices. The computation is @a A=B*C, where @a A, @a B, and @a C are
 * stored row major. If this operation is used to compose two transformation
 * matrices, the compound transformation is the result of applying @a B, then @a C.
 */

MeINLINE void MEAPI
MeMatrix4MultiplyMatrix(MeMatrix4 A, const MeMatrix4 B, const MeMatrix4 C) {
    MeReal *a = (MeReal *) A;
    int i, j;
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
            *a++= B[i][0]*C[0][j] + B[i][1]*C[1][j] + B[i][2]*C[2][j] + B[i][3]*C[3][j];
    }
}



/**
 * Fill in a 4x4 matrix given a 3x3 rotation and a position.
 */
MeINLINE void MEAPI
MeMatrix4TMMakeFromRotationAndPosition(MeMatrix4 A,
                       const MeMatrix3 R,
                       const MeReal x,
                       const MeReal y, const MeReal z) {
    A[0][0] = R[0][0];
    A[0][1] = R[0][1];
    A[0][2] = R[0][2];
    A[0][3] = 0;
    A[1][0] = R[1][0];
    A[1][1] = R[1][1];
    A[1][2] = R[1][2];
    A[1][3] = 0;
    A[2][0] = R[2][0];
    A[2][1] = R[2][1];
    A[2][2] = R[2][2];
    A[2][3] = 0; A[3][0] = x; A[3][1] = y; A[3][2] = z; A[3][3] = 1;
}

/**
 * Determines whether a 4x4 matrix has any non-zero elements
 */
MeINLINE MeBool MEAPI
MeMatrix4IsZero(const MeMatrix4 m) {
    int i, j; for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
        if (m[i][j])
        return 0;}
    }
    return 1;
}

/* Some useful indexing functions */

/**
 * Provides an index into the triangle ABOVE AND INCLUDING
 * the diagonal in a matrix (row <= col).
 */
MeINLINE int MEAPI
MeUpperDiagonalIndex(const int inRow, const int inCol, const int inNCols)
{
    return inNCols * inRow + inCol - (((inRow + 1) * inRow) >> 1);
}

/**
 * Gives the number of elements ABOVE AND INCLUDING the diagonal in a matrix.
 */
MeINLINE int MEAPI
MeUpperDiagonalSize(const int inNCols) {
    return ((inNCols + 1) * inNCols) >> 1;
}

/**
 * Given any row and column, indexes into array (swaps if row > column).
 * Works on elements that are ABOVE OR INCLUDING the diagonal in a matrix.
 */
MeINLINE int MEAPI
MeSymUpperDiagonalIndex(const int inRow, const int inCol, const int inNCols) {
    MeI32 o = (inCol - inRow) >> 31; MeI32 d = ~o;
    MeI32 rowIndex = (d & inRow) | (o & inCol);
    MeI32 colIndex = (o & inRow) | (d & inCol);
    return MeUpperDiagonalIndex(rowIndex, colIndex, inNCols);
}

/**
 * Provides an index into the triangle ABOVE the diagonal in a matrix (r < c).
 */
MeINLINE int MEAPI
MeSuperDiagonalIndex(const int inRow, const int inCol, const int inNCols)
{
    return inNCols * inRow + inCol - (((inRow + 2) * (inRow + 1)) >> 1);
}

/**
 * Gives the number of elements ABOVE the diagonal in a matrix.
 */
MeINLINE int MEAPI
MeSuperDiagonalSize(const int inNCols) {
    return (inNCols * (inNCols - 1)) >> 1;
}

/**
 * Given any row and column (row != column), indexes into array
 * (swaps if row > column). Works on elements that are ABOVE the
 * diagonal in a matrix.
 */
MeINLINE int MEAPI
MeSymSuperDiagonalIndex(const int inRow, const int inCol, const int inNCols) {
    int o =   (inCol - inRow) >> 31; int d = ~o;
    int rowIndex = (d & inRow) | (o & inCol);
    int colIndex = (o & inRow) | (d & inCol);
    return MeSuperDiagonalIndex(rowIndex, colIndex, inNCols);
}

/**
 * Tests an array of MeReals of length @a elements for zero.
 * Returns 1 if all elements were zero, 0 if any are found to be non zero.
 */
MeINLINE MeBool MEAPI
MeRealArrayIsZero(MeReal *v, int elements) {
    int i; for (i = 0; i < elements; i++) {
    if (v[i]) return 0;}
    return 1;
}

/**
 * Generates a random number between two MeReals, @a start and @a end.
 */
MeINLINE MeReal MEAPI
MeRealRandomInRange(MeReal start, MeReal end) {
    return start + ((end - start) * rand()) / (MeReal) RAND_MAX;
}

#endif              /* MeDEFINE */

#endif              /* _MEMATH_H */
