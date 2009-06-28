
#ifndef _VXVIEWER_H_
#define _VXVIEWER_H_

#include "Vx/Vx.h"
#include "Vx/VxConstraint.h"  // required by VxConstraintControllerSubscriber declaration

struct MeApp;
struct _RRender;
struct RPerformanceBar;

namespace Vx
{

struct HelpStringList;
struct LineList;
struct VxKey2Subscriber;

class VxUniverse;
class VxSensor;
class VxViewer;
class VxCollisionGeometry;
class VxPart;
class VxConstraint;
class VxTerrain;
class VxEntity;

#define PARTICLE_SYSTEM

// tolua_begin
class VxViewerBase
{
public:
    // tolua_end
    VxViewerBase();
    virtual ~VxViewerBase();

    struct Line { void* ptr; };

    class Subscriber
    {
        public:
            virtual void notify(int key) = 0;
    };

    class VxViewerInternalSub : public Subscriber
    {
    public:

        void notify(int key);
    
        // those can be change manually, changes will take effect nect time mDisplayXXX will
        // be toggle to true;
        float mInertiaColor[4];
        float mComColor[4];
        float mGeometryColor[4];
        float mConstraintColor[2][4];
        float mContactColor[4];
        float mConstraintScale;
        float mContactScale;
        float mInertiaScale;
        float mComScale;

    protected:
        friend class VxViewerBase;

        VxViewerInternalSub(VxViewerBase* viewer);
        ~VxViewerInternalSub() {}

        VxViewerBase* mViewer;
        void updateDisplayPart();
        void updateDisplayConstraint();

        bool mAdded;
        bool mDisplayGeometry;
        bool mDisplayInertia;
        bool mDisplayContact;
        bool mDisplayConstraint;
        bool mDisplayCOM;

    };


    // tolua_begin
    virtual void setUniverse(VxUniverse* universe);

    virtual int update()=0; // for user controlled event loop

    void addDisplayCollisionGeometryFromUniverse(VxUniverse* p) { addDisplayCollisionGeometryFromUniverse(p,0,0); }
    void addDisplayCollisionGeometryFromSensor(VxSensor* s, float color[4]);
    // tolua_end
    void addDisplayCollisionGeometryFromUniverse(VxUniverse* p, float colors[][4], int numColors);
    // tolua_begin

    void addDisplayCenterOfMass(VxPart* p, float color[4], float scale=1);
    void addDisplayInertia(VxPart* p, float color[4], float scale=1);
    // tolua_end
    void addDisplayConstraint(VxConstraint* c, float color[2][4], float scale=1);
    // tolua_begin

    void addDisplayConstraintFromUniverse(VxUniverse* c, float scale);
    virtual void addDisplayCollisionGeometry(VxCollisionGeometry* p, float color[4], char* textureName=0) = 0;
    // tolua_end

    virtual void addDisplayFileASE(char* filename, float xScale, float yScale, float zScale,
                                 const float color[4], VxReal44Ptr matrix, char* textureName=0) = 0;

    void removeConstraintDisplay(VxConstraint* constraint);
    void removeEntityDisplay(VxEntity* e);
    void removePartDisplay(VxPart* part) { removeEntityDisplay((VxEntity*)part); }
    void removeCollisionGeometryDisplay(VxCollisionGeometry* geom);
    virtual void deleteGraphicsObject(void *object, int is2d)=0;
    virtual void removeGraphicsRelatedToUserData(void* userData)=0;

    virtual Line* createLine(VxReal3Ptr p0, VxReal3Ptr p1, float color[4], VxReal44Ptr m=0, void* userData=0)=0;
    virtual void moveLine(Line* line, VxReal3Ptr p0, VxReal3Ptr p1)=0;
    virtual void deleteLine(Line* line)=0;

    // tolua_begin
    virtual void setColor(void* graphicsData, const float *color)=0;
    void addHelpString(const char* helpstring);
    virtual void createHelp()=0;
    virtual void toggleHelp()=0;
    virtual void displayHelp(bool b)=0;

    // tolua_end


    virtual void addDrawLine(MeVector3 from, MeVector3 to, float color[4], VxReal44Ptr m=0)=0;
    void addDrawBBox(MeVector3 low, MeVector3 hi, float color[4], Vx::VxReal44Ptr tm=0);
    void addDrawPosition(Vx::VxReal3 p, Vx::VxReal size, float* color, Vx::VxReal44Ptr tm=0);

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
        friend class VxViewerBase;
        int x, y;
        char str[128];
        float color[4];
        void* disp;
    };

    virtual TextDisplay *createTextDisplay(int x, int y, char* str);
    virtual void updateTextDisplay(char* str, TextDisplay *disp);
    virtual void updateTextDisplayObject(int x, int y, char* str, TextDisplay *disp)=0;
    void deleteTextDisplay(TextDisplay *);

    enum KeysEnum
    {
        UpArrow = 101,
        DownArrow = 103,
        LeftArrow = 100,
        RightArrow = 102,
        KeysCount
    };

    virtual void addActionKeySubscriber(char c, const char* helpstring, Subscriber* s) = 0;
    virtual void addActionKeySubscriber(KeysEnum key, const char* helpstring, Subscriber* s) = 0;
    virtual void removeActionKeySubscriber(char key, Subscriber* s) = 0;

    enum DrawContactTypeEnum {  DrawContactNormal=0, 
                                DrawContactForce, 
                                DrawContactTorque, 
                                DrawContactPenetration,
                                DrawContactDirection,
                                DrawContactEnumMax};
    virtual void drawContactLength(float len) = 0;
    virtual void drawContactColor(float color[4]) = 0;

    virtual void drawContactEnable(bool b=true) = 0;
    virtual bool drawContactEnabled() = 0;
    virtual void drawContactType(DrawContactTypeEnum type) = 0;

    void addInternalSubscribers(bool addHelp=true);
    VxViewerInternalSub* getInternalSub() { return mInternalSub; }

    VxUniverse* getUniverse() { return mUniverse; }

protected:
    VxUniverse* mUniverse;

    HelpStringList* mHelpStringList;
    char** mHelpStrings;

    LineList* mLineList;
    unsigned int mNumLines;
    VxKey2Subscriber* mSubscriberMap;

    VxViewerInternalSub* mInternalSub;

    bool mHelpCreated;

}; // tolua_export


// tolua_begin
class VxViewer : public VxViewerBase
{
public:
    VxViewer();
    ~VxViewer();

    // tolua_end
    typedef void (MEAPI *VxMainLoopCB)(VxViewer *viewer, void *userdata);
    bool configure(int argc, const char *argv[], int upAxisZ=0, int righthanded=0);
    // tolua_begin
    bool configure() { return configure(0,0,1,1); }

    void setWindowTitle(const char* title);
    void enablePerformanceBar(bool b);
    void mouseSpringEnable(bool b);

    void setUniverse(VxUniverse* universe);
    void addDisplayCollisionGeometry(VxCollisionGeometry* p, float color[4], char* textureName=0);
    // tolua_end
    void addDisplayCollisionTerrainFromTriangleTag(VxCollisionGeometry* c, float color[][4], int maxMaterialCount);
    // tolua_begin

    void addDisplayFile(const char *filename, VxReal xScale, VxReal yScale, VxReal zScale,
                       const float color[4], VxReal44Ptr matrix, bool is2D=false, 
                       bool bKeepAspectRatio=1, char* textureName=0);

    void addDisplayFileASE(char* filename, float xScale, float yScale, float zScale,
                                 const float color[4], VxReal44Ptr matrix, char* textureName=0);
    // tolua_end
#ifdef PARTICLE_SYSTEM
    void* addParticleSystem(int numParticles, VxReal3 *positions,
                           const char *tex_filename, const float color[4], VxReal tri_size );
    void setParticleCount(void* stream, int count);
    void createTextureFromRGBA(const char* fname,unsigned char *texture,int width, int height);
    void setParticleSystemSetColorAndSizeCB(void *ps, void* cb, void* particleGraphicInfo);
#endif
    // tolua_begin
    void setColor(void* graphicsData, const float *color);

    void setCameraView(float a, float b, float c);
    void setCameraLookAt(VxReal3 target);
    void setCameraLookAtAndPosition(VxReal3 target, VxReal3 pos);
    VxReal3Ptr getCameraPosition(VxReal3Ptr pos);
    void setPaused(bool paused);
    bool getPaused();

    void run();

    virtual int update(); // for user controlled event loop
    // tolua_end
    void setUpdateCallBack(VxMainLoopCB cb, void* userData);

    void run(VxMainLoopCB cb, void* userData);
    VxMainLoopCB getVxMainLoopCB() { return mVxMainLoopCB; }
    void* getMainLoopCBUserData() { return mMainLoopCBUserData; }
    // tolua_begin
    void step();

    // tolua_end
    
    virtual void addDrawLine(MeVector3 from, MeVector3 to, float color[4], VxReal44Ptr m=0);
    Line* createLine(VxReal3Ptr p0, VxReal3Ptr p1, float color[4], VxReal44Ptr m=0, void* userData=0);
    void moveLine(Line* line, VxReal3Ptr p0, VxReal3Ptr p1);
    void deleteLine(Line* line);

    // tolua_begin
    void drawContactInit(float color[4], int maxContact=100);
    void drawContactLength(float len);
    void drawContactColor(float color[4]);
    void drawContactEnable(bool b=true);
    bool drawContactEnabled();
    void drawContactType(DrawContactTypeEnum type);

    // tolua_end
    void addActionKeySubscriber(char c, const char* helpstring, Subscriber* s);
    void addActionKeySubscriber(KeysEnum key, const char* helpstring, Subscriber* s);
    void removeActionKeySubscriber(char key, Subscriber* s);
    //void addActionKeyCB(char c, const char* helpstring, void* func, void* userData);
    void buttonCB(int button);
    void buttonCB2(int button);

    // tolua_begin
    void createHelp();
    void toggleHelp();
    virtual void displayHelp(bool b);

    // tolua_end

    void removeGraphicsRelatedToUserData(void* userData);


    TextDisplay *createTextDisplay(int x, int y, char* str);
    virtual void updateTextDisplayObject(int x, int y, char* str, TextDisplay *disp);

    int getUpAxis() const { return mUpAxis; }

    VxReal getRenderingTime() const;
    VxReal getIdleTime() const;

protected:
    void deleteGraphicsObject(void *object, int is2d);

    MeApp* mMeapp;
    _RRender* mRc;
    bool mRConfigured;
    int mUpAxis;


    RPerformanceBar *mPerformanceBar;

    int nkeyCounter;

    void* mMainLoopCBUserData;
    VxMainLoopCB mVxMainLoopCB;
}; // tolua_export


class VxConstraintControllerSubscriber : public VxViewer::Subscriber
{
public:
    VxConstraintControllerSubscriber(VxConstraintController* c, VxReal dir, VxReal force=VX_INFINITY)
        : mController(c), mDir(dir), mForce(force) {}
    void notify(int key) { mController->updateEvent(mDir, mForce); }
    VxConstraintController* mController;
    VxReal mDir;
    VxReal mForce;
};

class VxConstraintControllerSubscriber2 : public VxViewer::Subscriber
{
public:
    VxConstraintControllerSubscriber2(VxConstraintController* c, int kup, VxReal force=VX_INFINITY)
        : mController(c), mK(kup), mForce(force) {}
    void notify(int key) { 
        VxReal dir = key == mK ? 1.0f : -1.0f;
        mController->updateEvent(dir, mForce);
    }
    VxConstraintController* mController;
    int mK;
    VxReal mForce;
};

} // namespace Vx

#endif // _VXVIEWER_H_
