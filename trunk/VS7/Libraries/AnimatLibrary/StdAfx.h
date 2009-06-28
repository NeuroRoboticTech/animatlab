// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__AC7276DB_F5B0_4434_A20E_8194C391BF3C__INCLUDED_)
#define AFX_STDAFX_H__AC7276DB_F5B0_4434_A20E_8194C391BF3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
 
#include <windows.h>

#define ANIMAT_PORT __declspec( dllexport )

#define STD_TRACING_ON

//#include "NeuralLibrary.h"
#include "StdUtils.h"
#include "AnimatConstants.h"
#include "AnimatUtils.h"

//Simulation Objects
namespace AnimatLibrary
{
	class Simulator;
	//class ClassFactory;
	class Node;
	class ActivatedItem;
	class ActivatedItemMgr;

	namespace Adapters
	{
		class Adapter;
		class NodeToNodeAdapter;
		class NodeToPhysicalAdapter;
		class PhysicalToNodeAdapter;
	}

	namespace Behavior
	{
		class NervousSystem;
		class NeuralModule;
	}

	namespace Charting
	{
		class DataChart;
		class DataChartMgr;
		class DataColumn;
	}

	namespace Environment
	{
		class RigidBody;
		class Joint;
		class Structure;
		class Organism;

		namespace Bodies
		{
			class Box;
			class Cylinder;
			class Cone;
			class Muscle;
			class MuscleAttachment;
			class Plane;
			class Sensor;
			class Sphere;
		}

		namespace Joints
		{
			class Hinge;
			class Static;
		}
	}

	namespace ExternalStimuli
	{
		class ExternalStimuliMgr;
		class ExternalStimulus;
	}

	namespace Gains
	{
		class BellGain;
		class EquationGain;
		class Gain;
		class PolynomialGain;
		class SigmoidGain;
	}

	namespace Recording
	{
		class KeyFrame;
		class SimulationRecorder;
	}
}

using namespace AnimatLibrary;
using namespace AnimatLibrary::Adapters;
using namespace AnimatLibrary::Behavior;
using namespace AnimatLibrary::Charting;
using namespace AnimatLibrary::Environment;
using namespace AnimatLibrary::Environment::Bodies;
using namespace AnimatLibrary::Environment::Joints;
using namespace AnimatLibrary::ExternalStimuli;
using namespace AnimatLibrary::Gains;
using namespace AnimatLibrary::Recording;


#include "AnimatUtils.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__AC7276DB_F5B0_4434_A20E_8194C391BF3C__INCLUDED_)
