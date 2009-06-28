// VsPlane.h: interface for the VsPlane class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VSPLANE_H__76C24FAB_0AFC_4822_BCA0_D3055E824E6E__INCLUDED_)
#define AFX_VSPLANE_H__76C24FAB_0AFC_4822_BCA0_D3055E824E6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

namespace VortexAnimatLibrary
{
	namespace Environment
	{
		namespace Bodies
		{

			class VORTEX_PORT VsPlane : public Plane, public VsRigidBody  
			{
			protected:
				MeMatrix4 m_aryGroundTM; 

			public:
				VsPlane();
				virtual ~VsPlane();

				virtual void CreateParts(Simulator *lpSim, Structure *lpStructure);
				
				virtual void EnableCollision(RigidBody *lpBody);
				virtual void DisableCollision(RigidBody *lpBody);
			};

		}		//Bodies
	}			// Environment
}				//VortexAnimatLibrary

#endif // !defined(AFX_VSPLANE_H__76C24FAB_0AFC_4822_BCA0_D3055E824E6E__INCLUDED_)
