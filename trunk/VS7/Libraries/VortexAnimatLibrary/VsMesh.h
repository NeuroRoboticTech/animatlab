// VsMesh.h: interface for the VsMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VSMESH_H__F74F0855_9701_4D03_82C4_EA3E5755910A__INCLUDED_)
#define AFX_VSMESH_H__F74F0855_9701_4D03_82C4_EA3E5755910A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

namespace VortexAnimatLibrary
{
	namespace Environment
	{
		namespace Bodies
		{

			class VORTEX_PORT VsMesh : public Mesh, public VsRigidBody
			{
			protected:
				virtual void InitializeContactSensor(Simulator *lpSim);
				virtual void ResetNode(Simulator *lpSim, Structure *lpStructure);

			public:
				VsMesh();
				virtual ~VsMesh();

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

#endif // !defined(AFX_VSMESH_H__F74F0855_9701_4D03_82C4_EA3E5755910A__INCLUDED_)
