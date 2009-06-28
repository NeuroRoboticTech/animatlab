#if !defined(VX_TERRAIN_H)
#define VX_TERRAIN_H

/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/04/20 20:39:02 $

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


// VxTerrain.h: interface for the VxTerrain class.
//
//////////////////////////////////////////////////////////////////////

#include "Vx/VxGeometry.h"

namespace Vx
{

class VxTriangleBin;

/**
 * Terrain geometry. 
 * Makes any non-moving loaded graphics into a collidable environment.
 * Subdivides the environment into vertical cells whose X-Y size is
 * specified using setGridSubdivision().
 * The vertical direction can be changed in createTerrain().
 * The maximum size (bounding sphere radius) of any object that 
 * is to collide with the terrain and the grid size can be specified
 * using setCollidingGeometryRadius() before
 * calling createTerrain() which builds collision optimizing internal data structures.
 * The triangles which might collide
 * with another object whose bounding radius is specified using
 * are stored in each cell.
 *
 * Several versions of the createFromNode() function are provided.
 * 
 * Note also that the triangles vertices must be oriented counter clockwise when
 * looking from the outsize of the object to obtain correct collision normals.
 * In addition, thin constructs that might be approached from both sides 
 * such as zero thickness fences or walls will not work properly if approached
 * from one of the sides. This will be fixed in the future.
 * Using a Visualizer class to display dynamic contacts might help
 * in solving such problems. Contacts should point from the terrain outside.
 */
class VX_DLLSPEC VxTerrain : public VxGeometry
{

public:
    VxTerrain();
    virtual ~VxTerrain();
    
    static int getClassType();
    virtual const char* getClassName() const { return "VxTerrain"; }
    

    class AcceptTriangleBuildSubscriber // rename to Filter?
    {
    public:
        /** return true to accept triangle. @a triangleIndex is the index in the original list set by setExternalVertices or loaded from file (after its own filter, if any). */
        virtual bool accept(MeVector3 p0, MeVector3 p1, MeVector3 p2, int triangleIndex) = 0;
    };

    class AcceptTriangleSubscriber // rename to AcceptTriangleLoadSubscriber?
    {
    public:
        virtual bool accept(MeVector3 p0, MeVector3 p1, MeVector3 p2) = 0; //, int triangleIndex, char* textureName) = 0;
    };

    /** @warning deprecated, use loadFromFile */
    bool loadFromObjFile(const char *fileName, VxReal scale=1, int flipNormals=0, VxReal44Ptr tm=0, AcceptTriangleSubscriber* sub=0)
    {
        loadFromFile(fileName, scale, flipNormals, tm, sub);
        return true;
    }
    
    bool loadFromFile(const char *fileName, VxReal scale=1, int flipNormals=0, VxReal44Ptr tm=0, AcceptTriangleSubscriber* sub=0);
    void setExternalVerticesFromTerrain(VxTerrain* terrain);
    void setExternalVertices(VxReal3* vertices, VxReal3Ptr* verticesPtr, int vCount, int tCount);
    void takeVertexOwnership();
    
    bool createTerrain(void* node, VxReal maxRadius, int numCellX, int numCellY);
    bool createTerrain(int numCellX, int numCellY, int upAxis=2);
    bool createTerrain(VxReal3Ptr bbMin, VxReal3Ptr bbMax, VxReal cellSize, int upAxis=2);
    bool createTerrain(int numCellU, int numCellV, int upAxis, AcceptTriangleBuildSubscriber *sub);
    void destroyTerrain();
    
    VxTriangleBin* getVxTriangleBin();
    VxTriangleBin* getVxTriangleBin() const;
    void setVxTriangleBin(VxTriangleBin* tb);

    void getNumCellGrid(int& nu, int& nv) const;
    int getCellInfo(int i, int j, VxReal3Ptr low, VxReal3Ptr hi) const;

    int getTriangleCount() const;
    void getTriangle(int i, VxReal3Ptr p0, VxReal3Ptr p1, VxReal3Ptr p2, VxReal3Ptr normal) const;
    void getTriangleVertexPointers(int i, VxReal3Ptr* p0, VxReal3Ptr* p1, VxReal3Ptr* p2) const;
    void getTriangleVertexAndNormalPointers(int i, VxReal3Ptr* p0, VxReal3Ptr* p1, VxReal3Ptr* p2, VxReal3Ptr* normal) const;
    int getTriangleTag(int i) const;
    void setTriangleTag(int i, int tag);

    VxReal getElevation(VxReal U, VxReal V, VxReal3Ptr outNormal, int* outTriangleIndex) const;
    VxReal getElevation(VxReal3 pos, VxReal3Ptr outNormal, int* outTriangleIndex) const;
    VxReal getMinMaxElevation(VxReal4 pos, VxReal rad, VxReal *outMinHeight, VxReal *outMaxHeight) const;
    VxReal getMinMaxElevation(VxReal3 bbmin, VxReal3 bbmax, VxReal *outMinHeight, VxReal *outMaxHeight) const;

    int getUpAxis() const;

    void setCellMaxSize(int maxsize);
    int getCellMaxSize();

protected:

    VxReal3* mVerticesFromFile;
    MeVector3Ptr* mVertexPtrFromFile;
    int mVertexCountFromFile;
    int mTriCountFromFile;
    bool mExternalVertices;
    int mTriListSize;
};

} // namespace Vx

#endif // VX_TERRAIN_H
