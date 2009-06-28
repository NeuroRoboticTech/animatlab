// VsCone.h: interface for the VsCone class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VSCONE_H__B6B13C0B_D733_44AF_917D_372FE21A4A2D__INCLUDED_)
#define AFX_VSCONE_H__B6B13C0B_D733_44AF_917D_372FE21A4A2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

namespace VortexAnimatLibrary
{
	namespace Environment
	{
		namespace Bodies
		{

			class VORTEX_PORT VsCone : public Cone, public VsRigidBody
			{
			protected:
				virtual void ResetNode(Simulator *lpSim, Structure *lpStructure);

			public:
				VsCone();
				virtual ~VsCone();

				virtual CStdFPoint GetCurrentPosition();

				virtual void CreateParts(Simulator *lpSim, Structure *lpStructure);
				virtual void CreateJoints(Simulator *lpSim, Structure *lpStructure);
				virtual void StepSimulation(Simulator *lpSim, Structure *lpStructure);
				virtual float *GetDataPointer(string strDataType);

				virtual void EnableCollision(RigidBody *lpBody);
				virtual void DisableCollision(RigidBody *lpBody);

				virtual void AddForce(Simulator *lpSim, float fltPx, float fltPy, float fltPz, float fltFx, float fltFy, float fltFz);
				virtual void AddTorque(Simulator *lpSim, float fltTx, float fltTy, float fltTz);
			};

		}		//Bodies
	}			// Environment
}				//VortexAnimatLibrary

#endif // !defined(AFX_VSCONE_H__B6B13C0B_D733_44AF_917D_372FE21A4A2D__INCLUDED_)
