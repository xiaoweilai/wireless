// testbmpcp.h : main header file for the TESTBMPCP application
//

#if !defined(AFX_TESTBMPCP_H__486E2001_29A6_4DDD_B97C_CC72EC6A5299__INCLUDED_)
#define AFX_TESTBMPCP_H__486E2001_29A6_4DDD_B97C_CC72EC6A5299__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestbmpcpApp:
// See testbmpcp.cpp for the implementation of this class
//

class CTestbmpcpApp : public CWinApp
{
public:
	CTestbmpcpApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestbmpcpApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestbmpcpApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTBMPCP_H__486E2001_29A6_4DDD_B97C_CC72EC6A5299__INCLUDED_)
