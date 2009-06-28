
#include "Vx/Vx.h"
#include "Vx/VxUniverse.h"
#include "Vx/VxFrame.h"
#include "Vx/VxBox.h"
#include "Vx/VxCylinder.h"
#include "Vx/VxSphere.h"
#include "Vx/VxPart.h"
#include "Vx/VxPlane.h"
#include "Vx/VxCollisionGeometry.h"
#include "Vx/VxMaterialPair.h"
#include "Vx/VxTerrain.h"
#include "Vx/VxTriangleMesh.h"
#include "Vx/VxHinge.h"

#define USE_PLANE


#include "VxWheeledVehicle.h"
//#include "VxVehicleDisplay.h"
#include "VxDefaultSteeringSub.h"
#include "MeMath.h"

//#include "MeViewerTypes.h" // for the arrows....

// OSG includes
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Texture2D>

#include <osgProducer/Viewer>
#include <osg/Math>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osgProducer/Viewer>
#include <osg/Projection>

#include <osg/Transform>
#include <osg/MatrixTransform>
#include <osgSim/DOFTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Switch>
#include <osg/Notify>
#include <osg/Geometry>

#include <osg/Light>
#include <osg/LightSource>

#include <osgText/Text>

#include <osgDB/Registry>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osg/LineWidth>
#include <osg/PolygonStipple>
#include <osgGA/TrackballManipulator>

#include "VxOSGTriangleMesh.h"
#include "VxOSGTerrain.h"
#include "VxOSGVisualizer.h"
#include "VxOSG.h"
#include "VxOSGTraversal.h"

#include <stdio.h>

using namespace Vx;
using namespace VxVehicle;

class VxVehicleDisplay;

class VxJoystickAxesSub : public VxViz::Subscriber
{
public:
    VxJoystickAxesSub() : mDelta(0.002), mSteeringSub(0) {}
    void notify(int inputSource,int sourceIndex, int inputAction, bool value){};
    void notify(int inputSource,int sourceIndex, int inputAction, int value){};

    void notify(int inputSource,int sourceIndex, int inputAction, float value);
    VxReal mDelta;
    VxDefaultSteeringSub* mSteeringSub;
    VxVehicleBase* mVehicle;

};




class VxJoystickBtnDownSub : public VxViz::Subscriber
{
public:
    VxJoystickBtnDownSub(VxReal delta=100, VxReal max=1000) : mVehicle(0)
    { 
        mDelta = delta; mMax = max; 
    }
    
    void notify(int inputSource,int sourceIndex, int inputAction, bool value){};
    void notify(int inputSource,int sourceIndex, int inputAction, int value){};

    void notify(int inputSource,int sourceIndex, int inputAction, float value);
    
    VxReal mDelta;
    VxReal mMax;
    VxVehicleBase* mVehicle;
    VxAutomaticNGearTransmission* mTransmission;

};


class VxKeyboardSteerSub : public VxViz::Subscriber
{
public:
    VxKeyboardSteerSub() : mDelta(0.02), mSteeringSub(0) {}
    void notify(int inputSource,int sourceIndex, int inputAction, bool value){};
    void notify(int inputSource,int sourceIndex, int inputAction, int value){};

    void notify(int inputSource,int sourceIndex, int inputAction, float value);
    VxReal mDelta;
    VxDefaultSteeringSub* mSteeringSub;
};


class VxKeyboardThrottleSub : public VxViz::Subscriber
{
public:
    VxKeyboardThrottleSub() : mDelta(0.1), mVehicle(0) {}
    void notify(int inputSource,int sourceIndex, int inputAction, bool value){};
    void notify(int inputSource,int sourceIndex, int inputAction, int value){};

    void notify(int inputSource,int sourceIndex, int inputAction, float value);
    VxReal mDelta;
    VxVehicleBase* mVehicle;
};

class VxKeyboardBrakeSub : public VxViz::Subscriber
{
public:
    VxKeyboardBrakeSub(VxReal delta=100, VxReal max=1000) : mVehicle(0)
    { 
        mDelta = delta; mMax = max; 
    }
    
    void notify(int inputSource,int sourceIndex, int inputAction, bool value){};
    void notify(int inputSource,int sourceIndex, int inputAction, int value){};
    void notify(int inputSource,int sourceIndex, int inputAction, float value);

    VxReal mDelta;
    VxReal mMax;
    VxVehicleBase* mVehicle;
};

class VxKeyboardShifterSub : public VxViz::Subscriber
{
public:
    VxKeyboardShifterSub() : mTransmission(0) {}

    void notify(int inputSource,int sourceIndex, int inputAction, bool value){};
    void notify(int inputSource,int sourceIndex, int inputAction, int value){};

    void notify(int inputSource,int sourceIndex, int inputAction, float value);
    VxAutomaticNGearTransmission* mTransmission;
};


class VxCommonVehicleSub : public VxViz::Subscriber
{
public:
    VxCommonVehicleSub();

    void setDrivenVehicle(VxVehicle::VxWheeledVehicle* veh, VxViz* viz);
    void notify(int inputSource,int sourceIndex, int inputAction, bool value){};
    void notify(int inputSource,int sourceIndex, int inputAction, float value){};
    void notify(int inputSource,int sourceIndex, int inputAction, int value);

    int mCurrVehicleIndex;
    bool mDisplaySteering;


const int vehicleCount;
const int strykerCount;
const int bigLogCount;
const int smallLogCount;

VxReal throttleStart ;  //1; //0;
int shifterStartMin ;  //2; //1;
int shifterStartMax ;  //3; //1;

};



