// VsOdorSensor.cpp: implementation of the VsOdorSensor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "StdAfx.h"
#include "VsJoint.h"
#include "VsRigidBody.h"
#include "VsOdorSensor.h"
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

VsOdorSensor::VsOdorSensor()
{
	m_lLastUpdateTime = -1;
	m_lpGraphic = NULL;
}

VsOdorSensor::~VsOdorSensor()
{

try
{
	m_lpGraphic = NULL;
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of VsOdorSensor\r\n", "", -1, FALSE, TRUE);}
}

CStdFPoint VsOdorSensor::CalculatePosition(Simulator *lpSim)
{
	UpdateAttachmentPointPosition(lpSim);
	return m_oAbsPosition;
}

void VsOdorSensor::UpdateAttachmentPointPosition(Simulator *lpSim)
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

		m_lLastUpdateTime = lpSim->TimeSlice();
	}
}

void VsOdorSensor::CreateParts(Simulator *lpSim, Structure *lpStructure)
{
	TRACE_DEBUG("Statring Parts Creation for " + m_strName);
	TRACE_DETAIL_NS("PartType: OdorSensor");

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

	TRACE_DEBUG("Ending Parts Creation for " + m_strName);
}


void VsOdorSensor::StepSimulation(Simulator *lpSim, Structure *lpStructure)
{
	UpdateAttachmentPointPosition(lpSim);

	if(m_lpOdorType)
		m_fltOdorValue = m_lpOdorType->CalculateOdorValue(lpSim, m_oAbsPosition);
}

		}		//Bodies
	}			// Environment
}				//VortexAnimatLibrary

