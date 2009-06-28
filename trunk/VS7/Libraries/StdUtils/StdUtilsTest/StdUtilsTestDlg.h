// StdUtilsTestDlg.h : header file
//

#if !defined(AFX_STDUTILSTESTDLG_H__8E075EBC_C581_46DC_9274_51022614EA08__INCLUDED_)
#define AFX_STDUTILSTESTDLG_H__8E075EBC_C581_46DC_9274_51022614EA08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CStdUtilsTestDlg dialog

class CStdUtilsTestDlg : public CDialog
{
// Construction
public:
	CStdUtilsTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CStdUtilsTestDlg)
	enum { IDD = IDD_STDUTILSTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStdUtilsTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CStdUtilsTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTestErrorinfo();
	afx_msg void OnTestXml();
	afx_msg void OnTestMarkup();
	afx_msg void OnTestCompress();
	afx_msg void OnTestSerialize();
	afx_msg void OnTestPoint();
	afx_msg void OnTestBinarySection();
	afx_msg void OnTestGreyCode();
	afx_msg void OnTestTrace();
	afx_msg void OnTestPostFixEval();
	afx_msg void OnTestMaps();
	afx_msg void OnTestStrings();
	afx_msg void OnTestVariant();
	afx_msg void OnTestOdbc();
	afx_msg void OnTestRandom();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDUTILSTESTDLG_H__8E075EBC_C581_46DC_9274_51022614EA08__INCLUDED_)
