// VsOdorSensor.h: interface for the VsOdorSensor class.
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

class VORTEX_PORT VsOdorSensor : public OdorSensor
{
protected:
	long m_lLastUpdateTime;
	MeMatrix4 m_aryBodyTM;

	//A pointer to the graphics instance.
	RGraphic *m_lpGraphic;

	virtual void UpdateAttachmentPointPosition(Simulator *lpSim);

public:
	VsOdorSensor();
	virtual ~VsOdorSensor();

	virtual CStdFPoint CalculatePosition(Simulator *lpSim);

	virtual void CreateParts(Simulator *lpSim, Structure *lpStructure);
	virtual void StepSimulation(Simulator *lpSim, Structure *lpStructure);
};

		}		//Bodies
	}			// Environment
}				//VortexAnimatLibrary

#endif // !defined(AFX_VSODOR_SENSOR_H__8438B067_2454_459B_8092_E74ABF23B265__INCLUDED_)
