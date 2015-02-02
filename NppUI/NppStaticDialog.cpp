
#include "NppLib.h"
#include "NppStaticDialog.h"
#include "SysMsg.h"

#define WS_EX_LAYOUTRTL 0x00400000L


void CNppStaticDialog::goToCenter()
{
    RECT rc;
	HWND hParen = NULL;
	if( m_hParent == NULL)
	{
		hParen = ::GetDesktopWindow();
	}
	else
		hParen = m_hParent;
	
    ::GetClientRect(hParen, &rc);
    POINT center;
    center.x = rc.left + (rc.right - rc.left)/2;
    center.y = rc.top + (rc.bottom - rc.top)/2;
    ::ClientToScreen(hParen, &center);

	int x = center.x - (_rc.right - _rc.left)/2;
	int y = center.y - (_rc.bottom - _rc.top)/2;

	::SetWindowPos(m_hSelf, HWND_TOP, x, y, _rc.right - _rc.left, _rc.bottom - _rc.top, SWP_SHOWWINDOW);
}

HGLOBAL CNppStaticDialog::makeRTLResource(int dialogID, DLGTEMPLATE **ppMyDlgTemplate)
{
	// Get Dlg Template resource
	HRSRC  hDialogRC = ::FindResource(m_hInst, MAKEINTRESOURCE(dialogID), RT_DIALOG);
	HGLOBAL  hDlgTemplate = ::LoadResource(m_hInst, hDialogRC);
	DLGTEMPLATE *pDlgTemplate = (DLGTEMPLATE *)::LockResource(hDlgTemplate);
	
	// Duplicate Dlg Template resource
	unsigned long sizeDlg = ::SizeofResource(m_hInst, hDialogRC);
	HGLOBAL hMyDlgTemplate = ::GlobalAlloc(GPTR, sizeDlg);
	*ppMyDlgTemplate = (DLGTEMPLATE *)::GlobalLock(hMyDlgTemplate);

	::memcpy(*ppMyDlgTemplate, pDlgTemplate, sizeDlg);
	
	DLGTEMPLATEEX *pMyDlgTemplateEx = (DLGTEMPLATEEX *)*ppMyDlgTemplate;
	if (pMyDlgTemplateEx->signature == 0xFFFF)
		pMyDlgTemplateEx->exStyle |= WS_EX_LAYOUTRTL;
	else
		(*ppMyDlgTemplate)->dwExtendedStyle |= WS_EX_LAYOUTRTL;

	return hMyDlgTemplate;
}

void CNppStaticDialog::create(int dialogID, bool isRTL)
{
	if (isRTL)
	{
		DLGTEMPLATE *pMyDlgTemplate = NULL;
		HGLOBAL hMyDlgTemplate = makeRTLResource(dialogID, &pMyDlgTemplate);
		m_hSelf = ::CreateDialogIndirectParam(m_hInst, pMyDlgTemplate, m_hParent, (DLGPROC)dlgProc, (LPARAM)this);
		::GlobalFree(hMyDlgTemplate);
	}
	else
		m_hSelf = ::CreateDialogParam(m_hInst, MAKEINTRESOURCE(dialogID), m_hParent, (DLGPROC)dlgProc, (LPARAM)this);

	if (!m_hSelf)
	{
		systemMessage(_T("CNppStaticDialog"));
		throw int(666);
	}
	// diaog 大小不可以改变
	CNppBaseWnd::setWndStyle(getWndStyle() & ~WS_SIZEBOX);
	// ::SendMessage(m_hParent, WM_MODELESSDIALOG, MODELESSDIALOGADD, (WPARAM)m_hSelf);
}

BOOL CALLBACK CNppStaticDialog::dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message) 
	{
		case WM_INITDIALOG :
		{
			CNppStaticDialog *pStaticDlg = (CNppStaticDialog *)(lParam);
			pStaticDlg->m_hSelf = hwnd;
			::SetWindowLong(hwnd, GWL_USERDATA, (long)lParam);
			::GetWindowRect(hwnd, &(pStaticDlg->_rc));
            pStaticDlg->run_dlgProc(hwnd, message, wParam, lParam);
			
			return TRUE;
		}

		default :
		{
			CNppStaticDialog *pStaticDlg = reinterpret_cast<CNppStaticDialog *>(::GetWindowLong(hwnd, GWL_USERDATA));
			if (!pStaticDlg)
				return FALSE;
			return pStaticDlg->run_dlgProc(hwnd, message, wParam, lParam);
		}
	}
}
BOOL CALLBACK CNppStaticDialog::run_dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}
void CNppStaticDialog::alignWith(HWND handle, HWND handle2Align, PosAlign pos, POINT & point)
{
    RECT rc, rc2;
    ::GetWindowRect(handle, &rc);

    point.x = rc.left;
    point.y = rc.top;

    switch (pos)
    {
        case ALIGNPOS_LEFT :
            ::GetWindowRect(handle2Align, &rc2);
            point.x -= rc2.right - rc2.left;
            break;

        case ALIGNPOS_RIGHT :
            ::GetWindowRect(handle, &rc2);
            point.x += rc2.right - rc2.left;
            break;

        case ALIGNPOS_TOP :
            ::GetWindowRect(handle2Align, &rc2);
            point.y -= rc2.bottom - rc2.top;
            break;

        default : //ALIGNPOS_BOTTOM
            ::GetWindowRect(handle, &rc2);
            point.y += rc2.bottom - rc2.top;
            break;
    }
    
    ::ScreenToClient(m_hSelf, &point);
}
