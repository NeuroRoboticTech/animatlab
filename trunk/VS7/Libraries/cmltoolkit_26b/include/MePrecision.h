#ifndef _MEPRECISION_H
#define _MEPRECISION_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/09/27 21:31:02 $
   
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
 * Precision and platform dependent globals
 */

#if defined(CML_WITH_INTEL_MATH) && defined(__INTEL_COMPILER) 
#include <mathimf.h>
#else
#include <math.h>
#endif

#include <float.h>
#include <limits.h>
#include "MeCall.h"

/*
  This header is intended to manage different precisions on different
  platforms, for different products.

  Simply comment in or out the following lines to switch between
  precisions. This only has an effect for platforms on which you get a
  choice, namely Windows and Linux. The default is single precision for
  PlayStation2, Windows and Linux, and double precision for Irix.

  Note that changing the precision used for compilation requires that
  you link with an appropriate set of Toolkit libraries that use the
  specified precision as well. This will require that you receive and
  install the alternate precision set of libraries from Critical Mass.
  It will also require that you link against this alternate set of
  libraries. This may require that you change your library path
  manually in a Visual Studio project file (Visual Studio users) or
  that you change appropriate environment settings (for command-line
  builds).

*/

#if  ! defined _ME_API_DOUBLE    && ! defined _ME_API_SINGLE
#if defined IRIX 
#define _ME_API_DOUBLE
#else
#define _ME_API_SINGLE
#endif
#endif

/*
  The following precision, or floating point error tolerance, values are
  expected to depend on the floating-point accuracy (float, double etc.)
  being used, and not specifically on the platform.
*/
#ifdef _ME_API_DOUBLE
#   define ME_MIN_EPSILON       (2.8e-17)
#   define ME_SMALL_EPSILON     (1.0e-14)
#   define ME_MEDIUM_EPSILON    (1.0e-7)
#else /* _ME_API_SINGLE */
    /** Maximum achievable fractional accuracy. */
#   define ME_MIN_EPSILON       (1.2e-7f)
    /** "High" fractional accuracy. */
#   define ME_SMALL_EPSILON     (1.0e-6f)
    /** "Moderate" fractional accuracy. */
#   define ME_MEDIUM_EPSILON    (1.0e-3f)
#endif



/*
  Some macros which test whether floating-point numbers are "nearly"
  equal or "close" to zero:
*/
#define ME_IS_ZERO_TOL(x, e)    (MeFabs((x)) < (e))
#define ME_IS_ZERO(x)           (MeFabs( (x) ) < ME_SMALL_EPSILON)
#define ME_ARE_EQUAL_TOL(x,y,e) (ME_IS_ZERO_TOL(y,e) ? ME_IS_ZERO_TOL(x,e) \
                                  : MeFabs(((x)-(y))/(y)) < (e))
#define ME_ARE_EQUAL( x, y )    ME_ARE_EQUAL_TOL(x, y, ME_SMALL_EPSILON)
/*
  Some extensions to the above macros to handle the comparison of small vectors:
 */
#define ME_IS_ZERO_3VEC_TOL( x, e )  (ME_IS_ZERO_TOL( x[0], e ) && \
                                      ME_IS_ZERO_TOL( x[1], e ) && \
                                      ME_IS_ZERO_TOL( x[2], e ))
#define ME_IS_ZERO_4VEC_TOL( x, e )  (ME_IS_ZERO_3VEC_TOL( x, e ) && \
                                      ME_IS_ZERO_TOL( x[3], e ))
#define ME_ARE_EQUAL_3VEC_TOL( x, y, e ) \
                                   (ME_ARE_EQUAL_TOL( x[0], y[0], e ) && \
                                    ME_ARE_EQUAL_TOL( x[1], y[1], e ) && \
                                    ME_ARE_EQUAL_TOL( x[2], y[2], e ))
#define ME_ARE_EQUAL_4VEC_TOL( x, y, e ) \
                                   (ME_ARE_EQUAL_3VEC_TOL( x, y, e ) && \
                                    ME_ARE_EQUAL_TOL( x[3], y[3], e ))
#define ME_IS_ZERO_3VEC( x )        ME_IS_ZERO_3VEC_TOL( x, ME_SMALL_EPSILON )
#define ME_IS_ZERO_4VEC( x )        ME_IS_ZERO_4VEC_TOL( x, ME_SMALL_EPSILON )
#define ME_ARE_EQUAL_3VEC( x, y )   ME_ARE_EQUAL_3VEC_TOL( x, y, ME_SMALL_EPSILON )
#define ME_ARE_EQUAL_4VEC( x, y )   ME_ARE_EQUAL_4VEC_TOL( x, y, ME_SMALL_EPSILON )

/*
  Explicit boolean type.

  0 means false,
  anything else means true.
*/

typedef int MeBool;

#define MePtrSizeInt size_t

#define MEFALSE (0)

/*
  Unsigned integral type the same size as a pointer, so we can do bitwise
  operations on it.
  NB stdint doesn't seem to exist on Windows, so we'll have to do something
  a bit more careful for WIN64. Bug #1836.
*/
#if defined (PS2)
#include <eetypes.h>
#define MeUintPtr u_int
#elif defined (WIN32)
#define MeUintPtr MeU32
#else
#if defined (LINUX) || defined (LINUX_ICC) || defined (LINUX_IA64) || defined (LINUX_IA64_ECC) || defined(LINUX_IA64_GCC3)
#include <stdint.h>
#elif defined (IRIX)
#include <inttypes.h>
#endif
#define MeUintPtr uintptr_t
#endif

#if defined PS2
/*
  PlayStation 2
*/

/** Vortex floating point number */
typedef float                           MeReal;

#define MEINFINITY                      ((MeReal) 0x7fffffff)

/*
  ACME renderer/OpenGL specific
*/
typedef float   AcmeReal;

#define APPEND_F_OR_D(function)         function##f
#define APPEND_FV_OR_DV(function)       function##fv

/*
  Integer types of explicit length
*/

typedef u_char          MeU8;
typedef u_short         MeU16;
typedef u_int           MeU32;
typedef u_long          MeU64;
typedef u_long128       MeU128;

typedef char            MeI8;
typedef short           MeI16;
typedef int             MeI32;
typedef long            MeI64;
typedef long128         MeI128;

#elif defined WIN32
/*
  Windows
*/

/*
  On windows we may want to choose the precision.  The default is single
  precision.
*/

#ifdef  _ME_API_DOUBLE

/** Vortex floating point number */
typedef double                          MeReal;

#define MEINFINITY                      (MeReal)DBL_MAX

/*
  ACME renderer/OpenGL specific
*/
typedef double                          AcmeReal;

#define APPEND_F_OR_D(function)         function##d
#define APPEND_FV_OR_DV(function)       function##dv

#else

/** Vortex floating point number */
typedef float                           MeReal;

#define MEINFINITY                      (MeReal)FLT_MAX

/*
  ACME renderer/OpenGL specific
*/
typedef float                           AcmeReal;

#define APPEND_F_OR_D(function)         function##f
#define APPEND_FV_OR_DV(function)       function##fv

#endif

typedef unsigned __int8         MeU8;
typedef unsigned __int16        MeU16;
typedef unsigned __int32        MeU32;
typedef unsigned __int64        MeU64;
typedef struct {
    unsigned __int32 v[4];
}                               MeU128;

typedef __int8                  MeI8;
typedef __int16                 MeI16;
typedef __int32                 MeI32;
typedef __int64                 MeI64;
typedef struct {
    __int32 v[4];
}                               MeI128;
#elif defined LINUX
/*
  LINUX
*/

/*
  On Linux we may want to choose the precision. The default is single
  precision
*/

#include <sys/types.h>

#ifdef _ME_API_DOUBLE

/** Vortex floating point number */
typedef double                          MeReal;

#define MEINFINITY                      (MeReal)DBL_MAX

/*
  ACME renderer/OpenGL specific
*/
typedef double                          AcmeReal;

#define APPEND_F_OR_D(function)         function##d
#define APPEND_FV_OR_DV(function)       function##dv

#else /* _ME_API_DOUBLE */
/** Vortex floating point number */
typedef float                           MeReal;

#define MEINFINITY                      (MeReal)FLT_MAX

/*
  ACME renderer/OpenGL specific
*/
typedef float                           AcmeReal;

#define APPEND_F_OR_D(function)         function##f
#define APPEND_FV_OR_DV(function)       function##fv

#endif /* _ME_API_DOUBLE */

/*
  Integer types of explicit length
*/

typedef u_int8_t                MeU8;
typedef u_int16_t               MeU16;
typedef u_int32_t               MeU32;
#ifdef __GNUC__
typedef u_int64_t               MeU64;
#else
typedef u_int64_t               MeU64;
#endif /* __GNUC__ */
typedef struct {
    u_int32_t v[4];
}                               MeU128;

typedef int8_t                  MeI8;
typedef int16_t                 MeI16;
typedef int32_t                 MeI32;
#ifdef __GNUC__
typedef int64_t                 MeI64;
#else
typedef struct {
    int32_t v[2];
}                               MeI64;
#endif /* __GNUC__ */
typedef struct {
    int32_t v[4];
}                               MeI128;

#elif defined IRIX
/*
  IRIX
*/

/* IRIX is always double precision */

/* we need some way of telling */
#ifndef _ME_API_DOUBLE
#   define _ME_API_DOUBLE
#endif

#include <sys/types.h>

/** Vortex floating point number */
typedef double                          MeReal;

#define MEINFINITY                      (MeReal)DBL_MAX
/*
  For ACME renderer/OpenGL.
*/
typedef double  AcmeReal;

#define APPEND_F_OR_D(function)         function##d
#define APPEND_FV_OR_DV(function)       function##dv

/*
  Integer types of explicit length
*/

typedef uint8_t         MeU8;
typedef uint16_t        MeU16;
typedef uint32_t        MeU32;
typedef uint64_t        MeU64;
typedef struct {
    uint32_t v[4];
}                       MeU128;

typedef int8_t          MeI8;
typedef int16_t         MeI16;
typedef int32_t         MeI32;
typedef int64_t         MeI64;
typedef struct {
    int32_t v[4];
}                       MeI128;

#ifdef IRIX_O32
#   define true         1
#   define false        0
    typedef unsigned    bool;
#endif

#elif defined ELATE /* endid IRIX */
/*
  ELATE
*/

/*
  On Elate we may want to choose the precision. The default is single
  precision
*/

#include <sys/types.h>

#ifdef _ME_API_DOUBLE

/** Vortex floating point number */
typedef double                          MeReal;

#define MEINFINITY                      (MeReal)DBL_MAX

/*
  ACME renderer/OpenGL specific
*/
typedef double                          AcmeReal;

#define APPEND_F_OR_D(function)         function##d
#define APPEND_FV_OR_DV(function)       function##dv

#else /* _ME_API_DOUBLE */
/** Vortex floating point number */
typedef float                           MeReal;

#define MEINFINITY                      (MeReal)FLT_MAX

/*
  ACME renderer/OpenGL specific
*/
typedef float                           AcmeReal;

#define APPEND_F_OR_D(function)         function##f
#define APPEND_FV_OR_DV(function)       function##fv

#endif /* _ME_API_DOUBLE */

/*
  Integer types of explicit length
*/

typedef char            MeI8;
typedef short           MeI16;
typedef int             MeI32;
typedef long            MeI64;

typedef unsigned char            MeU8;
typedef unsigned short           MeU16;
typedef unsigned int             MeU32;
typedef unsigned long            MeU64;

typedef struct { MeI64 v[2]; }   MeU128;
typedef struct { MeU64 v[4]; }   MeI128;

#endif /* defined ELATE */

/*
  Vector and matrix typedefs
*/

/** Vortex 3 long MeReal vector. */
typedef MeReal          MeVector3[3];
typedef MeReal*         MeVector3Ptr;

/** Vortex 3 long AcmeReal vector */
typedef AcmeReal        AcmeVector3[3];

/** Vortex 4 long MeReal vector */
typedef MeReal          MeVector4[4];
typedef MeReal*         MeVector4Ptr;

/** Vortex 3x3 MeReal matrix */
typedef MeVector3       MeMatrix3[3];
typedef MeVector3*      MeMatrix3Ptr;

/** Vortex 4x4 MeReal matrix */
typedef MeVector4       MeMatrix4[4];
typedef MeVector4*      MeMatrix4Ptr;

/*
  Generic maths functions and defines
*/

#define MeSqr(x)        ((x)*(x))

#ifdef _USE_INTEL_COMPILER
#   define MeSin        MeSinFn
#   define MeAsin       MeAsinFn
#   define MeCos        MeCosFn
#   define MeAcos       MeAcosFn
#   define MeTan        MeTanFn
#   define MeAtan       MeAtanFn
#   define MeLog        MeLogFn
#   define MeExp        MeExpFn
#   define MeAtan2      MeAtan2Fn
#   define MePow        MePowFn
#endif

#if defined PS2
#   define MeSqrt       MEEEsqrt
#   define MeSin        sinf
#   define MeAsin       asinf
#   define MeCos        cosf
#   define MeAcos       acosf
#   define MeTan        tanf
#   define MeAtan       atanf
#   define MeLog        ((MeReal)log((MeReal)(x)))
#   define MeAtan2      atan2f
#   define MePow        powf
#   define MeFabs       MEEEfabs
#   define MeExp(x)     ((MeReal)exp((MeReal)(x)))
#   define MeRecip(x)   ((MeReal)(1.0)/(x))
#   define MeRecipSqrt  MEEErsqrt
#   define ME_PI        (3.14159265358979323846f)

   /* These definitions are valid on any MIPS IV compliant
      architecture. */

    static inline float MEEEsqrt(float x)
    {
        float rc;
        __asm__ __volatile__("    sqrt.s    %0,%1"
            : "=f" (rc) : "f" (x) );
        return rc;
    }

    static inline float MEEErsqrt(float x)
    {
        float rc;
        __asm__ __volatile__("    rsqrt.s    %0,%1,%2"
            : "=f"(rc) : "f"(1.0f),"f"(x));
        return rc;
    }

    static inline float MEEEfabs(float n)
    {
        float rc;
        __asm__ __volatile__("    abs.s    %0,%1"
            : "=f"(rc) : "f"(n));
        return rc;
    }

#elif defined _ME_API_DOUBLE
#   define MeSqrt       sqrt
#ifndef _USE_INTEL_COMPILER
#   define MeSin        sin
#   define MeAsin       asin
#   define MeCos        cos
#   define MeAcos       acos
#   define MeTan        tan
#   define MeAtan       atan
#   define MeLog        log
#   define MeExp        exp
#   define MeAtan2      atan2
#   define MePow        pow
#endif
#   define MeFabs       fabs
#   define MeRecip(x)   (1.0/(x))
#   define MeRecipSqrt(x) (1.0/sqrt(x))
#   define ME_PI        (3.14159265358979323846)
#elif (defined _MSC_VER && ! defined __cplusplus) || defined ELATE
/*
  The MS Compiler does not really have a single precision math library,
  and nor does Elate, so we might as well just punt to double precision
  functions.
*/
#   define MeSqrt(x)    (MeReal)(sqrt((double) (x)))
#ifndef _USE_INTEL_COMPILER
#   define MeSin(x)     (MeReal)(sin((double) (x)))
#   define MeAsin(x)    (MeReal)(asin((double) (x)))
#   define MeCos(x)     (MeReal)(cos((double) (x)))
#   define MeAcos(x)    (MeReal)(acos((double) (x)))
#   define MeTan(x)     (MeReal)(tan((double) (x)))
#   define MeAtan(x)    (MeReal)(atan((double) (x)))
#   define MeLog(x)     (MeReal)(log((double) (x)))
#   define MeExp(x)     (MeReal)(exp((MeReal)(x)))
#   define MeAtan2(x,y) (MeReal)(atan2((double) (x),(double) (y)))
#   define MePow(x,y)   (MeReal)(pow((double) (x),(double) (y)))
#endif
#   define MeFabs(x)    (MeReal)(fabs((double) (x)))
#   define MeRecip(x)   (MeReal)(1.0f/(x))
#   define MeRecipSqrt(x) (MeReal)(1.0f/sqrt((double)(x)))
#   define ME_PI        (MeReal)(3.14159265358979323846f)
#else
#   define MeSqrt       sqrtf
#ifndef _USE_INTEL_COMPILER
#   define MeSin        sinf
#   define MeAsin       asinf
#   define MeCos        cosf
#   define MeAcos       acosf
#   define MeTan        tanf
#   define MeAtan       atanf
#   define MeLog(x)     ((MeReal)log((MeReal)(x)))
#   define MeExp(x)     (MeReal)exp((MeReal)(x))
#   define MeAtan2      atan2f
#   define MePow        powf
#endif
#   define MeFabs       fabsf
#   define MeRecip(x)   (1.0f/(x))
#   define MeRecipSqrt(x) (1.0f/sqrtf(x))
#   define ME_PI        (3.14159265358979323846f)
#endif

#ifdef _ME_API_DOUBLE
#ifndef _BUILD_VANILLA
#   define _BUILD_VANILLA
#endif
#endif

#ifdef __cplusplus
extern    "C" {
#endif

MeReal MEAPI MeCosFn(MeReal);
MeReal MEAPI MeSinFn(MeReal);
MeReal MEAPI MeAsinFn(MeReal);
MeReal MEAPI MeAcosFn(MeReal);
MeReal MEAPI MeTanFn(MeReal);
MeReal MEAPI MeAtanFn(MeReal);
MeReal MEAPI MeLogFn(MeReal);
MeReal MEAPI MeExpFn(MeReal);
MeReal MEAPI MeAtan2Fn(MeReal, MeReal);
MeReal MEAPI MePowFn(MeReal, MeReal);

#ifdef __cplusplus
}
#endif

#endif /* _MEPRECISION_H */
