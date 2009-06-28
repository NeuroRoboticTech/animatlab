// VsForceStimulus.cpp: implementation of the VsForceStimulus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "VsJoint.h"
#include "VsRigidBody.h"
#include "VsSimulator.h"

#include "VsForceStimulus.h"
 
namespace VortexAnimatLibrary
{
	namespace ExternalStimuli
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VsForceStimulus::VsForceStimulus()
{
	m_lpStructure = NULL;
	m_lpBody = NULL;
	m_lpVsBody = NULL;

	m_lpForceXEval = NULL;
	m_lpForceYEval = NULL;
	m_lpForceZEval = NULL;

	m_fltForceX = 0;
	m_fltForceY = 0;
	m_fltForceZ = 0;

	m_lpTorqueXEval = NULL;
	m_lpTorqueYEval = NULL;
	m_lpTorqueZEval = NULL;

	m_fltTorqueX = 0;
	m_fltTorqueY = 0;
	m_fltTorqueZ = 0;
}

VsForceStimulus::~VsForceStimulus()
{

try
{
	m_lpStructure = NULL;
	m_lpBody = NULL;
	m_lpVsBody = NULL;

	if(m_lpForceXEval) delete m_lpForceXEval;
	if(m_lpForceYEval) delete m_lpForceYEval;
	if(m_lpForceZEval) delete m_lpForceZEval;

	if(m_lpTorqueXEval) delete m_lpTorqueXEval;
	if(m_lpTorqueYEval) delete m_lpTorqueYEval;
	if(m_lpTorqueZEval) delete m_lpTorqueZEval;
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of VsForceStimulus\r\n", "", -1, FALSE, TRUE);}
}

void VsForceStimulus::Initialize(Simulator *lpSim)
{
	if(!lpSim)
		THROW_ERROR(Al_Err_lSimulationNotDefined, Al_Err_strSimulationNotDefined);

	ExternalStimulus::Initialize(lpSim);

	//Lets try and get the node we will dealing with.
	m_lpStructure = lpSim->FindStructureFromAll(m_strStructureID);
	m_lpBody = m_lpStructure->FindRigidBody(m_strBodyID);
	m_lpVsBody = dynamic_cast<VsRigidBody *>(m_lpBody);
}

void VsForceStimulus::Activate(Simulator *lpSim)
{}

void VsForceStimulus::StepSimulation(Simulator *lpSim)
{
	try
	{
		//IMPORTANT! This stimulus applies a force to the physics engine, so it should ONLY be called once for every time the physcis
		//engine steps. If you do not do this then the you will accumulate forces being applied during the neural steps, and the total
		//for you apply will be greater than what it should be. To get around this we will only call the code in step simulation if
		//the physics step count is equal to the step interval.
		if(lpSim->PhysicsStepCount() == lpSim->PhysicsStepInterval())
		{
			//Why do we multiply by the mass units here? The reason is that we have to try and keep the 
			//length and mass values in a range around 1 for the simulator to be able to function appropriately.
			//So say we are uing grams and centimeters. This means that if we have a 1cm^3 box that weights 1 gram
			//it will come in with a density of 1 g.cm^3 and we will set its density to 1. But the simulator treats this
			//as 1 Kg and not 1g. So forces/torques and so on are scaled incorrectly. We must scale the force to be applied
			//so it is acting against kilograms instead of grams. So a 1N force would be 1000N to produce the same effect.
			if(m_lpForceXEval || m_lpForceYEval || m_lpForceZEval)
			{
				m_fltForceX = m_fltForceY = m_fltForceZ = 0;

				if(m_lpForceXEval)
				{
					m_lpForceXEval->SetVariable("t", lpSim->Time());
					m_fltForceX = m_lpForceXEval->Solve() * lpSim->InverseMassUnits() * lpSim->InverseDistanceUnits();
				}

				if(m_lpForceYEval)
				{
					m_lpForceYEval->SetVariable("t", lpSim->Time());
					m_fltForceY = m_lpForceYEval->Solve() * lpSim->InverseMassUnits() * lpSim->InverseDistanceUnits();
				}

				if(m_lpForceZEval)
				{
					m_lpForceZEval->SetVariable("t", lpSim->Time());
					m_fltForceZ = m_lpForceZEval->Solve() * lpSim->InverseMassUnits() * lpSim->InverseDistanceUnits();
				}

				m_oForcePosition = m_lpVsBody->BodyToWorldCoords(m_oRelativePosition);

				if(m_fltForceX || m_fltForceY || m_fltForceZ)
					MdtBodyAddForceAtPosition(m_lpVsBody->BodyID(), m_fltForceX, m_fltForceY, m_fltForceZ, 
																		m_oForcePosition.x, m_oForcePosition.y, m_oForcePosition.z);
			}

			if(m_lpTorqueXEval || m_lpTorqueYEval || m_lpTorqueZEval)
			{
				m_fltTorqueX = m_fltTorqueY = m_fltTorqueZ = 0;

				if(m_lpTorqueXEval)
				{
					m_lpTorqueXEval->SetVariable("t", lpSim->Time());
					m_fltTorqueX = m_lpTorqueXEval->Solve() * lpSim->InverseMassUnits() * lpSim->InverseDistanceUnits() * lpSim->InverseDistanceUnits();
				}

				if(m_lpTorqueYEval)
				{
					m_lpTorqueYEval->SetVariable("t", lpSim->Time());
					m_fltTorqueY = m_lpTorqueYEval->Solve() * lpSim->InverseMassUnits() * lpSim->InverseDistanceUnits() * lpSim->InverseDistanceUnits();
				}

				if(m_lpTorqueZEval)
				{
					m_lpTorqueZEval->SetVariable("t", lpSim->Time());
					m_fltTorqueZ = m_lpTorqueZEval->Solve() * lpSim->InverseMassUnits() * lpSim->InverseDistanceUnits() * lpSim->InverseDistanceUnits();
				}

				if(m_fltTorqueX || m_fltTorqueY || m_fltTorqueZ)
					MdtBodyAddTorque(m_lpVsBody->BodyID(), m_fltTorqueX, m_fltTorqueY, m_fltTorqueZ);
			}
		}
	}
	catch(...)
	{
		LOG_ERROR("Error Occurred while setting Joint Velocity");
	}
}

void VsForceStimulus::Deactivate(Simulator *lpSim)
{}

void VsForceStimulus::Load(Simulator *lpSim, CStdXml &oXml)
{
	ActivatedItem::Load(lpSim, oXml);

	oXml.IntoElem();  //Into Simulus Element

	m_strStructureID = oXml.GetChildString("StructureID");
	if(Std_IsBlank(m_strStructureID)) 
		THROW_ERROR(Al_Err_lIDBlank, Al_Err_strIDBlank);

	m_strBodyID = oXml.GetChildString("BodyID");
	if(Std_IsBlank(m_strBodyID)) 
		THROW_ERROR(Al_Err_lBodyIDBlank, Al_Err_strBodyIDBlank);

	m_strForceXEquation = oXml.GetChildString("ForceX", "");
	m_strForceYEquation = oXml.GetChildString("ForceY", "");
	m_strForceZEquation = oXml.GetChildString("ForceZ", "");

	m_strTorqueXEquation = oXml.GetChildString("TorqueX", "");
	m_strTorqueYEquation = oXml.GetChildString("TorqueY", "");
	m_strTorqueZEquation = oXml.GetChildString("TorqueZ", "");

	Std_LoadPoint(oXml, "RelativePosition", m_oRelativePosition);

	//We need to scale the distance values to be appropriate. They 
	//will be saved as centimeters or some such in the config file, 
	//but we need them to be in "unit" values.
	m_oRelativePosition *= lpSim->InverseDistanceUnits();

	oXml.OutOfElem(); //OutOf Simulus Element

	//Initialize the postfix evaluator.
	if(m_lpForceXEval) 
	{delete m_lpForceXEval; m_lpForceXEval = NULL;}

	if(m_lpForceYEval) 
	{delete m_lpForceYEval; m_lpForceYEval = NULL;}

	if(m_lpForceZEval) 
	{delete m_lpForceZEval; m_lpForceZEval = NULL;}

	if(m_lpTorqueXEval) 
	{delete m_lpTorqueXEval; m_lpTorqueXEval = NULL;}

	if(m_lpTorqueYEval) 
	{delete m_lpTorqueYEval; m_lpTorqueYEval = NULL;}

	if(m_lpTorqueZEval) 
	{delete m_lpTorqueZEval; m_lpTorqueZEval = NULL;}

	if(!Std_IsBlank(m_strForceXEquation))
	{
		m_lpForceXEval = new CStdPostFixEval;
		m_lpForceXEval->AddVariable("t");
		m_lpForceXEval->Equation(m_strForceXEquation);
	}

	if(!Std_IsBlank(m_strForceYEquation))
	{
		m_lpForceYEval = new CStdPostFixEval;
		m_lpForceYEval->AddVariable("t");
		m_lpForceYEval->Equation(m_strForceYEquation);
	}

	if(!Std_IsBlank(m_strForceZEquation))
	{
		m_lpForceZEval = new CStdPostFixEval;
		m_lpForceZEval->AddVariable("t");
		m_lpForceZEval->Equation(m_strForceZEquation);
	}

	if(!Std_IsBlank(m_strTorqueXEquation))
	{
		m_lpTorqueXEval = new CStdPostFixEval;
		m_lpTorqueXEval->AddVariable("t");
		m_lpTorqueXEval->Equation(m_strTorqueXEquation);
	}

	if(!Std_IsBlank(m_strTorqueYEquation))
	{
		m_lpTorqueYEval = new CStdPostFixEval;
		m_lpTorqueYEval->AddVariable("t");
		m_lpTorqueYEval->Equation(m_strTorqueYEquation);
	}

	if(!Std_IsBlank(m_strTorqueZEquation))
	{
		m_lpTorqueZEval = new CStdPostFixEval;
		m_lpTorqueZEval->AddVariable("t");
		m_lpTorqueZEval->Equation(m_strTorqueZEquation);
	}

}

void VsForceStimulus::Save(Simulator *lpSim, CStdXml &oXml)
{
}

	}			//ExternalStimuli
}				//VortexAnimatLibrary




