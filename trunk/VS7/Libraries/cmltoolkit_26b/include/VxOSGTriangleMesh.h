#ifndef _VX_OSG_TRIANGLE_MESH_H_
#define _VX_OSG_TRIANGLE_MESH_H_

// VxOSGTriangleMesh.h: interface for the VxOSGTriangleMesh class.
//
//////////////////////////////////////////////////////////////////////

#include <osg/Node>
namespace Vx { class VxTriangleMesh; }

class VxOSGTriangleMesh  
{
public:
    VxOSGTriangleMesh(){};
    virtual ~VxOSGTriangleMesh(){};

};

Vx::VxTriangleMesh* createVxTriangleMeshFromOSGNode(osg::Node *pNode);

#endif // _VX_OSG_TRIANGLE_MESH_H_