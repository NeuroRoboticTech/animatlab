#ifndef _VX_OSG_TRAVERSAL_H_
#define _VX_OSG_TRAVERSAL_H_

/*
   Copyright (c) 2000-2003 CMLabs Simulations Inc. 
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/02/17 22:26:07 $
   
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

#include <osg/Node>
#include <osg/Matrix>

#include "Vx/Vx.h"
#include "Vx/VxSceneGraphInterface.h"
#include "Vx/VxMatrixStack.h"

#include <map>
#include <vector>

namespace Vx { class VxEntity; }

/**
@internal

  Build Traversal:
  - setup VxEntity parent pointers.
  - for each entity, find transform relative to parent if any
  ( not including the entity transforms )
  - for each dynamic part, position the part from the scenegraph.
  - for each non-dynamic part, position the part from the scenegraph.
  
    PreStep Update: update absolute vortex transforms for non-dynamic entitys
    Implementation 1: 
  - traverse scenegraph, accumulating the matrix stack
    and for each transform node with an associated non-dynamic entity,
    set its absolute transform.
    Should keep a list of root nodes to traverse (highest node with a transform) in
    order to minimize the amount of traversal.
    
    Implementation 2: use child entity pointers and entity relativeToParent transforms
    instead of the scenegraph. Assumes the relativeToParent transforms don't change,
    i.e. any transform node that might change has an associated entity.
  
    PostStep Update:
  - for each dynamic part, update corresponding node transform using
    parent entity pointer and optional relativeToParent transform.
    
    mOptimizeLevel: 
    0   maintain relative transform for dynamic entity (NOT IMPLEMENTED)
    1   add all entity in the traversal, 
        will behave well if isUpdateFromSceneGraph changes for a given entity
    2   add only entity for which isUpdateFromSceneGraph() returns true in the traversal
          
*/

class VxBranchDescription
{
public:
    VxBranchDescription() : mNode(0), mParent(0), mEntity(0), mNumVisitChild(0) {}
    ~VxBranchDescription() {}
    
    void reset() { mNode = mParent = 0; mEntity = 0; mNumVisitChild = 0; }
    
    VxNode mNode;
    VxNode mParent;
    Vx::VxEntity* mEntity;
    int mNumVisitChild;
};

class VxScenegraphRoot
{
public:
    VxScenegraphRoot() 
        : mRoot(0), 
        mBranchDescription(0), 
        mSize(0), 
        mNumUsedItem(0), 
        mNeedToBeBuilt(false) 
    {}
    ~VxScenegraphRoot() { delete[] mBranchDescription; }
    
    void reset() 
    { 
        mRoot = 0;
        delete[] mBranchDescription; 
        mBranchDescription = 0; 
        mSize = mNumUsedItem = 0; 
        mNeedToBeBuilt = false; 
    }
    
    bool isEmpty() const { return mNumUsedItem == 0; }
    
    void compact(int start = 0)
    {
        int i = start;
        while (i < mNumUsedItem && mBranchDescription[i].mNode != NULL)
            i++;
        
        int ii = i; // ii is first NULL item
        i++; // next
        
        for ( ; i<mNumUsedItem; i++)
        {
            if (mBranchDescription[i].mNode != NULL)
            {
                mBranchDescription[ii] = mBranchDescription[i];
                ii++;
            }
        }
        mNumUsedItem = ii;
    }
    int numEntities() const 
    { 
        int n = 0; 
        for (int i = 0; i < mNumUsedItem; i++)
        {
            if (mBranchDescription[i].mEntity)
                n++;
        }
        return n;
    }
    
    VxNode mRoot;
    VxBranchDescription* mBranchDescription;
    int mSize;
    int mNumUsedItem;
    bool mNeedToBeBuilt;
};

class osgMatrixStack;

struct VxNODE2ENTITY: public std::map<VxNode , void * >{};
//struct VxROOTARRAY: public std::vector<VxNode>{};
struct VxSCENEGRAPHROOTARRAY: public std::vector<VxScenegraphRoot*>{};

class VxOSGTraversal: public VxSceneGraphInterface
{
public:
    VxOSGTraversal(int optLevel = 0);
    virtual ~VxOSGTraversal();
    
    // OSG Interface functions.
    // see VxSceneGraphInterface class
    int computeNodeAbsoluteTransform(VxNode node, VxReal44Ptr outTm, VxReal3Ptr outScale, VxReal44Ptr tmParentOut);
    int setNodeLocalTransform(VxNode osgNode, VxReal44Ptr vxTM);
    int copyTransformFromSGTM(VxSGTransformID sgtmIn, VxReal44Ptr tmOut, VxReal3Ptr scaleOut);
    int copyTransformToSGTM(VxSGTransformID sgtmIn, VxReal44Ptr tmOut);
    int getEntityFromNode(VxNode node, void** entity);
    int preStepTraversal();
    int buildTraversal();
    int addNodeToSceneGraph(VxNode node, void* entity);
    int removeNodeFromSceneGraph(VxNode node);    
    int buildVisit(osg::Node *node, VxBranchDescription* branchDescription,int &currIndex,osgMatrixStack *stackFromAncestorEntity);
    int preStepVisit(VxBranchDescription** branchDescription);
    int optimizeLevel(int level);

protected:	
    VxNode getNode(int index);	
    osg::Node* getParentNode(osg::Node* node);
    int getRootNode(VxNode node, VxNode *outRoot);
    int nodeCount(osg::Node *node);
    int copyVxTransformToDOFtm(osg::Node *node, VxReal44Ptr m);    
    int buildTraversalInternal(VxScenegraphRoot* sgroot, int optimizeLevel);
    int preStepTraversalInternal(VxScenegraphRoot* sgroot, int optimizeLevel);    
    void reset();
    osgMatrixStack *getMatrixStack(){ return m_matStack;}
    osgMatrixStack *getLocalMatrixStack(){ return m_matStackLocal;}
    VxStack *getNodeStack(){ return &m_nodeStack;}
    void addRoot(VxNode root);

protected:
    VxNODE2ENTITY nodesMap;
    //VxROOTARRAY mRootArr;
    //VxROOTARRAY mKinematicRootArr;
    VxSCENEGRAPHROOTARRAY mSceneGraphRootArr;
    int mOptimizeLevel;

    osg::Matrix _m, _ml;
    /**
     * Cumulative matrix stack
     */
    osgMatrixStack *m_matStack;
    
    /**
     * Cumulative matrix stack since the last dynamic node
     */
    osgMatrixStack *m_matStackLocal;
    
    /**
     * Cumulative scale factor
     */
    //std::stack<double,std::vector<double> > m_scaleStack;
    
    VxStack m_nodeStack;
};


int computeNodeAbsoluteTransform(VxNode node, osg::Matrix& tmOut, VxReal3Ptr scaleOut, osg::Matrix& tmParentOut);
void setDOFNodeLocalTransform(osg::Node *node,osg::Matrix &lm);
int s_getRootNode(VxNode node, VxNode *outRoot);

void displayOSGSubTree(VxNode node);


#endif // _VX_OSG_TRAVERSAL_H_