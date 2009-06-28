// BaseTest.h: interface for the CBaseTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASETEST_H__D5AC2D43_9488_4D9F_B812_0223F625F387__INCLUDED_)
#define AFX_BASETEST_H__D5AC2D43_9488_4D9F_B812_0223F625F387__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBaseTest : public CStdSerialize  
{
public:
	CBaseTest();
	virtual ~CBaseTest();

	virtual void Load(CStdXml &oXml);
	virtual void Save(CStdXml &oXml);
};

#endif // !defined(AFX_BASETEST_H__D5AC2D43_9488_4D9F_B812_0223F625F387__INCLUDED_)
