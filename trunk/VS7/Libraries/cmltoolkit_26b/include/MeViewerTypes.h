#ifndef _MEVIEWERTYPES_H
#define _MEVIEWERTYPES_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/01/28 15:52:46 $
   
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

#include "MePrecision.h"

#define CM_PROCESS_FRAMELOCK   40046
#define CM_STRICT_FRAMELOCK   40047
#define CM_NO_FRAMELOCK   40048
#define CM_CONTROLLED_FRAMELOCK   40049


#ifdef __cplusplus
extern "C"
{
#endif

typedef struct RMenu                RMenu;
typedef struct RMenuBaseEntry       RMenuBaseEntry;
typedef struct RMenuToggleEntry     RMenuToggleEntry;
typedef struct RMenuValueEntry      RMenuValueEntry;
typedef struct RMenuSubmenuEntry    RMenuSubmenuEntry;


#define RDeleteRenderContext RDestroyRenderContext /* deprecated */

/* These use lower case names for backwards compatibility with
   the old RRenderType enum */
typedef enum {
    kRDefault,
    kROpenGL,
    kRD3D,
    kRPs2,
    kRNull,
    kRBenchmark
} RRenderType;


/* render quality currently ignored
   typedef is here for backwards-compatibility */
typedef enum
{
    kRQualityWireframe = 0,
    kRFilled,
    kRQualityFlat,
    kRQualitySmooth
} RRenderQuality;

typedef enum
{
    kRAmbient,
    kRDirect1,
    kRDirect2,
    kRPoint
} RRenderLight;

typedef enum {
    kRType2D = 0,
    kRTypeRegular,
    kRTypeLine,
    kRTypeBillboard,
    kRTypeProcedural,
    kRTypeCount /* must be last, represents number of types */
} RGraphicType_enum;

typedef struct RObjectHeader
{ /* Size: 24 */
    MeMatrix4 m_Matrix; /* (composite) transformation matrix */ /* 0x0 */
    float m_ColorAmbient[4]; /* RGBA color  - cannot be double for OpenGL */ /* 0x40 */
    float m_ColorDiffuse[4];
    float m_ColorSpecular[4];
    float m_ColorEmissive[4];
    int m_nNumVertices; /* number of vertices used by object */
    int m_nTextureID; /* which texture should be applied */
    int m_bIsWireFrame; /* whether the object should be rendered as wireframe */
    RGraphicType_enum m_eType; /* type of object */
    AcmeReal m_SpecularPower;
} RObjectHeader
#ifdef PS2
__attribute__((aligned(16)))
#endif
    ;

typedef struct RObjectVertex
{ /* Size: 0x30 (for PS2) */
    AcmeReal m_X; /* x,y,z co-ords */
    AcmeReal m_Y;
    AcmeReal m_Z;
#ifdef PS2
    AcmeReal pack0;
#endif
    AcmeReal m_NX; /* x,y,z normals */
    AcmeReal m_NY;
    AcmeReal m_NZ;
#ifdef PS2
    AcmeReal pack1;
#endif
    AcmeReal m_U; /* u,v texture co-ords */
    AcmeReal m_V;
#ifdef PS2
    AcmeReal pack2;
    AcmeReal pack3;
#endif
} RObjectVertex
#ifdef PS2
__attribute__((aligned(16)))
#endif
    ;

typedef void (MEAPI *RUserDrawFn)(void *userData);

typedef struct RGraphic *RGraphicPtr;

typedef struct RGraphic
{
    /* anthing else that needs to associated with an individual
       object, but not passed to the final renderer goes here */

    RObjectHeader *m_pObject; /* pointer to associated Object */
    RObjectVertex *m_pVertices; /* points to the beginning of the vertex list. */
    /* This should immediately follow the RObjectHeader struct */
    /* see the object creation routines for detail */

    RGraphicPtr m_pNext; /* pointer to next in list */
    RGraphicPtr m_pNextByType; /* pointer to next by type list */
    /* RGraphicType_enum m_eType; *//* type */
    int m_nMaxNumVertices; /* number of vertices for which memory is allocated */
    MeMatrix4Ptr m_pLWMatrix; /* pointer to local->world matrix */
    RUserDrawFn m_pUserDrawFn;
    void* m_pUserDrawData;

} RGraphic;

typedef struct RRenderCallBacks *RRenderCallBacksPtr;

typedef struct RDirectLight
{ /* Size: 0x40 */
    AcmeReal m_Direction[3]; /* 0x0 */
    int m_bUseLight;
    AcmeReal m_rgbAmbient[3]; /* 0x10 */
    int pack0;
    AcmeReal m_rgbDiffuse[3]; /* 0x20 */
    int pack1;
    AcmeReal m_rgbSpecular[3]; /* 0x30 */
    int pack2;
} RDirectLight
#ifdef PS2
__attribute__((aligned(16)))
#endif
    ;

typedef struct RPointLight
{ /* Size: 0x50 */
    AcmeReal m_Position[3]; /* 0x0 */
    int m_bUseLight;
    float m_rgbAmbient[3]; /* 0x10 */ /* cannot be double! */
    int pack0;
    float m_rgbDiffuse[3]; /* 0x20 */
    int pack1;
    float m_rgbSpecular[3]; /* 0x30 */
    int pack2;
    AcmeReal m_AttenuationConstant; /* 0x40 */
    AcmeReal m_AttenuationLinear;
    AcmeReal m_AttenuationQuadratic;
    int pack3;
} RPointLight
#ifdef PS2
__attribute__((aligned(16)))
#endif
    ;


typedef struct RParticleSystem *RPSystemPtr;

typedef void (MEAPI *RParticleSystemGetParticleColorAndSizeCB)(void *userData, int particleID, float *colorOut, AcmeReal *size);

typedef struct RParticleSystem
{
    float m_rgbAmbient[4];
    float m_rgbDiffuse[4];
    float m_rgbSpecular[4];
    float m_rgbEmissive[4];
    AcmeReal m_TriangleSize;
    MeVector3 *m_Positions;
    int m_nNumParticles;
    int m_nTextureID;
    RParticleSystemGetParticleColorAndSizeCB m_pColorCB;
    void* m_pColorCBData;
    RPSystemPtr m_pNextSystem;
} RParticleSystem
#ifdef PS2
__attribute__((aligned(16)))
#endif
    ;

#if 0
typedef struct RProceduralObject *RProceduralObjectPtr;
typedef struct RProceduralObject
{
    float m_rgbAmbient[4];
    float m_rgbDiffuse[4];
    float m_rgbSpecular[4];
    float m_rgbEmissive[4];
    RProceduralFn m_fn;
    void * m_fnData;
    RProceduralObjectPtr m_pNext;
    MeMatrix4Ptr m_pLWMatrix; /* pointer to local->world matrix */
} RProceduralObject
#ifdef PS2
__attribute__((aligned(16)))
#endif
    ;
#endif



typedef struct RPerformanceBar
{
    RGraphic *m_DynBar;
    RGraphic *m_ColBar;
    RGraphic *m_RenBar;
    RGraphic *m_IdleBar;
    RGraphic *m_DynMaxBar;
    RGraphic *m_ColMaxBar;

    RGraphic *m_DynText;
    RGraphic *m_ColText;
    RGraphic *m_RenText;

    RGraphic *m_Scale0;
    RGraphic *m_Scale4;
    RGraphic *m_Scale8;
    RGraphic *m_Scale12;
    RGraphic *m_Scale16;

    RGraphic *m_Ticks;
} RPerformanceBar;


typedef struct RImage
{
    unsigned char    *m_pImage;
    int              m_height;
    int              m_width;
} RImage;

/*
  Options for a render context - the type, and whether or not to dump help
  information to the console.
 */
typedef struct RRenderOptions {
    RRenderType m_renderType;
    MeBool m_bDisplayHelpOnConsole;
    MeBool m_bWireFrame;
    MeBool m_bAlphaBlend;
    MeBool m_bTextures;
} RRenderOptions;

#define NUM_ACTION_CALLBACKS (40)
#define NUM_TEXTURES (25)

/* Structure for renderer globals */
typedef struct _RRender
{
    MeMatrix4 m_CamMatrix; /* current camera matrix */
    MeMatrix4 m_ProjMatrix; /* current projection matrix */
    MeMatrix4 m_CamMatrix2D;
    MeMatrix4 m_SkydomeMatrix;

    RDirectLight m_DirectLight1; /* 0x100 */
    RDirectLight m_DirectLight2; /* 0x140 */
    RPointLight m_PointLight; /* 0x180 */

    AcmeReal m_rgbAmbientLightColor[4]; /* 0xd0 */
    AcmeReal m_backgroundColour[4];

    AcmeReal m_CameraLookAt[3];

    RParticleSystem *m_pPS_First; /* pointer to first RParticleSystem in list */
    RGraphic *m_pRG_First; /* pointer to first RGraphic in linked list */
		int m_iTextureCount;
    char *m_TextureList[NUM_TEXTURES]; /* TextureID->Filename */
    int m_TextureTransparencyColor[NUM_TEXTURES][3];
    RImage m_TextureRGBA[NUM_TEXTURES]; /* rgba data pointers for in memory textures */
    RGraphic *m_pRG_First2D; /* pointer to first RGraphic in 2D list */
    RGraphic *m_pRG_FirstByType[kRTypeCount]; /* pointer to first typed RGraphic in list */

    int m_bUseAmbientLight;
    int m_bForceDirectLight1Update;
    int m_bForceDirectLight2Update;
    int m_bForcePointLightUpdate;
    int m_bPause;

    int m_bQuitNextFrame; /*Set to 1 if renderer is to quit next frame */
    AcmeReal m_CameraDist;
    AcmeReal m_CameraTheta;
    AcmeReal m_CameraPhi;

    AcmeReal m_AspectRatio; /*should be set to 640/448 for PS2 compatibility */
    AcmeReal m_Fov;
    RRenderCallBacksPtr m_pCallBacks;
    RGraphic        *m_UserHelpG;

    RGraphic        *m_fpsG;
		RGraphic				*m_timeG;
    int             m_isHelpDisplayed;
    RPerformanceBar *m_PerformanceBar;
    char    m_ButtonText[10+NUM_ACTION_CALLBACKS][20];

    char    m_AppName[40]; /* Name to be substituted for $APPNAME in text */
    int     m_nTimeout;
    int     m_bProfiling;
    int m_capabilities;

    RGraphic *m_PausedG;
		int			m_bDrawingFrame;

    /* platform independent renderer globals can go here */
    RMenu*  m_MenuCurrent;
    RMenu*  m_MenuDefault;
    RGraphic* m_Skydome;

    RRenderOptions m_options;
		AcmeReal	m_Fps;
    int m_bDisplayFps;

    MeU16 m_dirty;
    int m_keyModifiers;

		int m_bDisplayTime;
		long m_lTimeSlice;
		float m_fltStepIncrement;

		float m_fltPlaybackSpeed;
		int m_iSimStepsPerSecond;
		float m_fltRealTimeForStep;

		void *m_hWnd;

    int m_lastKey;

    MeU16 m_rightHanded;
    AcmeReal m_handSign;
    AcmeReal m_handSign2;
    MeU16 m_upAxisIndex; /* 1 for y, 2 for z */
    MeU16 m_inScreenAxisIndex; /* 2 for y up, 1 for z up */
    AcmeReal m_renderingTime;
    AcmeReal m_idleTime;

		AcmeReal m_fltNearZCameraPlane;
		AcmeReal m_fltFarZCameraPlane;
		AcmeReal m_fltZoomFactor;

    int m_ExternalLoop;
		MeBool m_bTrackingObject;
		
} RRender
#ifdef PS2
__attribute__((aligned(16)))
#endif
    ;

/* Globals */
extern char *R_LastObjectFilename;
extern char *R_ObjectFileBuffer;
extern int R_ObjectFileLength;
extern int R_CharacterWidths[10][10];

typedef enum {
    kREventNone = 0,
    kRNewlyPressed = 1,
    kRStillPressed,
    kRNewlyReleased
} RMouseButtonEvent;

typedef enum {
    kRLeftButton = 1,
    kRMiddleButton,
    kRRightButton
} RMouseButtonWhich;

typedef enum {
    kRButtonNull = 0,
    kRButtonDelete = 127,
    kRButtonLastASCII = 255,
    kRButtonUp, kRButtonFirstActionButton = kRButtonUp,
    kRButtonDown,
    kRButtonLeft,
    kRButtonRight,
    kRButtonUp2,
    kRButtonDown2,
    kRButtonLeft2,
    kRButtonRight2,
    kRButtonModMouse,
    kRButtonJoystick,
    kRButtonAction0,
    kRButtonAction5 = kRButtonAction0+5,
    kRButtonLastActionButton = kRButtonAction5, 

    kRButtonHome,
    kRButtonEnd,
    kRButtonPageUp,
    kRButtonPageDown,
    kRButtonInsert,

    kRButtonF1,
    kRButtonF2,
    kRButtonF3,
    kRButtonF4,
    kRButtonF5,
    kRButtonF6,
    kRButtonF7,
    kRButtonF8,
    kRButtonF9,
    kRButtonF10,
    kRButtonF11,
    kRButtonF12,
    kRButtons,

    kRButtonActionButtons = kRButtonLastActionButton - kRButtonFirstActionButton + 1,

} RButtonList;

/* Modifiers */

#define RCONTROL (1 << 0)
#define RSHIFT (1 << 1)

/* Not generally supported */
#define RALT (1 << 2)


/* Backend Additional Capabilities */
/* Not currently used */
#define RJOYSTICK (1 << 0)
#define RMOUSE (1 << 1)

#define RTEXT (1 << 2)

#define RLINES (1 << 3)
#define RLINESTRIPS (1 << 4)
#define RPOINTS (1 << 5)
#define RQUADSTRIPS (1 << 6)
#define RTRIFANS (1 << 7)
#define RTRISTRIPS (1 << 8)

#define RAMBIENT (1 << 9)
#define RDIFFUSE (1 << 10)
#define REMISSIVE (1 << 11)
#define RSPECULAR (1 << 12)

#define RWIREFRAME (1 << 13)
#define RFLATSHADED (1 << 14)
#define RSMOOTH (1 << 15)
#define RTEXTURES (1 << 16)

#define MENUS (1 << 17)
#define ASSIGNABLEBUTTONS (1 << 18)

/* Dirtiness bitfield for backends that need to know */

#define AMBLIGHT 1
#define DIRLIGHT1 2
#define DIRLIGHT2 4
#define POINTLIGHT 8
#define CAMERA 16

/* Function pointers and callbacks */
typedef void (MEAPI *RMainLoopCallBack)(RRender *rc, void *userdata);
typedef void (MEAPI *RButtonPressCallBack)(RRender *rc, void* userdata);
typedef int (MEAPI *RKeyPressCallBack)(RRender *rc, int modifiers, int key, void* userdata); /* return 1 if handled */
typedef void (MEAPI *RJoystickCallBack)(RRender *rc, AcmeReal x, AcmeReal y, void* userdata);
typedef void (MEAPI *RMouseCallBack)(RRender *rc, int x, int y,
			  int modifiers, RMouseButtonWhich which, RMouseButtonEvent event, void *userdata);

extern void (*RRenderSetPausedCallback)(RRender* rc, RMainLoopCallBack func, void *userdata);
extern void (*RRun)(RRender* rc, RMainLoopCallBack func, void *userdata);
extern int (*RInit)(RRender* rc);
extern void (*RConfigure)(RRender*rc, RMainLoopCallBack func, void *userdata);
extern int (*RUpdate)();

extern void (*RRenderSetWindowTitle)(RRender* rc, const char * title);
extern int (*RDumpWindowToFile)(RRender* rc, char*filename);

/* Menu Entry Callbacks */
typedef void (MEAPI *RMenuToggleCallback)(MeBool on);
typedef void (MEAPI *RMenuValueCallback)(MeReal value);
typedef void (MEAPI *RMenuToggleCallbackWithIndex)(MeBool on,    int index);
typedef void (MEAPI *RMenuValueCallbackWithIndex) (MeReal value, int index);

typedef void (MEAPI *RMenuEntryFunc)(RMenuBaseEntry* entry);
typedef void (MEAPI *RMenuEntryDisplayFunc)(RMenuBaseEntry* entry,
                         MeReal x, MeReal y);

typedef struct RCallBackDataButton
{
    RButtonPressCallBack m_CallBack;
    void* m_Userdata;
    char* m_Text;
} RCallBackDataButton;

typedef struct RCallBackDataJoystick
{
    RJoystickCallBack m_CallBack;
    void* m_Userdata;
    char* m_Text;
} RCallBackDataJoystick;

typedef struct RCallBackDataMouse
{
    RMouseCallBack m_CallBack;
    void* m_Userdata;
    char* m_Text;
} RCallBackDataMouse;

typedef struct RRenderCallBacks
{
    RCallBackDataButton   m_Button[kRButtons];
    RCallBackDataMouse    m_Mouse;
    RCallBackDataJoystick m_Joystick;
    RCallBackDataButton   m_Action[NUM_ACTION_CALLBACKS];

    unsigned char m_actionKeys[NUM_ACTION_CALLBACKS - 2];
} RRenderCallBacks;



/** Currently displayed menu (should be in MeViewer/App?) */
struct RMenuBaseEntry
{
    RMenu*                  menu;
    RMenuBaseEntry*         next;
    RMenuBaseEntry*         prev;

    char                    name[50];
    RGraphic*               nameGraphic;

    RMenuEntryDisplayFunc   display;
    RMenuEntryFunc          undisplay;

    RMenuEntryFunc          highlight;
    RMenuEntryFunc          unhighlight;

    RMenuEntryFunc          execute1;
    RMenuEntryFunc          execute2;
};

struct RMenuToggleEntry
{
    RMenuBaseEntry          base;

    RGraphic*               statusGraphic;
    MeReal                  statusPos[2];
    MeBool                  toggleValue;
    RMenuToggleCallback     toggleCallback;
/*BX*/
	int                     index;
    RMenuToggleCallbackWithIndex     toggleCallbackWithIndex;
/*EX*/

};

struct RMenuValueEntry
{
    RMenuBaseEntry          base;

    RGraphic*               valueGraphic;
    MeReal                  valuePos[2];
    MeReal                  hi;
    MeReal                  lo;
    MeReal                  increment;
    MeReal                  value;
    RMenuValueCallback      valueCallback;
/*BX*/
	int                     index;
    RMenuValueCallbackWithIndex     valueCallbackWithIndex;
/*EX*/

};

struct RMenuSubmenuEntry
{
    RMenuBaseEntry          base;

    RMenu*                  subMenu;
};

struct RMenu
{
    RRender*                rc;
    char                    name[50];
    RGraphic*               nameGraphic;
    RGraphic*               helpGraphic;
    RMenuBaseEntry*         entryHead;
    RMenuBaseEntry*         entryCurrent;
    RMenu*                  parentMenu;
    float                titleColor[4];
    float                textColor[4];
    float                highlightColor[4];
};

#ifdef __cplusplus
}
#endif

#endif
