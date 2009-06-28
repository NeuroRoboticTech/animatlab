#ifndef _VX_OSG_H_
#define _VX_OSG_H_

#include "Vx/Vx.h"

#include <osg/Node>
#include <osg/Matrix>
#include <osgProducer/Viewer>

// VxOSGTraversal.cpp
void MEAPI VxOsgSceneGraphInterfaceRegister(VxNode s);
void MEAPI VxOsgSceneGraphInterfaceUnRegister(VxNode frame);

void getDOFNodeLocalTransform(osg::Node *node, osg::Matrix *lm);
void setDOFNodeLocalTransform(osg::Node *node, osg::Matrix &lm);


#include "osgUtility.h"
#include "osgShadow.h"

#endif // _VX_OSG_H_