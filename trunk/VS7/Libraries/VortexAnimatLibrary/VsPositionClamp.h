// VsPositionClamp.h: interface for the VsPositionClamp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POSITION_CLAMP_H__AEBF2DF9_E7A0_4ED2_83CD_BE74B7D74E59__INCLUDED_)
#define AFX_POSITION_CLAMP_STIMULUS_H__AEBF2DF9_E7A0_4ED2_83CD_BE74B7D74E59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace VortexAnimatLibrary
{
	namespace ExternalStimuli
	{

		class VORTEX_PORT VsPositionClamp  : public AnimatLibrary::ExternalStimuli::ExternalStimulus
		{
		protected:
			Structure *m_lpStructure;
			string m_strStructureID;

			string m_strBodyID;
			Joint *m_lpJoint;
			VsJoint *m_lpVsJoint;

			///The ID of the primary attachment site for this muscle.
			string m_strPrimaryAttachmentID;

			///The ID of the secondary attachment site for this muscle.
			string m_strSecondaryAttachmentID;

			///A pointer to the primary attachment part.
			MuscleAttachment *m_lpPrimaryAttachment;

			///A pointer to the secondary attachment part.
			MuscleAttachment *m_lpSecondaryAttachment;

			VsRigidBody *m_lpAttachParent;

			float m_fltPGain;
			float m_fltDGain;
			float m_fltIGain;
			float m_fltMaxIValue;

			BOOL m_bUsePosition;
			float m_fltTarget;
			float m_fltValue;
			float m_fltVelocity;
			float m_fltPrevVelocity;
			float m_fltMaxForce;

			float m_fltForce;
			float m_fltPError;
			float m_fltDError;
			float m_fltIError;

			CStdFPoint m_oPrimPos, m_oSecPos, m_oForce;

			void CalculateForceVector(Simulator *lpSim, CStdFPoint &oPrimPos, CStdFPoint &oSecPos, float fltTension, CStdFPoint &oForce);

		public:
			VsPositionClamp();
			virtual ~VsPositionClamp();

			string PrimaryAttachmentID() {return m_strPrimaryAttachmentID;};
			void PrimaryAttachmentID(string strVal)	{m_strPrimaryAttachmentID = strVal;};

			string SecondaryAttachmentID() {return m_strSecondaryAttachmentID;};
			void SecondaryAttachmentID(string strVal) {m_strSecondaryAttachmentID = strVal;};

			MuscleAttachment *PrimaryAttachment() {return m_lpPrimaryAttachment;};
			void PrimaryAttachment(MuscleAttachment *lpVal) {m_lpPrimaryAttachment = lpVal;};

			MuscleAttachment *SecondaryAttachment() {return m_lpSecondaryAttachment;};
			void SecondaryAttachment(MuscleAttachment *lpVal) {m_lpSecondaryAttachment = lpVal;};

			virtual void Load(Simulator *lpSim, CStdXml &oXml);
			virtual void Save(Simulator *lpSim, CStdXml &oXml);

			virtual float *GetDataPointer(string strDataType);

			//ActiveItem overrides
			virtual string Type() {return "PositionClamp";};
			virtual void Initialize(Simulator *lpSim);
			virtual void Activate(Simulator *lpSim);
			virtual void StepSimulation(Simulator *lpSim);
			virtual void Deactivate(Simulator *lpSim);
		};

	}			//ExternalStimuli
}				//VortexAnimatLibrary

#endif // !defined(AFX_POSITION_CLAMP_STIMULUS_H__AEBF2DF9_E7A0_4ED2_83CD_BE74B7D74E59__INCLUDED_)
