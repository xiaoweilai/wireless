// testbmpcpDlg.h : header file
//

#if !defined(AFX_TESTBMPCPDLG_H__F6EF4036_159C_48A5_9F56_DCCCD421C68E__INCLUDED_)
#define AFX_TESTBMPCPDLG_H__F6EF4036_159C_48A5_9F56_DCCCD421C68E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestbmpcpDlg dialog

class CTestbmpcpDlg : public CDialog
{
// Construction
public:
	void drawbitmap();
	HBITMAP hBitmap;
	int scrx;
	int scry;
	int scrxlm;
	int scrylm;
	int capflag;
	CTestbmpcpDlg(CWnd* pParent = NULL);	// standard constructor
	void WriteBmpToFile(HBITMAP hBitmap,CString strFileName);

// Dialog Data
	//{{AFX_DATA(CTestbmpcpDlg)
	enum { IDD = IDD_TESTBMPCP_DIALOG };
	CScrollBar	m_vbar;
	CScrollBar	m_hbar;
	CString	m_filename;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestbmpcpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestbmpcpDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTBMPCPDLG_H__F6EF4036_159C_48A5_9F56_DCCCD421C68E__INCLUDED_)
