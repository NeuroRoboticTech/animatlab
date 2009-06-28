// MultiSegmentSpring.h: interface for the MultiSegmentSpring class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AL_MULTISEG_SPRINGJOINT_H__FB4AFDAA_982E_4893_83F3_05BFF60F5643__INCLUDED_)
#define AFX_AL_MULTISEG_SPRINGJOINT_H__FB4AFDAA_982E_4893_83F3_05BFF60F5643__INCLUDED_

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

			class ANIMAT_PORT MultiSegmentSpring : public RigidBody   
			{
			protected:
				///Keeps track of the initial state of the enabled flag.
				BOOL m_bInitEnabled;

				float m_fltNaturalLength;
				float m_fltStiffness;
				float m_fltDamping;
				float m_fltLength;
				float m_fltDisplacement;
				float m_fltDisplacementRatio;
				float m_fltTension;
				float m_fltSpringTension;
				float m_fltDampingTension;

				int m_iForceType;

				///The maximum tension that this muscle can ever generate. This is an upper limit to prevent unrealistic tension values.
				float m_fltMaxTension;

				///Current length of the spring.
				float m_fltSpringLength;

				///Length of the spring in the previous timestep
				float m_fltPrevSpringLength;

				///The velocity of shortenting of the spring.
				float m_fltVspring;

				///velocity of the spring in the previous timestep
				float m_fltPrevVspring;

				///The acceleration of shortenting of the spring.
				float m_fltAspring;

				///The potential energy of spring.
				float m_fltPotEnergy;

				///The ID's of the attachment points for this muscle. This is used during the load/initialization process.
				CStdArray<string> m_aryAttachmentPointIDs;

				///A pointer to the primary attachment part.
				CStdArray<MuscleAttachment *> m_aryAttachmentPoints;

				void CalculateTension(Simulator *lpSim);
				int ConvertForceType(string strOrigType);

			public:
				MultiSegmentSpring();
				virtual ~MultiSegmentSpring();

				float Tension() {return m_fltTension;};
				void Tension(float fltVal) {m_fltTension = fltVal;};

				CStdArray<MuscleAttachment *>  AttachmentPoints() {return m_aryAttachmentPoints;};

				virtual void Enabled(BOOL bVal);

				virtual float CalculateSpringLength(Simulator *lpSim);

				virtual void CreateParts(Simulator *lpSim, Structure *lpStructure);
				virtual void CreateJoints(Simulator *lpSim, Structure *lpStructure);
				virtual void Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
				virtual float *GetDataPointer(string strDataType);
				virtual void AddExternalNodeInput(Simulator *lpSim, Structure *lpStructure, float fltInput);
			};

		}		//Joints
	}			// Environment
}				//AnimatLibrary

#endif // !defined(AFX_AL_MULTISEG_SPRINGJOINT_H__FB4AFDAA_982E_4893_83F3_05BFF60F5643__INCLUDED_)
