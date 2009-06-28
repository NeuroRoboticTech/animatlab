#ifndef __REAL_NET_INCLUDES_H__
#define __REAL_NET_INCLUDES_H__

#define ADV_NEURAL_PORT __declspec( dllexport )

#include "AnimatLibrary.h"
#include "RealisticNeuralNetConstants.h"

//Simulation Objects
namespace RealisticNeuralNet
{
	//class ClassFactory;
	class RealisticNeuralModule;
	class Neuron;

	namespace DataColumns
	{
		class NeuronData;
	}

	namespace ExternalStimuli
	{
		//class EventTrigger;
		//class CurrentStimulus;
	}

	namespace Synapses
	{
		class Connexion;
		class ElectricalSynapse;
		class NonSpikingChemicalSynapse;
		class SpikingChemicalSynapse;
	}

	namespace Utilities
	{
		class DoubleList;
	}
}

using namespace RealisticNeuralNet;
using namespace RealisticNeuralNet::DataColumns;
using namespace RealisticNeuralNet::ExternalStimuli;
using namespace RealisticNeuralNet::Synapses;
using namespace RealisticNeuralNet::Utilities;

#include "DoubleList.h"
#include "NeuralUtils.h"

#endif // __REAL_NET_INCLUDES_H__
