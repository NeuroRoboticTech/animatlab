// ExternalInputStimulus.cpp: implementation of the ExternalInputStimulus class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Node.h"
#include "Gain.h"
#include "Adapter.h"
#include "Joint.h"
#include "ReceptiveField.h"
#include "ContactSensor.h"
#include "RigidBody.h"
#include "IODataEntry.h"
#include "Microcontroller.h"
#include "Structure.h"
#include "NeuralModule.h"
#include "NervousSystem.h"
#include "Organism.h"
#include "ActivatedItem.h"
#include "ActivatedItemMgr.h"
#include "DataChartMgr.h"
#include "ExternalStimuliMgr.h"
#include "ExternalStimulus.h"
#include "ExternalInputStimulus.h"
#include "KeyFrame.h"
#include "SimulationRecorder.h"
#include "OdorType.h"
#include "Odor.h"
#include "Simulator.h"

namespace AnimatLibrary
{
	namespace ExternalStimuli
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ExternalInputStimulus::ExternalInputStimulus()
{
	m_lpStructure = NULL;
	m_lpNode = NULL;
	m_lpEval = NULL;
	m_fltInput = 0;
}

ExternalInputStimulus::~ExternalInputStimulus()
{

try
{
	m_lpStructure = NULL;
	m_lpNode = NULL;
	if(m_lpEval) delete m_lpEval;
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of ExternalInputStimulus\r\n", "", -1, FALSE, TRUE);}
}


void ExternalInputStimulus::Activate(Simulator *lpSim)
{}

void ExternalInputStimulus::StepSimulation(Simulator *lpSim)
{
	try
	{
		m_lpEval->SetVariable("t", lpSim->Time());
		m_fltInput = m_lpEval->Solve();
		m_lpNode->AddExternalNodeInput(lpSim, m_lpStructure, m_fltInput);
	}
	catch(...)
	{
		LOG_ERROR("Error Occurred while setting Joint Velocity");
	}
}

void ExternalInputStimulus::Deactivate(Simulator *lpSim)
{}

void ExternalInputStimulus::Load(Simulator *lpSim, CStdXml &oXml)
{
	ActivatedItem::Load(lpSim, oXml);

	oXml.IntoElem();  //Into Simulus Element

	m_strStructureID = oXml.GetChildString("StructureID");
	if(Std_IsBlank(m_strStructureID)) 
		THROW_ERROR(Al_Err_lIDBlank, Al_Err_strIDBlank);

	m_strInputEquation = oXml.GetChildString("Input", "0");

	oXml.OutOfElem(); //OutOf Simulus Element

	//Initialize the postfix evaluator.
	if(m_lpEval) 
	{delete m_lpEval; m_lpEval = NULL;}

	m_lpEval = new CStdPostFixEval;

	m_lpEval->AddVariable("t");
	m_lpEval->AddVariable("x");
	m_lpEval->Equation(m_strInputEquation);
}

void ExternalInputStimulus::Save(Simulator *lpSim, CStdXml &oXml)
{
}

	}			//ExternalStimuli
}				//VortexAnimatLibrary




