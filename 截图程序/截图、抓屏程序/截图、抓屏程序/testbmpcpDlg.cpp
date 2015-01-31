// testbmpcpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "testbmpcp.h"
#include "testbmpcpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestbmpcpDlg dialog

CTestbmpcpDlg::CTestbmpcpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestbmpcpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestbmpcpDlg)
	m_filename = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestbmpcpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestbmpcpDlg)
	DDX_Control(pDX, IDC_SCROLLBAR2, m_vbar);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_hbar);
	DDX_Text(pDX, IDC_EDIT1, m_filename);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestbmpcpDlg, CDialog)
	//{{AFX_MSG_MAP(CTestbmpcpDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestbmpcpDlg message handlers

BOOL CTestbmpcpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	scrx=0;
	scry=0;
	scrxlm=100;
	scrylm=100;
	capflag=0;
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestbmpcpDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestbmpcpDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestbmpcpDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestbmpcpDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	HWND hWnd = ::GetDesktopWindow();//获得屏幕的HWND.
	HDC hScreenDC = ::GetDC(hWnd);   //获得屏幕的HDC.
	HDC MemDC = ::CreateCompatibleDC(hScreenDC);
	RECT rect; 
	::GetWindowRect(hWnd,&rect);
	SIZE screensize;
	screensize.cx=rect.right-rect.left;
	screensize.cy=rect.bottom-rect.top;
	hBitmap = ::CreateCompatibleBitmap(hScreenDC,screensize.cx,screensize.cy);
	HGDIOBJ hOldBMP = ::SelectObject(MemDC,hBitmap);
	::BitBlt(MemDC,0,0,screensize.cx,screensize.cy,hScreenDC,rect.left,rect.top,SRCCOPY);
	::SelectObject(MemDC,hOldBMP);
	::DeleteObject(MemDC);
	::ReleaseDC(hWnd,hScreenDC);
    CDC *pDC=GetDC();
	HDC dDlgDC=pDC->m_hDC;
	HDC MemDC1 = CreateCompatibleDC(dDlgDC);
	HGDIOBJ hOldBMP1 = ::SelectObject(MemDC1,hBitmap);
	::StretchBlt(dDlgDC,0,0,600,400,MemDC1,0,0,screensize.cx,screensize.cy,SRCCOPY);
	::SelectObject(MemDC1,hOldBMP1);
	::DeleteObject(MemDC1);
	::ReleaseDC(m_hWnd,dDlgDC);

	scrylm=screensize.cy-500;
    scrxlm=screensize.cx-700;
	m_vbar.SetScrollRange(0,screensize.cy-500,TRUE);
	m_vbar.SetScrollPos(0,TRUE);
	m_hbar.SetScrollRange(0,screensize.cx-700,TRUE);
	m_hbar.SetScrollPos(0,TRUE);
	capflag=1;

		 

}
void CTestbmpcpDlg::WriteBmpToFile(HBITMAP hBitmap,CString strFileName)
{
    HDC hDC =::CreateDC("DISPLAY",NULL,NULL,NULL); 
    int iBits = ::GetDeviceCaps(hDC, BITSPIXEL) * ::GetDeviceCaps(hDC, PLANES);//当前分辨率下每个像素所占字节数  
    ::DeleteDC(hDC);

    WORD   wBitCount;   //位图中每个像素所占字节数    
    if (iBits <= 1)
		wBitCount = 1;
    else if (iBits <= 4)
		wBitCount = 4;
    else if (iBits <= 8)
		wBitCount = 8;
    else if (iBits <= 24)
		wBitCount = 24;
    else
		wBitCount = iBits;
		    
	DWORD   dwPaletteSize=0;	//调色板大小， 位图中像素字节大小 
    if (wBitCount <= 8)		
		dwPaletteSize = (1 << wBitCount) *	sizeof(RGBQUAD);	
	

	BITMAP  bm;        //位图属性结构
    ::GetObject(hBitmap, sizeof(bm), (LPSTR)&bm);  

    
	BITMAPINFOHEADER   bi,bi1;       //位图信息头结构     
	bi.biSize            = sizeof(BITMAPINFOHEADER);  
    bi.biWidth           = bm.bmWidth;
    bi.biHeight          = bm.bmHeight;
    bi.biPlanes          = 1;
    bi.biBitCount        = wBitCount;
    bi.biCompression     = BI_RGB; //BI_RGB表示位图没有压缩
    bi.biSizeImage       = 0;
    bi.biXPelsPerMeter   = 0;
    bi.biYPelsPerMeter   = 0;
    bi.biClrUsed         = 0;
    bi.biClrImportant    = 0;
	bi1=bi;
	bi1.biBitCount=24;
	
    DWORD dwBmBitsSize = ((bm.bmWidth * wBitCount+31)/32) * 4 * bm.bmHeight;    
    HANDLE hDib  = ::GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));  //为位图内容分配内存
    LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    *lpbi = bi;
	
    HANDLE hPal = ::GetStockObject(DEFAULT_PALETTE);  // 处理调色板 
	HANDLE  hOldPal=NULL; 
    if (hPal)
    {
		hDC = ::GetDC(NULL);
		hOldPal = SelectPalette(hDC,(HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
    }
	int nOutputBytes = 0;
	unsigned char *pJpp,*m_pDibBits;
	pJpp = new unsigned char [dwBmBitsSize];
	m_pDibBits = new unsigned char [dwBmBitsSize];
    //::GetDIBits(hDC, hBitmap, 0, (UINT) bm.bmHeight,m_pDibBits,(BITMAPINFO*)lpbi,DIB_RGB_COLORS);
    ::GetDIBits(hDC, hBitmap, 0, (UINT) bm.bmHeight,(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)+dwPaletteSize,(BITMAPINFO*)lpbi,DIB_RGB_COLORS);// 获取该调色板下新的像素值
    if (hOldPal)//恢复调色板
    {
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
    }
/*	memcpy(m_pDibBits,(LPSTR)lpbi+sizeof(BITMAPINFOHEADER)+dwPaletteSize,dwBmBitsSize);
	int i=0,j=0;
	
	for(j=0;j<dwBmBitsSize;j++)
	{
		if(j%4==3)
			j++;
        m_pDibBits[i]=m_pDibBits[j];		
		i++;
	}*/

   
	BITMAPFILEHEADER   bmfHdr; //位图文件头结构     
	bmfHdr.bfType = 0x4D42;  // "BM"  	// 设置位图文件头
	DWORD dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;  
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	
	HANDLE hFile = CreateFile(strFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);//创建位图文件   
	DWORD dwWritten;
	WriteFile(hFile, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);	// 写入位图文件头
	WriteFile(hFile, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);// 写入位图文件其余内容


	GlobalUnlock(hDib);   //清除   
	GlobalFree(hDib);
	CloseHandle(hFile); 
    delete []pJpp;
	delete []m_pDibBits;
}

void CTestbmpcpDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	WriteBmpToFile(hBitmap,m_filename);
}

void CTestbmpcpDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	    int nPos1=m_vbar.GetScrollPos();
		if(nSBCode==SB_LINEDOWN)
		{
			scry=scry+10;
			if(scry>scrylm)
				scry=scrylm;
			nPos1+=10;
			drawbitmap();
			 
		}
		if(nSBCode==SB_LINEUP)
		{
			scry=scry-10;
			if(scry<0)
				scry=0;
			if(nPos1>10)
               nPos1-=10;
			drawbitmap();
		
		}
		if(nSBCode==SB_THUMBTRACK)
		{
			scry=nPos;
			nPos1=nPos;
			if(nPos1>scrylm)
                nPos1=scrylm;
			if(scry<0)
				scry=0;
			if(scry>scrylm)
				scry=scrylm;
			drawbitmap();
			
		}
         m_vbar.SetScrollPos(nPos1);
      
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CTestbmpcpDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
        int nPos1=m_hbar.GetScrollPos();
		if(nSBCode==SB_LINERIGHT)
		{
			scrx=scrx+10;
			if(scrx>scrxlm)
				scrx=scrxlm;
			nPos1+=10;
			 drawbitmap();
		}
		if(nSBCode==SB_LINELEFT)
		{
			scrx=scrx-10;
			if(scrx<0)
				scrx=0;
			if(nPos1>10)
               nPos1-=10;
			 drawbitmap();
		}
		if(nSBCode==SB_THUMBTRACK)
		{
			scrx=nPos;
			nPos1=nPos;
			if(nPos1>scrxlm)
                nPos1=scrxlm;

			if(scrx<0)
				scrx=0;
			if(scrx>scrxlm)
				scrx=scrxlm;
			drawbitmap();
		
		}
		m_hbar.SetScrollPos(nPos1,TRUE);
		
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTestbmpcpDlg::drawbitmap()
{
	if(capflag==0)
		return;
	CDC *pDC=GetDC();
	HDC dDlgDC=pDC->m_hDC;
	HDC MemDC1 = CreateCompatibleDC(dDlgDC);
	HGDIOBJ hOldBMP1 = ::SelectObject(MemDC1,hBitmap);
	::StretchBlt(dDlgDC,0,0,700,500,MemDC1,scrx,scry,700,500,SRCCOPY);
	::SelectObject(MemDC1,hOldBMP1);
	::DeleteObject(MemDC1);
	::ReleaseDC(m_hWnd,dDlgDC);

}
