// NonSpikingChemSyn.h: interface for the NonSpikingChemicalSynapse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NONSPIKINGCHEMSYN_H__364646F7_CCAC_49C0_8B61_BFCCDC72E7F0__INCLUDED_)
#define AFX_NONSPIKINGCHEMSYN_H__364646F7_CCAC_49C0_8B61_BFCCDC72E7F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace RealisticNeuralNet
{
	namespace Synapses
	{

		class ADV_NEURAL_PORT NonSpikingChemicalSynapse : public CStdSerialize  
		{
		public:
			NonSpikingChemicalSynapse();
			virtual ~NonSpikingChemicalSynapse();
			virtual void Load(CStdXml &oXml);

		protected:
			string m_strName;
			double m_dEquil;
			double m_dSynAmp;
			double m_dThreshV;
			double m_dSaturateV;

		friend class RealisticNeuralModule;
		};

	}			//Synapses
}				//RealisticNeuralNet

#endif // !defined(AFX_NONSPIKINGCHEMSYN_H__364646F7_CCAC_49C0_8B61_BFCCDC72E7F0__INCLUDED_)
