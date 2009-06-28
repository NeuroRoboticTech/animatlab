// JointDataColumn.cpp: implementation of the JointDataColumn class.
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
#include "JointDataColumn.h"

namespace AnimatLibrary
{
	namespace Charting
	{
		namespace DataColumns
		{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JointDataColumn::JointDataColumn()
{

}

JointDataColumn::~JointDataColumn()
{

}


void JointDataColumn::Initialize(Simulator *lpSim)
{
	Joint *lpJoint = lpSim->FindJoint(m_strStructureID, m_strJointID);

	m_lpDataValue = lpJoint->GetDataPointer(m_strDataType);

	if(!m_lpDataValue)
		THROW_TEXT_ERROR(Al_Err_lDataPointNotFound, Al_Err_strDataPointNotFound, 
		("DataColumn: " + m_strID + " StructureID: " + m_strStructureID + " JointID: " +  m_strJointID + " DataType: " + m_strDataType));

	m_bInitialized = TRUE;
}

void JointDataColumn::Load(Simulator *lpSim, CStdXml &oXml)
{

	if(!lpSim)
		THROW_ERROR(Al_Err_lSimulationNotDefined, Al_Err_strSimulationNotDefined);

	DataColumn::Load(lpSim, oXml);

	oXml.IntoElem();  //Into DataChart Element

	m_strStructureID = oXml.GetChildString("StructureID");

	if(Std_IsBlank(m_strStructureID)) 
		THROW_TEXT_ERROR(Al_Err_lIDBlank, Al_Err_strIDBlank, "StructureID");

	m_strJointID = oXml.GetChildString("JointID");

	if(Std_IsBlank(m_strJointID)) 
		THROW_TEXT_ERROR(Al_Err_lIDBlank, Al_Err_strIDBlank, "Joint ID");

	m_strDataType = oXml.GetChildString("DataType");
	
	if(Std_IsBlank(m_strDataType)) 
		THROW_PARAM_ERROR(Al_Err_lDataTypeBlank, Al_Err_strDataTypeBlank, "JointID", m_strJointID);

	oXml.OutOfElem(); //OutOf DataChart Element
}

		}		//DataColumns
	}			//Charting
}				//VortexAnimatLibrary
