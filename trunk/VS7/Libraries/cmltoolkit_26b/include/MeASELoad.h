#ifndef _MEASELOAD_H
#define _MEASELOAD_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:51:07 $
   
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
 * @file
 * 3DSMax .ASE File Loader Utility
 */

#include "MePrecision.h"


typedef struct  MeASEUV             MeASEUV;
typedef struct  MeASEMaterial       MeASEMaterial;
typedef struct  MeASEFace           MeASEFace;
typedef struct  MeASEObject         MeASEObject;
typedef struct  MeASESubMaterial    MeASESubMaterial;


typedef enum
{
    MeASEMaterialFlagNone = 0,
    MeASEMaterialFlagTexture,
    MeASEMaterialFlagColor
} MeASEMaterialFlags;

struct MeASEUV
{
    MeReal u;
    MeReal v;
};

struct MeASESubMaterial
{
    MeASEMaterialFlags  type;
    float               ambient[4];
    float               diffuse[4];
    float               specular[4];
    char                texFilename[256];
};

struct MeASEMaterial
{
    int                 numSubs;
    MeASESubMaterial    *subMaterials;
};

struct MeASEFace
{
    int             vertexId[3];
    MeVector3       normal;
    MeVector3       vNormal[3];
    MeASEUV         map[3];
    int             materialId;
    int             subMaterialId;
};

struct MeASEObject
{
    MeBool              isLoaded;

    int                 numVerts;
    int                 numFaces;
    int                 numUvs;
    int                 numMaterials;

    MeVector3           *verts;
    MeASEFace           *faces;
    MeASEUV             *uvs;
    MeASEMaterial       *materials;
};

#ifdef __cplusplus
extern "C" {
#endif

MeASEObject* MEAPI  MeASEObjectLoad(char* filename,
                        MeReal xScale, MeReal yScale, MeReal zScale);
void         MEAPI  MeASEObjectDestroy(MeASEObject* object);

#ifdef __cplusplus
}
#endif

#endif
