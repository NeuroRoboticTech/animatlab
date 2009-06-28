#ifndef _OSG_LINE_H_
#define _OSG_LINE_H_

// osgLine.h: interface for the osgLine class.
//
//////////////////////////////////////////////////////////////////////
//#pragma warning( disable : 4786 )
//#pragma warning( disable : 4541 )
#include "MePrecision.h"

#include <osg/Node>
#include <osg/Geometry>

osg::Node *drawLine(MeVector3 point1,MeVector3 point2, float* color);
int lineMoveEnds( osg::Geometry* line, MeReal *s, MeReal *e );
osg::Node* drawAxes(const osg::Vec3& corner,const osg::Vec3& xdir,const osg::Vec3& ydir,const osg::Vec3& zdir);
int moveAxes( osg::Node* axes, const osg::Vec3& corner,const osg::Vec3& xdir,const osg::Vec3& ydir,const osg::Vec3& zdir);

#endif // _OSG_LINE_H_