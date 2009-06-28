// CurrentStimulus.cpp: implementation of the CurrentStimulus class.
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
#include "CurrentStimulus.h"
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

CurrentStimulus::CurrentStimulus()
{
	m_lpOrganism = NULL;
	m_lpNode = NULL;
	m_lpExternalCurrent = NULL;
	m_lTargetNodeID = 0;
	m_fltCycleOnDuration = 0;
	m_fltCycleOffDuration = 0;
	m_fltBurstOnDuration = 0;
	m_fltBurstOffDuration = 0;
	m_lCycleOnDuration = 0;
	m_lCycleOffDuration = 0;
	m_lBurstOnDuration = 0;
	m_lBurstOffDuration = 0;
	m_fltCurrentOn = (float) 10e-9;
	m_fltCurrentOff = 0;
	m_fltCurrentBurstOff = 0;
	m_fltActiveCurrent = m_fltCurrentOn;
	m_lCycleStart = 0;
	m_lBurstStart = 0;
	m_bCycleOn = TRUE;
	m_bBurstOn = TRUE;
	m_lpCurrentOnEval = NULL;
	m_iType = AL_TONIC_CURRENT;
}

CurrentStimulus::~CurrentStimulus()
{

try
{
	m_lpOrganism = NULL;
	m_lpNode = NULL;
	m_lpExternalCurrent = NULL;
	if(m_lpCurrentOnEval) 
		delete m_lpCurrentOnEval;

}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of CurrentStimulus\r\n", "", -1, FALSE, TRUE);}
}

void CurrentStimulus::Initialize(Simulator *lpSim)
{
	if(!lpSim)
		THROW_ERROR(Al_Err_lSimulationNotDefined, Al_Err_strSimulationNotDefined);

	ExternalStimulus::Initialize(lpSim);

	m_lCycleOnDuration = (long) (m_fltCycleOnDuration / lpSim->TimeStep() + 0.5);
	m_lCycleOffDuration = (long) (m_fltCycleOffDuration / lpSim->TimeStep() + 0.5);
	m_lBurstOnDuration = (long) (m_fltBurstOnDuration / lpSim->TimeStep() + 0.5);
	m_lBurstOffDuration = (long) (m_fltBurstOffDuration / lpSim->TimeStep() + 0.5);

	//Lets try and get the node we will dealing with.
	m_lpOrganism = lpSim->FindOrganism(m_strOrganismID);
	m_lpNode = m_lpOrganism->NervousSystem()->FindNode(m_strNeuralModule, m_lTargetNodeID);

	m_lpExternalCurrent = m_lpNode->GetDataPointer("ExternalCurrent");

	if(!m_lpExternalCurrent)
		THROW_TEXT_ERROR(Al_Err_lDataPointNotFound, Al_Err_strDataPointNotFound, 
		("Stimulus: " + m_strID + " OrganismID: " + m_strOrganismID + " NeuralModule: " +  
 		 m_strNeuralModule + "Node: " + STR(m_lTargetNodeID) + " DataType: ExternalCurrent"));
}

void CurrentStimulus::Modify(Simulator *lpSim, ActivatedItem *lpItem)
{
	CurrentStimulus *lpStim = dynamic_cast<CurrentStimulus *>(lpItem);

	//If stim is a current stimulus type and it is currently active then lets do the modify thing.
	//We will copy these key data values so that when we start this one up again after re-adding it
	//it will start back with the same behavior.
	if(lpStim &&  ( (lpStim->StartSlice() <= lpSim->TimeSlice()) && (lpStim->EndSlice() >= lpSim->TimeSlice()) ))
	{
		m_bModified = TRUE;
		m_lCycleStart = lpStim->m_lCycleStart;
		m_lBurstStart = lpStim->m_lBurstStart;
		m_bCycleOn = lpStim->m_bCycleOn;
		m_bBurstOn = lpStim->m_bBurstOn;
		m_fltActiveCurrent = lpStim->m_fltActiveCurrent;
	}
}

float CurrentStimulus::GetCurrentOn(Simulator *lpSim)
{
	if(m_lpCurrentOnEval)
	{
		m_lpCurrentOnEval->SetVariable("t", (lpSim->Time()-m_fltStartTime) );
		return 1e-9*m_lpCurrentOnEval->Solve();
	}
	else
		return m_fltCurrentOn;

}

void CurrentStimulus::Activate(Simulator *lpSim)
{
	//if(this->m_strName == "L Flex Command")
	//	m_bModified = m_bModified;

	//Start a cycle and a burst.
	if(!m_bModified)
	{
		m_lCycleStart = lpSim->TimeSlice();
		m_lBurstStart = m_lCycleStart;
	}

	*m_lpExternalCurrent = *m_lpExternalCurrent + m_fltActiveCurrent;

	m_bModified = FALSE;
}

void CurrentStimulus::StepSimulation(Simulator *lpSim)
{
	long lCycleDiff = lpSim->TimeSlice() - m_lCycleStart;
	long lBurstDiff = lpSim->TimeSlice() - m_lBurstStart;

	if(m_bBurstOn)
	{
		if( (m_bCycleOn && (lCycleDiff >= m_lCycleOnDuration)) )
		{
			m_bCycleOn = FALSE;
			m_lCycleStart = lpSim->TimeSlice();
			
			*m_lpExternalCurrent = *m_lpExternalCurrent - m_fltActiveCurrent;
			m_fltActiveCurrent = m_fltCurrentOff;
			*m_lpExternalCurrent = *m_lpExternalCurrent + m_fltActiveCurrent;
		}
		else if( (!m_bCycleOn && (lCycleDiff >= m_lCycleOffDuration)) )
		{
			m_bCycleOn = TRUE;
			m_lCycleStart = lpSim->TimeSlice();

			*m_lpExternalCurrent = *m_lpExternalCurrent - m_fltActiveCurrent;
			m_fltActiveCurrent = GetCurrentOn(lpSim);
			*m_lpExternalCurrent = *m_lpExternalCurrent + m_fltActiveCurrent;
		}
	}

	if( (m_bBurstOn && (lBurstDiff >= m_lBurstOnDuration)) )
	{
		m_bCycleOn = FALSE;
		m_bBurstOn = FALSE;
		m_lBurstStart = lpSim->TimeSlice();

		*m_lpExternalCurrent = *m_lpExternalCurrent - m_fltActiveCurrent;
		m_fltActiveCurrent = m_fltCurrentBurstOff;
		*m_lpExternalCurrent = *m_lpExternalCurrent + m_fltActiveCurrent;
	}
	else if( (!m_bBurstOn && (lBurstDiff >= m_lBurstOffDuration)) )
	{
		m_bCycleOn = TRUE;
		m_bBurstOn = TRUE;
		m_lBurstStart = lpSim->TimeSlice();
		m_lCycleStart = m_lBurstStart;

		*m_lpExternalCurrent = *m_lpExternalCurrent - m_fltActiveCurrent;
		m_fltActiveCurrent = GetCurrentOn(lpSim);
		*m_lpExternalCurrent = *m_lpExternalCurrent + m_fltActiveCurrent;
	}
	else if(m_iType == AL_TONIC_CURRENT && m_lpCurrentOnEval)
	{
		*m_lpExternalCurrent = *m_lpExternalCurrent - m_fltActiveCurrent;
		m_fltActiveCurrent = GetCurrentOn(lpSim);
		*m_lpExternalCurrent = *m_lpExternalCurrent + m_fltActiveCurrent;
	}

}

void CurrentStimulus::Deactivate(Simulator *lpSim)
{		
	*m_lpExternalCurrent = *m_lpExternalCurrent - m_fltActiveCurrent;
}

void CurrentStimulus::Load(Simulator *lpSim, CStdXml &oXml)
{
	ActivatedItem::Load(lpSim, oXml);

	oXml.IntoElem();  //Into Simulus Element

	m_strNeuralModule = oXml.GetChildString("ModuleName");
	if(Std_IsBlank(m_strNeuralModule)) 
		THROW_ERROR(Al_Err_lNeuralModuleNameBlank, Al_Err_strNeuralModuleNameBlank);

	m_strOrganismID = oXml.GetChildString("OrganismID");

	if(Std_IsBlank(m_strOrganismID))
		THROW_PARAM_ERROR(Al_Err_lOrganismIDBlank, Al_Err_strOrganismIDBlank, "ID", m_strID);

	string strType = oXml.GetChildString("CurrentType", "TONIC");
	strType = Std_ToUpper(Std_Trim(strType));
	if(strType == "TONIC")
		m_iType = AL_TONIC_CURRENT;
	else if(strType == "REPETITIVE")
		m_iType = AL_REPETITIVE_CURRENT;
	else if(strType == "BURST")
		m_iType = AL_BURST_CURRENT;

	m_lTargetNodeID = oXml.GetChildLong("TargetNodeID");

	m_fltCycleOnDuration = oXml.GetChildFloat("CycleOnDuration", m_fltCycleOnDuration);
	m_fltCycleOffDuration = oXml.GetChildFloat("CycleOffDuration", m_fltCycleOffDuration);
	m_fltBurstOnDuration = oXml.GetChildFloat("BurstOnDuration", m_fltBurstOnDuration);
	m_fltBurstOffDuration = oXml.GetChildFloat("BurstOffDuration", m_fltBurstOffDuration);

	m_fltCurrentOn = oXml.GetChildFloat("CurrentOn", m_fltCurrentOn);
	m_fltCurrentOff = oXml.GetChildFloat("CurrentOff", m_fltCurrentOff);
	m_fltCurrentBurstOff = oXml.GetChildFloat("CurrentBurstOff", m_fltCurrentBurstOff);
	
	string strEquation = oXml.GetChildString("CurrentOnEquation", "");

	if(!Std_IsBlank(strEquation))
	{
		//Initialize the postfix evaluator.
		if(m_lpCurrentOnEval) 
		{delete m_lpCurrentOnEval; m_lpCurrentOnEval = NULL;}

		m_lpCurrentOnEval = new CStdPostFixEval;

		m_lpCurrentOnEval->AddVariable("t");
		m_lpCurrentOnEval->Equation(strEquation);
		m_lpCurrentOnEval->SetVariable("t", 0);
		m_fltActiveCurrent = m_lpCurrentOnEval->Solve();
	}
	else
		m_fltActiveCurrent = m_fltCurrentOn;

	Std_IsAboveMin((float) 0, m_fltCycleOnDuration, TRUE, "CycleOnDuration", FALSE);
	Std_IsAboveMin((float) 0, m_fltCycleOffDuration, TRUE, "CycleOffDuration", TRUE);

	Std_IsAboveMin((float) 0, m_fltBurstOnDuration, TRUE, "BurstOnDuration", FALSE);
	Std_IsAboveMin((float) 0, m_fltBurstOffDuration, TRUE, "BurstOffDuration", TRUE);

	//If we have always active then we need to reset some of the durations to make sure
	//they last for a very long time.
	if(m_bAlwaysActive)
	{
		if(strType == "TONIC")
		{
			m_fltCycleOnDuration = 10000;
			m_fltBurstOnDuration = 10000;
		}
		else if(strType == "REPETITIVE")
			m_fltBurstOnDuration = 10000;
	}

	oXml.OutOfElem(); //OutOf Simulus Element
}

void CurrentStimulus::Save(Simulator *lpSim, CStdXml &oXml)
{
}

void CurrentStimulus::Trace(ostream &oOs)
{
	oOs << "CurrentStimulus"  << ", Name: " << m_strName << " Time (" << m_fltStartTime << ", " << m_fltEndTime << ") Slice: (" << m_lStartSlice << ", " << m_lEndSlice << ")";
}

	}			//ExternalStimuli
}				//VortexAnimatLibrary




