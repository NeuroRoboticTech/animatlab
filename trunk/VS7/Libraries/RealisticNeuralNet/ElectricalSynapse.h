// ElecSyn.h: interface for the ElectricalSynapse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELECSYN_H__3D41A66D_6746_48F6_8F0C_685EF62CC07C__INCLUDED_)
#define AFX_ELECSYN_H__3D41A66D_6746_48F6_8F0C_685EF62CC07C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace RealisticNeuralNet
{
	namespace Synapses
	{

		class ADV_NEURAL_PORT ElectricalSynapse : public CStdSerialize  
		{
		public:
			ElectricalSynapse();
			virtual ~ElectricalSynapse();
			virtual void Load(CStdXml &oXml);

		protected:
			string m_strName;
			double m_dLowCoup;
			double m_dHiCoup;
			double m_dTurnOnV;
			double m_dSaturateV;

		friend class RealisticNeuralModule;
		};

	}			//Synapses
}				//RealisticNeuralNet

#endif // !defined(AFX_ELECSYN_H__3D41A66D_6746_48F6_8F0C_685EF62CC07C__INCLUDED_)
