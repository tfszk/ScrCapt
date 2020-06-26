#pragma once

#include "resource.h"

// CTranspDlg Dialog

class CTranspDlg : public CDialog
{
	DECLARE_DYNAMIC(CTranspDlg)

public:
	CTranspDlg(CWnd* pParent = NULL);
	virtual ~CTranspDlg();

	enum { IDD = IDD_TRANSP_DLG };

public:
    double  unit;
    CString unit_str;

	RECT GetRect() const {
		CRect r = CRect(st_po, po_old);
		r.NormalizeRect();
		return r;
	}

protected:
    CPoint st_po;
    CPoint po_old;
    enum {StFree, StHold} state_st_capt;
    CWnd   infWnd;

    void redraw_line(CPoint point);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
