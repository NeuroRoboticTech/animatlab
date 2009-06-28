// GatedSynapse.h: interface for the GatedSynapse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GATEDSYNAPSE_H__21B27420_19DA_47AD_846E_7EBEDC8147D9__INCLUDED_)
#define AFX_GATEDSYNAPSE_H__21B27420_19DA_47AD_846E_7EBEDC8147D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace FastNeuralNet
{
	namespace Synapses
	{

		class FAST_NET_PORT GatedSynapse : public Synapse    
		{
		protected:
			unsigned char m_iInitialGateValue;

		public:
			GatedSynapse();
			virtual ~GatedSynapse();

			virtual unsigned char Type()
			{return GATED_SYNAPSE;};

			virtual float CalculateModulation(FastNeuralModule *lpModule);

			virtual void Load(Simulator *lpSim, Structure *lpStructure, Neuron *lpNeuron, CStdXml &oXml);
			virtual void Save(Simulator *lpSim, Structure *lpStructure, Neuron *lpNeuron, CStdXml &oXml);
		};

	}			//Synapses
}				//FastNeuralNet

#endif // !defined(AFX_GATEDSYNAPSE_H__21B27420_19DA_47AD_846E_7EBEDC8147D9__INCLUDED_)
