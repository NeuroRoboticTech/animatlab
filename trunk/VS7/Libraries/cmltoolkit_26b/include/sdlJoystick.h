// sdlJoystick.h: interface for the sdlJoystick class.
//
//////////////////////////////////////////////////////////////////////

#include "vsControlStick.h"

class sdlJoystick: public vsControlStick  
{
public:
	sdlJoystick(VxJoystick *userData = 0);
	virtual ~sdlJoystick();
    static sdlJoystick *instance();

    void setVxJoystickCB(VxJoystick *vxj){mUserData = vxj;};
	bool update();
	void close();
    int getJoystickCount();

protected:
    bool _initialize();
    static sdlJoystick *s_sdlJoystick;
};
