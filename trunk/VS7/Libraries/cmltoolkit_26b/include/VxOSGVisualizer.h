#ifndef _VX_OSG_VISUALIZER_H_
#define _VX_OSG_VISUALIZER_H_

// VxOSGVisualizer.h: interface for the VxOSGVisualizer class.
//
//////////////////////////////////////////////////////////////////////
// Vx includes
#include "Vx/Vx.h"
#include "Vx/VxViz.h"

#include <osg/Node>
#include <osgProducer/Viewer>

class sdlJoystick;
struct MeshLineObjects;

namespace osg {
    class Group;
    class MatrixTransform;
    class Projection;
    class Geode;
}
namespace osgText {
    class Font;
}

class VxOSGPickHandler;

class VxOSGVisualizer : public VxViz 
{
public:
    VxOSGVisualizer();
    virtual ~VxOSGVisualizer();
    // OSG primitives
    osg::Node *createOSGBox(VxReal3 v, float color[4],VxReal3Ptr pos = 0);
    osg::Node *createOSGSphere(VxReal r, float color[4],VxReal3Ptr pos = 0);
    osg::Node *createOSGCylinder(VxReal r,VxReal l, float color[4],VxReal3Ptr pos = 0);
    osg::Node *createOSGCone(float r, float h, float color[4],VxReal3Ptr pos = 0);


    // Line utility functions
     Line* createLine(VxReal3Ptr p0, VxReal3Ptr p1, float color[4], VxReal44Ptr m=0);
     void moveLine(Line* line, VxReal3Ptr p0, VxReal3Ptr p1);
     void createGroupNode(LineList *nlist, VxCollisionGeometry *cg, VxNode parentNode);
     void deleteGraphicsObject(void *object, VxNode group, int is2d = 0);

     // User controlled event loop
     int update(); 
     void updateGraphicsTransform(VxNode gx, VxReal44Ptr matrix); 
     void preStepUpdate();
     void postStepUpdate(){};

     void setRContext(osg::Node* scene, osgProducer::Viewer *rc = 0)
     {
         VxViz::setRContext((osg::Node*)scene,(osgProducer::Viewer *)rc);
         enableMouseSpring(true);
     }

     void displayCollisionGeometry(VxCollisionGeometry* p, float color[4], char* textureName=0);
     void displayFileASE(char* filename, float xScale, float yScale, float zScale,
                         const float color[4], VxReal44Ptr matrix, char* textureName=0){};

     void setColor(void* graphicsData, const float *color){};
     // Displaying Text and help
     void createHelp(){};
     void toggleHelp(){};
     void displayHelp(bool b){};

     TextDisplay *createTextDisplay(int x, int y, const char* str);
     void updateTextDisplayObject(int x, int y, const char* str, TextDisplay *disp);
     // Keyboard related functions
     bool initializeInputDevices();
     void onKeyChange(KeysEnum key, int value = 0);

     void enableMouseSpring(bool enable);

     // Contact
     void setContactColor(float color[4]){};

     osgProducer::Viewer * getOSGViewer() { return (osgProducer::Viewer *)mRc; }

     //
protected:

    void initializeText();
    osg::Group *mTextNode;
    osg::Geode* mTextGeode;
    osgText::Font* mFont;
    float mTextColor[4];
    float mCharacterSize;

    VxOSGPickHandler *mPicker;
};

#endif // _VX_OSG_VISUALIZER_H_
