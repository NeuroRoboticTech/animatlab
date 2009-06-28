// MotorVelocityStimulus.cpp: implementation of the VsMotorVelocityStimulus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "VsJoint.h"
#include "VsRigidBody.h"
#include "VsSimulator.h"

#include "VsMotorVelocityStimulus.h"

namespace VortexAnimatLibrary
{
	namespace ExternalStimuli
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VsMotorVelocityStimulus::VsMotorVelocityStimulus()
{
	m_lpJoint = NULL;
	m_lpEval = NULL;
	m_fltVelocity = 0;
	m_bDisableMotorWhenDone = FALSE;
	m_lpPosition = NULL;
	m_lpVelocity = NULL;
}

VsMotorVelocityStimulus::~VsMotorVelocityStimulus()
{

try
{
	m_lpJoint = NULL;
	if(m_lpEval) delete m_lpEval;
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of VsMotorVelocityStimulus\r\n", "", -1, FALSE, TRUE);}
}


void VsMotorVelocityStimulus::Initialize(Simulator *lpSim)
{
	if(!lpSim)
		THROW_ERROR(Al_Err_lSimulationNotDefined, Al_Err_strSimulationNotDefined);

	ExternalStimulus::Initialize(lpSim);

	//Lets try and get the joint we will be injecting.
	m_lpJoint = lpSim->FindJoint(m_strStructureID, m_strJointID);
	m_lpPosition = m_lpJoint->GetDataPointer("JOINTPOSITION");
	m_lpVelocity = m_lpJoint->GetDataPointer("JOINTACTUALVELOCITY");
}

void VsMotorVelocityStimulus::Activate(Simulator *lpSim)
{
	m_lpJoint->EnableMotor(TRUE);
	m_lpJoint->DesiredVelocity(0);
}


void VsMotorVelocityStimulus::StepSimulation(Simulator *lpSim)
{
	//float fltVel=0;

	try
	{
		//IMPORTANT! This stimulus applies a stimulus to the physics engine, so it should ONLY be called once for every time the physcis
		//engine steps. If you do not do this then the you will accumulate forces being applied during the neural steps, and the total
		//for you apply will be greater than what it should be. To get around this we will only call the code in step simulation if
		//the physics step count is equal to the step interval.
		if(lpSim->PhysicsStepCount() == lpSim->PhysicsStepInterval())
		{
			m_lpEval->SetVariable("t", (lpSim->Time()-m_fltStartTime) );
			
			if(m_lpPosition)
				m_lpEval->SetVariable("p",  *m_lpPosition);
			
			if(m_lpVelocity)
				m_lpEval->SetVariable("v", *m_lpVelocity);

			m_fltVelocity = m_lpEval->Solve();
			//fltVel = -sin(6.28*(lpSim->Time()-m_fltStartTime));

			if(!m_lpJoint->UsesRadians())
				m_fltVelocity *= lpSim->InverseDistanceUnits();

			m_lpJoint->DesiredVelocity(m_fltVelocity);
		}
	}
	catch(...)
	{
		LOG_ERROR("Error Occurred while setting Joint Velocity");
	}
}


void VsMotorVelocityStimulus::Deactivate(Simulator *lpSim)
{
		m_lpJoint->DesiredVelocity(0);
		if(m_bDisableMotorWhenDone)
			m_lpJoint->EnableMotor(FALSE);
}

void VsMotorVelocityStimulus::Load(Simulator *lpSim, CStdXml &oXml)
{
	ActivatedItem::Load(lpSim, oXml);

	oXml.IntoElem();  //Into Simulus Element

	m_strStructureID = oXml.GetChildString("StructureID");
	if(Std_IsBlank(m_strStructureID)) 
		THROW_ERROR(Al_Err_lIDBlank, Al_Err_strIDBlank);

	m_strJointID = oXml.GetChildString("JointID");
	if(Std_IsBlank(m_strStructureID)) 
		THROW_ERROR(Al_Err_lIDBlank, Al_Err_strIDBlank);

	m_strVelocityEquation = oXml.GetChildString("Velocity");

	m_bDisableMotorWhenDone = oXml.GetChildBool("DisableMotorWhenDone", m_bDisableMotorWhenDone);

	oXml.OutOfElem(); //OutOf Simulus Element

	//Initialize the postfix evaluator.
	if(m_lpEval) 
	{delete m_lpEval; m_lpEval = NULL;}

	m_lpEval = new CStdPostFixEval;

	m_lpEval->AddVariable("t");
	m_lpEval->AddVariable("p");
	m_lpEval->AddVariable("v");
	m_lpEval->Equation(m_strVelocityEquation);
}

void VsMotorVelocityStimulus::Save(Simulator *lpSim, CStdXml &oXml)
{
}

void VsMotorVelocityStimulus::Trace(ostream &oOs)
{
	oOs << "MotorVelocityStimulus"  << ", Name: " << m_strName << " Time (" << m_fltStartTime << ", " << m_fltEndTime << ") Slice: (" << m_lStartSlice << ", " << m_lEndSlice << ")";
}

	}			//ExternalStimuli
}				//VortexAnimatLibrary




