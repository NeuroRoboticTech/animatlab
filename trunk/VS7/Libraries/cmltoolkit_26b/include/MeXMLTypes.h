#ifndef _MEXMLTYPES_H
#define _MEXMLTYPES_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:51:03 $
   
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
    XML IO structures.
*/


#include "MePrecision.h"
#include "MeXMLParser.h"
#include "MeHash.h"


#ifdef __cplusplus
extern "C" {
#endif

/** used in loading user information */
typedef MeXMLError (MEAPI* Userdata_Handler)(MeXMLElement *);

/** used in loading */
typedef void *     (MEAPI* RGraphicUserCB)(void *rc,char *data);

typedef struct MeLoadContext      MeLoadContext;
typedef struct MeLoadCallbacks    MeLoadCallbacks;
typedef struct MeLoadOpenFileNode MeLoadOpenFileNode;
typedef struct NoInteractionNode  NoInteractionNode;
typedef struct CompositeNode      CompositeNode;
typedef struct cleanupNode        cleanupNode;
typedef struct MeLoadDatum        MeLoadDatum;

/** Callback used in saving user information */
typedef MeBool (MEAPI* Save_UserdataCB)(MeHash *,void *userdata,MeStream);

typedef struct MeSaveContext      MeSaveContext;
typedef struct MeSaveCallbacks    MeSaveCallbacks;
typedef struct MeSaveDatum        MeSaveDatum;

/* used in loading and saving */
typedef enum
{
    /* core Vortex structures */
    kMdtWorld,
    kMdtBody,
    kMdtConstraint,
    kMcdSpace,
    kMcdGeometry,
    kMcdModel,
    kMcdModelFrozen,
    kMcdSubModel,
    kMstBridge,

    /* Vortex internal application stuff */
    kRGraphic,

    kMeDatumTypeParameter

    /* External user stuff */
} MeDatumType_enum;

/* used in loading */
typedef enum
{
    kMeActionNone = 0,
    kMeGetTMFromBody = 1,
    kMeGetTMFromModel = 2,
    kMeLoadActionAddToComposite = 4,
    kMeLoadActionMask = 0xFF, /* mask above actions with this */
    kMeLoadActionAutoMassProperties = 256
} MeLoadAction_enum;


#define ME_STRING_BUFFER 256
#define ME_LOADERR(x,y) \
do { \
    MeInfo(0,"(%s line %d): \n", MeShortenPath(__FILE__), __LINE__); \
    x; \
    MeInfo(0,"This was generated while parsing %s\n\n",y); \
} while (0)

struct RGraphic;
struct MeApp;
struct McdModel;
struct _RRender;

struct MeLoadCallbacks
{
    /* core Vortex stuff */

    /*
     * The Mst callback glues dynamics and collision together. In particular it
     * sets the transforms of those models which use a dynamics body transform.
     * It also deals with relative transforms, frozen models and non-interacting
     * models.
     */
    void (MEAPI* MstCB)(MeLoadContext *lc);
    MeXMLError (MEAPI* Mdt_Handler)(MeXMLElement *); /* parses Mdt */
    MeXMLError (MEAPI* Mcd_Handler)(MeXMLElement *); /* parses Mcd */
    MeXMLError (MEAPI* Mst_Handler)(MeXMLElement *); /* parses Mst */

    MeXMLError (MEAPI* McdTriangleMesh_Handler)(MeXMLElement *);
    MeXMLError (MEAPI* McdConvexMesh_Handler)(MeXMLElement *);
    MeXMLError (MEAPI* McdRGHeightField_Handler)(MeXMLElement *);
    MeXMLError (MEAPI* McdComposite_Handler)(MeXMLElement *);

    /* Vortex internal application stuff */
    MeXMLError (MEAPI* Render_Handler)(MeXMLElement *); /* parses render */
    struct RGraphic *(MEAPI* RGraphicCreateFromModel)(struct _RRender *rc,
                                        struct McdModel *model,float color[4]);
    RGraphicUserCB graphicCreateCB;
    void (MEAPI* RGraphicSetTM)(MeHash*);     /* sets transforms of graphics based on
                                                 models or bodies */

    /* External user stuff */
    Userdata_Handler userCB; /* parses user data */
    void *userdata;          /* userdata passed to userCB */
};

/* used in stack of open files */
struct MeLoadOpenFileNode
{
    char *file;
    MeLoadOpenFileNode *next;
};


struct NoInteractionNode
{
    struct McdModel *model1;
    struct McdModel *model2;
    NoInteractionNode *next;
};


struct CompositeNode
{
    struct McdModel *model;
    MeMatrix4Ptr relTM;
    CompositeNode *next;
};

struct cleanupNode
{
    void *ptr;
    struct cleanupNode *next;
};

struct MdtWorld;
struct _McdSpace;
struct MdtBaseConstraint;

/* this gets passed down the XML parser hierarchy as user data */
/** Load context data structure containg hash table and other related data */
struct MeLoadContext
{
    /* common to everything */
    MeLoadCallbacks cbs;
    int options;
    MeHash *hash;
    MeMatrix4 offset;
    MeReal scale;       /* scale factor to be applied down the hierarchy */
    char *IDpath;       /* Stack of .me file ids in the form: id1.id2.id3. etc.
                           Used to give loaded objects unique IDs */
    int depth;          /* depth in the hierarchy of current .me file */
    MeLoadOpenFileNode *openFileHead; /* list of files of open files */
    char *workingDirectory;
    cleanupNode *cleanupHead; /* list of objects to cleanup in MeLoadContextDestroy() */

    /* Core Vortex stuff */
    struct MdtWorld *world;
    struct _McdSpace *space;
    NoInteractionNode *noInteractionHead;    /* list of non-interacting models */
    CompositeNode *compositeHead;
    struct MdtBaseConstraint *constraint;
    struct MstBridge *bridge;
    struct MdtBclContactParams * contactParams; /* current contact params */
    struct McdModel *currentModel; /* for composite */

    /* Vortex internal application stuff */
    struct _RRender *rc;   /* render context */
    struct MeApp *app;  /* MeApp structure */

    /* External user stuff */
};


struct MeLoadDatum
{
    char *key;                 /* copy of the key */
    void *ptr;                 /* MdtBodyID/MdtConstraintID/McdGeometryID/
                                  McdModelID etc */
    MeDatumType_enum type;     /* indicates which one of the above */
    char *associatedKey;       /* used if a model refers to a body for example */
    MeLoadAction_enum action;  /* action to be carried out */
    char *me_file;             /* The .me file that this datum was created in */
};


/* used in saving */

struct MeSaveCallbacks
{
    /* core Vortex stuff */
    MeBool (MEAPI* Save_Mdt)(MeSaveContext *,MeStream);
    MeBool (MEAPI* Save_Mcd)(MeSaveContext *,MeStream);
    MeBool (MEAPI* Save_Mst)(MeSaveContext *,MeStream);
    void (MEAPI* SerializeTriangleMesh)(void *,char *, MeSaveContext *, MeStream);
    void (MEAPI* SerializeConvexMesh)(void *,char *, MeSaveContext *, MeStream);
    void (MEAPI* SerializeRGHeightField)(void *,char *, MeSaveContext *, MeStream);
    void (MEAPI* SerializeComposite)(void *,char *, MeSaveContext *, MeStream);

    /* Vortex internal application stuff */

    /* External user stuff */
    Save_UserdataCB userCB;
    void *userdata;
};

/** Save context data structure containg hash table. */
struct MeSaveContext
{
    MeSaveCallbacks cbs;
    MeHash *hash;
    int options;
};


struct MeSaveDatum
{
    void *key;
    char *stringID;
    MeDatumType_enum type;
};

#ifdef __cplusplus
}
#endif

#endif /* _MEXMLTYPES_H */
