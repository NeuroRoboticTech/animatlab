// VsLinearHillStretchReceptor.h: interface for the VsMuscle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VSLINEAR_HILL_STRETCH_RECEPTOR_H__8438B067_2454_459B_8092_E74ABF23B265__INCLUDED_)
#define AFX_VSLINEAR_HILL_STRETCH_RECEPTOR_H__8438B067_2454_459B_8092_E74ABF23B265__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

namespace VortexAnimatLibrary
{
	namespace Environment
	{
		namespace Bodies
		{

class VORTEX_PORT VsLinearHillStretchReceptor : public LinearHillStretchReceptor
{
protected:	
	//A pointer to the graphics instance.
	CStdArray<RGraphic *> m_aryLines;

	void CalculateForceVector(Simulator *lpSim, MuscleAttachment *lpPrim, MuscleAttachment *lpSec, float fltTension, CStdFPoint &oPrimPos, CStdFPoint &oSecPos, CStdFPoint &oPrimForce);
	void DrawMuscle(Simulator *lpSim, BOOL bInitDraw);

public:
	VsLinearHillStretchReceptor();
	virtual ~VsLinearHillStretchReceptor();

	virtual void CreateJoints(Simulator *lpSim, Structure *lpStructure);
	virtual void StepSimulation(Simulator *lpSim, Structure *lpStructure);
};

		}		//Bodies
	}			// Environment
}				//VortexAnimatLibrary

#endif // !defined(AFX_VSLINEAR_HILL_STRETCH_RECEPTOR_H__8438B067_2454_459B_8092_E74ABF23B265__INCLUDED_)
