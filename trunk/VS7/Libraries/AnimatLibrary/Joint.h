// Joint.h: interface for the Joint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALJOINT_H__93EDBBFE_2FA0_467C_970F_1775454FE94E__INCLUDED_)
#define AFX_ALJOINT_H__93EDBBFE_2FA0_467C_970F_1775454FE94E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

namespace AnimatLibrary
{
	namespace Environment
	{

		/*! \brief 
			The base class for all of the joint type of objects.

			\remarks
			This class provides the base functionality for a joint. 
			A joint is what glues the different rigid bodies together
			into a complete structure. Each object is connected by a 
			joint to its parent part. The only exception to this rule
			is the root rigid body of a structure/organism. The joint is
			positioned relative to the center of the parent object. 
			This can be a little confusing becuase in the configuration
			file the joint is actually contained within the child object
			tag. So you may be tempted to try and make the joint relative
			to the child object. The different sub classes will define 
			data parameters that specify the motion that they joint can 
			perform. For instance, a CAlHingeJoint is constrained to rotate
			about one axis just like the hinge on a door. But if you simply
			wanted one part act as if it is phyiscally connected to its parent
			part then you would use a CAlStaticJoint.

			\sa
			Joint, CAlHingeJoint, CAlStaticJoint
			 
			\ingroup AnimatLibrary
		*/

		class ANIMAT_PORT Joint : public Node  
		{
		protected:
			///The parent rigid body for this joint. 
			RigidBody *m_lpParent;

			///The child rigid body for this joint. 
			RigidBody *m_lpChild;

			///The relative position of the of this joint
			///in relation to the center of its parent rigid body. 
			CStdFPoint m_oRelPosition;

			///The absolute position of the joint in world coordinates.
			///This is calcualted during loading of the joint using the position of 
			///the parent part and the relative position specified in the configuration file.
			CStdFPoint m_oAbsPosition;

			///This is the velocity to use for the motorized joint. The motor must be enabled
			///for this parameter to have any effect.
			float m_fltSetVelocity;
			float m_fltDesiredVelocity;
			float m_fltMaxVelocity;
			float m_fltPrevVelocity;

			BOOL m_bEnableMotor;
			BOOL m_bEnableLimits;
			BOOL m_bEnableLock;
			float m_fltDamping;
			float m_fltRestitution;
			float m_fltStiffness;

			virtual void SetVelocityToDesired();

		public:
			Joint();
			virtual ~Joint();

			//Sometimes we need to know if the joint rotates or moves linearly. 
			//This param tells us which it does.
			virtual BOOL UsesRadians() {return TRUE;};

			CStdFPoint RelativePosition() {return m_oRelPosition;};
			void RelativePosition(CStdFPoint &oPoint) {m_oRelPosition = oPoint;};

			CStdFPoint AbsolutePosition() {return m_oAbsPosition;};
			void AbsolutePosition(CStdFPoint &oPoint)	{m_oAbsPosition = oPoint;};

			RigidBody *Parent() {return m_lpParent;};
			void Parent(RigidBody *lpValue) {m_lpParent = lpValue;};

			RigidBody *Child() {return m_lpChild;};
			void Child(RigidBody *lpValue) {m_lpChild = lpValue;};

			float SetVelocity() {return m_fltSetVelocity;};
			float DesiredVelocity() {return m_fltDesiredVelocity;};
			virtual void DesiredVelocity(float fltVelocity) {m_fltDesiredVelocity = fltVelocity;};
			virtual void MotorInput(float fltInput) {m_fltDesiredVelocity = fltInput;}
			virtual float JointPosition() {return 0;};
			virtual float JointVelocity() {return 0;};

			virtual void EnableMotor(BOOL bVal);

			virtual void CreateJoint(Simulator *lpSim, Structure *lpStructure);

			//Node Overrides
			virtual void Reset(Simulator *lpSim, Structure *lpStructure);
			virtual void AddExternalNodeInput(Simulator *lpSim, Structure *lpStructure, float fltInput);
			virtual float *GetDataPointer(string strDataType);
			virtual void StepSimulation(Simulator *lpSim, Structure *lpStructure);
			virtual void Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
			virtual void Save(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
		};

	}			// Environment
}				//AnimatLibrary

#endif // !defined(AFX_ALJOINT_H__93EDBBFE_2FA0_467C_970F_1775454FE94E__INCLUDED_)
