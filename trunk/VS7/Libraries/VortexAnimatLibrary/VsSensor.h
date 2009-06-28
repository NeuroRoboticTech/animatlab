// VsSensor.h: interface for the VsSensor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VSODOR_SENSOR_H__8438B067_2454_459B_8092_E74ABF23B265__INCLUDED_)
#define AFX_VSODOR_SENSOR_H__8438B067_2454_459B_8092_E74ABF23B265__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

namespace VortexAnimatLibrary
{
	namespace Environment
	{
		namespace Bodies
		{

class VORTEX_PORT VsSensor : public RigidBody
{
protected:
	float m_fltRadius;
	long m_lLastUpdateTime;
	MeMatrix4 m_aryBodyTM;

	//A pointer to the graphics instance.
	RGraphic *m_lpGraphic;

	virtual void UpdateAttachmentPointPosition(Simulator *lpSim);

public:
	VsSensor();
	virtual ~VsSensor();

	virtual CStdFPoint CalculatePosition(Simulator *lpSim);

	virtual float *GetDataPointer(string strDataType);
	virtual void CreateParts(Simulator *lpSim, Structure *lpStructure);
	virtual void CreateJoints(Simulator *lpSim, Structure *lpStructure);
	virtual void StepSimulation(Simulator *lpSim, Structure *lpStructure);
	virtual void Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml);
};

		}		//Bodies
	}			// Environment
}				//VortexAnimatLibrary

#endif // !defined(AFX_VSODOR_SENSOR_H__8438B067_2454_459B_8092_E74ABF23B265__INCLUDED_)
