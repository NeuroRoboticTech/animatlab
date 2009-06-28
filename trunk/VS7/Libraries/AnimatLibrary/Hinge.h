// Hinge.h: interface for the Hinge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALHINGEJOINT_H__FB4AFDAA_982E_4893_83F3_05BFF60F5643__INCLUDED_)
#define AFX_ALHINGEJOINT_H__FB4AFDAA_982E_4893_83F3_05BFF60F5643__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

namespace AnimatLibrary
{
	namespace Environment
	{
		namespace Joints
		{

			/*! \brief 
				A hinge type of joint.
			   
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
				Joint, Hinge, CAlStaticJoint
				 
				\ingroup AnimatLibrary
			*/

			class ANIMAT_PORT Hinge : public Joint    
			{
			protected:
				///The normalized axis of rotation. If you just want to rotate about the x axis then 
				///you would specify <1, 0, 0>. But you can also rotate about a rotated axis.
				CStdFPoint m_oRotationAxis;

				///This is the minimum radian value that the joint can rotate about its axis.
				///Its orginal position is used as zero radians.
				float m_fltConstraintLow;

				///This is the maximum radian value that the joint can rotate about its axis.
				///Its orginal position is used as zero radians.
				float m_fltConstraintHigh;

				float m_fltMaxTorque;

				float m_fltTorque;

				float m_ftlServoGain;
				BOOL m_bServoMotor;

				float m_fltRotation;
				float m_fltVelocity;

			public:
				Hinge();
				virtual ~Hinge();

				void ServoMotor(BOOL bServo) {m_bServoMotor = bServo;};
				BOOL ServoMotor() {return m_bServoMotor;};

				virtual float JointPosition() {return m_fltRotation;};
				virtual float JointVelocity() {return m_fltVelocity;};

				virtual void Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
			};

		}		//Joints
	}			// Environment
}				//AnimatLibrary

#endif // !defined(AFX_ALHINGEJOINT_H__FB4AFDAA_982E_4893_83F3_05BFF60F5643__INCLUDED_)
