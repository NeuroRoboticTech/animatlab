// VoltageClamp.cpp: implementation of the VoltageClamp class.
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
#include "VoltageClamp.h"
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

VoltageClamp::VoltageClamp()
{
	m_lpOrganism = NULL;
	m_lpNode = NULL;
	m_lpExternalCurrent = NULL;
	m_lpTotalCurrent = NULL;
	m_lpVrest = NULL;
	m_lpGm = NULL;
	m_lTargetNodeID = 0;
	m_fltVtarget = -0.70f;
	m_fltActiveCurrent = 0;
	m_fltTargetCurrent = 0;
}

VoltageClamp::~VoltageClamp()
{

try
{
	m_lpOrganism = NULL;
	m_lpNode = NULL;
	m_lpExternalCurrent = NULL;
	m_lpTotalCurrent = NULL;
	m_lpVrest = NULL;
	m_lpGm = NULL;
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of VoltageClamp\r\n", "", -1, FALSE, TRUE);}
}

void VoltageClamp::Initialize(Simulator *lpSim)
{
	if(!lpSim)
		THROW_ERROR(Al_Err_lSimulationNotDefined, Al_Err_strSimulationNotDefined);

	ExternalStimulus::Initialize(lpSim);

	//Lets try and get the node we will dealing with.
	m_lpOrganism = lpSim->FindOrganism(m_strOrganismID);
	m_lpNode = m_lpOrganism->NervousSystem()->FindNode(m_strNeuralModule, m_lTargetNodeID);

	m_lpExternalCurrent = m_lpNode->GetDataPointer("ExternalCurrent");
	m_lpTotalCurrent = m_lpNode->GetDataPointer("TotalCurrent");
	m_lpVrest = m_lpNode->GetDataPointer("Vrest");
	m_lpGm = m_lpNode->GetDataPointer("Gm");

	//Calculate the target current to keep the voltage at the target level.
	m_fltTargetCurrent = (m_fltVtarget - *m_lpVrest)*(*m_lpGm);

	if(!m_lpExternalCurrent)
		THROW_TEXT_ERROR(Al_Err_lDataPointNotFound, Al_Err_strDataPointNotFound, 
		("Stimulus: " + m_strID + " OrganismID: " + m_strOrganismID + " NeuralModule: " +  
 		 m_strNeuralModule + "Node: " + STR(m_lTargetNodeID) + " DataType: ExternalCurrent"));
}

void VoltageClamp::Modify(Simulator *lpSim, ActivatedItem *lpItem)
{
	VoltageClamp *lpStim = dynamic_cast<VoltageClamp *>(lpItem);

	//If stim is a current stimulus type and it is currently active then lets do the modify thing.
	//We will copy these key data values so that when we start this one up again after re-adding it
	//it will start back with the same behavior.
	if(lpStim &&  ( (lpStim->StartSlice() <= lpSim->TimeSlice()) && (lpStim->EndSlice() >= lpSim->TimeSlice()) ))
	{
		m_bModified = TRUE;
		m_fltVtarget = lpStim->m_fltVtarget;
		m_fltActiveCurrent = lpStim->m_fltActiveCurrent;
	}
}

void VoltageClamp::Activate(Simulator *lpSim)
{
	m_fltActiveCurrent = m_fltTargetCurrent - (*m_lpTotalCurrent);
	*m_lpExternalCurrent = *m_lpExternalCurrent + m_fltActiveCurrent;
	m_bModified = FALSE;
}

void VoltageClamp::StepSimulation(Simulator *lpSim)
{
	m_fltActiveCurrent = m_fltTargetCurrent - (*m_lpTotalCurrent);
	*m_lpExternalCurrent = *m_lpExternalCurrent + m_fltActiveCurrent;
}

void VoltageClamp::Deactivate(Simulator *lpSim)
{		
	//*m_lpExternalCurrent = *m_lpExternalCurrent - m_fltActiveCurrent;
}

float *VoltageClamp::GetDataPointer(string strDataType)
{
	float *lpData=NULL;
	string strType = Std_CheckString(strDataType);

	if(strType == "CLAMPCURRENT")
		lpData = &m_fltActiveCurrent;
	else
		THROW_TEXT_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "StimulusName: " + STR(m_strName) + "  DataType: " + strDataType);

	return lpData;
} 

void VoltageClamp::Load(Simulator *lpSim, CStdXml &oXml)
{
	ActivatedItem::Load(lpSim, oXml);

	oXml.IntoElem();  //Into Simulus Element

	m_strNeuralModule = oXml.GetChildString("ModuleName");
	if(Std_IsBlank(m_strNeuralModule)) 
		THROW_ERROR(Al_Err_lNeuralModuleNameBlank, Al_Err_strNeuralModuleNameBlank);

	m_strOrganismID = oXml.GetChildString("OrganismID");

	if(Std_IsBlank(m_strOrganismID))
		THROW_PARAM_ERROR(Al_Err_lOrganismIDBlank, Al_Err_strOrganismIDBlank, "ID", m_strID);

	m_lTargetNodeID = oXml.GetChildLong("TargetNodeID");
	m_fltVtarget = oXml.GetChildFloat("Vtarget", m_fltVtarget);

	oXml.OutOfElem(); //OutOf Simulus Element
}

void VoltageClamp::Save(Simulator *lpSim, CStdXml &oXml)
{
}

void VoltageClamp::Trace(ostream &oOs)
{
	oOs << "VoltageClamp"  << ", Name: " << m_strName << " Time (" << m_fltStartTime << ", " << m_fltEndTime << ") Slice: (" << m_lStartSlice << ", " << m_lEndSlice << ")";
}

	}			//ExternalStimuli
}				//VortexAnimatLibrary




