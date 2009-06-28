// JointInputStimulus.h: interface for the JointInputStimulus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JOINT_INPUT_STIMULUS_H__AEBF2DF9_E7A0_4ED2_83CD_BE74B7D74E59__INCLUDED_)
#define AFX_JOINT_INPUT_STIMULUS_H__AEBF2DF9_E7A0_4ED2_83CD_BE74B7D74E59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace AnimatLibrary
{
	namespace ExternalStimuli
	{

		class ANIMAT_PORT JointInputStimulus  : public ExternalInputStimulus
		{
		protected:
			string m_strJointID;

		public:
			JointInputStimulus();
			virtual ~JointInputStimulus();

			virtual void Load(Simulator *lpSim, CStdXml &oXml);
			virtual void Save(Simulator *lpSim, CStdXml &oXml);

			//ActiveItem overrides
			virtual string Type() {return "JointInput";};
			virtual void Initialize(Simulator *lpSim);
		};

	}			//ExternalStimuli
}				//VortexAnimatLibrary

#endif // !defined(AFX_JOINT_INPUT_STIMULUS_H__AEBF2DF9_E7A0_4ED2_83CD_BE74B7D74E59__INCLUDED_)
