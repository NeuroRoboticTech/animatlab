// VsSpring.h: interface for the VsSpring class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VSSPRINGJOINT_H__FB4AFDAA_982E_4893_83F3_05BFF60F5643__INCLUDED_)
#define AFX_VSSPRINGJOINT_H__FB4AFDAA_982E_4893_83F3_05BFF60F5643__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

namespace VortexAnimatLibrary
{
	namespace Environment
	{
		namespace Bodies
		{

			class VORTEX_PORT VsSpring : public VsRigidBody, public Spring     
			{
			protected:
				MdtSpringID m_lpSpringID;

				float m_fltT1, m_fltT2, m_fltT3, m_fltT4;

				void DrawSpring(Simulator *lpSim);

			public:
				VsSpring();
				virtual ~VsSpring();

				virtual void Enabled(BOOL bVal);

				virtual void CreateJoints(Simulator *lpSim, Structure *lpStructure);
				virtual float *GetDataPointer(string strDataType);
				virtual void StepSimulation(Simulator *lpSim, Structure *lpStructure);
			};

		}		//Joints
	}			// Environment
}				//VortexAnimatLibrary

#endif // !defined(AFX_VSSPRINGJOINT_H__FB4AFDAA_982E_4893_83F3_05BFF60F5643__INCLUDED_)
