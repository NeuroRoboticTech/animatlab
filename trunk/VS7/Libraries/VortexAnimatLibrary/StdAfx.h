// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#define VORTEX_PORT __declspec( dllexport )

//#define STD_TRACING_ON

#include "StdUtils.h"
#include "AnimatLibrary.h"

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

#include "VortexAnimatConstants.h"

//Simulation Objects
namespace VortexAnimatLibrary
{
	class VsClassFactory;
	class VsSimulator;

	namespace ExternalStimuli
	{
			class VsMotorVelocityStimulus;
	}

	namespace Environment
	{
		class VsRigidBody;
		class VsJoint;

		namespace Bodies
		{
			class VsBox;
			class VsCylinder;
			class VsMuscle;
			class VsPlane;
			class VsSpring;
		}

		namespace Joints
		{
			class VsBallSocket;
			class VsHinge;
			class VsPrismatic;
			class VsStatic;
		}

		namespace Organisms
		{
			class VsMuscleRobot;
			class VsSwimmer;
			class VsWalkingRobot;
		}
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

