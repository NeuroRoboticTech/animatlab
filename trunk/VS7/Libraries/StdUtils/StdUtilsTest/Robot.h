// Robot.h: interface for the CRobot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROBOT_H__22ABE019_7B01_4807_82BD_2E9E1AAB0BEF__INCLUDED_)
#define AFX_ROBOT_H__22ABE019_7B01_4807_82BD_2E9E1AAB0BEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRobot : public COtherClass, public CBaseTest
{
protected:
	CStdPtrArray<CRobotLeg> m_aryLegs;
	
	CStdString m_strName;
	int m_iXPos;
	int m_iYPos;
	float m_fltRotation;

public:
	CRobot();
	virtual ~CRobot();

	virtual void Load(CStdXml &oXml);
	virtual void Save(CStdXml &oXml);
};

#endif // !defined(AFX_ROBOT_H__22ABE019_7B01_4807_82BD_2E9E1AAB0BEF__INCLUDED_)
