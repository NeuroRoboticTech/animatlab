// VsBallSocket.h: interface for the VsBallSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VSBALLSOCKETJOINT_H__FB4AFDAA_982E_4893_83F3_05BFF60F5643__INCLUDED_)
#define AFX_VSBALLSOCKETJOINT_H__FB4AFDAA_982E_4893_83F3_05BFF60F5643__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

namespace VortexAnimatLibrary
{
	namespace Environment
	{
		namespace Joints
		{

			class VORTEX_PORT VsBallSocket : public VsJoint, public BallSocket     
			{
			protected:
				MdtConeLimitID m_lpConeLimitID;

				virtual void SetVelocityToDesired();

			public:
				VsBallSocket();
				virtual ~VsBallSocket();

				virtual void EnableMotor(BOOL bVal);
				virtual void CreateJoint(Simulator *lpSim, Structure *lpStructure);
				virtual float *GetDataPointer(string strDataType);
				//virtual void StepSimulation(Simulator *lpSim, Structure *lpStructure);
			};

		}		//Joints
	}			// Environment
}				//VortexAnimatLibrary

#endif // !defined(AFX_VSBALLSOCKETJOINT_H__FB4AFDAA_982E_4893_83F3_05BFF60F5643__INCLUDED_)
