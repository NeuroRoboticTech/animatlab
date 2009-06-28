// VsJoint.h: interface for the VsJoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VSJOINT_H__93EDBBFE_2FA0_467C_970F_1775454FE94E__INCLUDED_)
#define AFX_VSJOINT_H__93EDBBFE_2FA0_467C_970F_1775454FE94E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

namespace VortexAnimatLibrary
{
	namespace Environment
	{

		/*! \brief 
			A common class for all joint data specific to vortex.

			\remarks
			This is a common class for the joint objects that 
			specifically holds vortex data and methods. The 
			reasoning behind this class is the same as that for
			the VsRigidBody. Please see that class for an explanation
			of why this class is needed.

			\sa
			RigidBody, CVsAlJoint
		*/

		class VORTEX_PORT VsJoint
		{
		protected:
			MdtConstraintID m_lpConstraintID;
			MdtLimitID  m_lpLimitID;
			BOOL m_bMotorOn;
			BOOL m_bLockOn;

			//Stores the current position of the joint
			BOOL m_bCollectData;
			MeVector3 m_vPosition;

			void EnableMotor(BOOL bOn, float fltDesiredVelocity, float fltMaxForce);
			void EnableLock(MeBool bOn, float fltPosition, float fltMaxLockForce);
			void SetVelocity(float fltDesiredVelocity, float fltMaxForce);

			void CollectJointData(Simulator *lpSim);
			float *GetVortexDataPointer(string &strDataType);

		public:
			VsJoint();
			virtual ~VsJoint();

			virtual float JointPosition() 
			{
				if(m_lpLimitID)
					return m_lpLimitID->position;
				else
					return 0;
			};

			virtual float JointVelocity()
			{
				if(m_lpLimitID)
					return m_lpLimitID->velocity;
				else
					return 0;
			}

			MdtConstraintID ConstraintID();
			MdtLimitID  LimitID();

		};

	}			// Environment
}				//VortexAnimatLibrary

#endif // !defined(AFX_VSJOINT_H__93EDBBFE_2FA0_467C_970F_1775454FE94E__INCLUDED_)
