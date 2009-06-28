// CurrentInjection.h: interface for the CurrentInjection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INJECTION_H__AEBF2DF9_E7A0_4ED2_83CD_BE74B7D74E59__INCLUDED_)
#define AFX_INJECTION_H__AEBF2DF9_E7A0_4ED2_83CD_BE74B7D74E59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace FastNeuralNet
{
	namespace ExternalStimuli
	{

		class FAST_NET_PORT CurrentInjection  : public ExternalStimulus
		{
		protected:
			string m_strOrganismID;
			CStdIPoint m_oPosition;
			Neuron *m_lpNeuron;

			float m_fltCurrent;

		public:
			CurrentInjection();
			virtual ~CurrentInjection();

			string OrganismID() {return m_strOrganismID;};
			void OrganismID(string strVal) {m_strOrganismID = strVal;};

			CStdIPoint Position() {return m_oPosition;};
			void Position(CStdIPoint oVal) {m_oPosition = oVal;};

			float Current() {return m_fltCurrent;};
			void Current(float fltVal) {m_fltCurrent = fltVal;};

			virtual void Load(Simulator *lpSim, CStdXml &oXml);
			virtual void Save(Simulator *lpSim, CStdXml &oXml);

			//ActiveItem overrides
			virtual string Type() {return "CurrentInjection";};
			virtual void Initialize(Simulator *lpSim);
			virtual void Activate(Simulator *lpSim);
			virtual void StepSimulation(Simulator *lpSim);
			virtual void Deactivate(Simulator *lpSim);
		};

	}			//ExternalStimuli
}				//FastNeuralNet

#endif // !defined(AFX_INJECTION_H__AEBF2DF9_E7A0_4ED2_83CD_BE74B7D74E59__INCLUDED_)
