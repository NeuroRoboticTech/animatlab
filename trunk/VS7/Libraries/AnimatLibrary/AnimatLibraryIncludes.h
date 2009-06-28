#ifndef __ANIMAT_INCLUDES_H__
#define __ANIMAT_INCLUDES_H__

#define ANIMAT_PORT __declspec( dllexport )

#define STD_TRACING_ON

#include "StdUtils.h"
#include "AnimatConstants.h"
#include "AnimatUtils.h"

//Simulation Objects
namespace AnimatLibrary
{
	class Simulator;
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

#endif // __ANIMAT_INCLUDES_H__
