// osgKeyboard.h: interface for the osgKeyboardModel class.
//
//////////////////////////////////////////////////////////////////////
#include <osgProducer/Viewer>
#include <Producer/Camera>

#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Switch>
#include <osg/Notify>
#include <osg/Geometry>
#include <osg/Node>

#include <osgText/Text>

#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/WriteFile>
#include "Vx/VxViz.h"

class Vx::VxViz;

class osgKeyboardEventHandler : public osgGA::GUIEventHandler
{
public:
    
    osgKeyboardEventHandler(Vx::VxViz *vis){_mVxViz = vis;}
        virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us)
        {
            int key = ea.getKey();
            bool ret = false;

            // disable some osg keys, should make them wotk with alt...
            switch (key)
            {
            case 'z':
            case 'Z':
            case 's':
            case 'f':
            case 't':
            case 'b':
            case 'h':
            /*
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            */
                ///if (keymod...)
                ret = true;
                break;
            case osgGA::GUIEventAdapter::KEY_F9:

                unsigned int width, height, wr, hr;
                static int ww(0),wh(0);
                osgProducer::Viewer* viewer = ((VxOSGVisualizer*)_mVxViz)->getOSGViewer();
                if (!viewer) return false;

                Producer::Camera *cam = viewer->getCamera(0);
                int camCount = viewer->getNumberOfCameras();
                Producer::RenderSurface* rs = cam->getRenderSurface();

                if (ww == 0 && wh == 0)
                {
                    ww = rs->getWindowWidth();
                    wh = rs->getWindowHeight();
                }

                if(ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN)
                {
                    
                    static VxReal4 *renderPos = 0;
                    static bool full = false;
                    static bool tiled = false;
                    static int x(0), y(0);
                    int xw(0), yw(0);

					if (x==0 ||y==0)
					{
                        getDesktopMaxSize(viewer , &x, &y);
                        rs->getWindowRectangle(xw,yw,width,height );
						renderPos = getWindowRenderArea(viewer,x/width, y/height);						
					}

                    rs->getWindowRectangle(xw,yw,wr,hr );
                    if (tiled)
                    {
                        //rs->setWindowRectangle( 0,0, 2560, 1024 );
                        //rs->setWindowRectangle( 0,0, x, y);
                        tileWindows(viewer,x/wr, y/hr);               
                        //splitOSGWindow(viewer,renderPos,camCount);               
                        
                        tiled = false;
                    }
                    else
                    {
                        //rs->setWindowRectangle( 1281,0, 1280, 1024 );
//                        rs->setWindowRectangle( 0,0, width, height );
						collapseWindows(viewer,xw/wr, yw/hr, ww, wh);
                        tiled = true;
                    }

                    rs->getScreenSize(width , height );
                    // full size windows
                    if (wr == width && hr == height)
                    {
                        full = true;
                    }
                    else// reduced windows
                        full = false;

#ifndef WIN32                    
                        rs->useBorder(!full);
#else                        
//                        rs->fullScreen(full);
#endif
                }
                break;

            }

            switch(ea.getEventType())
            {
                case(osgGA::GUIEventAdapter::KEYDOWN):
                {
                    _mVxViz->onKeyChange((VxViz::KeysEnum)key,1);
                    break;
                }
                case(osgGA::GUIEventAdapter::KEYUP):
                {
                    _mVxViz->onKeyChange((VxViz::KeysEnum)key,0);
                    us.requestRedraw();
                    break;
                }
                default:
                    break;
            }

           return ret;
        }

        virtual void accept(osgGA::GUIEventHandlerVisitor& v)
        {
            v.visit(*this);
        }
        
        Vx::VxViz  *_mVxViz;

};

