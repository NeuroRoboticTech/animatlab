#ifndef __VORTEX_ANIMAT_LIB_DLL_H__
#define __VORTEX_ANIMAT_LIB_DLL_H__

#ifndef _VORTEX_ANIMAT_LIB_DLL_NOFORCELIBS
	#if _MSC_VER > 1300  // VC 7
		#ifdef _DEBUG
			#pragma comment(lib, "VortexAnimatLibrary_vc7D.lib")
		#else
			#pragma comment(lib, "VortexAnimatLibrary_vc7.lib")
		#endif
	#else   // VC 6
		#ifdef _DEBUG
			#pragma comment(lib, "VortexAnimatLibrary_vc6D.lib")
		#else
			#pragma comment(lib, "VortexAnimatLibrary_vc6.lib")
		#endif
	#endif
#endif          // _VORTEX_ANIMAT_LIB_DLL_NOFORCELIBS

#define VORTEX_PORT __declspec( dllimport )

#include "AnimatLibrary.h"
#include "VortexAnimatConstants.h"

//Simulation Objects
namespace VortexAnimatLibrary
{
	class VsSimulator;
	class VsClassFactory;

	namespace Environment
	{
		class VsJoint;
		class VsOrganism;
		class VsRigidBody;
		class VsStructure;

		namespace Bodies
		{
			class VsBox;
			class VsBoxContactSensor;
			class VsCone;
			class VsCylinder;
			class VsCylinderContactSensor;
			class VsMuscleBase;
			class VsLinarHillMuscle;
			class VsLinarHillStretchReceptor;
			class VsMesh;
			class VsMouth;
			class VsMultisegmentSpring;
			class VsOdorSensor;
			class VsPlane;
			class VsSensor;
			class VsSphere;
			class VsSpring;
		}

		namespace Joints
		{
			class VsBallSocket;
			class VsHinge;
			class VsPrismatic;
			class VsStatic;
		}
	}

	namespace ExternalStimuli
	{
		class VsForceStimulus;
		class VsInverseMuscleCurrent;
		class VsMotorVelocityStimulus;
		class VsPositionClamp;
	}

	namespace Recording
	{
		namespace KeyFrames
		{
			class VsVideoKeyFrame;
			class VsSnapshotKeyFrame;
		}

		class VsSimulationRecorder;
	}
}

using namespace VortexAnimatLibrary;
using namespace VortexAnimatLibrary::ExternalStimuli;
using namespace VortexAnimatLibrary::Environment;
using namespace VortexAnimatLibrary::Environment::Bodies;
using namespace VortexAnimatLibrary::Environment::Joints;
using namespace VortexAnimatLibrary::Recording;
using namespace VortexAnimatLibrary::Recording::KeyFrames;

#include "Mst.h"
#include "MeApp.h"
#include "Mdt.h"
#include "MeViewer.h"

#include "MdtPlayback.h"
#include "MdtRecorder.h"
#include "MeAppRecorder.h"

#include "McdConvexMesh.h"
#include "McduConvexMeshIO.h"
#include "RConvex.h"

#include "McdRGHeightField.h"
#include "McduRGHeightFieldIO.h"
#include "RRGHeightfield.h"

#include "McdTriangleMesh.h"
#include "McduTriangleMeshIO.h"
#include "RTriangleMesh.h"

#include "VsRigidBody.h"
#include "VsPlane.h"
#include "VsBox.h"
#include "VsBoxContactSensor.h"
#include "VsCylinder.h"
#include "VsCylinderContactSensor.h"
#include "VsCone.h" 
#include "VsSphere.h"
#include "VsJoint.h"
#include "VsBallSocket.h"
#include "VsHingeJoint.h"
#include "VsPrismatic.h"
#include "VsSpring.h"
#include "VsMultiSegmentSpring.h"
#include "VsStaticJoint.h"
#include "VsMesh.h"
#include "VsMuscleBase.h"
#include "VsLinearHillMuscle.h"
#include "VsLinearHillStretchReceptor.h"
#include "VsMuscleAttachment.h"
#include "VsSensor.h"
#include "VsOdorSensor.h"
#include "VsMouth.h"
#include "VsSimulator.h"
#include "VsStructure.h"
#include "VsOrganism.h"

#include "VsVideoKeyFrame.h"
#include "VsSnapshotKeyFrame.h"

#include "VsMotorVelocityStimulus.h"
#include "VsForceStimulus.h"
#include "VsPositionClamp.h"
#include "VsInverseMuscleCurrent.h"

#endif // __VORTEX_ANIMAT_LIB_DLL_H__
