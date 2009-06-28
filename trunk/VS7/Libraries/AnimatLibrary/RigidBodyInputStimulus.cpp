// RigidBodyInputStimulus.cpp: implementation of the RigidBodyInputStimulus class.
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
#include "RigidBodyInputStimulus.h"
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

RigidBodyInputStimulus::RigidBodyInputStimulus()
{
}

RigidBodyInputStimulus::~RigidBodyInputStimulus()
{

try
{
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of RigidBodyInputStimulus\r\n", "", -1, FALSE, TRUE);}
}

void RigidBodyInputStimulus::Initialize(Simulator *lpSim)
{
	if(!lpSim)
		THROW_ERROR(Al_Err_lSimulationNotDefined, Al_Err_strSimulationNotDefined);

	ExternalInputStimulus::Initialize(lpSim);

	//Lets try and get the node we will dealing with.
	m_lpStructure = lpSim->FindStructureFromAll(m_strStructureID);
	m_lpNode = m_lpStructure->FindRigidBody(m_strBodyID);
}

void RigidBodyInputStimulus::Load(Simulator *lpSim, CStdXml &oXml)
{
	ExternalInputStimulus::Load(lpSim, oXml);

	oXml.IntoElem();  //Into Simulus Element

	m_strBodyID = oXml.GetChildString("BodyID");
	if(Std_IsBlank(m_strBodyID)) 
		THROW_ERROR(Al_Err_lBodyIDBlank, Al_Err_strBodyIDBlank);

	oXml.OutOfElem(); //OutOf Simulus Element

}

void RigidBodyInputStimulus::Save(Simulator *lpSim, CStdXml &oXml)
{
}

	}			//ExternalStimuli
}				//VortexAnimatLibrary




