// CompoundSynapse.h: interface for the CompoundSynapse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPOUNDSYNAPSE_H__D2C20749_096C_4772_8942_C587EA2DCA9A__INCLUDED_)
#define AFX_COMPOUNDSYNAPSE_H__D2C20749_096C_4772_8942_C587EA2DCA9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace FastNeuralNet
{
	namespace Synapses
	{

		class FAST_NET_PORT CompoundSynapse : public Synapse     
		{
		protected:
			CStdPtrArray<Synapse> m_arySynapses;

			Synapse *LoadSynapse(Simulator *lpSim, Structure *lpStructure, Neuron *lpNeuron, CStdXml &oXml);

		public:
			CompoundSynapse();
			virtual ~CompoundSynapse();

			virtual unsigned char Type()
			{return COMPOUND_SYNAPSE;};

			virtual float CalculateModulation(FastNeuralModule *lpModule);
			
			virtual Synapse *GetCompoundSynapse(short iCompoundIndex);
			virtual float *GetDataPointer(short iCompoundIndex, short iDataType);

			virtual void Load(Simulator *lpSim, Structure *lpStructure, Neuron *lpNeuron, CStdXml &oXml);
			virtual void Save(Simulator *lpSim, Structure *lpStructure, Neuron *lpNeuron, CStdXml &oXml);
		};

	}			//Synapses
}				//FastNeuralNet

#endif // !defined(AFX_COMPOUNDSYNAPSE_H__D2C20749_096C_4772_8942_C587EA2DCA9A__INCLUDED_)
