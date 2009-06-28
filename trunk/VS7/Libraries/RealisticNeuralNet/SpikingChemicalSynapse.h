// SpikingChemSyn.h: interface for the SpikingChemicalSynapse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPIKINGCHEMSYN_H__3C55C58A_9DA9_4B5F_BC16_96675C022F6D__INCLUDED_)
#define AFX_SPIKINGCHEMSYN_H__3C55C58A_9DA9_4B5F_BC16_96675C022F6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace RealisticNeuralNet
{
	namespace Synapses
	{

		class ADV_NEURAL_PORT SpikingChemicalSynapse : public CStdSerialize  
		{
		public:
			SpikingChemicalSynapse();
			virtual ~SpikingChemicalSynapse();
			virtual void Load(CStdXml &oXml);

		protected:
			string m_strName;
			double m_dEquil;
			double m_dSynAmp;		// base syn amp, before vd or hebb
			double m_dDecay;
			double m_dRelFacil;
			double m_dFacilDecay;

			BOOL m_bVoltDep;
			double m_dMaxRelCond;
			double m_dSatPSPot;
			double m_dThreshPSPot;

			BOOL m_bHebbian;
			double m_dMaxAugCond;
			double m_dLearningInc;
			double m_dLearningTime;
			BOOL m_bAllowForget;
			double m_dForgetTime;
			double m_dConsolidation;

		friend class RealisticNeuralModule;
		friend class Neuron;
		};

	}			//Synapses
}				//RealisticNeuralNet

#endif // !defined(AFX_SPIKINGCHEMSYN_H__3C55C58A_9DA9_4B5F_BC16_96675C022F6D__INCLUDED_)
