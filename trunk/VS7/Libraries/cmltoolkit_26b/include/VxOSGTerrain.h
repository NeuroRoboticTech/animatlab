
#ifndef VX_OSG_TERRAIN_H
#define VX_OSG_TERRAIN_H

#include "Vx/VxTerrain.h"
#include <osg/Node>

VxTerrain* createVxTerrainFromOSGNode(osg::Node *pNode, int upAxis, VxReal radius, 
                                      VxReal xsize, VxReal ysize);



#endif // VX_OSG_TERRAIN_H