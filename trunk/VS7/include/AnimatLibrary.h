#ifndef __ANIMAT_LIB_DLL_H__
#define __ANIMAT_LIB_DLL_H__

#ifndef _ANIMAT_LIB_DLL_NOFORCELIBS
	#ifndef _WIN32_WCE
		#if _MSC_VER > 1300  // VC 7
			#ifdef _DEBUG
				#pragma comment(lib, "AnimatLibrary_vc7D.lib")
			#else
				#pragma comment(lib, "AnimatLibrary_vc7.lib")
			#endif
		#else   // VC 6
			#ifdef _DEBUG
				#pragma comment(lib, "AnimatLibrary_vc6D.lib")
			#else
				#pragma comment(lib, "AnimatLibrary_vc6.lib")
			#endif
		#endif
	#else
		#ifdef _DEBUG
			#pragma comment(lib, "AnimatLibrary_CED.lib")
		#else
			#pragma comment(lib, "AnimatLibrary_CE.lib")
		#endif          
	#endif
#endif          // _ANIMAT_LIB_DLL_NOFORCELIBS

#define ANIMAT_PORT __declspec( dllimport )

#include "StdUtils.h"
#include "AnimatConstants.h"

//Simulation Objects
namespace AnimatLibrary
{
	class Simulator;
	class Node;
	class ActivatedItem;
	class ActivatedItemMgr;
	class DelayLine;

	namespace Adapters
	{
		class Adapter;
		class NodeToNodeAdapter;
		class NodeToPhysicalAdapter;
		class PhysicalToNodeAdapter;
		class ContactAdapter;
	}

	namespace Behavior
	{
		class NervousSystem;
		class NeuralModule;
	}

	namespace Charting
	{
		class DataChart;
		class FileChart;
		class ArrayChart;
		class MemoryChart;
		class DataChartMgr;
		class DataColumn;

		namespace DataColumns
		{
			class JointDataColumn;
			class RigidBodyDataColumn;
			class StimulusDataColumn;
		}
	}

	namespace Environment
	{
		class RigidBody;
		class Joint;
		class Structure;
		class Organism;
		class IODataEntry;
		class Microcontroller;
		class ContactSensor;
		class ReceptiveField;
		class ReceptiveFieldPair;
		class Odor;
		class OdorType;

		namespace Bodies
		{
			class Box;
			class Cylinder;
			class Cone;
			class Mesh;
			class MuscleBase;
			class LinearHillMuscle;
			class LinearHillStretchReceptor;
			class MuscleAttachment;
			class OdorSensor;
			class Plane;
			class Sensor;
			class Sphere;
			class Spring;
			class MultiSegmentSpring;
			class Stomach;
			class Mouth;
		}

		namespace Joints
		{
			class BallSocket;
			class Hinge;
			class Prismatic;
			class Static;
		}
	}

	namespace ExternalStimuli
	{
		class ExternalStimuliMgr;
		class ExternalStimulus;
		class ExternalInputStimulus;
		class NodeInputStimulus;
		class RigidBodyInputStimulus;
		class JointInputStimulus;
		class CurrentStimulus;
		class EnablerStimulus;
		class VoltageClamp;
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
using namespace AnimatLibrary::Charting::DataColumns;
using namespace AnimatLibrary::Environment;
using namespace AnimatLibrary::Environment::Bodies;
using namespace AnimatLibrary::Environment::Joints;
using namespace AnimatLibrary::ExternalStimuli;
using namespace AnimatLibrary::Gains;
using namespace AnimatLibrary::Recording;

#include "DelayLine.h"
#include "Gain.h"
#include "BellGain.h"
#include "EquationGain.h"
#include "PolynomialGain.h"
#include "SigmoidGain.h"
#include "Node.h"
#include "ReceptiveField.h"
#include "ReceptiveFieldPair.h"
#include "Adapter.h"
#include "NodeToNodeAdapter.h"
#include "NodeToPhysicalAdapter.h"
#include "PhysicalToNodeAdapter.h"
#include "ContactAdapter.h"
#include "Joint.h"
#include "BallSocket.h"
#include "Hinge.h"
#include "Prismatic.h"
#include "Static.h"
#include "ContactSensor.h"
#include "Odor.h"
#include "OdorType.h"
#include "RigidBody.h"
#include "Plane.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Box.h"
#include "Mesh.h"
#include "MuscleAttachment.h"
#include "MuscleBase.h" 
#include "LinearHillMuscle.h" 
#include "LinearHillStretchReceptor.h" 
#include "OdorSensor.h"
#include "Sensor.h"
#include "Sphere.h"
#include "Spring.h"
#include "MultiSegmentSpring.h"
#include "Stomach.h"
#include "Mouth.h"
#include "IODataEntry.h"
#include "Microcontroller.h"
#include "Structure.h"
#include "NervousSystem.h"
#include "NeuralModule.h"
#include "Organism.h"
#include "ActivatedItem.h"
#include "ActivatedItemMgr.h"
#include "DataColumn.h"
#include "DataChart.h"
#include "FileChart.h"
#include "ArrayChart.h"
#include "MemoryChart.h"
#include "DataChartMgr.h"
#include "JointDataColumn.h"
#include "RigidBodyDataColumn.h"
#include "StimulusDataColumn.h"
#include "ExternalStimulus.h"
#include "ExternalStimuliMgr.h"
#include "ExternalInputStimulus.h"
#include "NodeInputStimulus.h"
#include "RigidBodyInputStimulus.h"
#include "JointInputStimulus.h"
#include "CurrentStimulus.h"
#include "EnablerStimulus.h"
#include "VoltageClamp.h"
#include "KeyFrame.h"
#include "SimulationRecorder.h"
#include "Simulator.h"

#include "AnimatUtils.h"

#endif // __ANIMAT_LIB_DLL_H__
