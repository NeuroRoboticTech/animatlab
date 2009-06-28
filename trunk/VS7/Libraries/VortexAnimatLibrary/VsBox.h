// VsBox.h: interface for the VsBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VSBOX_H__F74F0855_9701_4D03_82C4_EA3E5755910A__INCLUDED_)
#define AFX_VSBOX_H__F74F0855_9701_4D03_82C4_EA3E5755910A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

namespace VortexAnimatLibrary
{
	namespace Environment
	{
		namespace Bodies
		{

			class VORTEX_PORT VsBox : public Box, public VsRigidBody
			{
			protected:
				virtual void ResetNode(Simulator *lpSim, Structure *lpStructure);

			public:
				VsBox();
				virtual ~VsBox();

				virtual CStdFPoint GetCurrentPosition();

				virtual void CreateParts(Simulator *lpSim, Structure *lpStructure);
				virtual void CreateJoints(Simulator *lpSim, Structure *lpStructure);
				virtual void StepSimulation(Simulator *lpSim, Structure *lpStructure);
				virtual float *GetDataPointer(string strDataType);

				virtual void EnableCollision(RigidBody *lpBody);
				virtual void DisableCollision(RigidBody *lpBody);

				virtual void AddForce(Simulator *lpSim, float fltPx, float fltPy, float fltPz, float fltFx, float fltFy, float fltFz);
				virtual void AddTorque(Simulator *lpSim, float fltTx, float fltTy, float fltTz);

				//Test comment
				//void operator=(VsBox &oOrig);

				//CStdSerialize overloads
				//virtual CStdSerialize *Clone();
				//virtual void Trace(ostream &oOs);
				//virtual void Load(CStdXml &oXml);
			};

		}		//Bodies
	}			// Environment
}				//VortexAnimatLibrary

#endif // !defined(AFX_VSBOX_H__F74F0855_9701_4D03_82C4_EA3E5755910A__INCLUDED_)
