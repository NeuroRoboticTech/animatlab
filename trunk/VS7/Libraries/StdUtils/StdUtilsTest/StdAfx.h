// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma warning(disable:4786)


#if !defined(AFX_STDAFX_H__E769FD18_08F5_4B2D_82C1_70B7AFE0217B__INCLUDED_)
#define AFX_STDAFX_H__E769FD18_08F5_4B2D_82C1_70B7AFE0217B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "StdUtils.h"

#include "BaseTest.h"
#include "OtherClass.h"
#include "RobotLeg.h"
#include "Robot.h"
#include "RobotWorld.h"

#define WINVER 0x0501

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E769FD18_08F5_4B2D_82C1_70B7AFE0217B__INCLUDED_)
