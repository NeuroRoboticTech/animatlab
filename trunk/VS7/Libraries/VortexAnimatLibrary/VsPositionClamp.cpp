// VsPositionClamp.cpp: implementation of the VsPositionClamp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "VsJoint.h"
#include "VsRigidBody.h"
#include "VsSimulator.h"

#include "VsPositionClamp.h"

namespace VortexAnimatLibrary
{
	namespace ExternalStimuli
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VsPositionClamp::VsPositionClamp()
{
	m_lpStructure = NULL;
	m_lpJoint = NULL;
	m_lpVsJoint = NULL;

	m_lpPrimaryAttachment = NULL;
	m_lpSecondaryAttachment = NULL;
	m_lpAttachParent = NULL;

	m_bUsePosition = TRUE;
	m_fltPGain = 1;
	m_fltDGain = 0.1f;
	m_fltIGain = 0;
	m_fltMaxIValue = 0.1f;
	m_fltTarget = 0;
	m_fltValue = 0;
	m_fltVelocity = 0;
    m_fltPrevVelocity = 0;

	m_fltPError = 0;
	m_fltDError = 0;
	m_fltIError = 0;

	m_fltMaxForce = 100;
}

VsPositionClamp::~VsPositionClamp()
{

try
{
	m_lpStructure = NULL;
	m_lpJoint = NULL;
	m_lpVsJoint = NULL;
	m_lpPrimaryAttachment = NULL;
	m_lpSecondaryAttachment = NULL;
	m_lpAttachParent = NULL;
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of VsPositionClamp\r\n", "", -1, FALSE, TRUE);}
}

void VsPositionClamp::Initialize(Simulator *lpSim)
{
	if(!lpSim)
		THROW_ERROR(Al_Err_lSimulationNotDefined, Al_Err_strSimulationNotDefined);

	ExternalStimulus::Initialize(lpSim);

	//Lets try and get the node we will dealing with.
	m_lpStructure = lpSim->FindStructureFromAll(m_strStructureID);
	m_lpJoint = m_lpStructure->FindJoint(m_strBodyID);
	m_lpVsJoint = dynamic_cast<VsJoint *>(m_lpJoint);

	if(!Std_IsBlank(m_strPrimaryAttachmentID))
	{
		m_lpPrimaryAttachment = dynamic_cast<MuscleAttachment *>(m_lpStructure->FindRigidBody(m_strPrimaryAttachmentID));

		if(!m_lpPrimaryAttachment)
			THROW_PARAM_ERROR(Al_Err_lPartNotAttachment, Al_Err_strPartNotAttachment, "AttachmentID", m_strPrimaryAttachmentID);

		m_lpAttachParent = dynamic_cast<VsRigidBody *>(m_lpPrimaryAttachment->Parent());
	}

	if(!Std_IsBlank(m_strSecondaryAttachmentID))
	{
		m_lpSecondaryAttachment = dynamic_cast<MuscleAttachment *>(m_lpStructure->FindRigidBody(m_strSecondaryAttachmentID));

		if(!m_lpSecondaryAttachment)
			THROW_PARAM_ERROR(Al_Err_lPartNotAttachment, Al_Err_strPartNotAttachment, "AttachmentID", m_strSecondaryAttachmentID);
	}

	//If this joint does not use radians then we need to convert its position
	//into the correct scale value
	if(!m_lpJoint->UsesRadians())
		m_fltTarget *= lpSim->InverseDistanceUnits();

}

void VsPositionClamp::Activate(Simulator *lpSim)
{}

void VsPositionClamp::CalculateForceVector(Simulator *lpSim, CStdFPoint &oPrimPos, CStdFPoint &oSecPos, float fltTension, CStdFPoint &oForce)
{
		oForce = oSecPos - oPrimPos;
		oForce.Normalize();
		oForce *= (fltTension  * lpSim->InverseMassUnits() * lpSim->InverseDistanceUnits());
}

void VsPositionClamp::StepSimulation(Simulator *lpSim)
{
	try
	{
		//IMPORTANT! This stimulus applies a force to the physics engine, so it should ONLY be called once for every time the physcis
		//engine steps. If you do not do this then the you will accumulate forces being applied during the neural steps, and the total
		//for you apply will be greater than what it should be. To get around this we will only call the code in step simulation if
		//the physics step count is equal to the step interval.
		if(lpSim->PhysicsStepCount() == lpSim->PhysicsStepInterval())
		{
			if(m_bUsePosition)
			{
				m_fltValue = m_lpVsJoint->JointPosition();
				m_fltVelocity = m_lpVsJoint->JointVelocity();
			}
			else
			{
				m_fltValue = m_lpVsJoint->JointVelocity();
				//Velocity in this case is really acceleration
				m_fltVelocity = (m_fltValue - m_fltPrevVelocity)/lpSim->PhysicsTimeStep();
				m_fltPrevVelocity = m_fltValue;
			}

			m_fltPError = m_fltPGain*(m_fltTarget - m_fltValue);
			m_fltDError = -m_fltDGain*m_fltVelocity;
			m_fltIError += (m_fltIGain *m_fltPError);

			if(m_fltIError > m_fltMaxIValue)
				m_fltIError = m_fltMaxIValue;
			if(m_fltIError < -m_fltMaxIValue)
				m_fltIError = -m_fltMaxIValue;

			m_fltForce = m_fltPError + m_fltIError + m_fltDError;
	 
			if(m_fltForce)
			{
				m_oPrimPos = m_lpPrimaryAttachment->AbsolutePosition();
				m_oSecPos =  m_lpSecondaryAttachment->AbsolutePosition();

				if(m_fltForce > m_fltMaxForce)
					m_fltForce = m_fltMaxForce;

				if(m_fltForce < -m_fltMaxForce)
					m_fltForce = -m_fltMaxForce;

				if(m_fltForce > 0)
					CalculateForceVector(lpSim, m_oPrimPos, m_oSecPos, m_fltForce, m_oForce);
				else
					CalculateForceVector(lpSim, m_oSecPos, m_oPrimPos, -m_fltForce, m_oForce);

				MdtBodyAddForceAtPosition(m_lpAttachParent->BodyID(), m_oForce.x, m_oForce.y, m_oForce.z, m_oPrimPos.x, m_oPrimPos.y, m_oPrimPos.z);
			}
		}
	}
	catch(...)
	{
		LOG_ERROR("Error Occurred while setting Joint Velocity");
	}
}

float *VsPositionClamp::GetDataPointer(string strDataType)
{
	float *lpData=NULL;
	string strType = Std_CheckString(strDataType);

	if(strType == "FORCE")
		lpData = &m_fltForce;
	else if(strType == "PERROR")
		lpData = &m_fltPError;
	else if(strType == "DERROR")
		lpData = &m_fltDError;
	else if(strType == "IERROR")
		lpData = &m_fltIError;
	else if(strType == "POSITION")
		lpData = &m_fltValue;
	else if(strType == "VELOCITY")
		lpData = &m_fltVelocity;
	else
		THROW_TEXT_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "StimulusName: " + STR(m_strName) + "  DataType: " + strDataType);

	return lpData;
}

void VsPositionClamp::Deactivate(Simulator *lpSim)
{}

void VsPositionClamp::Load(Simulator *lpSim, CStdXml &oXml)
{
	ActivatedItem::Load(lpSim, oXml);

	oXml.IntoElem();  //Into Simulus Element

	m_strStructureID = oXml.GetChildString("StructureID");
	if(Std_IsBlank(m_strStructureID)) 
		THROW_ERROR(Al_Err_lIDBlank, Al_Err_strIDBlank);

	m_strBodyID = oXml.GetChildString("BodyID");
	if(Std_IsBlank(m_strBodyID)) 
		THROW_ERROR(Al_Err_lBodyIDBlank, Al_Err_strBodyIDBlank);

	m_strPrimaryAttachmentID = oXml.GetChildString("PrimaryAttachID", "");
	m_strSecondaryAttachmentID = oXml.GetChildString("SecondaryAttachID", "");

	if(Std_IsBlank(m_strPrimaryAttachmentID))
		THROW_PARAM_ERROR(Al_Err_lPartNotAttachment, Al_Err_strPartNotAttachment, "AttachmentID", m_strPrimaryAttachmentID);

	if(Std_IsBlank(m_strSecondaryAttachmentID))
		THROW_PARAM_ERROR(Al_Err_lPartNotAttachment, Al_Err_strPartNotAttachment, "AttachmentID", m_strSecondaryAttachmentID);

	m_fltPGain = oXml.GetChildFloat("PGain", m_fltPGain);
	m_fltDGain = oXml.GetChildFloat("DGain", m_fltDGain);
	m_fltIGain = oXml.GetChildFloat("IGain", m_fltIGain);
	m_fltMaxIValue = oXml.GetChildFloat("MaxIVal", m_fltMaxIValue);
	m_bUsePosition = oXml.GetChildBool("UsePosition", TRUE);

	if(m_bUsePosition)
		m_fltTarget = oXml.GetChildFloat("Position", m_fltTarget);
	else
		m_fltTarget = oXml.GetChildFloat("Velocity", m_fltTarget);

	m_fltMaxForce = AnimatLibrary::LoadScaledNumber(oXml, "MaxForce", FALSE, m_fltMaxForce);

	Std_IsAboveMin((float) 0, m_fltMaxForce, TRUE, "MaxForce", TRUE);

	oXml.OutOfElem(); //OutOf Simulus Element
}

void VsPositionClamp::Save(Simulator *lpSim, CStdXml &oXml)
{
}

	}			//ExternalStimuli
}				//VortexAnimatLibrary




