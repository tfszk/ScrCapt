// ScrCaptDlg.cpp : implementation file
// 2020.6 tfs, All rights reserved.


#include "stdafx.h"
#include "ScrCapt.h"
#include "ScrCaptDlg.h"
#include "afxdialogex.h"

#include "TranspDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CScrCaptDlg dialog



CScrCaptDlg::CScrCaptDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SCRCAPT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_frontWnd = NULL;
	m_capture = FALSE;
}

CScrCaptDlg::~CScrCaptDlg()
{
	delete m_frontWnd;
}


void CScrCaptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScrCaptDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CScrCaptDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_SELAREA, &CScrCaptDlg::OnBnClickedSelectArea)
END_MESSAGE_MAP()


// CScrCaptDlg message handlers

BOOL CScrCaptDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	CRect dskRct;
	GetDesktopWindow()->GetWindowRect(&dskRct);

	m_frontWnd = new CWnd();
	m_frontWnd->CreateEx(WS_EX_LAYERED/*| WS_EX_TRANSPARENT*/,
		_T("#32770"), _T(""), // XXX Dialog class 
		WS_POPUPWINDOW,
		dskRct, this, 0);
	m_frontWnd->SetLayeredWindowAttributes(0, 96, LWA_ALPHA);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CScrCaptDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CScrCaptDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CScrCaptDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CScrCaptDlg::OnBnClickedSelectArea()
{
	this->ShowWindow(SW_HIDE);
	CTranspDlg dlg;
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);

	CRect r = dlg.GetRect();
	m_captRect = r;

	SetDlgItemInt(IDC_EDIT1, r.TopLeft().x);
	SetDlgItemInt(IDC_EDIT2, r.TopLeft().y);

	SetDlgItemInt(IDC_EDIT3, r.BottomRight().x);
	SetDlgItemInt(IDC_EDIT4, r.BottomRight().y);
}

void CScrCaptDlg::OnBnClickedOk()
{
	CDC *pBgDC = GetDesktopWindow()->GetDC();

	CRect rct;
	GetDesktopWindow()->GetWindowRect(&rct);

	CBitmap bmp;
	CDC dc;
	dc.CreateCompatibleDC(pBgDC);
	bmp.CreateCompatibleBitmap(pBgDC,
		m_captRect.Width(), m_captRect.Height());
	HGDIOBJ old = dc.SelectObject(&bmp);

	dc.BitBlt(0, 0,
		m_captRect.Width(), m_captRect.Height(),
		pBgDC, m_captRect.left, m_captRect.top, SRCCOPY);


	dc.SelectObject(old);

	OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, bmp.GetSafeHandle());
	CloseClipboard();

	// CDialogEx::OnOK();
}
