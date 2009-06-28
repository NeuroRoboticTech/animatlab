#ifndef _MCDMODEL_H
#define _MCDMODEL_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:50:40 $
   
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

/** @file McdModel.h
    The collision model object and utility functions

    An McdModel is a collision model whose geometrical shape is
    specified by a McdGeometry object. An McdModel also holds the
    transformation matrix describing its position and orientiation in 3D
    space.

    To an McdModel can be associate dynamic behaviour.
    Rigid body dynamics behaviour can be obtained by associating an
    MdtBody using MstModelAndBodyCreate().

    Collision models are the
    principal object type in the Mcd system. Typically there is a
    one-to-one correspondence between a collision model and a 3D graphics
    model that is rendered on the screen.

    You can query geometrical properties of an isolated McdModel object
    using the McdModel interface, which uses the McdGeometry
    protocol and takes into account its transformation matrix.

    You can query the geometrical relationship between a pair of McdModel
    objects in close proximity, by creating a McdModelPair object and using
    any of the pairwise queries declared in McdInteractions.h.

    You can insert McdModel objects into McdSpace, which will keep track of
    which pairs are in close proximity.

*/

#include "McdGeometry.h"

#ifdef __cplusplus
extern "C" {
#endif

  /** @internal */
typedef struct McdModel McdModel;
  /** @internal */
typedef void (MEAPI *McdModelUpdateFn)(McdModel *model);

struct MePool;


/*---------------------------------------------------------------
 * McdModel
 *---------------------------------------------------------------
 */

  /**
    An McdModel represents simulation objects
    whose geometrical shape is
    specified by an McdGeometry. An McdModel also holds the
    transformation matrix describing its position and orientiation in 3D
    space.
 */
struct McdModel
{
  /** @internal */
  McdGeometry *mGeometry;  /**< the models geometry */

  /** @internal */
  MeMatrix4Ptr mGlobalTM;   /**< global transformation to world coordinates */
  /** @internal */
  MeReal *linearVelocity;   /**< linear velocity of model for this time step */
  /** @internal */
  MeReal *angularVelocity;  /**< angular velocity of model for this time step */
  /** @internal */
  McdModel* mParent;        /**< for use in Composite objects [non-]rigid bodies */
  /** @internal */
  MeReal mPadding;          /**< tolerance for contact generation. */

  /** @internal */
  void *mData;              /**< user data */
  /** @internal */
  void *mAuxTM;             /**< other transform, such as graphics data */

  /** @internal */
  void *mSpace;             /**< McdSpace this model belongs to */
  /** @internal */
  MeI32 mSpaceID;           /**< its internal indentifier in that space */
  /** @internal */
  McdModelUpdateFn mTransformUpdateFn;
  /** @internal */
  MeMatrix4Ptr mRelTM;
  /** @internal */
  MeMatrix4Ptr mRefTM;
  /** @internal */
  void *mBody;              /**< used by Mst to point to the corresponding body or particle system */
  /** @internal */
  McdModel* mNext;          /**< used to indicate the next model associated with the same body */
  /** @internal */
  MeReal mDensity;          /**< used for mass property calculations */
  /** @internal */
  int mMaterial;            /**< used by Mst to select contact properties */
  /** @internal */
  int mRequestID;           /**< used by the optional McduRequestTable */
  /** @internal */
  int mResponseID;          /**< used by Mst to identify which reponse module to invoque */

  /** useful for setting names */
  char *mName; 
};


  /** @internal */
typedef McdModel *McdModelID;

  /** @internal */
MeBool            MEAPI McdModelInit( int modelMaxCount );

  /** @internal */
void              MEAPI McdModelTerm();

  /** @internal */
int               MEAPI McdModelGetModelMaxCount();

  /** @internal */
int               MEAPI McdModelGetModelCount();
McdModelID        MEAPI McdModelCreate( McdGeometryID g );
McdModelID        MEAPI McdModelCreateInPool( McdGeometryID g, struct MePool *pool );
void              MEAPI McdModelDestroy( McdModelID cm );
void              MEAPI McdModelDestroyInPool( McdModelID cm, struct MePool *pool );

McdGeometryID     MEAPI McdModelGetGeometry( McdModelID g );
void              MEAPI McdModelSetGeometry( McdModelID cm, McdGeometryID g );

void              MEAPI McdModelSetTransformPtr( McdModelID cm,
                         MeMatrix4 geometryTM );
MeMatrix4Ptr      MEAPI McdModelGetTransformPtr( McdModelID cm );
void              MEAPI McdModelSetReferenceTransformPtr( McdModelID cm, MeMatrix4 geometryTM );

MeMatrix4Ptr      MEAPI McdModelGetReferenceTransformPtr( McdModelID cm);

void              MEAPI McdModelSetRelativeTransformPtrs( McdModelID cm,
                                 MeMatrix4 relTM,
                                 MeMatrix4 refTM,
                   MeMatrix4 compoundTM);

void              MEAPI McdModelGetRelativeTransformPtrs( McdModelID cm,
                              MeMatrix4Ptr *relTM,
                              MeMatrix4Ptr *refTM);

void              MEAPI McdModelUpdate( McdModelID cm );
void              MEAPI McdModelMoveTo( McdModelID cm, MeMatrix4 newTM, MeReal timeStep );
void              MEAPI McdModelSetUpdateCallback( McdModelID cm, McdModelUpdateFn f );
McdModelUpdateFn  MEAPI McdModelGetUpdateCallback( McdModelID cm );
void              MEAPI McdModelCompoundTransforms( McdModelID cm );

void              MEAPI McdModelSetLinearVelocityPtr( McdModelID cm, MeReal *);
MeReal*           MEAPI McdModelGetLinearVelocityPtr( McdModelID cm );
void              MEAPI McdModelSetAngularVelocityPtr(McdModelID cm, MeReal *);
MeReal*           MEAPI McdModelGetAngularVelocityPtr(McdModelID cm );

void              MEAPI McdModelGetBSphere( McdModelID cm, MeVector3 center,
                            MeReal *radius );

void              MEAPI McdModelGetAABB( McdModelID cm, MeVector3 minCorner,
                            MeVector3 maxCorner );
void              MEAPI McdModelGetAABB_PathCompatible( McdModelID cm, MeVector3 minCorner,
                            MeVector3 maxCorner, MeReal dummy );

void              MEAPI McdModelGetPathAABB( McdModelID cm, MeVector3 minCorner, MeVector3 maxCorner,
                     MeReal motionDuration );
void              MEAPI McdModelSetContactTolerance(McdModelID cm, MeReal tol);
MeReal            MEAPI McdModelGetContactTolerance(McdModelID cm );

void              MEAPI McdModelSetUserData( McdModelID cm, void *data );
void*             MEAPI McdModelGetUserData( McdModelID cm );

void              MEAPI McdModelSetName( McdModelID cm, char *name );
char*             MEAPI McdModelGetName( McdModelID cm );

void              MEAPI McdModelSetDensity( McdModelID cm, MeReal density );
MeReal            MEAPI McdModelGetDensity( McdModelID cm );

  /** @internal */
void*             MEAPI McdModelGetBodyData( McdModelID cm );

  /** @internal */
void              MEAPI McdModelSetBodyData( McdModelID cm, void * body );

  /** Set the models material ID. Used in conjunction with Mst. */
void              MEAPI McdModelSetMaterialID( McdModelID cm, int material );

  /** Read the models material id. Used in conjunction with Mst. */
int               MEAPI McdModelGetMaterialID( McdModelID cm );

  /** Macros for backward compatibility */
#define McdModelSetMaterial( model, material ) McdModelSetMaterialID( model, material )
#define McdModelGetMaterial( model ) McdModelGetMaterialID( model )


  /** Set the models request ID. Used in conjunction with McduRequestTable. */
void              MEAPI McdModelSetRequestID( McdModelID cm, int requestId );

  /** Read the models request ID. Used in conjunction with McduRequestTable. */
int               MEAPI McdModelGetRequestID( McdModelID cm );


  /** Set the models response ID. Used in conjunction with Mst. */
void              MEAPI McdModelSetResponseID( McdModelID cm, int responseId );

  /** Read the models response ID. Used in conjunction with Mst. */
int               MEAPI McdModelGetResponseID( McdModelID cm );

  /** Compute the models mass properties using the model density, returning the volume, mass, and center of mass
      (in the translation of relTM). */
MeI16 MEAPI       McdModelGetMassProperties( McdModelID cm, MeMatrix4 relTM, MeMatrix3 m, MeReal *volume, MeReal *mass );

#define McdModelGetDefaultRequestID() (0)
#define McdModelGetDefaultResponseID() (0)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MCDMODEL_H */
