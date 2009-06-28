// VsSensor.cpp: implementation of the VsSensor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "StdAfx.h"
#include "VsJoint.h"
#include "VsRigidBody.h"
#include "VsSensor.h"
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

VsSensor::VsSensor()
{
	m_aryColor[0] = 1;
	m_aryColor[1] = 0;
	m_aryColor[2] = 0;
	m_aryColor[3] = 1;
	m_fltDensity = 0;
	m_lpJointToParent = NULL;
	m_fltRadius = 1;
	m_lLastUpdateTime = -1;
	m_lpGraphic = NULL;
	m_bUsesJoint = FALSE;
}

VsSensor::~VsSensor()
{

try
{
	m_lpGraphic = NULL;
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of VsSensor\r\n", "", -1, FALSE, TRUE);}
}

CStdFPoint VsSensor::CalculatePosition(Simulator *lpSim)
{
	UpdateAttachmentPointPosition(lpSim);
	return m_oAbsPosition;
}

void VsSensor::UpdateAttachmentPointPosition(Simulator *lpSim)
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

void VsSensor::CreateParts(Simulator *lpSim, Structure *lpStructure)
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

void VsSensor::CreateJoints(Simulator *lpSim, Structure *lpStructure)
{}

float *VsSensor::GetDataPointer(string strDataType)
{
	float *lpData=NULL;
	string strType = Std_CheckString(strDataType);

	if(strType == "BODYPOSITIONX")
		lpData = &m_oAbsPosition.x;
	else if(strType == "BODYPOSITIONY")
		lpData = &m_oAbsPosition.y;
	else if(strType == "BODYPOSITIONZ")
		lpData = &m_oAbsPosition.z;
	else
		THROW_TEXT_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "RigidBodyID: " + STR(m_strName) + "  DataType: " + strDataType);

	return lpData;
}

void VsSensor::StepSimulation(Simulator *lpSim, Structure *lpStructure)
{
	UpdateAttachmentPointPosition(lpSim);
}

void VsSensor::Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{ 
	if(!m_lpParent)
		THROW_ERROR(Al_Err_lParentNotDefined, Al_Err_strParentNotDefined);

	oXml.IntoElem();  //Into RigidBody Element

	m_strType = oXml.GetChildString("Type");
	m_strID = Std_CheckString(oXml.GetChildString("ID"));
	m_strName = oXml.GetChildString("Name", "");

	if(Std_IsBlank(m_strID)) 
		THROW_ERROR(Al_Err_lIDBlank, Al_Err_strIDBlank);

	if(Std_IsBlank(m_strName)) 
		m_strName = m_strID;

	Std_LoadPoint(oXml, "RelativePosition", m_oRelPosition);
	m_oAbsPosition = m_lpParent->AbsolutePosition() + m_oRelPosition;

	m_fltRadius = oXml.GetChildFloat("Radius", m_fltRadius);
	Std_IsAboveMin((float) 0,m_fltRadius, TRUE, "Radius");

	m_bIsVisible = oXml.GetChildBool("IsVisible", m_bIsVisible);

	if(oXml.FindChildElement("Color", FALSE))
	{
		oXml.IntoChildElement("Color");
		m_aryColor[0] = oXml.GetAttribFloat("Red");
		m_aryColor[1] = oXml.GetAttribFloat("Green");
		m_aryColor[2] = oXml.GetAttribFloat("Blue");
		m_aryColor[3] = oXml.GetAttribFloat("Alpha");
		oXml.OutOfElem();
	}
	else
	{
		m_aryColor[0] = 1;
		m_aryColor[1] = 0;
		m_aryColor[2] = 0;
		m_aryColor[3] = 1;
	}

	//Muscle attachments have no parents and no joints.

	oXml.OutOfElem(); //OutOf RigidBody Element
}
		}		//Bodies
	}			// Environment
}				//VortexAnimatLibrary

