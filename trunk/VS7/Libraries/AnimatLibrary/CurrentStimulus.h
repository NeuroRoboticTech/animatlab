// CurrentStimulus.h: interface for the CurrentStimulus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CURRENT_STIMULUS_H__AEBF2DF9_E7A0_4ED2_83CD_BE74B7D74E59__INCLUDED_)
#define AFX_CURRENT_STIMULUS_H__AEBF2DF9_E7A0_4ED2_83CD_BE74B7D74E59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace AnimatLibrary
{
	namespace ExternalStimuli
	{

		class ANIMAT_PORT CurrentStimulus  : public ExternalStimulus
		{
		protected:
			Organism *m_lpOrganism;
			string m_strOrganismID;

			string m_strNeuralModule;
			long m_lTargetNodeID;

			Node *m_lpNode;
			float *m_lpExternalCurrent;

			BOOL m_bUseEquation;
			CStdPostFixEval *m_lpCurrentOnEval;
			int m_iType;  //Current type

			float m_fltActiveCurrent;
			float m_fltCurrentOn;
			float m_fltCurrentOff;
			float m_fltCurrentBurstOff;

			//There are the durations in time.
			float m_fltCycleOnDuration;
			float m_fltCycleOffDuration;
			float m_fltBurstOnDuration;
			float m_fltBurstOffDuration;

			//The durations are converted to time slice values for easier comparisons.
			long m_lCycleOnDuration;
			long m_lCycleOffDuration;
			long m_lBurstOnDuration;
			long m_lBurstOffDuration;

			long m_lCycleStart;
			long m_lBurstStart;

			BOOL m_bCycleOn;
			BOOL m_bBurstOn;

			float GetCurrentOn(Simulator *lpSim);

		public:
			CurrentStimulus();
			virtual ~CurrentStimulus();

			virtual void Load(Simulator *lpSim, CStdXml &oXml);
			virtual void Save(Simulator *lpSim, CStdXml &oXml);
			virtual void Trace(ostream &oOs);

			//ActiveItem overrides
			virtual string Type() {return "CurrentStimulus";};
			virtual void Initialize(Simulator *lpSim);  
			virtual void Activate(Simulator *lpSim);
			virtual void StepSimulation(Simulator *lpSim);
			virtual void Deactivate(Simulator *lpSim);

			virtual void Modify(Simulator *lpSim, ActivatedItem *lpItem);
		};

	}			//ExternalStimuli
}				//VortexAnimatLibrary

#endif // !defined(AFX_CURRENT_STIMULUS_H__AEBF2DF9_E7A0_4ED2_83CD_BE74B7D74E59__INCLUDED_)
