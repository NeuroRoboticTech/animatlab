// VsForceStimulus.h: interface for the VsForceStimulus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FORCE_STIMULUS_H__AEBF2DF9_E7A0_4ED2_83CD_BE74B7D74E59__INCLUDED_)
#define AFX_FORCE_INPUT_STIMULUS_H__AEBF2DF9_E7A0_4ED2_83CD_BE74B7D74E59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace VortexAnimatLibrary
{
	namespace ExternalStimuli
	{

		class VORTEX_PORT VsForceStimulus  : public AnimatLibrary::ExternalStimuli::ExternalStimulus
		{
		protected:
			Structure *m_lpStructure;
			string m_strStructureID;

			string m_strBodyID;
			RigidBody *m_lpBody;
			VsRigidBody *m_lpVsBody;

			CStdFPoint m_oRelativePosition;
			CStdFPoint m_oForcePosition;

			string m_strForceXEquation;
			string m_strForceYEquation;
			string m_strForceZEquation;

			CStdPostFixEval *m_lpForceXEval;
			CStdPostFixEval *m_lpForceYEval;
			CStdPostFixEval *m_lpForceZEval;

			float m_fltForceX;
			float m_fltForceY;
			float m_fltForceZ;

			string m_strTorqueXEquation;
			string m_strTorqueYEquation;
			string m_strTorqueZEquation;

			CStdPostFixEval *m_lpTorqueXEval;
			CStdPostFixEval *m_lpTorqueYEval;
			CStdPostFixEval *m_lpTorqueZEval;

			float m_fltTorqueX;
			float m_fltTorqueY;
			float m_fltTorqueZ;

		public:
			VsForceStimulus();
			virtual ~VsForceStimulus();

			virtual void Load(Simulator *lpSim, CStdXml &oXml);
			virtual void Save(Simulator *lpSim, CStdXml &oXml);

			//ActiveItem overrides
			virtual string Type() {return "ForceInput";};
			virtual void Initialize(Simulator *lpSim);
			virtual void Activate(Simulator *lpSim);
			virtual void StepSimulation(Simulator *lpSim);
			virtual void Deactivate(Simulator *lpSim);
		};

	}			//ExternalStimuli
}				//VortexAnimatLibrary

#endif // !defined(AFX_FORCE_INPUT_STIMULUS_H__AEBF2DF9_E7A0_4ED2_83CD_BE74B7D74E59__INCLUDED_)
