/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/05/04 16:42:51 $
   
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


/*
  This file implements the mouse spring
*/

// VxOSG includes
#include "VxOSG.h"

namespace Vx
{
    class VxPart;
};

// Vx includes
#include "Vx/Vx.h"
#include "VxOSGVisualizer.h"
#include "MeMath.h"

// OSG includes
#include <osg/MatrixTransform>
#include <osgDB/FileUtils>
#include <osgSim/DOFTransform>


// class to handle events with a pick
class VxOSGPickHandler : public osgGA::GUIEventHandler 
{
public: 
    enum dragType 
    {
        spring, translate, rotate, freeze
    };

    VxOSGPickHandler(VxOSGVisualizer* visual, osgProducer::Viewer* viewer, osg::Node* root);
   
    virtual ~VxOSGPickHandler() 
    {
// CRASHES / called from producer          mVisual->destroyLine(mLine);
    }
        
    bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);

    virtual void pick(const osgGA::GUIEventAdapter& ea);
    void drag(const osgGA::GUIEventAdapter& ea);
    void release();

    void getClipCoordinates(VxReal *x, VxReal *y, int camIndex);

    void update();
    
protected:
    VxEntity* findVxEntity(osgUtil::Hit*& hit);
    osgUtil::IntersectVisitor::HitList mHitList;

    VxReal mMouseDistance;
    VxReal mMouseSpringStiffness;
    VxReal mMouseSpringDamping;
    VxReal3 mGrabPosWorld, mDesiredPosition, mLocalGrabPosition;

    osgProducer::Viewer* mViewer;
    VxOSGVisualizer* mVisual;
    int mCamIndex;
    osg::Node *mRoot;
    VxPart *mGrabbedPart;
    bool mWasFrozen;
    bool mMouseSpringModifyerOn;
    dragType mDragType;

    VxOSGVisualizer::Line* mLine;
};

