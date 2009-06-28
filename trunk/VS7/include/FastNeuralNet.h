#ifndef __FAST_NEURAL_NET_LIB_DLL_H__
#define __FAST_NEURAL_NET_LIB_DLL_H__

#ifndef _FAST_NEURAL_NET_LIB_DLL_NOFORCELIBS
	#if _MSC_VER > 1300  // VC 7
		#ifdef _DEBUG
			#pragma comment(lib, "FastNeuralNet_vc7D.lib")
		#else
			#pragma comment(lib, "FastNeuralNet_vc7.lib")
		#endif
	#else   // VC 6
		#ifdef _DEBUG
			#pragma comment(lib, "FastNeuralNet_vc6D.lib")
		#else
			#pragma comment(lib, "FastNeuralNet_vc6.lib")
		#endif
	#endif
#endif          // _FAST_NEURAL_NET_LIB_DLL_NOFORCELIBS

#define FAST_NET_PORT __declspec( dllimport )

#include "StdUtils.h"
#include "FastNeuralNetConstants.h"

//Simulation Objects
namespace FastNeuralNet
{
	class CNlClassFactory;
	class CNlNervousSystem;

	namespace DataColumns
	{
		class CNlNeuronDataColumn;
	}

	namespace ExternalStimuli
	{
		class CNlCurrentInjection;
	}

	namespace Neurons
	{
		class CNlNeuron;
		class CNlPacemakerNeuron;
		class CNlRandomNeuron;
		class CNlTonicNeuron;
	}

	namespace Synapses
	{
		class CNlCompoundSynapse;
		class CNlGatedSynapse;
		class CNlModulatedSynapse;
		class CNlSynapse;
	}
}

using namespace FastNeuralNet;
using namespace FastNeuralNet::DataColumns;
using namespace FastNeuralNet::ExternalStimuli;
using namespace FastNeuralNet::Neurons;
using namespace FastNeuralNet::Synapses;

#include "NeuralUtils.h"

#endif // __FAST_NEURAL_NET_LIB_DLL_H__
