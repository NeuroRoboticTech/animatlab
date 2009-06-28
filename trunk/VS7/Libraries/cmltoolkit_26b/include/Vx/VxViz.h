
#ifndef _VXVIZ_H_
#define _VXVIZ_H_

#include "Vx/Vx.h"
#include "Vx/VxConstraint.h"  // required by VxConstraintControllerSubscriber declaration
#include "Vx/VxUniverse.h"
#include "MeMath.h"


namespace Vx
{

struct HelpStringList;
struct LineList;
struct VxKey2Subscriber;
struct ContactList;
struct VxJoystickActionMap;
struct UserGraphics2VxData;

class VxUniverse;
class VxSensor;
class VxViewer;
class VxCollisionGeometry;
class VxPart;
class VxConstraint;
class VxTerrain;
class VxEntity;
class VxVizIntersectSubscriber;
class VxDynamicsContactInput;

#define PARTICLE_SYSTEM

class VxViz
{
public:
    // tolua_end
    VxViz();
    virtual ~VxViz();

    // Line utility functions
    typedef void Line;

    virtual Line* createLine(VxReal3Ptr p0, VxReal3Ptr p1, float color[4], VxReal44Ptr m=0)=0;
    virtual void addLine(MeVector3 from, MeVector3 to ,float color[4], VxReal44Ptr m = 0);
    virtual void createGroupNode(LineList *llist, VxCollisionGeometry *cg, VxNode parentNode)=0;
    virtual void moveLine(Line* line, VxReal3Ptr p0, VxReal3Ptr p1)=0;
    virtual void updateGraphicsTransform(VxNode gx, VxReal44Ptr matrix) = 0; 
    virtual void destroyLine(Line* line);
    int getLineCount(){ return mLinesCount;}
    void setLineLength(float len){mLineLength = len;};
    VxReal getLineLength(){return mLineLength;};
    void drawCrossHairAtPosition(Vx::VxReal3 p, Vx::VxReal size, float* color, Vx::VxReal44Ptr tm=0);
    void getLocalColor(float color[4], float *inColor);
    // Maps the user graphics to Vortex data. Usually gx node to VxVollisionGeometry.
    void addGraphicsToUserData(VxNode userGx,VxNode userData);
    void removeGraphicsToUserData(VxNode userGx);
    int  getGraphicsToUserDataCount();
    virtual VxNode getVxGeometryFromUserGraphics(VxNode *userGx);


    // For user controlled event loop
    virtual int update(); 
    virtual void preStepUpdate()=0;
    virtual void postStepUpdate()=0;

    // Subscriber functions
    class Subscriber
    {
        public:
        virtual void notify(int inputSource,int sourceIndex, int inputAction, bool value) = 0;
        virtual void notify(int inputSource,int sourceIndex, int inputAction, int value) = 0;
        virtual void notify(int inputSource,int sourceIndex, int inputAction, float value) = 0;
        virtual void reset(VxReal ) { }
     };

    class VxViewerInternalSub : public Subscriber
    {
    public:

        void notify(int inputSource,int sourceIndex, int inputAction, bool value);
        void notify(int inputSource,int sourceIndex, int inputAction, int value);
        void notify(int inputSource,int sourceIndex, int inputAction, float value);
        virtual void update();
    
        // those can be change manually, changes will take effect nect time mDisplayXXX will
        // be toggle to true;
        float mInertiaColor[4];
        float mGeometryColor[4];
        float mConstraintColor[2][4];
        float mContactColor[4];
        bool  mDisplayContact;
        float mConstraintScale;
        float mContactScale;
        float mInertiaScale;
        float mComScale;

    protected:
        friend class VxViz;

        VxViewerInternalSub(VxViz* viewer);
        ~VxViewerInternalSub() {}
        void _notify(int key);

        VxViz* mViewer;
        void updateDisplayConstraint();
        void updateDisplayPart();
        bool mAdded;
        bool mDisplayGeometry;
        bool mDisplayInertia;
        bool mDisplayConstraint;
        bool mDisplayNormal;
        bool mDisplayCOM;

    };
    VxViewerInternalSub* getInternalSub() { return mInternalSub; }


    // Collision geometry
    virtual void displayCollisionGeometry(VxCollisionGeometry* p, float color[4], char* textureName=0) = 0;
    virtual void displayFileASE(char* filename, float xScale, float yScale, float zScale,
                                 const float color[4], VxReal44Ptr matrix, char* textureName=0) = 0;
    void displayAllCollisionGeometries(VxUniverse* p) { displayAllCollisionGeometries(p,0,0); }
    void displayAllCollisionGeometries(VxUniverse* p, float colors[][4], int numColors);
    void displaySensorCollisionGeometry(VxSensor* s, float color[4]);
    void drawBBox(MeVector3 low, MeVector3 hi, float color[4], Vx::VxReal44Ptr tm=0);
    void removeCollisionGeometryDisplay(VxCollisionGeometry* geom);
    // Parts and Entities display
    void removeEntityDisplay(VxEntity* e);
    void removePartDisplay(VxPart* part) { removeEntityDisplay((VxEntity*)part); }

    // Graphics related functions
    virtual void setRContext(VxNode scene, VxNode rc =NULL);
    VxNode getRContext(){return mRc;}
    VxNode getScene(){return mScene;}
    virtual void deleteGraphicsObject(void *object, VxNode group, int is2d = 0)=0;
    virtual void setColor(void* graphicsData, const float *color)=0;  
    virtual void deleteAllGraphicsRefByUserData(void* userData);

    // Draw Constraints, COM, inertia
    void displayConstraint(VxConstraint* c, float color[2][4], float scale=1);
    void removeConstraintDisplay(VxConstraint* constraint);
    void displayAllConstraints(VxUniverse* u, float scale);
    virtual void displayCenterOfMass(VxPart* p, float scale=1);
    virtual void displayInertia(VxPart* p, float color[4], float scale=1);

    // Draw Contact types
    enum ContactTypeEnum {  ContactNormal=0, 
                            ContactForce, 
                            ContactTorque, 
                            ContactPenetration,
                            ContactDirection,
                            ContactEnumMax,
                            NoContact
                        };
    virtual void setContactColor(float color[4]) = 0;
    virtual void displayContacts(ContactTypeEnum type=NoContact);
    virtual void removeContacts();
    int getContactMaxCount(){ return mContactsMaxCount;}
    ContactTypeEnum getContactType(){ return mContactType;}

    // Draw mesh Normals
    void displayMeshNormals(bool on = false);


    // Text and Help functions
    virtual void createHelp()=0;
    virtual void toggleHelp()=0;
    virtual void displayHelp(bool b)=0;
    void addHelpString(const char* helpstring);

    class TextDisplay
    {
    public:
        TextDisplay() : x(0), y(0), disp(0) 
        {
            color[0] = 1.0f;
            color[1] = 1.0f;
            color[2] = 1.0f;
            color[3] = 0.7f;
        }
        friend class VxViz;
        int x, y;
        char str[128];
        float color[4];
        void* disp;
    };

    virtual TextDisplay *createTextDisplay(int x, int y, const char* str);
    virtual void updateTextDisplay(const char* str, TextDisplay *disp);
    virtual void updateTextDisplayObject(int x, int y, const char* str, TextDisplay *disp)=0;
    void removeTextDisplay(TextDisplay *);

    enum InputSourceEnum
    {
        Keyboard,
        Joystick,
        SourceCount
    };
    // Keyboard functions
    enum KeysEnum
    {
        UpArrow=300,
        DownArrow,
        LeftArrow,
        RightArrow,
        Enter,
        LeftShift,   
        RightShift,  
        LeftControl, 
        RightControl, 
        CapsLock, 
        ShiftLock,
        F1key,
        F2key,
        F3key,
        F4key,
        F5key,
        F6key,
        F7key,
        F8key,
        F9key,
        F10key,
        F11key,
        F12key,
        KeysCount
    };

    virtual bool initializeInputDevices() = 0;
    virtual void onKeyChange(KeysEnum key, int value = 0) = 0;

    void notifyInputSourceSubscribers(InputSourceEnum inputSource,int sourceIndex, int sourceType, int inputAction, float value); 
    virtual void addInputSubscriber(InputSourceEnum inputSource, int inputAction, Subscriber* s);
    virtual void addInputSubscriber(InputSourceEnum inputSource, int inputAction,const char* helpstring, Subscriber* s);
    virtual void removeInputSubscriber(InputSourceEnum inputSource, int inputAction, Subscriber* s);

    enum JoystickEnum
    {
        Axes,
        Ball,
        Hat,
        BtnDown,
        BtnUp,
        JoystickCount
    };

    VxUniverse* getUniverse() { return mUniverse; }
    bool isPaused() const { return mPaused && !mOneFrame; }
    void setPaused(bool b, bool doOneFrame=0) { mPaused = b; mOneFrame = doOneFrame; }

    void setDisplayConstraintType(int type) { mDisplayConstraintType = type; }

    void addInternalSubscribers();
    void removeInternalSubscribers();

protected:
    VxUniverse              *mUniverse;
    VxNode                  mRc;
    VxNode                  mScene;
    HelpStringList          *mHelpStringList;
    char                    **mHelpStrings;
    VxNode                  mJoystick;

    LineList                *mLineList;
    LineList                *mTempLinesGroup;
    LineList                *mContactMaxList;
    unsigned int            mNumLines;
    VxKey2Subscriber        *mKeyboardSubscriberMap;
    VxJoystickActionMap     *mJoystickSubscriberMap;
    UserGraphics2VxData     *mUserGraphics2VxDataMap;

    VxViewerInternalSub     *mInternalSub;
    VxVizIntersectSubscriber*mVxSensorIntersectSubscriber;

    bool                    mHelpCreated;

    VxReal                  mLineLength;
    int                     mLinesCount;
    unsigned int            mContactsMaxCount;
    ContactTypeEnum         mContactType;
    int                     mDisplayConstraintType;

    bool mPaused;
    bool mOneFrame;

    void _setUniverse(VxUniverse* universe);
    void _drawContactType(ContactTypeEnum type);
    Line *_getLineContact(int lineIndex);
    Line *VxViz::_getNewLineContact(VxReal3Ptr p0, VxReal3Ptr p1);
    Line *getLine(int lineIndex);

}; 


class VxVizConstraintControllerSubscriber : public VxViz::Subscriber
{
public:
    VxVizConstraintControllerSubscriber(VxConstraintController* c, VxReal dir, VxReal force=VX_INFINITY)
        : mController(c), mDir(dir), mForce(force) {}
    void notify(int inputSource,int sourceIndex, int inputAction, bool value){};
    void notify(int inputSource,int sourceIndex, int inputAction, int value){};
    void notify(int inputSource,int sourceIndex, int inputAction, float value)
                                   { mController->updateEvent(mDir, mForce); }
    VxConstraintController* mController;
    VxReal mDir;
    VxReal mForce;
};

class VxVizConstraintControllerSubscriber2 : public VxViz::Subscriber
{
public:
    VxVizConstraintControllerSubscriber2(VxConstraintController* c, int kup, VxReal force=VX_INFINITY)
        : mController(c), mK(kup), mForce(force) {}
    void notify(int inputSource,int sourceIndex, int inputAction, bool value){};
    void notify(int inputSource,int sourceIndex, int inputAction, int value){};
    void notify(int inputSource,int sourceIndex, int inputAction, float value)
    { 
        VxReal dir = inputAction == mK ? 1.0f : -1.0f;
        mController->updateEvent(dir, mForce);
    }
    virtual void reset( VxReal inVal ) { mController->reset(inVal); }
    VxConstraintController* mController;
    int mK;
    VxReal mForce;
};


} // namespace Vx

#endif // _VXVIZ_H_
