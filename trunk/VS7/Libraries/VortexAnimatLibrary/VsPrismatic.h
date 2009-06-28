// VsPrismatic.h: interface for the VsPrismatic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VSPRISMATICJOINT_H__FB4AFDAA_982E_4893_83F3_05BFF60F5643__INCLUDED_)
#define AFX_VSPRISMATICJOINT_H__FB4AFDAA_982E_4893_83F3_05BFF60F5643__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

namespace VortexAnimatLibrary
{
	namespace Environment
	{
		namespace Joints
		{

			class VORTEX_PORT VsPrismatic : public VsJoint, public Prismatic     
			{
			protected:
				float m_fltDistanceUnits;
				float m_fltReportPosition;
				float m_fltReportActualVelocity;
				float m_fltReportSetVelocity;

				virtual void SetVelocityToDesired();
				void CalculateServoVelocity();

			public:
				VsPrismatic();
				virtual ~VsPrismatic();

				virtual float JointPosition() {return VsJoint::JointPosition();};
				virtual float JointVelocity() {return VsJoint::JointVelocity();};

				virtual void Enabled(BOOL bValue) 
				{
					EnableMotor(bValue);
					m_bEnabled = bValue;
				};

				virtual void EnableMotor(BOOL bVal);
				virtual void CreateJoint(Simulator *lpSim, Structure *lpStructure);
				virtual float *GetDataPointer(string strDataType);
				virtual void StepSimulation(Simulator *lpSim, Structure *lpStructure);
			};

		}		//Joints
	}			// Environment
}				//VortexAnimatLibrary

#endif // !defined(AFX_VSPRISMATICJOINT_H__FB4AFDAA_982E_4893_83F3_05BFF60F5643__INCLUDED_)
