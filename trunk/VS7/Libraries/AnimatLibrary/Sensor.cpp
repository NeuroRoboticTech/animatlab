// Sensor.cpp: implementation of the Sensor class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Node.h"
#include "Joint.h"
#include "ReceptiveField.h"
#include "ContactSensor.h"
#include "RigidBody.h"
#include "Sensor.h"
#include "IODataEntry.h"
#include "Microcontroller.h"
#include "Structure.h"
#include "Organism.h"
#include "ActivatedItem.h"
#include "ActivatedItemMgr.h"
#include "DataChartMgr.h"
#include "ExternalStimuliMgr.h"
#include "KeyFrame.h"
#include "SimulationRecorder.h"
#include "OdorType.h"
#include "Odor.h"
#include "Simulator.h"

namespace AnimatLibrary
{
	namespace Environment
	{
		namespace Bodies
		{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Sensor::Sensor()
{
}

Sensor::~Sensor()
{

}

// There are no parts or joints to create for muscle attachment points.
void Sensor::CreateParts(Simulator *lpSim, Structure *lpStructure)
{}

void Sensor::CreateJoints(Simulator *lpSim, Structure *lpStructure)
{}

void Sensor::Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	if(!m_lpParent)
		THROW_ERROR(Al_Err_lParentNotDefined, Al_Err_strParentNotDefined);

	//We do NOT want to call the rigid body load here. The reason is that
	//a sensor is a leaf node. It can not have children. So the load is 
	//a lot simpler.
	oXml.IntoElem();  //Into RigidBody Element

	m_strType = oXml.GetChildString("Type");
	m_strID = Std_CheckString(oXml.GetChildString("ID"));
	m_strName = oXml.GetChildString("Name", "");

	if(Std_IsBlank(m_strID)) 
		THROW_ERROR(Al_Err_lIDBlank, Al_Err_strIDBlank);

	if(Std_IsBlank(m_strName)) 
		m_strName = m_strID;

	Std_LoadPoint(oXml, "RelativePosition", m_oRelPosition);
	m_oAbsPosition = m_lpParent->AbsolutePosition() + m_oRelPosition;

	oXml.OutOfElem(); //OutOf RigidBody Element
}

		}		//Bodies
	}			//Environment
}				//AnimatLibrary
