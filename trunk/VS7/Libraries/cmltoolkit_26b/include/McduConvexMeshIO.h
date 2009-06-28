#ifndef _MCDUCONVEXMESHIO_H
#define _MCDUCONVEXMESHIO_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:50:41 $
   
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
   @file McduConvexMeshIO.h

   Function for in-memory construction of a mesh from the contents
   of a description file, and saving into .obj file format.

 */

#include "McdConvexMesh.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
Create a convex mesh by reading vertices from a .obj file and
computing their convex hull
*/
McdConvexMeshID MEAPI McduConvexMeshCreateHullFromObjWithOptions( const char *filename,
                            MeReal scale, int center, McdConvexMeshOptions *options);
#define McduConvexMeshCreateHullFromObj(filename, scale, center) McduConvexMeshCreateHullFromObjWithOptions(filename, scale, center, 0)

/**
    Save a ConvexMesh to .obj format.
    Preserves vertex identity.
*/
int MEAPI
McduConvexMeshWriteToObj( const char *filename, McdConvexMeshID mesh );

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MCDUCONVEXMESHIO_H */
