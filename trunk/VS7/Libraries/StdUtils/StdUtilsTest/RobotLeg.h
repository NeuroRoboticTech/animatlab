// RobotLeg.h: interface for the CRobotLeg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROBOTLEG_H__FDAAB4A8_7BB4_420E_8A8A_B704E6520947__INCLUDED_)
#define AFX_ROBOTLEG_H__FDAAB4A8_7BB4_420E_8A8A_B704E6520947__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRobotLeg : public CBaseTest
{
public:
	int m_iLength;
	float m_fltRotation;
	
public:
	CRobotLeg();
	virtual ~CRobotLeg();

	virtual CStdSerialize *Clone();
	virtual void Trace(ostream &oOs);
	virtual void Load(CStdXml &oXml);
	virtual void Save(CStdXml &oXml);
};

//ostream& operator<<(ostream& os, CRobotLeg &leg);
//ostream& operator<<(ostream& os, CRobotLeg *leg);

#endif // !defined(AFX_ROBOTLEG_H__FDAAB4A8_7BB4_420E_8A8A_B704E6520947__INCLUDED_)
