// StimulusDataColumn.cpp: implementation of the StimulusDataColumn class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Node.h"
#include "Joint.h"
#include "ReceptiveField.h"
#include "ContactSensor.h"
#include "RigidBody.h"
#include "IODataEntry.h"
#include "Microcontroller.h"
#include "Structure.h"
#include "Organism.h"
#include "ActivatedItem.h"
#include "ActivatedItemMgr.h"
#include "ExternalStimulus.h"
#include "DataColumn.h"
#include "DataChartMgr.h"
#include "ExternalStimuliMgr.h"
#include "KeyFrame.h"
#include "SimulationRecorder.h"
#include "OdorType.h"
#include "Odor.h"
#include "Simulator.h"
#include "StimulusDataColumn.h"

namespace AnimatLibrary
{
	namespace Charting
	{
		namespace DataColumns
		{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

StimulusDataColumn::StimulusDataColumn()
{
	m_fltInternalValue = 0;
}

StimulusDataColumn::~StimulusDataColumn()
{

}


void StimulusDataColumn::Initialize(Simulator *lpSim)
{
	ExternalStimulus *lpStim = dynamic_cast<ExternalStimulus *>(lpSim->ExternalStimuliMgr()->Find(m_strStimulusID, FALSE));

	if(lpStim)
	{
		m_lpDataValue = lpStim->GetDataPointer(m_strDataType);

		if(!m_lpDataValue)
			THROW_TEXT_ERROR(Al_Err_lDataPointNotFound, Al_Err_strDataPointNotFound, 
			("StimulusID: " + m_strID + " DataType: " + m_strDataType));

		m_bInitialized = TRUE;
	}
	else
	{
		//If an external stimulus is not enabled then it will not be in the
		//simulator system and can not be found. In this case we want to "fake it"
		//so that the simulator can still run and the charted value will just send
		//back a default value of 0.
		m_lpDataValue = &m_fltInternalValue;
		m_bInitialized = TRUE;
	}

}

void StimulusDataColumn::Load(Simulator *lpSim, CStdXml &oXml)
{

	if(!lpSim)
		THROW_ERROR(Al_Err_lSimulationNotDefined, Al_Err_strSimulationNotDefined);

	DataColumn::Load(lpSim, oXml);

	oXml.IntoElem();  //Into DataChart Element

	m_strStimulusID = oXml.GetChildString("StimulusID");

	m_strDataType = oXml.GetChildString("DataType");
	
	if(Std_IsBlank(m_strDataType)) 
		THROW_PARAM_ERROR(Al_Err_lDataTypeBlank, Al_Err_strDataTypeBlank, "StimulusID", m_strStimulusID);

	oXml.OutOfElem(); //OutOf DataChart Element
}

		}		//DataColumns
	}			//Charting
}				//VortexAnimatLibrary
