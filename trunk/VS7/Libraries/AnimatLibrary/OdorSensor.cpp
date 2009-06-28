// OdorSensor.cpp: implementation of the OdorSensor class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Node.h"
#include "Joint.h"
#include "ReceptiveField.h"
#include "ContactSensor.h"
#include "RigidBody.h"
#include "OdorType.h"
#include "Odor.h"
#include "OdorSensor.h"
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

/*! \brief 
   Constructs a muscle attachment object..
   		
   \param lpParent This is a pointer to the parent of this rigid body. 
	          If this value is null then it is assumed that this is
						a root object and no joint is loaded to connect this
						part to the parent.

	 \return
	 No return value.

   \remarks
	 The constructor for a muscle attachment. 
*/

OdorSensor::OdorSensor()
{
	m_aryColor[0] = 1;
	m_aryColor[1] = 0;
	m_aryColor[2] = 0;
	m_aryColor[3] = 1;
	m_fltDensity = 0;
	m_lpJointToParent = NULL;
	m_fltRadius = 1;
	m_fltOdorValue = 0;
	m_lpOdorType = NULL;
	m_bUsesJoint = FALSE;
}

/*! \brief 
   Destroys the muscle attachment object..
   		
	 \return
	 No return value.

   \remarks
   Destroys the muscle attachment object..	 
*/

OdorSensor::~OdorSensor()
{
	m_lpOdorType = NULL;
}

CStdFPoint OdorSensor::CalculatePosition(Simulator *lpSim)
{
	//Just add some default behavior so this can be used with the ce simulator.
	CStdFPoint oPoint;
	return oPoint;
}

// There are no parts or joints to create for muscle attachment points.
void OdorSensor::CreateParts(Simulator *lpSim, Structure *lpStructure)
{}

void OdorSensor::CreateJoints(Simulator *lpSim, Structure *lpStructure)
{}

float *OdorSensor::GetDataPointer(string strDataType)
{
	float *lpData=NULL;
	string strType = Std_CheckString(strDataType);

	if(strType == "BODYPOSITIONX")
		lpData = &m_oAbsPosition.x;
	else if(strType == "BODYPOSITIONY")
		lpData = &m_oAbsPosition.y;
	else if(strType == "BODYPOSITIONZ")
		lpData = &m_oAbsPosition.z;
	else if(strType == "ODORVALUE")
		lpData = &m_fltOdorValue;
	else
		THROW_TEXT_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "RigidBodyID: " + STR(m_strName) + "  DataType: " + strDataType);

	return lpData;
}

void OdorSensor::Load(Simulator *lpSim, Structure *lpStructure, CStdXml &oXml)
{
	if(!m_lpParent)
		THROW_ERROR(Al_Err_lParentNotDefined, Al_Err_strParentNotDefined);

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

	m_fltRadius = oXml.GetChildFloat("Radius", m_fltRadius);
	Std_IsAboveMin((float) 0,m_fltRadius, TRUE, "Radius");

	string strOdorTypeID = oXml.GetChildString("OdorTypeID", "");

	m_bIsVisible = oXml.GetChildBool("IsVisible", m_bIsVisible);

	if(oXml.FindChildElement("Color", FALSE))
	{
		oXml.IntoChildElement("Color");
		m_aryColor[0] = oXml.GetAttribFloat("Red");
		m_aryColor[1] = oXml.GetAttribFloat("Green");
		m_aryColor[2] = oXml.GetAttribFloat("Blue");
		m_aryColor[3] = oXml.GetAttribFloat("Alpha");
		oXml.OutOfElem();
	}
	else
	{
		m_aryColor[0] = 1;
		m_aryColor[1] = 0;
		m_aryColor[2] = 0;
		m_aryColor[3] = 1;
	}

	//Muscle attachments have no parents and no joints.

	oXml.OutOfElem(); //OutOf RigidBody Element

	if(!Std_IsBlank(strOdorTypeID))
		m_lpOdorType = lpSim->FindOdorType(strOdorTypeID);
}

		}		//Bodies
	}			//Environment
}				//AnimatLibrary

