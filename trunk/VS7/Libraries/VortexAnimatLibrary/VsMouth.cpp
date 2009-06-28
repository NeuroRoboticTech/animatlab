// VsMouth.cpp: implementation of the VsMouth class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "StdAfx.h"
#include "VsJoint.h"
#include "VsRigidBody.h"
#include "VsMouth.h"
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

VsMouth::VsMouth()
{
	m_lLastUpdateTime = -1;
	m_lpGraphic = NULL;
}

VsMouth::~VsMouth()
{

try
{
	m_lpGraphic = NULL;
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of VsMouth\r\n", "", -1, FALSE, TRUE);}
}

CStdFPoint VsMouth::CalculatePosition(Simulator *lpSim)
{
	UpdateAttachmentPointPosition(lpSim);
	return m_oAbsPosition;
}

void VsMouth::UpdateAttachmentPointPosition(Simulator *lpSim)
{
	//If the last time we attempted to update was a previous time slice then
	//lets update again. But if it was in this time slice then we already have 
	//the correct position and there is no need to recalculate it.
	if(m_lLastUpdateTime < lpSim->TimeSlice() )
	{
		VsRigidBody *lpParent = dynamic_cast<VsRigidBody *>(this->Parent());

		MeMatrix4 aryParentTM;
		MeVector3 aryFinalPos, aryTranslate;
		CStdFPoint oPos = this->RelativePosition();
		aryTranslate[0] = oPos.x;
		aryTranslate[1] = oPos.y;
		aryTranslate[2] = oPos.z;
		//aryTranslate[3] = 1.0;

		MdtBodyGetTransform(lpParent->BodyID(), aryParentTM);
		//MeMatrix4MultiplyVector(aryFinalPos, aryParentTM, aryTranslate);
		MeMatrix4TMTransformVector3(aryFinalPos, aryParentTM, aryTranslate);

		m_oAbsPosition.x = aryFinalPos[0];
		m_oAbsPosition.y = aryFinalPos[1];
		m_oAbsPosition.z = aryFinalPos[2];

		m_aryBodyTM[3][0] = m_oAbsPosition.x;
		m_aryBodyTM[3][1] = m_oAbsPosition.y;
		m_aryBodyTM[3][2] = m_oAbsPosition.z;

		m_vPosition[0] = m_aryBodyTM[3][0] * lpSim->DistanceUnits();
		m_vPosition[1] = m_aryBodyTM[3][1] * lpSim->DistanceUnits();
		m_vPosition[2] = m_aryBodyTM[3][2] * lpSim->DistanceUnits();

		m_lLastUpdateTime = lpSim->TimeSlice();
	}
}

void VsMouth::CreateParts(Simulator *lpSim, Structure *lpStructure)
{
	TRACE_DEBUG("Statring Parts Creation for " + m_strName);
	TRACE_DETAIL_NS("PartType: Mouth");

	UpdateAttachmentPointPosition(lpSim);

	if(m_bIsVisible)
	{
		VsSimulator *lpVsSim = dynamic_cast<VsSimulator *>(lpSim);
		if(!lpVsSim)
			THROW_ERROR(Vs_Err_lUnableToConvertToVsSimulator, Vs_Err_strUnableToConvertToVsSimulator);

		MeMatrix4 aryRotationTM;
		MeMatrix3 aryRotation;

		MeMatrix4TMSetRotationFromEulerAngles(aryRotationTM, m_oRotation.x, m_oRotation.y, m_oRotation.z);
		MeMatrix4TMGetRotation(aryRotation, aryRotationTM) ;
		MeMatrix4TMMakeFromRotationAndPosition(m_aryBodyTM, aryRotation, m_oAbsPosition.x, m_oAbsPosition.y, m_oAbsPosition.z);

		m_lpGraphic = RGraphicSphereCreate(lpVsSim->RenderContext(), m_fltRadius, m_aryColor, m_aryBodyTM);
	}

	//We need to try and find a link to the stomach.
	m_lpStomach = dynamic_cast<Stomach *>(lpStructure->FindRigidBody("STOMACH", FALSE));

	TRACE_DEBUG("Ending Parts Creation for " + m_strName);
}

float *VsMouth::GetDataPointer(string strDataType)
{
	string strType = Std_CheckString(strDataType);
	float *lpData = NULL;

	if(strType == "BODYPOSITIONX")
		return (&m_vPosition[0]);

	if(strType == "BODYPOSITIONY")
		return (&m_vPosition[1]);

	if(strType == "BODYPOSITIONZ")
		return (&m_vPosition[2]);

	lpData = Mouth::GetDataPointer(strDataType);
	if(lpData)
		return lpData;

	THROW_TEXT_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "RigidBodyID: " + STR(m_strName) + "  DataType: " + strDataType);

	return NULL;
}

void VsMouth::StepSimulation(Simulator *lpSim, Structure *lpStructure)
{
	UpdateAttachmentPointPosition(lpSim);
	Mouth::StepSimulation(lpSim, lpStructure);
}

		}		//Bodies
	}			// Environment
}				//VortexAnimatLibrary

