// NodeInputStimulus.cpp: implementation of the NodeInputStimulus class.
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
#include "NodeInputStimulus.h"
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

NodeInputStimulus::NodeInputStimulus()
{
}

NodeInputStimulus::~NodeInputStimulus()
{

try
{
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of NodeInputStimulus\r\n", "", -1, FALSE, TRUE);}
}

void NodeInputStimulus::Initialize(Simulator *lpSim)
{
	if(!lpSim)
		THROW_ERROR(Al_Err_lSimulationNotDefined, Al_Err_strSimulationNotDefined);

	ExternalInputStimulus::Initialize(lpSim);

	//Lets try and get the node we will dealing with.
	m_lpStructure = lpSim->FindOrganism(m_strStructureID);
	Organism *lpOrganism = dynamic_cast<Organism *>(m_lpStructure);
	m_lpNode = lpOrganism->NervousSystem()->FindNode(m_strNeuralModule, m_lTargetNodeID);
}

void NodeInputStimulus::Load(Simulator *lpSim, CStdXml &oXml)
{
	ExternalInputStimulus::Load(lpSim, oXml);

	oXml.IntoElem();  //Into Simulus Element

	m_strNeuralModule = oXml.GetChildString("NeuralModule");
	if(Std_IsBlank(m_strNeuralModule)) 
		THROW_ERROR(Al_Err_lNeuralModuleNameBlank, Al_Err_strNeuralModuleNameBlank);

	m_lTargetNodeID = oXml.GetChildLong("TargetNodeID");

	oXml.OutOfElem(); //OutOf Simulus Element

}

void NodeInputStimulus::Save(Simulator *lpSim, CStdXml &oXml)
{
}

	}			//ExternalStimuli
}				//VortexAnimatLibrary




