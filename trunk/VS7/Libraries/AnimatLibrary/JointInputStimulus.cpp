// JointInputStimulus.cpp: implementation of the JointInputStimulus class.
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
#include "JointInputStimulus.h"
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

JointInputStimulus::JointInputStimulus()
{
}

JointInputStimulus::~JointInputStimulus()
{

try
{
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of JointInputStimulus\r\n", "", -1, FALSE, TRUE);}
}

void JointInputStimulus::Initialize(Simulator *lpSim)
{
	if(!lpSim)
		THROW_ERROR(Al_Err_lSimulationNotDefined, Al_Err_strSimulationNotDefined);

	ExternalInputStimulus::Initialize(lpSim);

	//Lets try and get the node we will dealing with.
	m_lpStructure = lpSim->FindStructureFromAll(m_strStructureID);
	m_lpNode = m_lpStructure->FindJoint(m_strJointID);
}

void JointInputStimulus::Load(Simulator *lpSim, CStdXml &oXml)
{
	ExternalInputStimulus::Load(lpSim, oXml);

	oXml.IntoElem();  //Into Simulus Element

	m_strJointID = oXml.GetChildString("JointID");
	if(Std_IsBlank(m_strJointID)) 
		THROW_ERROR(Al_Err_lJointIDBlank, Al_Err_strJointIDBlank);

	oXml.OutOfElem(); //OutOf Simulus Element

}

void JointInputStimulus::Save(Simulator *lpSim, CStdXml &oXml)
{
}

	}			//ExternalStimuli
}				//VortexAnimatLibrary




