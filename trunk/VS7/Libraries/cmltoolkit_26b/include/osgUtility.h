#ifndef _OSG_UTILITY_H_
#define _OSG_UTILITY_H_

/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2003 Robert Osfield 
*
* This application is open source and may be redistributed and/or modified   
* freely and without restriction, both in commericial and non commericial applications,
* as long as this copyright notice is maintained.
* 
* This application is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include <osgProducer/Viewer>

Producer::CameraConfig* openOSGWindow(char *title, int posX, int posY, int width, int height,int screenNum = 1);
Producer::CameraConfig	*openOSGWindowSplit(char *title, int camCount,VxReal4* renderPos, Producer::CameraConfig* cConfig = NULL);
void splitOSGWindow(osgProducer::Viewer *viewer,VxReal4* renderPos, int vpCount = 1);
void getDesktopMaxSize(osgProducer::Viewer *viewer , int *width, int *height, int X = 1, int Y = 1);
void tileWindows(osgProducer::Viewer *viewer, const VxReal X, const VxReal Y, VxReal4 *renderPos =0);
VxReal4 *getWindowRenderArea(osgProducer::Viewer *viewer, const VxReal X, const VxReal Y);
void collapseWindows(osgProducer::Viewer *viewer, const VxReal X, const VxReal Y, int width = 0, int height = 0);

/**
 *  OSG Utility functions.
 */
osg::Node* loadSceneFromFile(const char* file, const osg::Vec3 &pos, const char* nodeName=0, float scale=1);
osg::Node* loadSceneFromFile2(const char* file, const osg::Vec3 &pos, const char* nodeName, float scalex, float scaley, float scalez);
osg::Node* loadSceneFromFileWithRelTM(const char* file, const osg::Matrixd * const relmat);
void nodeTraversal(osg::Node* loadedModel);
osg::Node* findNode(const char* osgObjectName, osg::Node* node);
osg::Node* findNodeFromFile(const char* osgObjectName, const char * file);
int findNodeFromParent(osg::Node* parent, const char * trName, osg::Node** child);
osg::Node* createLight(osg::StateSet* rootStateSet, float light_height = 2000, float light_radius = 50);
void setOSGGeometryColor( osg::Geometry * gx, float color[4]);
void setOSGColorTransparency(osg::Geometry * gx, bool transp, float alpha);

void copyOsgMatrix_to_VxReal44(osg::Matrix osgm, VxReal44Ptr m);
void copyVxReal44_to_OsgMatrix(osg::Matrix osgm, VxReal44Ptr m);
void hookOSGCameraToNode(osgProducer::Viewer* osgViewer, osg::Node *node, VxReal3 offset, VxReal44Ptr tm);

bool switchNode(osg::Node* node, bool value);

#endif // _OSG_UTILITY_H_