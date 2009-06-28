// VsSpring.cpp: implementation of the VsSpring class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "VsJoint.h"
#include "VsRigidBody.h"
#include "VsSpring.h"
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

VsSpring::VsSpring()
{
	m_lpSpringID = NULL;
	m_fltT1 = 0;
	m_fltT2 = 0;
	m_fltT3 = 0;
	m_fltT4 = 0;
}

VsSpring::~VsSpring()
{

}

void VsSpring::Enabled(BOOL bVal)
{
	m_bEnabled = bVal;

	if(bVal)
		MdtSpringEnable(m_lpSpringID);
	else
		MdtSpringDisable(m_lpSpringID);
}

void VsSpring::CreateJoints(Simulator *lpSim, Structure *lpStructure)
{
	VsSimulator *lpVsSim = dynamic_cast<VsSimulator *>(lpSim);
	if(!lpVsSim)
		THROW_ERROR(Vs_Err_lUnableToConvertToVsSimulator, Vs_Err_strUnableToConvertToVsSimulator);

	m_lpPrimaryAttachment = dynamic_cast<MuscleAttachment *>(lpStructure->FindRigidBody(m_strPrimaryAttachmentID));
	m_lpSecondaryAttachment = dynamic_cast<MuscleAttachment *>(lpStructure->FindRigidBody(m_strSecondaryAttachmentID));

	if(!m_lpPrimaryAttachment)
		THROW_PARAM_ERROR(Al_Err_lPartNotAttachment, Al_Err_strPartNotAttachment, "AttachmentID", m_strPrimaryAttachmentID);

	if(!m_lpSecondaryAttachment)
		THROW_PARAM_ERROR(Al_Err_lPartNotAttachment, Al_Err_strPartNotAttachment, "AttachmentID", m_strSecondaryAttachmentID);

	VsRigidBody *lpVsPrimary = dynamic_cast<VsRigidBody *>(m_lpPrimaryAttachment->Parent());
	VsRigidBody *lpVsSecondary = dynamic_cast<VsRigidBody *>(m_lpSecondaryAttachment->Parent());

	m_lpSpringID = MdtSpringCreate(lpVsSim->World());

	MdtConstraintSetBodies(m_lpSpringID, lpVsPrimary->BodyID(), lpVsSecondary->BodyID());

	MdtSpringSetNaturalLength(m_lpSpringID, m_fltNaturalLength);
	MdtSpringSetStiffness(m_lpSpringID, m_fltStiffness);
	MdtSpringSetDamping(m_lpSpringID, m_fltDamping);

	CStdFPoint vPrimPos = m_lpPrimaryAttachment->AbsolutePosition();
	CStdFPoint vSecPos = m_lpSecondaryAttachment->AbsolutePosition();
	
	MdtConstraintBodySetPosition(m_lpSpringID, 0, vPrimPos.x, vPrimPos.y, vPrimPos.z);
	MdtConstraintBodySetPosition(m_lpSpringID, 1, vSecPos.x, vSecPos.y, vSecPos.z);

	if(m_bEnabled)
		MdtSpringEnable(m_lpSpringID);

	DrawSpring(lpSim);
}

void VsSpring::DrawSpring(Simulator *lpSim)
{
	if(m_bEnabled)
	{
		VsSimulator *lpVsSim = dynamic_cast<VsSimulator *>(lpSim);
		if(!lpVsSim)
			THROW_ERROR(Vs_Err_lUnableToConvertToVsSimulator, Vs_Err_strUnableToConvertToVsSimulator);

		MeReal lineOrigin[3], lineEnd[3];

		MdtConstraintBodyGetPosition(m_lpSpringID,  0, lineOrigin);
		MdtConstraintBodyGetPosition(m_lpSpringID, 1, lineEnd);
		m_fltLength = sqrt(pow(lineOrigin[0]-lineEnd[0], 2) + pow(lineOrigin[1]-lineEnd[1], 2) + pow(lineOrigin[2]-lineEnd[2], 2));
		m_fltDisplacement = (m_fltLength - m_fltNaturalLength) * lpSim->DistanceUnits();
		m_fltLength *= lpSim->DistanceUnits();

		m_fltT1 = V3_MAG(m_lpSpringID->head.resultForce[0]);
		m_fltT2 = V3_MAG(m_lpSpringID->head.resultForce[1]);
		m_fltT3 = V3_MAG(m_lpSpringID->head.resultForce[2]);
		m_fltT4 = V3_MAG(m_lpSpringID->head.resultForce[3]);

		if(m_fltT1>0)
			m_fltTension = m_fltT1 * lpSim->MassUnits() * lpSim->DistanceUnits();
		else if(m_fltT2>0)
			m_fltTension = m_fltT2 * lpSim->MassUnits() * lpSim->DistanceUnits();
		else if(m_fltT3>0)
			m_fltTension = m_fltT3 * lpSim->MassUnits() * lpSim->DistanceUnits();
		else if(m_fltT4>0)
			m_fltTension = m_fltT4 * lpSim->MassUnits() * lpSim->DistanceUnits();
		else
			m_fltTension = 0;

		if(m_bIsVisible)
		{
			if(!m_lpGraphic)
				m_lpGraphic = RGraphicLineCreate(lpVsSim->RenderContext(), lineOrigin, lineEnd, m_aryColor, NULL);
			else
				RGraphicLineMoveEnds(m_lpGraphic,lineOrigin, lineEnd);
		}
	}
	else
	{
		m_fltLength = 0;
		m_fltDisplacement = 0;
		m_fltTension = 0;
	}

}

void VsSpring::StepSimulation(Simulator *lpSim, Structure *lpStructure)
{
	DrawSpring(lpSim);
}

float *VsSpring::GetDataPointer(string strDataType)
{
	string strType = Std_CheckString(strDataType);
	float *lpData = NULL;

	if(strType == "SPRINGLENGTH")
		return &m_fltLength;

	if(strType == "DISPLACEMENT")
		return &m_fltDisplacement;

	if(strType == "TENSION")
		return &m_fltTension;

	lpData = Spring::GetDataPointer(strDataType);
	if(lpData) return lpData;

	lpData = VsRigidBody::GetVortexDataPointer(strDataType);
	if(lpData) return lpData;

	THROW_TEXT_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "RigidBodyID: " + STR(m_strName) + "  DataType: " + strDataType);

	return NULL;
}

		}		//Joints
	}			// Environment
}				//VortexAnimatLibrary
