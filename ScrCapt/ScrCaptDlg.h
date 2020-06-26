
// ScrCaptDlg.h : header file
//

#pragma once


// CScrCaptDlg dialog
class CScrCaptDlg : public CDialogEx
{
// Construction
public:
	CScrCaptDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CScrCaptDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCRCAPT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	int   m_capture;
	CWnd *m_frontWnd;

	CRect m_captRect;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSelectArea();
};
