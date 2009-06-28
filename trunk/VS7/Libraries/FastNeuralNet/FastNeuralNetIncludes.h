#ifndef __FAST_NET_INCLUDES_H__
#define __FAST_NET_INCLUDES_H__

#define FAST_NET_PORT __declspec( dllexport )

#include "AnimatLibrary.h"
#include "FastNeuralNetConstants.h"

//Simulation Objects
namespace FastNeuralNet
{
	class ClassFactory;
	class FastNeuralModule;

	namespace DataColumns
	{
		class NeuronData;
	}

	namespace ExternalStimuli
	{
		class CurrentInjection;
	}

	namespace Neurons
	{
		class MotorNeuron;
		class Neuron;
		class PacemakerNeuron;
		class RandomNeuron;
		class SensoryNeuron;
		class TonicNeuron;
	}

	namespace Synapses
	{
		class CompoundSynapse;
		class GatedSynapse;
		class ModulatedSynapse;
		class Synapse;
	}
}

using namespace FastNeuralNet;
using namespace FastNeuralNet::DataColumns;
using namespace FastNeuralNet::ExternalStimuli;
using namespace FastNeuralNet::Neurons;
using namespace FastNeuralNet::Synapses;

#include "NeuralUtils.h"

#endif // __FAST_NET_INCLUDES_H__
