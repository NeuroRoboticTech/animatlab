// RigidBodyDataColumn.cpp: implementation of the RigidBodyDataColumn class.
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
#include "DataColumn.h"
#include "DataChartMgr.h"
#include "ExternalStimuliMgr.h"
#include "KeyFrame.h"
#include "SimulationRecorder.h"
#include "OdorType.h"
#include "Odor.h"
#include "Simulator.h"
#include "RigidBodyDataColumn.h"

namespace AnimatLibrary
{
	namespace Charting
	{
		namespace DataColumns
		{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RigidBodyDataColumn::RigidBodyDataColumn()
{
}

RigidBodyDataColumn::~RigidBodyDataColumn()
{

}

void RigidBodyDataColumn::Initialize(Simulator *lpSim)
{
	RigidBody *lpBody= lpSim->FindRigidBody(m_strStructureID, m_strRigidBodyID);

	m_lpDataValue = lpBody->GetDataPointer(m_strDataType);

	if(!m_lpDataValue)
		THROW_TEXT_ERROR(Al_Err_lDataPointNotFound, Al_Err_strDataPointNotFound, 
		("DataColumn: " + m_strID + " StructureID: " + m_strStructureID + " BodyID: " +  m_strRigidBodyID + " DataType: " + m_strDataType));

	m_bInitialized = TRUE;
}


void RigidBodyDataColumn::Load(Simulator *lpSim, CStdXml &oXml)
{

	if(!lpSim)
		THROW_ERROR(Al_Err_lSimulationNotDefined, Al_Err_strSimulationNotDefined);
		
	DataColumn::Load(lpSim, oXml);

	oXml.IntoElem();  //Into DataChart Element

	m_strStructureID = oXml.GetChildString("StructureID");

	if(Std_IsBlank(m_strStructureID)) 
		THROW_TEXT_ERROR(Al_Err_lIDBlank, Al_Err_strIDBlank, "StructureID");

	m_strRigidBodyID = oXml.GetChildString("RigidBodyID");

	if(Std_IsBlank(m_strRigidBodyID)) 
		THROW_TEXT_ERROR(Al_Err_lIDBlank, Al_Err_strIDBlank, "Rigid Body ID");

	m_strDataType = oXml.GetChildString("DataType");
	
	if(Std_IsBlank(m_strDataType)) 
		THROW_PARAM_ERROR(Al_Err_lDataTypeBlank, Al_Err_strDataTypeBlank, "Rigid Body ID", m_strRigidBodyID);

	oXml.OutOfElem(); //OutOf DataChart Element
}

		}		//DataColumns
	}			//Charting
}				//VortexAnimatLibrary
