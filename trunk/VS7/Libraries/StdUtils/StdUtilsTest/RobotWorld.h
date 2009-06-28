// RobotWorld.h: interface for the CRobotWorld class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROBOTWORLD_H__8B8B3B6E_F6E6_40D6_9281_4E5123109083__INCLUDED_)
#define AFX_ROBOTWORLD_H__8B8B3B6E_F6E6_40D6_9281_4E5123109083__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRobotWorld : public CStdSerialize
{
protected:
	CStdPtrArray<CRobot> m_aryRobots;
	
	CStdString m_strName;
	int m_iWorldWidth;
	int m_iWorldHeight;
	BOOL m_bConnectToDB;
	float m_fltScaleFactor;

public:
	CRobotWorld();
	virtual ~CRobotWorld();

	virtual void Load(CStdXml &oXml);
	virtual void Save(CStdXml &oXml);

	virtual void LoadFile(CStdString strFilename);
	virtual void SaveFile(CStdString strFilename);
};

#endif // !defined(AFX_ROBOTWORLD_H__8B8B3B6E_F6E6_40D6_9281_4E5123109083__INCLUDED_)
