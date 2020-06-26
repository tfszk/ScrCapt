// TranspDlg.cpp : Implementation File
// 2009.9 tfs, All rights reserved.

#include "stdafx.h"
#include "TranspDlg.h"

#include <math.h>

// CTranspDlg dialog

IMPLEMENT_DYNAMIC(CTranspDlg, CDialog)

CTranspDlg::CTranspDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTranspDlg::IDD, pParent)
{
    st_po  = CPoint(-1,-1);
    po_old = CPoint(-1,-1);
}

CTranspDlg::~CTranspDlg()
{
}

void CTranspDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTranspDlg, CDialog)
    ON_WM_LBUTTONDOWN()
    ON_WM_RBUTTONDOWN()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CTranspDlg Message Handler

BOOL CTranspDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    CRect dskRct;
    GetDesktopWindow()->GetWindowRect(&dskRct);

    SetWindowLong(m_hWnd, GWL_EXSTYLE,
                  GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(0, 96, LWA_ALPHA);
    MoveWindow(dskRct);

    infWnd.CreateEx(0, _T("edit"), _T(""),
                   WS_POPUPWINDOW | ES_MULTILINE | WS_VISIBLE,
                   CRect(0, 0, 180, 80), this, 0, NULL);
    infWnd.SetFont(GetFont());

    state_st_capt = StFree;
    st_po  = CPoint(-1,-1);
    po_old = CPoint(-1,-1);

    CDC dc;
    dc.CreateIC(_T("DISPLAY"), NULL, NULL, NULL);
    unit =  dc.GetDeviceCaps(HORZSIZE);
    unit /= dc.GetDeviceCaps(HORZRES);
    unit_str = _T("mm");

    return TRUE;
}


void CTranspDlg::redraw_line(CPoint point)
{
    CDC dc;
    dc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);

    dc.SetROP2(R2_NOTXORPEN);
    if (po_old.x >= 0) {
		CRect r = CRect(st_po, po_old); r.NormalizeRect();
		dc.Rectangle(r);
    }

    if (point.x >= 0) {
		CRect r = CRect(st_po, point); r.NormalizeRect();
		dc.Rectangle(r);
        po_old = point;
    }
}

void CTranspDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    ClientToScreen(&point);

    if (state_st_capt == StHold) {
        state_st_capt = StFree;
    }

    if (state_st_capt == StFree) {
        if (GetCapture() == this) {
            ReleaseCapture();
            // redraw_line(CPoint(-1,-1));
            state_st_capt = StHold;

            //state_st_capt = 0;
            //SetLayeredWindowAttributes(0, 255, LWA_ALPHA);
            //MoveWindow(org_rct);
        } else {
            SetCapture();
            st_po = point;
            po_old = CPoint(-1, -1);
        }
    }

    CDialog::OnLButtonDown(nFlags, point);
}

void CTranspDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    ClientToScreen(&point);
    if (GetCapture() == this) {
        redraw_line(point);
    }

    if (infWnd.GetSafeHwnd() && state_st_capt != StHold) {
        CString txt;
        int cx = point.x-st_po.x;
        int cy = point.y-st_po.y;
        double z = sqrt((double)(cx*cx + cy*cy));

        txt.Format(_T("X,Y:\t(%d,%d)\r\n"), point.x, point.y);
        txt.AppendFormat(_T("X0,Y0:\t(%d,%d)\r\n"), st_po.x, st_po.y);
        txt.AppendFormat(_T("dx,dy:\t(%d,%d) [%.2f, %.2f]\r\n"), cx, cy, cx*unit, cy*unit);
        txt.AppendFormat(_T("dist:\t%.2f [%.2f%s]\r\n"), z, z*unit, unit_str);
        infWnd.SetWindowText(txt);
    }    

    CDialog::OnMouseMove(nFlags, point);
}

void CTranspDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
    if (state_st_capt == StHold) {
        state_st_capt = StFree;
        redraw_line(CPoint(-1,-1));
        return;
    }

    if (state_st_capt == StFree) {
        if (GetCapture() == this) {
            ReleaseCapture();
            redraw_line(CPoint(-1,-1));
        }    
    }

    CDialog::OnRButtonDown(nFlags, point);
}
