
#ifndef _VXSIMPLESCENEGRAPHINTERFACE_H_
#define _VXSIMPLESCENEGRAPHINTERFACE_H_

#include "Vx/VxFrame.h"
#include "Vx/VxSceneGraphInterface.h"

//
// Trivial scene graph implementation
//
class VxTrivialSceneGraph : public VxSceneGraphInterface
{
public:
    virtual int setNodeLocalTransform(VxNode node,VxReal44Ptr inLocalTM) { return 1; }
    
    virtual int copyTransformFromSGTM(VxSGTransformID inSgtm, VxReal44Ptr outTm, VxReal3Ptr outScale) { return 1; }
    virtual int copyTransformToSGTM(VxSGTransformID inSgtm, VxReal44Ptr outTm) { return 1; }
    virtual int computeNodeAbsoluteTransform(VxNode node, VxReal44Ptr outTm, VxReal3Ptr outScale, VxReal44Ptr tmParentOut) { return 1; }
    virtual int getRootNode(VxNode node, VxNode *rootOut) { return 1; } // return root node to start update from for this node
    virtual int addNodeToSceneGraph(VxNode node, void* entity) { return 1; }
    virtual int removeNodeFromSceneGraph(VxNode node) { return 1; }
    virtual int buildTraversal() { return 1; }
    virtual int preStepTraversal() { return 1; }
    virtual int getEntityFromNode(VxNode node, void** entity) { return 1; }
    virtual int optimizeLevel(int level) { return 1; }
};

namespace Vx
{

void MEAPI VxSimpleSceneGraphInterfaceRegister(VxFrame *f);
void MEAPI VxTrivialSceneGraphInterfaceRegister(VxFrame *frame);

} // namespace Vx


#endif
