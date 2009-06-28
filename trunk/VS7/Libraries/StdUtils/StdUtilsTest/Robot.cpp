// Robot.cpp: implementation of the CRobot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRobot::CRobot()
{
	m_iXPos = 0;
	m_iYPos = 0;
	m_fltRotation = 0;
}

CRobot::~CRobot()
{

}

void CRobot::Load(CStdXml &oXml)
{
	CRobotLeg *lpLeg=NULL;
	int iNumLegs, iLeg;

try
{
	oXml.IntoElem();  //Into Robot Element

	m_strName = oXml.GetChildString("Name");
	m_iXPos = oXml.GetChildInt("XPos");
	m_iYPos = oXml.GetChildInt("YPos");

	oXml.IntoChildElement("Legs");  //Into Legs Element
	
	iNumLegs = oXml.NumberOfChildren();
	for(iLeg=0; iLeg<iNumLegs; iLeg++)
	{
		oXml.FindChildByIndex(iLeg);

		lpLeg = new CRobotLeg;
		lpLeg->Load(oXml);

		m_aryLegs.Add(lpLeg);
		lpLeg = NULL;
	}

	oXml.OutOfElem();  //OutOf Legs Element
	oXml.OutOfElem();  //OutOf Robot Element
}
catch(CStdErrorInfo oError)
{
	if(lpLeg) delete lpLeg;
	RELAY_ERROR(oError);
}
catch(...)
{
	if(lpLeg) delete lpLeg;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
}
}

void CRobot::Save(CStdXml &oXml)
{
	CRobotLeg *lpLeg=NULL;
	int iNumLegs, iLeg;

try
{
	oXml.AddChildElement("Robot");
	oXml.IntoElem();  //Into Robot Element

	oXml.AddChildElement("Name", m_strName);
	oXml.AddChildElement("XPos", m_iXPos);
	oXml.AddChildElement("YPos", m_iYPos);

	oXml.AddChildElement("Legs");
	oXml.IntoChildElement("Legs");  //Into Legs Element
	
	iNumLegs = m_aryLegs.GetSize();
	for(iLeg=0; iLeg<iNumLegs; iLeg++)
	{
		lpLeg = m_aryLegs[iLeg];
		lpLeg->Save(oXml);
	}

	oXml.OutOfElem();  //OutOf Legs Element
	oXml.OutOfElem();  //OutOf Robot Element
}
catch(CStdErrorInfo oError)
{RELAY_ERROR(oError);}
catch(...)
{THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);}
}
