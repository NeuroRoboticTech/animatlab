// vsControlStick.h: interface for the vsControlStick class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _VSCONTROLSTICK_H
#define _VSCONTROLSTICK_H

#include "Vx/Vx.h"

struct vsCONTROLLERS;
struct vsMOTION2SRC;

//typedef void  (*motionCB)(void* event, void *userData);
typedef void  (*onJoystickMotion)(void* event, void *userData);
typedef void  (*onJoystickNotify)(int sourceIndex, int motionIndex, float value);


class VxJoystick
{
public:
    VxJoystick(){}
    virtual ~VxJoystick(){}
    
    virtual void joystickAxes(int sourceIndex, int motionIndex, float value){};
    virtual void joystickBall(int sourceIndex, int motionIndex, float value){};
    virtual void joystickHat(int sourceIndex, int motionIndex, int value){};
    virtual void joystickButtonDown(int sourceIndex, int motionIndex, float value = 1){}
    virtual void joystickButtonUp(int sourceIndex, int motionIndex, float value = 0){};

};

class vsEvent
{
public:
	vsEvent():mType(-1),mSrcIndex(-1), mMotionIndex(-1),valBool(0)
	{valInt[0] =0;valInt[1] =0;valFloat[0] =0;valFloat[1] =0;};
	virtual ~vsEvent(){};

	int mType;
	int mSrcIndex;/* The joystick device index */
	int mMotionIndex; /* axis index(0,1), Hat button index (0-7),button index (0,1) */ 

	bool valBool;
	int  valInt[2];/* range: -32768 to 32767 */
	float valFloat[2];

};


class vsControlStick
{
public:
	vsControlStick();
	virtual ~vsControlStick();

enum eControllerMotion
{			
	kJoyAxis =0, /* Joystick axis motion */
	kJoyBall, /* Joystick trackball motion */
	kJoyHat, /* Joystick hat position change */
	kJoyButtonDown, /* Joystick button pressed */
	kJoyButtonUp, /* Joystick button released */
		
};
	virtual bool initialize();
	bool update();
	virtual void close();
	vsControlStick	* getController(int i);
    VxNode getControlStick(int index);

	vsControlStick *addController(VxNode controler);
	int		getControllerCount();
	void setEvent(vsEvent *event) { mEvent = event;}
	vsEvent *&getEvent(){return mEvent;} 
	onJoystickMotion getMotionCB(int motionType);
	void registerMotionCB(int motionType, onJoystickMotion cb, void * userData);
protected:
    VxNode mControler;
	vsEvent *mEvent;
    static void * mUserData;
	static vsCONTROLLERS	*controlSticks;
	// For later! Maps the type of motion (axes, buttons etc.) 
	// to the callback function.
	vsMOTION2SRC	*motionSrcs;

};

#endif //_VSCONTROLSTICK_H