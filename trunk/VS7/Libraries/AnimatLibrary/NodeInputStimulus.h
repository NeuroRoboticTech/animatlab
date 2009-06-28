// NodeInputStimulus.h: interface for the NodeInputStimulus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODE_INPUT_STIMULUS_H__AEBF2DF9_E7A0_4ED2_83CD_BE74B7D74E59__INCLUDED_)
#define AFX_NODE_INPUT_STIMULUS_H__AEBF2DF9_E7A0_4ED2_83CD_BE74B7D74E59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace AnimatLibrary
{
	namespace ExternalStimuli
	{

		class ANIMAT_PORT NodeInputStimulus  : public ExternalInputStimulus
		{
		protected:
			string m_strNeuralModule;
			long m_lTargetNodeID;

		public:
			NodeInputStimulus();
			virtual ~NodeInputStimulus();

			virtual void Load(Simulator *lpSim, CStdXml &oXml);
			virtual void Save(Simulator *lpSim, CStdXml &oXml);

			//ActiveItem overrides
			virtual string Type() {return "NodeInput";};
			virtual void Initialize(Simulator *lpSim);
		};

	}			//ExternalStimuli
}				//VortexAnimatLibrary

#endif // !defined(AFX_NODE_INPUT_STIMULUS_H__AEBF2DF9_E7A0_4ED2_83CD_BE74B7D74E59__INCLUDED_)
