// RobotLeg.cpp: implementation of the CRobotLeg class.
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

CRobotLeg::CRobotLeg()
{
	m_iLength = 0;
	m_fltRotation = 0;
}

CRobotLeg::~CRobotLeg()
{

}

CStdSerialize *CRobotLeg::Clone()
{
	CRobotLeg *lpLeg = new CRobotLeg;
	lpLeg->m_iLength = m_iLength;
	lpLeg->m_fltRotation = m_fltRotation;
	return lpLeg;
}

void CRobotLeg::Trace(ostream &oOs)
{oOs << "(Length: " << m_iLength << ", Rotation: " << m_fltRotation << ")";}

void CRobotLeg::Load(CStdXml &oXml)
{
	oXml.IntoElem();  //Into Leg Element

	m_iLength = oXml.GetChildInt("Length");

	oXml.OutOfElem(); //OutOf Leg Element
}

void CRobotLeg::Save(CStdXml &oXml)
{
	oXml.AddChildElement("Leg");
	oXml.IntoElem();  //Into Leg Element
	
	oXml.AddChildElement("Length", m_iLength);

	oXml.OutOfElem(); //OutOf Leg Element
}


/*
ostream& operator<<(ostream& os, CRobotLeg &leg)
{
	os << m_iLength << "  " << m_fltRotation;
	return os;
}

ostream& operator<<(ostream& os, CRobotLeg *leg)
{
	os << leg->m_iLength << "  " << leg->m_fltRotation;
	return os;
}
*/

