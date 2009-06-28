// RobotWorld.cpp: implementation of the CRobotWorld class.
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

CRobotWorld::CRobotWorld()
{
	m_iWorldWidth = 100;
	m_iWorldHeight = 100;
	m_bConnectToDB = false;
	m_fltScaleFactor = 0.0;
}

CRobotWorld::~CRobotWorld()
{

}

void CRobotWorld::LoadFile(CStdString strFilename)
{
	CStdXml oXml;

try
{
	oXml.Load(strFilename);

	//Set the indexes
	oXml.FindElement("World");
	oXml.FindChildElement("");

	Load(oXml);
}
catch(CStdErrorInfo oError)
{RELAY_ERROR(oError);}
catch(...)
{THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);}
}

void CRobotWorld::SaveFile(CStdString strFilename)
{
	CStdXml oXml;

try
{

	oXml.AddElement("World");

	Save(oXml);

	oXml.Save(strFilename);
}
catch(CStdErrorInfo oError)
{RELAY_ERROR(oError);}
catch(...)
{THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);}
}


void CRobotWorld::Load(CStdXml &oXml)
{
	CRobot *lpRobot=NULL;
	int iNumRobots, iRobot;

try
{

	m_strName = oXml.GetChildString("Name");
	m_iWorldWidth = oXml.GetChildInt("Width");
	m_iWorldHeight = oXml.GetChildInt("Height");
	
	oXml.IntoChildElement("Robots");
	
	iNumRobots = oXml.NumberOfChildren();
	for(iRobot=0; iRobot<iNumRobots; iRobot++)
	{
		oXml.FindChildByIndex(iRobot);

		lpRobot = new CRobot;
		lpRobot->Load(oXml);

		m_aryRobots.Add(lpRobot);
		lpRobot = NULL;
	}

	oXml.OutOfElem();

}
catch(CStdErrorInfo oError)
{
	if(lpRobot) delete lpRobot;
	RELAY_ERROR(oError);
}
catch(...)
{
	if(lpRobot) delete lpRobot;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
}
}


void CRobotWorld::Save(CStdXml &oXml)
{
	CRobot *lpRobot=NULL;
	int iNumRobots, iRobot;

try
{
	oXml.AddChildElement("Name", m_strName);
	oXml.AddChildElement("Width", m_iWorldWidth);
	oXml.AddChildElement("Height", m_iWorldHeight);

	oXml.AddChildElement("Robots");
	oXml.IntoChildElement("Robots");

	iNumRobots = m_aryRobots.GetSize();
	for(iRobot=0; iRobot<iNumRobots; iRobot++)
	{
		lpRobot = m_aryRobots[iRobot];

		lpRobot->Save(oXml);
	}
	
	/*
	m_strName = oXml.GetChildString("Name");
	m_iWorldWidth = oXml.GetChildInt("Width");
	m_iWorldHeight = oXml.GetChildInt("Height");
	
	oXml.IntoChildElement("Robots");
	
	iNumRobots = oXml.NumberOfChildren();
	for(iRobot=0; iRobot<iNumRobots; iRobot++)
	{
		oXml.FindChildByIndex(iRobot);

		lpRobot = new CRobot;
		lpRobot->Load(oXml);

		m_aryRobots.Add(lpRobot);
		lpRobot = NULL;
	}

	oXml.OutOfElem();
	*/
}
catch(CStdErrorInfo oError)
{RELAY_ERROR(oError);}
catch(...)
{THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);}
}
