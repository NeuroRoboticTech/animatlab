
#ifndef VXOSGGEOMETRYTRAVERSAL_H_
#define VXOSGGEOMETRYTRAVERSAL_H_

#include "VxOSG.h"
#include "osgMatrixStack.h"
#include "Vx/VxPrecision.h"

class osg::Node;

class VxOSGGeometryTraversal
{
public:
    VxOSGGeometryTraversal(const osg::Node* root) 
        :  mRoot(root), mCurrentVertex(NULL), mVertexCount(0)
    { }
    virtual ~VxOSGGeometryTraversal() {}

    osg::Node* getTraversalNodeAndTransform(osg::Node* node, osg::Matrix& localMat, osg::Matrix& parentMat);
    void preRecurse(osg::Node* node);
    void postRecurse(osg::Node* node);

    osgMatrixStack m_matStack;
    const osg::Node* mRoot;
    VxReal3* mCurrentVertex;
    unsigned int mVertexCount;
};


class VxOSGTriangleTraversal : public VxOSGGeometryTraversal
{
public:

    /** @internal. to be implemented. */
    class AcceptTriangleSubscriber
    {
    public:
        AcceptTriangleSubscriber() {}
        virtual ~AcceptTriangleSubscriber() {}

        virtual bool acceptNode(osg::Node* node)=0; 
    };

    VxOSGTriangleTraversal(const osg::Node* root, AcceptTriangleSubscriber* sub = 0)
        :VxOSGGeometryTraversal(root)
    { }
    virtual ~VxOSGTriangleTraversal() {}
    
    virtual void collectGeometryTriangles(void *node, bool test);
    virtual unsigned int countTris(void* pObject, VxReal &minX, VxReal &minY, VxReal &minZ, VxReal &maxX, VxReal &maxY, VxReal &maxZ);
    virtual unsigned int countTris(void* pObject);
  //  unsigned int countTrisRecursive(void* pObject);
    unsigned int countTrisRecursive(void* pObject, VxReal &minX, VxReal &minY, VxReal &minZ, VxReal &maxX, VxReal &maxY, VxReal &maxZ);
    void addTrisRecursive(osg::Node* node, bool test); 
    virtual bool addTri(bool test) = 0;
    
    int countGeometryTriangles(const osg::Matrixd &tmd, osg::Node *node);
    int countGeometryTriangles2(const osg::Matrixd &tmd, osg::Geode *node, VxReal &minX, VxReal &minY, VxReal &minZ, VxReal &maxX, VxReal &maxY, VxReal &maxZ);
    int countGeometryTrianglesFunctor(const osg::Matrixd &tmd, osg::Geode *node, VxReal &minX, VxReal &minY, VxReal &minZ, VxReal &maxX, VxReal &maxY, VxReal &maxZ);

protected:
    unsigned int triCount;

private:
    void mapTri(osg::Node* node, int prim, int tri);
    unsigned int currMapTri;
};




#endif // VXOSGGEOMETRYTRAVERSAL_H_