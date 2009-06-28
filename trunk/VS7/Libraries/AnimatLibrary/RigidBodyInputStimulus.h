// RigidBodyInputStimulus.h: interface for the RigidBodyInputStimulus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIGID_BODY_INPUT_STIMULUS_H__AEBF2DF9_E7A0_4ED2_83CD_BE74B7D74E59__INCLUDED_)
#define AFX_RIGID_BODY_INPUT_STIMULUS_H__AEBF2DF9_E7A0_4ED2_83CD_BE74B7D74E59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace AnimatLibrary
{
	namespace ExternalStimuli
	{

		class ANIMAT_PORT RigidBodyInputStimulus  : public ExternalInputStimulus
		{
		protected:
			string m_strBodyID;

		public:
			RigidBodyInputStimulus();
			virtual ~RigidBodyInputStimulus();

			virtual void Load(Simulator *lpSim, CStdXml &oXml);
			virtual void Save(Simulator *lpSim, CStdXml &oXml);

			//ActiveItem overrides
			virtual string Type() {return "RigidBodyInput";};
			virtual void Initialize(Simulator *lpSim);
		};

	}			//ExternalStimuli
}				//VortexAnimatLibrary

#endif // !defined(AFX_RIGID_BODY_INPUT_STIMULUS_H__AEBF2DF9_E7A0_4ED2_83CD_BE74B7D74E59__INCLUDED_)
