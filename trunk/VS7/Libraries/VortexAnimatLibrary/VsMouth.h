// VsMouth.h: interface for the VsMouth class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VSMOUTH_H__8438B067_2454_459B_8092_E74ABF23B265__INCLUDED_)
#define AFX_VSMOUTH_H__8438B067_2454_459B_8092_E74ABF23B265__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

namespace VortexAnimatLibrary
{
	namespace Environment
	{
		namespace Bodies
		{

class VORTEX_PORT VsMouth : public Mouth
{
protected:
	long m_lLastUpdateTime;
	MeMatrix4 m_aryBodyTM;

	//A pointer to the graphics instance.
	RGraphic *m_lpGraphic;

	virtual void UpdateAttachmentPointPosition(Simulator *lpSim);

	//Store the body data.
	MeVector3 m_vPosition;

public:
	VsMouth();
	virtual ~VsMouth();

	virtual CStdFPoint CalculatePosition(Simulator *lpSim);

	virtual float *GetDataPointer(string strDataType);
	virtual void CreateParts(Simulator *lpSim, Structure *lpStructure);
	virtual void StepSimulation(Simulator *lpSim, Structure *lpStructure);
};

		}		//Bodies
	}			// Environment
}				//VortexAnimatLibrary

#endif // !defined(AFX_VSMOUTH_H__8438B067_2454_459B_8092_E74ABF23B265__INCLUDED_)
