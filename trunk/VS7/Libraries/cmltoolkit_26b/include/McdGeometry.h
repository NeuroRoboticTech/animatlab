#ifndef _MCDGEOMETRY_H
#define _MCDGEOMETRY_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/05/02 10:10:09 $
   
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
   @file McdGeometry.h

An abstract base type for concrete geometry types.
All concrete geometrical
types share a common set of functionality defined by the McdGeometry
interface.

The McdGeometry protocol is a set of interface functions that are
implemented individually by each concrete geometry type. All functions taking a
McdGeometryID tag as argument can be passed a tag representing any concrete geometry
type that has been registered with the system. The McdGeometry protocol consists of the following functions:

McdGeometryDestroy(), McdGeometryGetAABB(), McdGeometryGetXYAABB(), , McdGeometryGetBSphere()
McdGeometryMaximumPoint(), McdGeometryGetMassProperties()
McdGeometryGetTypeName()

McdModel objects, the principle objects in the Mcd system, use a
McdGeometry object to define their local geometrical properties, and may share the same geometry with other McdModel objects.

*/

#include "MePrecision.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------
 *  McdGeometry
 *----------------------------------------------------------------
 */


  /** Base class for geometries. All geometries must include this struct as their first member */
typedef struct McdGeometry
{
  /** @internal */
  MeI16 mID;
  /** @internal */
  MeReal mBounds[6];

} McdGeometry ;

  /** @internal */
typedef McdGeometry* McdGeometryID;


/** The enumeration of geometry types supported by Mcd module. */
typedef enum
{
    kMcdGeometryPlane,
    kMcdGeometrySphere,
    kMcdGeometryBox,
    kMcdGeometryCylinder,
    kMcdGeometryCone,
    kMcdGeometryTriangleList,
    kMcdGeometryCapsule,
    kMcdGeometryThickDisk,
    kMcdGeometryRGHeightField,
    kMcdGeometryConvex,
    kMcdGeometryTriangleMesh,
    kMcdGeometryComposite,
    kMcdGeometryParticleSystem,
    kMcdGeometryTypeIdCount /* must be last */
}
McdGeometryTypeId;




  /** @internal */
void              MEAPI McdGeometryInit( int geoTypeMaxCount );
  /** @internal */
void              MEAPI McdGeometryTerm();
MeI16             MEAPI McdGeometryGetRegisteredTypeCount();
MeI16             MEAPI McdGeometryGetRegisteredTypeMaxId();
MeBool            MEAPI McdGeometryTypeIsRegistered( int typeId );

  /** return the geometry Id for @a g.
   Each concrete geometry type has a distinct geometry id
   ( eg. McdSphereGetTypeId(), McdConvexMeshGetTypeId() ) Each McdGeometry
  object holds an id value in order to identify its actual concrete type.
  */
#define McdGeometryGetTypeId(g) \
 ((g)->mID)

/* polymorphic functions */

void              MEAPI McdGeometryDestroy(McdGeometryID);

  /* AABB when oriented according to transform */
void              MEAPI McdGeometryGetAABB(McdGeometryID g, MeMatrix4 tm,
                            MeVector3 minCorner, MeVector3 maxCorner );

void              MEAPI McdGeometryGetXYAABB(McdGeometryID g, MeMatrix4 tm,
                            MeReal bounds[4] );

  /* BSphere in local geometry coordinate system */

void              MEAPI McdGeometryGetBSphere(McdGeometryID g,
                            MeVector3 center, MeReal *radius );
void              MEAPI McdGeometryMaximumPoint(McdGeometryID, MeMatrix4,
                            MeReal * const inDir, MeReal * const outPoint);
MeI16             MEAPI McdGeometryGetMassProperties( McdGeometryID g,
                            MeMatrix4 relativeTransform,
                            MeMatrix3 inertiaMatrix, MeReal *volume );
char*             MEAPI McdGeometryGetTypeName( McdGeometryID );
MeBool            MEAPI McdGeometryTypeIdIsValid( int id );


#ifdef MCDCHECK

char*             MEAPI McdGeometryGetTypeNameFromId( int typeId );
void              MEAPI McdGeometryShowTypes();
MeBool            MEAPI McdGeometryIsValid( McdGeometryID );

#endif

#ifdef MCDCHECK
#define MCDGEOMETRY_CHECK_ISVALID( g )\
McdGeometry_Check_IsValid( g );
#else
#define MCDGEOMETRY_CHECK_ISVALID( g )
#endif

/** Returns the number of geometries available in the entire collision system */
#define McdGeometryGetTotalTypeCount() (McdPrimitivesGetTypeCount()+5)




/*----------------------------------------------------------------
 * Plug-in mechanism for user-defined concrete geometry types.
 *----------------------------------------------------------------
 *
 * The macros MCD_DECLARE_GEOMETRY_TYPE and MCD_DEFINE_GEOMETRY_TYPE
 * define and implement a common set of functionality
 * for all concrete geometry types T
 *
 *----------------------------------------------------------------

 * The following are declared and implemented:

 McdTypeId TGetTypeId();

 void TRegisterType();

 * The following are declared only.
 * type-specific behaviour is defined by
 * implementing these function declarations
 * for each concrete type:

   void TDestroy( McdGeometryID );
   void TGetAABB( McdGeometryID, MeMatrix4,
                  MeVector3 minCorner, MeVector3 maxCorner );
   void TGetXYAABB( McdGeometryID, MeMatrix4,
                  MeReal bounds[4] );
   void TGetBSphere( McdGeometryID, MeVector3 center, MeReal *radius );
   void TMaximumPoint( McdGeometryID, MeMatrix4,
                       MeReal * const inDir,
                       MeReal * const outPoint);
   void TGetMassProperties( McdGeometryID, MeMatrix4, MeMatrix3, MeReal* );


 *
 *----------------------------------------------------------------
*/


/* declare common set of "C" functionality shared by
   all concrete geometry types */

#define MCD_DECLARE_GEOMETRY_TYPE( T )\
\
\
MeI16             MEAPI T##GetTypeId();\
extern MeI16 g##T##TypeId;\
\
void              MEAPI T##RegisterType();\
\
void              MEAPI T##Destroy( McdGeometryID);\
void              MEAPI T##GetAABB(McdGeometry*, MeMatrix4 transform,\
                            MeVector3 minCorner, MeVector3 maxCorner );\
void              MEAPI T##GetXYAABB(McdGeometry*, MeMatrix4 transform,\
                            MeReal bounds[4] );\
void              MEAPI T##GetBSphere(McdGeometryID,\
                            MeVector3 center, MeReal *radius );\
void              MEAPI T##MaximumPoint( McdGeometryID, MeMatrix4, \
                            MeReal * const inDir, MeReal * const outPoint);\
MeI16             MEAPI T##GetMassProperties( McdGeometryID, MeMatrix4, \
                            MeMatrix3, MeReal* );\
char*             MEAPI T##GetTypeName( McdGeometryID )


/* implement common set of "C" functionality shared by
   all concrete geometry types */

#define MCD_IMPLEMENT_GEOMETRY_TYPE( T, typeEnum, T_STRING )\
\
MeI16 g##T##TypeId = -1;\
char *g##T##TypeName = T_STRING;\
\
MeI16             MEAPI T##GetTypeId(){ return g##T##TypeId;}\
\
void              MEAPI T##RegisterType()\
{\
g##T##TypeId = \
 McdGeometryRegisterType( typeEnum, g##T##TypeName, T##Destroy, \
 T##GetAABB, T##GetXYAABB, T##GetBSphere, T##MaximumPoint,\
 T##GetMassProperties );\
}\


/* declarations and functions needed by plug-in macros */

typedef
void (MEAPI *McdGeometryDestroyFn)(McdGeometryID);

typedef
void (MEAPI *McdGeometryGetAABBFn)(McdGeometryID, MeMatrix4,
                 MeVector3 minCorner, MeVector3 maxCorner );

typedef
void (MEAPI *McdGeometryGetXYAABBFn)(McdGeometryID, MeMatrix4, MeReal bounds[4] );

typedef
void (MEAPI *McdGeometryGetBSphereFn)(McdGeometryID,
                MeVector3 center, MeReal *radius );

typedef
void (MEAPI *McdGeometryMaximumPointFn)(McdGeometryID, MeMatrix4,
                 MeReal * const inDir,
                 MeReal * const outPoint);
typedef
MeI16 (MEAPI *McdGeometryGetMassPropertiesFn)(McdGeometryID,
                       MeMatrix4,
                       MeMatrix3,
                       MeReal* );
typedef
char* (MEAPI *McdGeometryGetTypeNameFn)(McdGeometryID) ;

MeI16             MEAPI McdGeometryRegisterType( int typeEnum,
                            char *typeName,
                            McdGeometryDestroyFn,
                            McdGeometryGetAABBFn,
                            McdGeometryGetXYAABBFn,
                            McdGeometryGetBSphereFn,
                            McdGeometryMaximumPointFn,
                            McdGeometryGetMassPropertiesFn );


void              MEAPI McdGeometryInitObject( McdGeometryID g, MeI16 typeId );


void              MEAPI McdConvertBounds( MeReal* mBounds,
                            MeVector3 minCorner, MeVector3 maxCorner );


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MCDGEOMETRY_H */
