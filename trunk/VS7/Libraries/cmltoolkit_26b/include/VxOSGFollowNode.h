// VxOSGFollowNode.h: interface for the VxOSGFollowNode class.
//
//////////////////////////////////////////////////////////////////////
#ifndef VXOSGFOLLOWERNODE_H_
#define VXOSGFOLLOWERNODE_H_


#include <osgProducer/Viewer>
#include <osg/Node>

#include <osg/MatrixTransform>

#include <osgGA/MatrixManipulator>
#include <osg/PositionAttitudeTransform>

#include "Vx/Vx.h"

class followNodeMatrixManipulator;

struct followerWorldTransform
{
   followerWorldTransform();
   virtual ~followerWorldTransform(){};

   // class to allow access to matrix that represents accumlation of 
   //  matrices above specified node of scene graph.
   struct worldMatrix : public osg::NodeCallback
   {
       virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
       {
           matrix = osg::computeWorldToLocal(nv->getNodePath() );
           traverse(node,nv);
       }
       osg::Matrix matrix;
   };
   
   
   bool attachToGroup(osg::Group* g);
   osg::Matrix getMatrix();
protected:
   osg::ref_ptr<osg::Group> parent;
   osg::Node* node;
   worldMatrix* mpcb;
};

class VxOSGFollowNode  
{
public:
	VxOSGFollowNode(osg::Group *followGroup, osg::PositionAttitudeTransform *followerTM = 0);
	virtual ~VxOSGFollowNode();

    void setFollowerTM(osg::PositionAttitudeTransform *followerTM){mFollowerPosTM = followerTM;}

    followNodeMatrixManipulator* getFollowerManipulator(){ return mFollowNodeManipulator;}
    osg::Matrix getFollowerWorldTM(){return mFollowerWorldTM->getMatrix();}
    void setLeader(osg::Group *leader){}
    void setFollowerPos(osg::Vec3 pos);
    void setFollowerAtitude(VxReal angle, osg::Vec3 axis);

protected:
    osg::ref_ptr<osg::PositionAttitudeTransform> mFollowerPosTM;
    osg::Group *mLeader;
    followerWorldTransform*         mFollowerWorldTM;
    followNodeMatrixManipulator* mFollowNodeManipulator;
};


class followNodeMatrixManipulator : public osgGA::MatrixManipulator
{
public:
   followNodeMatrixManipulator( followerWorldTransform* ta) {worldCoordinatesOfNode = ta; theMatrix = osg::Matrixd::identity();}
   virtual ~followNodeMatrixManipulator() {}
   bool handle (const osgGA::GUIEventAdapter&ea, osgGA::GUIActionAdapter&aa);
   void updateTheMatrix();
   virtual void setByMatrix(const osg::Matrixd& mat); 
   virtual void setByInverseMatrix(const osg::Matrixd&mat);
   virtual osg::Matrixd getInverseMatrix() const;
   virtual osg::Matrixd getMatrix() const;
protected:
   followerWorldTransform* worldCoordinatesOfNode;
   osg::Matrixd theMatrix;
};

#endif //VXOSGFOLLOWERNODE_H_