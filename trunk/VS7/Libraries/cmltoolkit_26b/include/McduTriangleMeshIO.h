#ifndef _MCDUTRIANGLEMESHIO_H
#define _MCDUTRIANGLEMESHIO_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/01/04 19:46:24 $
   
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
   @file McduTriangleMeshIO.h

   Function for in-memory construction of a mesh from the contents
   of a description file.

 */

#ifdef __cplusplus
extern "C" {
#endif


/**
Load a triangle mesh by reading vertices from a .obj file.

MeStreamOpenWithSearch is used to open the file, so an additional search directory can be specified
using MeFileSearchPathAdd(path).

@param filename the name of the obj file,
@param mesh optional mesh geometry already created to which vertices will be added; can be 0 to create a new mesh geometry.
@param scale multiples vertices by this number,
@param center not used,
@param merge merges identical vertices for efficiency
@param flipNormals if 1, triangle normals will be flipped,
@param eps distance threshold between vertices to merge them,
@param opt triangle mesh options. Can be null for default.

@param vertexPtr (output) an array of vertices,
@param outVertexCount (output) the number of vertices

*/
McdTriangleMeshID MEAPI
McduTriangleMeshCreateFromObj( const char *filename, McdTriangleMeshID mesh, MeReal scale,
               int center, int merge, int flipNormals, MeReal eps,
               MeReal** vertexPtr, int *outVertexCount,
               McdTriangleMeshOptions *opt);

McdTriangleMeshID MEAPI
McduTriangleMeshCreateFromObjWithAcceptCB( const char *filename, McdTriangleMeshID mesh, MeReal scale,
               int center, int merge, int flipNormals, MeReal eps,
               MeReal** vertexPtr, int *outVertexCount,
               McdTriangleMeshOptions *opt, MeBool (*cb)(MeVector3 p0, MeVector3 p1, MeVector3 p2, void* data), void* userData);
McdTriangleMeshID MEAPI
McduTriangleMeshCreateFromASEWithAcceptCB( const char *filename, McdTriangleMeshID mesh, MeReal scale,
               int center, int merge, int flipNormals, MeReal eps,
               MeReal** vertexPtr, int *outVertexCount,
               McdTriangleMeshOptions *opt, MeBool (*cb)(MeVector3 p0, MeVector3 p1, MeVector3 p2, void* data), void* userData);

/**
    Save a TriangleMesh to .obj format.
    Preserves vertex identity, except if @a merge is non-zero, in which case
    vertices closer than @a eps will be merged.
*/
int MEAPI McduTriangleMeshWriteToObj( const char *filename,
                McdTriangleMeshID mesh,
                int merge, MeReal eps );

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _MCDUTRIANGLEMESHIO_H */
