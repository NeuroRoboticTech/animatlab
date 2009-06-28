// VsSimulator.cpp: implementation of the VsSimulator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdarg.h>
#include "VsClassFactory.h"
#include "VsJoint.h"
#include "VsRigidBody.h"
#include "VsSimulationRecorder.h"
#include "VsSimulator.h"

#define WM_MOUSEWHEEL	0x020A

namespace VortexAnimatLibrary
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//Global variable that keeps track of whether a vortex simulation error has occurred and what it was.
string g_strVortexErrorMessage;
BOOL g_bVortexError = FALSE;

VsSimulator::VsSimulator()
{
	m_iUniverse = NULL;
	m_iWorld = NULL;
	m_lpApp = NULL;
	m_iBridge = NULL;
	m_lpRenderContext = NULL;
	m_lpTrackBody = NULL;

	m_aryGroundTM[0][0] = 1;
	m_aryGroundTM[0][1] = 0;
	m_aryGroundTM[0][2] = 0;
	m_aryGroundTM[0][3] = 0;
		
	m_aryGroundTM[1][0] = 0;
	m_aryGroundTM[1][1] = 0;
	m_aryGroundTM[1][2] = -1;
	m_aryGroundTM[1][3] = 0;
		
	m_aryGroundTM[2][0] = 0;
	m_aryGroundTM[2][1] = 1;
	m_aryGroundTM[2][2] = 0;
	m_aryGroundTM[2][3] = 0;
		
	m_aryGroundTM[3][0] = 0;
	m_aryGroundTM[3][1] = -30;
	m_aryGroundTM[3][2] = 0;
	m_aryGroundTM[3][3] = 1;

	m_lTimer = 0;
}

VsSimulator::~VsSimulator()
{

try
{
	Reset();
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of Simulator\r\n", "", -1, FALSE, TRUE);}
}

MstUniverseID VsSimulator::Universe()
{return m_iUniverse;}

MdtWorldID VsSimulator::World()
{return m_iWorld;}

MeApp *VsSimulator::App()
{return m_lpApp;}

RRender *VsSimulator::RenderContext()
{return m_lpRenderContext;}
	
VsRigidBody *VsSimulator::TrackBody()
{return m_lpTrackBody;}

void VsSimulator::PlaybackRate(float fltVal)
{
	if(fltVal<0)
		THROW_PARAM_ERROR(Al_Err_lInvalidPlaybackRate, Al_Err_strInvalidPlaybackRate, "PlaybackRate", fltVal);

	m_fltPlaybackRate = fltVal;

	if(m_lpRenderContext)
		RRenderSetPlaybackRate(m_lpRenderContext, fltVal);
}

void VsSimulator::UseAlphaBlending(BOOL bVal)
{
	m_bUseAlphaBlending = bVal;

	if(m_lpRenderContext)
		RRenderSetAlphaBlend3D(m_lpRenderContext, m_bUseAlphaBlending);
}

void VsSimulator::TrackCamera(BOOL bTrackCamera, string strLookAtStructureID, string strLookAtBodyID)
{
	m_bTrackCamera = bTrackCamera;
	m_strLookAtStructureID = strLookAtStructureID;
	m_strLookAtBodyID = strLookAtBodyID;

	if(m_bTrackCamera)
	{
		m_lpTrackBody = NULL;

		if(!Std_IsBlank(m_strLookAtStructureID))
		{
			Structure *lpStructure = FindStructureFromAll(m_strLookAtStructureID);

			VsRigidBody *lpVsBody = NULL;
			RigidBody *lpBody=NULL;
			if(Std_IsBlank(m_strLookAtBodyID))
				lpBody = lpStructure->Body();
			else
				lpBody = lpStructure->FindRigidBody(m_strLookAtBodyID);

			if(lpBody)
			{
				lpVsBody = dynamic_cast<VsRigidBody *>(lpBody);
				MdtBodyGetPosition(lpVsBody->BodyID(), m_oTrackPos);
				RCameraSetLookAt(m_lpRenderContext, m_oTrackPos);
				
				m_lpTrackBody = lpVsBody;
			}
		} 
	}
	else
		m_lpTrackBody = NULL;

	if(!m_lpRenderContext)
	{
		if(m_lpTrackBody)
			RCameraTrackingObject(m_lpRenderContext, TRUE);
		else
			RCameraTrackingObject(m_lpRenderContext, FALSE);
	}
}

SimulationRecorder *VsSimulator::CreateSimulationRecorder()
{
	return new VsSimulationRecorder;
}

void VsSimulator::Reset()
{
	Simulator::Reset();

	if(m_bManualStepSimulation && m_lpRenderContext)
		RRenderRunCleanup(m_lpRenderContext);

	if(!m_lpAnimatClassFactory) 
		m_lpAnimatClassFactory = new VsClassFactory;

	if(m_iUniverse) 
		{MstUniverseDestroy(m_iUniverse); m_iUniverse = NULL;}

	if(m_lpApp) 
		{MeAppDestroy(m_lpApp); m_lpApp=NULL;}

	if(m_lpRenderContext) 
		{RRenderContextDestroy(m_lpRenderContext); m_lpRenderContext = NULL;}

	m_lpTrackBody = NULL;  //Do not delete this item.

	//Reset the vortex error message info
	g_strVortexErrorMessage = "";
	g_bVortexError = FALSE;
}


// tick() is a callback function called from the renderer's main loop
// to evolve the m_iWorld by 'step' seconds
void MEAPI tick(RRender * m_lpRenderContext, void* userData)
{
	VsSimulator *lpSim = (VsSimulator *)(userData);
	lpSim->StepSimulation();
}


/* Callback which is called for new colliding model pairs (that were 
   not colliding at the previous timestep. */
void MEAPI SensorContactBegin(McdIntersectResult* result, void **c, void *userData)
{
	VsSimulator *lpSim = (VsSimulator *)(userData);
	RigidBody *lpBody1 = (RigidBody *) McdModelGetUserData(result->pair->model1);
	RigidBody *lpBody2 = (RigidBody *) McdModelGetUserData(result->pair->model2);

	if(lpBody1 && lpBody2 && lpSim)
	{
		McdContact lpContact; 
		for(int iIndex=0; iIndex<result->contactCount; iIndex++)
		{
			lpContact = result->contacts[iIndex];
		}

		if(lpBody1->IsContactSensor())
			lpBody1->AddSurfaceContact(lpSim, lpBody2);
		else if(lpBody2->IsContactSensor())
			lpBody2->AddSurfaceContact(lpSim, lpBody1);
	}
}

void MEAPI SensorContactContinue(McdIntersectResult* result, void **c, void *userData)
{
	VsSimulator *lpSim = (VsSimulator *)(userData);
	RigidBody *lpBody1 = (RigidBody *) McdModelGetUserData(result->pair->model1);
	RigidBody *lpBody2 = (RigidBody *) McdModelGetUserData(result->pair->model2);

	if(lpBody1 && lpBody2 && lpSim)
	{
		McdContact lpContact; 
		for(int iIndex=0; iIndex<result->contactCount; iIndex++)
		{
			lpContact = result->contacts[iIndex];
		}
	}
}

/* Callback which is called for new non-colliding model pairs (that were 
   colliding at the previous timestep and are not colliding anymore. */
void MEAPI SensorContactEnd(McdModelPairID pair, void *userData)
{
	VsSimulator *lpSim = (VsSimulator *)(userData);
	RigidBody *lpBody1 = (RigidBody *) McdModelGetUserData(pair->model1);
	RigidBody *lpBody2 = (RigidBody *) McdModelGetUserData(pair->model2);

	if(lpBody1 && lpBody2 && lpSim)
	{
		if(lpBody1->IsContactSensor())
			lpBody1->RemoveSurfaceContact(lpSim, lpBody2);
		else
			lpBody2->RemoveSurfaceContact(lpSim, lpBody1);
	}
}

MeCommandLineOptions *CreateDefaultOptions()
{
	MeCommandLineOptions *options = (MeCommandLineOptions *) (MeMemoryAPI.create(sizeof(MeCommandLineOptions)));

	if (!options)
			MeFatalError(0, "Unable to allocate memory for command line options\n");
	options->m_argc = 1;
	options->p_argv = (const char **) (MeMemoryAPI.create(sizeof(char*) * (options->m_argc)));
	if (!(options->p_argv))
			MeFatalError(0, "Unable to allocate memory for command line options\n");
	options->p_argv[0] = "-d3d";
	//options->p_argv[1] = "-alphablend";
	return options;
}

HWND g_hSimulationWnd = NULL;
HWND g_hParentWnd = NULL;
WNDPROC g_PrevWndProc = NULL;
HWND g_hClientWnd=NULL;
HWND g_hFrameWnd=NULL;
int g_iHi=80, g_iLo=0;

long WINAPI MyWndProc(HWND win, UINT msg, WPARAM wparam, LPARAM lparam)
{
	long lVal;
	RECT rcScreenRect;
	string strVal;
	LPARAM lNewParam;
	POINT ptPoint;

	switch( msg )
  { 
  case WM_KEYDOWN:
  case WM_KEYUP: 
  case WM_CHAR:
	case WM_MOUSEWHEEL:
		RRenderWndProc(win, msg, wparam, lparam);
    break;

  case WM_PAINT:
		UpdateWindow(g_hSimulationWnd);			
    break;

  case WM_MOVE:
		ptPoint.x=0; ptPoint.y=0;
		MapWindowPoints(g_hParentWnd, NULL, &ptPoint, 1);
		lNewParam = MAKELPARAM(ptPoint.x, ptPoint.y);
		RRenderWndProc(win, msg, wparam, lNewParam);
		break;

	case WM_SIZE:
		GetClientRect(win, &rcScreenRect);

		if(rcScreenRect.right > 10 && rcScreenRect.bottom > 10)
		{
			SetWindowPos(g_hSimulationWnd, HWND_TOP, 0, 0, rcScreenRect.right, rcScreenRect.bottom, SWP_SHOWWINDOW);

			//These user defined messages are here because when the subclassed window is resized it does not always
			//repaint itself correctly. This does not happen with the non-subclassed window. By sending two unused 
			//windows message to the simulator message processor it forces the app to redraw itself. I tried this with
			//just one and still did not work completely correctly. Two seemed to do the trick. I will be interested
			//to see if this works correctly on different machines and video cards though?
			PostMessage(g_hSimulationWnd, 0x8000, 0, 0); 
 			PostMessage(g_hSimulationWnd, 0x8000, 0, 0); 
		}

   break;
  }

	lVal = CallWindowProc(g_PrevWndProc, win, msg, wparam, lparam);
	return lVal;
}


void MEAPI Pause(RRender* rc, void* userData)
{
	if(userData)
	{
		VsSimulator *lpSim = (VsSimulator *) (userData);
		if(lpSim) lpSim->TogglePauseSimulation();
	}
}

void MEAPI ResetCallback(RRender* rc, void* userData)
{
	if(userData)
	{
		VsSimulator *lpSim = (VsSimulator *) (userData);
		if(lpSim) lpSim->ResetStructures();
	}
}

int vsnprintf(char *const string,const size_t n,
                     const char *const format, va_list ap)
{
  (void) n;

  return vsprintf(string,format,ap);
}

int snprintf(char *const string,const size_t n,
                    const char *const format,...)
{
  va_list ap;
  int i;

  (void) n;

  va_start(ap,format);
  i = vsprintf(string,format,ap);
  va_end(ap);

  return i;
}

void MEAPI VortexWarning(const int level, const char *const format,va_list ap)
{
  if (level <= MeWarningLevel)
  {
    char message[MeMAXMESSAGE];

		g_bVortexError = TRUE;

    (void) snprintf(message,sizeof message,"MeWarning{%d}: ",level);
    g_strVortexErrorMessage = message;

    (void) vsnprintf(message,sizeof message,format,ap);
		g_strVortexErrorMessage += message;

		Std_TraceMsg(0, g_strVortexErrorMessage, __FILE__, __LINE__);
	}
}

void MEAPI VortexError(const int level, const char *const format,va_list ap)
{
    char message[MeMAXMESSAGE];

		g_bVortexError = TRUE;

		(void) snprintf(message,sizeof message,"MeWarning{%d}: ",level);
    g_strVortexErrorMessage = message;

    (void) vsnprintf(message,sizeof message,format,ap);
		g_strVortexErrorMessage += message;

		Std_TraceMsg(0, g_strVortexErrorMessage, __FILE__, __LINE__);
}

void VsSimulator::TogglePauseSimulation()
{
	m_bPaused = !m_bPaused;

	RRenderSetPause(m_lpRenderContext, m_bPaused);

	if(m_bPaused && m_lpSimRecorder)
		SnapshotStopFrame();

	if(m_lpManagedInstance)
	{
		if(!m_bPaused && m_lpStartSimulationCallback)
			m_lpStartSimulationCallback(m_lpManagedInstance);
		else if(m_bPaused && m_lpPauseSimulationCallback)
			m_lpPauseSimulationCallback(m_lpManagedInstance);
	}

	if(m_bPaused && m_hSimulationWnd)
	{
		PostMessage(m_hSimulationWnd, 0x8000, 0, 0); 
 		PostMessage(m_hSimulationWnd, 0x8000, 0, 0); 
	}

}

void VsSimulator::StopSimulation()
{
	if(!m_bPaused)
		TogglePauseSimulation();
}

void VsSimulator::Initialize(int argc, const char **argv)
{
	float color[6][4] = { {1, 1, 1, 1}, {0, 0, 1, 1},{1,0,1,1}, {1, 0, 0, 1}, {0, 1, 0, 1}, {0, 1, 1, 1} };

	//Set the random number generator seed
	if(m_bAutoGenerateRandomSeed)
		GenerateAutoSeed();
	else
	{
		Std_SRand(m_iManualRandomSeed);
		srand(m_iManualRandomSeed);
	}

	MeCommandLineOptions *lpOptions = NULL;

	//Find the number of timeslices that need to occur before the physics system is updated
	m_iPhysicsStepInterval = m_fltPhysicsTimeStep / m_fltTimeStep;

	//Now recaculate the physics time step using the minimum time step as the base.
	m_fltPhysicsTimeStep = m_fltTimeStep * m_iPhysicsStepInterval;

	MeSetWarningHandler(VortexWarning);
  MeSetFatalErrorHandler(VortexError);

	if(m_hParentWnd)
	{
	  lpOptions = CreateDefaultOptions();
	  m_lpRenderContext = RRenderContextCreateWithParent(lpOptions, 0, !MEFALSE, m_hParentWnd);
	}
	else
	{
		lpOptions = MeCommandLineOptionsCreate(argc, argv);

    if(MeCommandLineOptionsGetPos(lpOptions, "-runtime") != -1)
			m_fltEndSimTime = MeCommandLineOptionsGetFloat(lpOptions, "-runtime", 10);

	  m_lpRenderContext = RRenderContextCreate(lpOptions, 0, !MEFALSE);
	}

  if(!m_lpRenderContext)
		THROW_ERROR(Al_Err_lRenderContextNotDefined, Al_Err_strRenderContextNotDefined);

	if(!Std_IsBlank(m_strExecutablePath))
		MeFileSearchPathAdd( (m_strExecutablePath + "resources\\").c_str() );

	if(!Std_IsBlank(m_strProjectPath))
		MeFileSearchPathAdd(m_strProjectPath.c_str());

	m_hSimulationWnd = (HWND) m_lpRenderContext->m_hWnd;

	if(m_hParentWnd) 
	{
		m_PrevWndProc = SetWindowLongPtr(m_hParentWnd, GWLP_WNDPROC, (LONG_PTR) MyWndProc);
		g_hClientWnd = GetParent(m_hParentWnd);
		g_hFrameWnd = GetParent(g_hClientWnd);
	}

	g_hSimulationWnd = m_hSimulationWnd;
	g_hParentWnd = m_hParentWnd;
	g_PrevWndProc = (WNDPROC) m_PrevWndProc;

  if(lpOptions) MeCommandLineOptionsDestroy(lpOptions);

  #define NOBJECTS 100
 
  MstUniverseSizes oSizes;
  oSizes.collisionModelsMaxCount = NOBJECTS;
  oSizes.collisionPairsMaxCount = NOBJECTS*40 ;
  oSizes.collisionGeometryTypesMaxCount = McdPrimitivesGetTypeCount();
  oSizes.dynamicBodiesMaxCount = NOBJECTS;
  oSizes.dynamicConstraintsMaxCount = NOBJECTS + NOBJECTS*10  ;
  oSizes.materialsMaxCount = 10;

	m_iUniverse = MstUniverseCreate(&oSizes);
  m_iWorld = MstUniverseGetWorld(m_iUniverse);	
	m_iBridge = MstUniverseGetBridge(m_iUniverse);
  
  m_lpApp = MeAppCreateFromUniverse(m_iUniverse, m_lpRenderContext);
  RRenderSetMouseCallBack(m_lpRenderContext, MeAppMousePickCB, (void*)m_lpApp);

	//Make sure we register the other types of objects that can be used.
	if(m_bHasConvexMesh)
	{
		McdConvexMeshRegisterType();
		McdConvexMeshPrimitivesRegisterInteractions();
	}

	if(m_bHasTriangleMesh)
	{
		McdTriangleMeshRegisterType();
		McdTriangleMeshPrimitivesRegisterInteractions();
	}

	if(m_bHasHeightField)
	{
		McdRGHeightFieldRegisterType();
		McdRGHeightFieldPrimitivesRegisterInteractions();
		if(m_bHasConvexMesh) 
			McdConvexMeshRGHeightFieldRegisterInteraction();
	}

	MdtWorldSetGravity(m_iWorld, 0, m_fltGravity, 0);
	//MdtWorldSetComplianceParameters (m_iWorld, m_fltLinearCompliance, m_fltAngularCompliance, m_fltLinearDamping, m_fltAngularDamping, m_fltLinearKineticLoss, m_fltAngularKineticLoss);

	MdtWorldSetLinearCompliance(m_iWorld, m_fltLinearCompliance);
	MdtWorldSetAngularCompliance(m_iWorld, m_fltAngularCompliance);

	MdtWorldSetLinearDamping(m_iWorld, m_fltLinearDamping);
	MdtWorldSetAngularDamping(m_iWorld, m_fltAngularDamping);

	MdtWorldSetLinearKineticLoss(m_iWorld, m_fltLinearKineticLoss);
	MdtWorldSetAngularKineticLoss(m_iWorld, m_fltAngularKineticLoss);

	m_lpApp->mouseInfo.mouseSpringStiffness = m_fltMouseSpringStiffness;
	m_lpApp->mouseInfo.mouseSpringDamping = m_ftlMouseSpringDamping;

	//If we turn this on then we no longer get collision detection after the body stops moving.
	//We need to find a way to disable this for specific objects, like organisms only.
  MdtWorldSetAutoDisable(m_iWorld, 0); 

  //MstFixedModelCreate(m_iUniverse, McdPlaneCreate(), gt);
  //RGraphicSetTexture(m_lpRenderContext, RGraphicGroundPlaneCreate(m_lpRenderContext, 244.0f, 2, color[0], gt[3][1]), "checkerboard");

	InitializeStructures();

	//We have to disable collision for the water surface because the vortex simulator can only handle having one plane type of object
	//with collision enabled. It treats any planes with collision as the ground and if you have other objects below it then it will
	//treat them as being underground (penetrated the ground) and force them back to the surface. Disabling collisions disables this behavior.
	if(m_lpWaterSurface)
		DisableCollision(m_lpWaterSurface->Body());

	MdtContactParamsID oProps = MstBridgeGetContactParams(m_iBridge, MstBridgeGetDefaultMaterial(), MstBridgeGetDefaultMaterial());

	MdtContactParamsSetType(oProps, MdtContactTypeFriction2D);
	MdtContactParamsSetFriction(oProps, m_fltMaxSurfaceFriction);
  //MdtContactParamsSetRestitution(oProps, 0.3);

	RRenderSetWindowTitle(m_lpRenderContext, "Animat Simulator");

	//RRenderSetFrameLockType(m_lpRenderContext, CM_PROCESS_FRAMELOCK);
	RRenderSetFrameLockType(m_lpRenderContext, CM_CONTROLLED_FRAMELOCK);
	RRenderSetFrameLockRate(m_lpRenderContext, 30);
 
	// Set up camera 
	//RLightSwitchOff(m_lpRenderContext, kRAmbient);
	RLightSwitchOn(m_lpRenderContext, kRAmbient);
	RLightSwitchOn(m_lpRenderContext, kRDirect1);
	RLightSwitchOff(m_lpRenderContext, kRDirect2);
	RLightSwitchOff(m_lpRenderContext, kRPoint);

	//RLightSwitchOn(m_lpRenderContext, m
	////Directional light 1
	//m_lpRenderContext->m_DirectLight1.m_bUseLight = 1;


	//m_lpRenderContext->m_PointLight.m_bUseLight = 1;
	//m_lpRenderContext->m_PointLight.m_Position[0] = 50;
	//m_lpRenderContext->m_PointLight.m_Position[1] = 0
	//m_lpRenderContext->m_PointLight.m_Position[2] = 50;
	m_lpRenderContext->m_PointLight.m_rgbAmbient[0] = (float) 0.2;
	m_lpRenderContext->m_PointLight.m_rgbAmbient[1] = (float) 0.2;
	m_lpRenderContext->m_PointLight.m_rgbAmbient[2] = (float) 0.2;
    m_lpRenderContext->m_backgroundColour[0] = 0.6f;
    m_lpRenderContext->m_backgroundColour[1] = 0.613f;
    m_lpRenderContext->m_backgroundColour[2] = 0.938f;
	m_lpRenderContext->m_bUseAmbientLight = !MEFALSE;

	RRenderSetPause(m_lpRenderContext, m_bPaused);
	RRenderSetActionNCallBack(m_lpRenderContext, 5, Pause, this);
	RRenderSetActionNCallBack(m_lpRenderContext, 2, ResetCallback, this);
	RRenderCreateUserHelp(m_lpRenderContext, 0, 0);
	RRenderSetDisplayFps(m_lpRenderContext, 0);
	RRenderSetDisplayTime(m_lpRenderContext, 1, 0, m_fltTimeStep, m_fltPlaybackRate);
	RRenderSetAlphaBlend3D(m_lpRenderContext, m_bUseAlphaBlending);
	RRenderSetWireframe(m_lpRenderContext, m_bUseWireframe);

	TrackCamera(m_bTrackCamera, m_strLookAtStructureID, m_strLookAtBodyID);

	RCameraRotateAngle(m_lpRenderContext, (MeReal) m_fltCameraRotation);
	RCameraRotateElevation(m_lpRenderContext, (MeReal) m_fltCameraElevation);
	RCameraSetOffset(m_lpRenderContext, m_fltCameraOffset);

	m_oDataChartMgr.Initialize(this);
	m_oExternalStimuliMgr.Initialize(this);
	if(m_lpSimRecorder) m_lpSimRecorder->Initialize(this);

	/* setup callbacks between sensors and objects handled by Mdt */
	//MstBridgeSetIntersectNewCB(m_iBridge, kMstResponseBody, kMstResponseBody, SensorContactBegin, (void*) this);
	MstBridgeSetIntersectNewCB(m_iBridge, kMstResponseBody, kMstResponseSensor, SensorContactBegin, (void*) this);
	//MstBridgeSetIntersectActiveCB(m_iBridge, kMstResponseBody, kMstResponseBody, SensorContactContinue, (void*) this);
	//MstBridgeSetIntersectActiveCB(m_iBridge, kMstResponseBody, kMstResponseSensor, SensorContactContinue, (void*) this);
	MstBridgeSetDisjointCB(m_iBridge, kMstResponseBody, kMstResponseSensor, SensorContactEnd, (void*) this);
 
	//If it is starting up paused we need to send a couple of messages to get the subclassed window to paint correctly
	//when it first starts up.
	if(m_bPaused)
	{
		PostMessage(g_hSimulationWnd, 0x8000, 0, 0); 
 		PostMessage(g_hSimulationWnd, 0x8000, 0, 0); 
	}

	m_bInitialized = TRUE;
}

void VsSimulator::StepSimulation()
{
	//If the vortex simulator has given an error then lets stop the simulation and give back the error for the user.
	if(g_bVortexError)
	{
		StopSimulation();
		//ShutdownSimulation();
		THROW_ERROR(Al_Err_lVortexSimulationError, g_strVortexErrorMessage);
	}

	//Do not really step the simulation if we are playing back video.
	if(!m_lpVideoPlayback)
		Simulator::StepSimulation();
	else
	{
		m_lVideoSliceCount += (m_iPhysicsStepInterval*2);
		if(m_lVideoSliceCount > m_lpVideoPlayback->EndSlice())
		{
			m_lVideoSliceCount = m_lpVideoPlayback->StartSlice();
			m_iVideoLoops++;
		}
	}

	//Now check to see if we need to look at a specific body.
	if(m_lpTrackBody)
	{
		MdtBodyGetPosition(m_lpTrackBody->BodyID(), m_oTrackPos);
		RCameraSetLookAt(m_lpRenderContext, m_oTrackPos);		
	}

	MeAppStep(m_lpApp);

	if(m_lpVideoRecorder)//&& m_lpRenderContext->m_bDrawingFrame)
		m_lpVideoRecorder->RecordVideoFrame(this);
	else if(m_lpVideoPlayback)
		m_lpVideoPlayback->PlaybackVideoFrame(this);
	else
		MstUniverseStep(m_iUniverse, m_fltPhysicsTimeStep);

	if(m_bManualStepSimulation)
		RRenderStep(m_lpRenderContext);

	if(m_lpVideoPlayback)
		m_lpRenderContext->m_lTimeSlice = m_lVideoSliceCount;
	else
		m_lpRenderContext->m_lTimeSlice = m_lTimeSlice;

	//If we are running for a set time then lets stop once we reach that point.
	if(m_fltEndSimTime >0 && this->Time() >= m_fltEndSimTime)
	{
		if(m_lpManagedInstance && m_lpEndingSimulationCallback)
			m_lpEndingSimulationCallback(m_lpManagedInstance);
		else
			RRenderQuit(m_lpRenderContext);
	}

	//if(!(m_lTimeSlice % 2000))
	//{
		//m_lTimer = Std_GetTick() - m_lTimer;
		//Std_LogMsg(40, "Time Checkpoint: " + STR(m_lTimer));
		//m_lTimer = Std_GetTick();
	//}

	/*
	if(m_lpManagedInstance && m_lpUpdateDataCallback && m_lUpdateDataInterval > 0 )
	{
		if( (!m_lpVideoPlayback && (m_lUpdateDataCount == m_lUpdateDataInterval)) ||
		     (m_lpVideoPlayback && (m_lUpdateDataCount == m_lUpdateDataInterval)) )
		{
			m_lpUpdateDataCallback(m_lpManagedInstance);
			m_lUpdateDataCount = 1;
		}
		else
			m_lUpdateDataCount++;
	}
	*/

	//if(m_lTimeSlice > 10000)
	//	MoveSimulationToKeyFrame("Snap1");
	//if(m_lTimeSlice == 44004 && !m_lpVideoPlayback)
	//if(m_lTimeSlice == 60004 && !m_lpVideoPlayback)
	//{
	//	EnableVideoPlayback("VIDEO0000000000");
	//	StartVideoPlayback();
	//	//SaveVideo("C:\\Projects\\Documentation\\Results\\Siggraph Poster\\Robot Frames\\mp%02d-");
	//}
	//else if(m_iVideoLoops == 1)
	//{
	//	StopVideoPlayback();
	//	DisableVideoPlayback();
	//}
}

void VsSimulator::Simulate()
{
	m_lTimer = Std_GetTick();

	if(m_bManualStepSimulation)
		RRenderRunInit(m_lpRenderContext, NULL, NULL);	
	else
		RRun(m_lpRenderContext, tick, this);
}

void VsSimulator::ShutdownSimulation()
{
	//Lets to the application it needs to shutdown.
	if(m_hSimulationWnd)
		PostMessage(m_hSimulationWnd, WM_CLOSE, 0, 0); 
}

BOOL VsSimulator::PauseSimulation()
{
	if(m_hSimulationWnd)
		PostMessage(m_hSimulationWnd, WM_CHAR, 'p', 0); 
	
	return TRUE;
}

BOOL VsSimulator::StartSimulation()
{
	BOOL bVal = FALSE;

	if(m_lpRenderContext)
	{
		m_bPaused = FALSE;
		RRenderSetPause(m_lpRenderContext, m_bPaused);
		bVal = TRUE;
	}

	return bVal;
}

void VsSimulator::SnapshotStopFrame()
{
	if(m_lpSimStopPoint) delete m_lpSimStopPoint;
	m_lpSimStopPoint = dynamic_cast<KeyFrame *>(CreateObject("AnimatLab", "KeyFrame", "Snapshot"));
	if(!m_lpSimStopPoint)
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "KeyFrame");

	m_lpSimStopPoint->StartSlice(m_lTimeSlice);
	m_lpSimStopPoint->EndSlice(m_lTimeSlice);
	m_lpSimStopPoint->Activate(this);
}

VsSimulator *VsSimulator::ConvertSimulator(Simulator *lpSim)
{
	if(!lpSim)
		THROW_ERROR(Al_Err_lSimulationNotDefined, Al_Err_strSimulationNotDefined);

	VsSimulator *lpVsSim = dynamic_cast<VsSimulator *>(lpSim);

	if(!lpVsSim)
		THROW_ERROR(Vs_Err_lUnableToConvertToVsSimulator, Vs_Err_strUnableToConvertToVsSimulator);

	return lpVsSim;
}

/*
VsSimulator *VsSimulator::ConvertSimulator(Simulator *lpSim)
{
	if(!lpSim)
		THROW_ERROR(Al_Err_lSimulationNotDefined, Al_Err_strSimulationNotDefined);

	VsSimulator *lpVsSim = dynamic_cast<VsSimulator *>(lpSim);

	if(!lpVsSim)
		THROW_ERROR(Vs_Err_lUnableToConvertToVsSimulator, Vs_Err_strUnableToConvertToVsSimulator);

	return lpVsSim;
}
*/


/*
__declspec( dllexport ) void RunSimulator(void)
{
	VsSimulator oSim;
	oSim.ProjectPath("C:\\Projects\\bin\\Experiments\\Robot\\");

	oSim.Load("Robot.asim");
	oSim.Initialize(0, NULL);
	oSim.Simulate();
}
*/

}			//VortexAnimatLibrary
