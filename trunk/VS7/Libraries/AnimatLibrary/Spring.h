// Spring.h: interface for the Spring class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALSPRINGJOINT_H__FB4AFDAA_982E_4893_83F3_05BFF60F5643__INCLUDED_)
#define AFX_ALSPRINGJOINT_H__FB4AFDAA_982E_4893_83F3_05BFF60F5643__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

namespace AnimatLibrary
{
	namespace Environment
	{
		namespace Bodies
		{

			/*! \brief 
				A Spring type of joint.
			   
				\remarks
				This type of joint is constrained so that it can only
				rotate about one axis. You can define which axis it rotates
				around in the configuration file using the normalized 
				RotationAxis vector element. You can also specify the
				rotational constraints for this joint. This prevents it
				from rotating further than the constrained value.

				Also, this joint is motorized. So you can specify a desired
				velocity of motion at a given time step using the CNlInjectionMgr
				and the physics engine will automatically apply the forces
				necessary to move the joint at the desired velocity.

				\sa
				Joint, Spring, CAlStaticJoint
				 
				\ingroup AnimatLibrary
			*/

			class ANIMAT_PORT Spring : public RigidBody   
			{
			protected:
				///Keeps track of the initial state of the enabled flag.
				BOOL m_bInitEnabled;

				///The ID of the primary attachment site for this muscle.
				string m_strPrimaryAttachmentID;

				///The ID of the secondary attachment site for this muscle.
				string m_strSecondaryAttachmentID;

				///A pointer to the primary attachment part.
				MuscleAttachment *m_lpPrimaryAttachment;

				///A pointer to the secondary attachment part.
				MuscleAttachment *m_lpSecondaryAttachment;

				float m_fltNaturalLength;
				float m_fltStiffness;
				float m_fltDamping;
				float m_fltLength;
				float m_fltDisplacement;
				float m_fltTension;
				float m_fltEnergy;

			public:
				Spring();
				virtual ~Spring();

				string PrimaryAttachmentID() {return m_strPrimaryAttachmentID;};
				void PrimaryAttachmentID(string strVal)	{m_strPrimaryAttachmentID = strVal;};

				string SecondaryAttachmentID() {return m_strSecondaryAttachmentID;};
				void SecondaryAttachmentID(string strVal) {m_strSecondaryAttachmentID = strVal;};

				MuscleAttachment *PrimaryAttachment() {return m_lpPrimaryAttachment;};
				void PrimaryAttachment(MuscleAttachment *lpVal) {m_lpPrimaryAttachment = lpVal;};

				MuscleAttachment *SecondaryAttachment() {return m_lpSecondaryAttachment;};
				void SecondaryAttachment(MuscleAttachment *lpVal) {m_lpSecondaryAttachment = lpVal;};

				virtual void CreateParts(Simulator *lpSim, Structure *lpStructure);
				virtual void AddExternalNodeInput(Simulator *lpSim, Structure *lpStructure, float fltInput);
				virtual void Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
			};

		}		//Joints
	}			// Environment
}				//AnimatLibrary

#endif // !defined(AFX_ALSPRINGJOINT_H__FB4AFDAA_982E_4893_83F3_05BFF60F5643__INCLUDED_)
