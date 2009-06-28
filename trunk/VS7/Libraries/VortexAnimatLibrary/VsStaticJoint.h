// VsStaticJoint.h: interface for the VsStatic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VSSTATICJOINT_H__F3E79BB8_70BC_4577_9009_EF5F64932DC6__INCLUDED_)
#define AFX_VSSTATICJOINT_H__F3E79BB8_70BC_4577_9009_EF5F64932DC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

namespace VortexAnimatLibrary
{
	namespace Environment
	{
		namespace Joints
		{

			class VORTEX_PORT VsStatic : public VsJoint, public Static
			{
			protected:

				virtual void SetVelocityToDesired() {};

			public:
				VsStatic();
				virtual ~VsStatic();

				virtual void EnableMotor(BOOL bVal);
				virtual void CreateJoint(Simulator *lpSim, Structure *lpStructure);
			};

		}		//Joints
	}			// Environment
}				//VortexAnimatLibrary

#endif // !defined(AFX_VSSTATICJOINT_H__F3E79BB8_70BC_4577_9009_EF5F64932DC6__INCLUDED_)
