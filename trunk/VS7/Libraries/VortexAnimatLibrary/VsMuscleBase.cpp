// VsMuscleBase.cpp: implementation of the VsMuscleBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "StdAfx.h"
#include "VsJoint.h"
#include "VsRigidBody.h"
#include "VsMuscleBase.h"
#include "VsSimulator.h"

namespace VortexAnimatLibrary
{
	namespace Environment
	{
		namespace Bodies
		{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VsMuscleBase::VsMuscleBase()
{
}

VsMuscleBase::~VsMuscleBase()
{

try
{
	m_aryLines.Clear();
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of VsMuscleBase\r\n", "", -1, FALSE, TRUE);}
}

void VsMuscleBase::DrawMuscle(Simulator *lpSim, BOOL bInitDraw)
{
	if(m_bEnabled && m_bIsVisible)
	{
		if(m_aryLines.GetSize() <= 0)
			bInitDraw = TRUE;

		if(bInitDraw)
			m_aryLines.Clear();

		VsSimulator *lpVsSim = dynamic_cast<VsSimulator *>(lpSim);
		if(!lpVsSim)
			THROW_ERROR(Vs_Err_lUnableToConvertToVsSimulator, Vs_Err_strUnableToConvertToVsSimulator);

		int iCount = m_aryAttachmentPoints.GetSize();
		CStdFPoint oPrimPos, oSecPos;
		AcmeReal vStart[3], vEnd[3];
		RGraphic *lpGraphic = NULL;
		MuscleAttachment *lpAttach1 = m_aryAttachmentPoints[0], *lpAttach2 = NULL;

		for(int iIndex=1; iIndex<iCount; iIndex++)
		{
			lpAttach2 = m_aryAttachmentPoints[iIndex];
			oPrimPos = lpAttach1->AbsolutePosition();
			oSecPos = lpAttach2->AbsolutePosition();

			vStart[0] = oPrimPos.x; vStart[1] = oPrimPos.y; vStart[2] = oPrimPos.z;
			vEnd[0] = oSecPos.x; vEnd[1] = oSecPos.y; vEnd[2] = oSecPos.z;

			if(bInitDraw)
			{
				lpGraphic = RGraphicLineCreate(lpVsSim->RenderContext(), vStart, vEnd, m_aryColor, NULL);
				m_aryLines.Add(lpGraphic);
			}
			else
			{
				lpGraphic = m_aryLines[iIndex-1];
				RGraphicLineMoveEnds(lpGraphic, vStart, vEnd);
			}

			lpAttach1 = lpAttach2;
		}
	}

}

void VsMuscleBase::CreateJoints(Simulator *lpSim, Structure *lpStructure)
{
	MuscleBase::CreateJoints(lpSim, lpStructure);
	DrawMuscle(lpSim, TRUE);
}

void VsMuscleBase::CalculateForceVector(Simulator *lpSim, MuscleAttachment *lpPrim, MuscleAttachment *lpSec, float fltTension, CStdFPoint &oPrimPos, CStdFPoint &oSecPos, CStdFPoint &oPrimForce)
{
	oPrimPos = lpPrim->AbsolutePosition();
	oSecPos = lpSec->AbsolutePosition();

	oPrimForce = oSecPos - oPrimPos;
	oPrimForce.Normalize();
	oPrimForce *= (fltTension  * lpSim->InverseMassUnits() * lpSim->InverseDistanceUnits());
}

void VsMuscleBase::StepSimulation(Simulator *lpSim, Structure *lpStructure)
{
	if(m_bEnabled)
	{
		CalculateTension(lpSim);

		//Dont bother with this unless there is actually tension developed by the muscle.
		if(m_fltTension > 1e-5)
		{
			int iCount = m_aryAttachmentPoints.GetSize();
			MuscleAttachment *lpAttach1 = m_aryAttachmentPoints[0], *lpAttach2 = NULL;
			CStdFPoint oPrimPos, oPrimPlusPos, oSecPos, oSecMinusPos;
			CStdFPoint oPrimForce, oSecForce;
			VsRigidBody *lpAttach1Parent, *lpAttach2Parent;

			//Go through each set of muscle attachments and add the tension force pointing towards the other
			//attachment point at each connector.
			for(int iIndex=1; iIndex<iCount; iIndex++)
			{
				lpAttach2 = m_aryAttachmentPoints[iIndex];

				lpAttach1Parent = dynamic_cast<VsRigidBody *>(lpAttach1->Parent());
				lpAttach2Parent = dynamic_cast<VsRigidBody *>(lpAttach2->Parent());

				CalculateForceVector(lpSim, lpAttach1, lpAttach2, m_fltTension, oPrimPos, oPrimPlusPos, oPrimForce);
				CalculateForceVector(lpSim, lpAttach2, lpAttach1, m_fltTension, oSecPos, oSecMinusPos, oSecForce);

				MdtBodyAddForceAtPosition(lpAttach1Parent->BodyID(), oPrimForce.x, oPrimForce.y, oPrimForce.z, 
																oPrimPos.x, oPrimPos.y, oPrimPos.z);
				MdtBodyAddForceAtPosition(lpAttach2Parent->BodyID(), oSecForce.x, oSecForce.y, oSecForce.z, 
																oSecPos.x, oSecPos.y, oSecPos.z);

				lpAttach1 = lpAttach2;
			}
		}
	}

		DrawMuscle(lpSim, FALSE);
}

		}		//Bodies
	}			// Environment
}				//VortexAnimatLibrary

