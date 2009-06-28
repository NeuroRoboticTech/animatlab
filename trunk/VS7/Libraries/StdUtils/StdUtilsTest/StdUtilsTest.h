// StdUtilsTest.h : main header file for the STDUTILSTEST application
//

#if !defined(AFX_STDUTILSTEST_H__6A1B41C3_8C06_4292_B995_54CAD9408661__INCLUDED_)
#define AFX_STDUTILSTEST_H__6A1B41C3_8C06_4292_B995_54CAD9408661__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CStdUtilsTestApp:
// See StdUtilsTest.cpp for the implementation of this class
//

class CStdUtilsTestApp : public CWinApp
{
public:
	CStdUtilsTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStdUtilsTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CStdUtilsTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDUTILSTEST_H__6A1B41C3_8C06_4292_B995_54CAD9408661__INCLUDED_)
