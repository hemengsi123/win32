#include "NppBase.h"
#include "NppWnd.h"
#include "NppToolTip.h"

CNppToolTip::CNppToolTip(): m_bTrackMouse(FALSE), m_bLeftBtnDown(FALSE)
{

}
CNppToolTip::~CNppToolTip()
{

}
void CNppToolTip::destroy()
{
	if(m_hSelf)
	{
		::DestroyWindow(m_hSelf);
		m_hSelf        = NULL;
		m_bTrackMouse  = FALSE;
		m_bLeftBtnDown = FALSE;
	}
}
LPCTSTR CNppToolTip::getWndClassName()const
{
	return TOOLTIPS_CLASS;
}
void CNppToolTip::init(HINSTANCE hInst, HWND hParent, UINT iCtrlID)
{
	CNppCtrlWnd::init(hInst, hParent, iCtrlID);
}
HWND CNppToolTip::create(int x, int y, int cx, int cy)
{
	CNppCtrlWnd::create(NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, x, y, cx, cy, 0);
	setWndProc();
	return m_hSelf;
}
void CNppToolTip::show(RECT rectTitle, LPTSTR pszTitle, int iXOff, int iWidthOff)
{
	if (isVisible())
		destroy();

	if (_tcslen(pszTitle) == 0)
		return;

	// INITIALIZE MEMBERS OF THE TOOLINFO STRUCTURE
	m_ti.cbSize		= sizeof(TOOLINFO);
	m_ti.uFlags		= TTF_TRACK | TTF_ABSOLUTE;
	m_ti.hwnd		= ::GetParent(m_hParent);
	m_ti.hinst		= m_hInst;
	m_ti.uId			= 0;

	m_ti.rect.left	= rectTitle.left;
	m_ti.rect.top	= rectTitle.top;
	m_ti.rect.right	= rectTitle.right;
	m_ti.rect.bottom	= rectTitle.bottom;

	HFONT	hFont = (HFONT)::SendMessage(m_hParent, WM_GETFONT, 0, 0);	
	::SendMessage(m_hSelf, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);

	m_ti.lpszText	= pszTitle;
	::SendMessage(m_hSelf, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &m_ti);
	::SendMessage(m_hSelf, TTM_TRACKPOSITION, 0, (LPARAM)(DWORD) MAKELONG(m_ti.rect.left + iXOff, m_ti.rect.top + iWidthOff));
	::SendMessage(m_hSelf, TTM_TRACKACTIVATE, true, (LPARAM)(LPTOOLINFO) &m_ti);
}
LRESULT CNppToolTip::runCtrlProc(UINT uMsg, WPARAM wParam, LPARAM lParam, bool & bDone)
{
	bDone = true;
	 switch (uMsg)
    {
		case WM_MOUSEACTIVATE:
		{
			return MA_NOACTIVATE;
		}
		case WM_CREATE:
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.hwndTrack = m_hSelf;
			tme.dwFlags = TME_LEAVE | TME_HOVER;
			tme.dwHoverTime = 5000;
			m_bTrackMouse = ::_TrackMouseEvent(&tme);
			break;
		}
    	case WM_LBUTTONDOWN:
		{
			m_bLeftBtnDown = TRUE;
			//SendMessageToParent(WM_LBUTTONDOWN);
			return TRUE;
		}
		case WM_LBUTTONUP:
		{
			m_bLeftBtnDown = FALSE;
			//SendMessageToParent(message);
			return TRUE;
		}    	
		case WM_RBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
		{
			//SendMessageToParent(message);
			return TRUE;
		}
		case WM_MOUSEMOVE:
		{
			if ((m_bLeftBtnDown == TRUE) && (MK_LBUTTON & wParam))
			{
				//SendMessageToParent(message);
			}
			if (!m_bTrackMouse)
			{
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(tme);
				tme.hwndTrack = m_hSelf;
				tme.dwFlags = TME_LEAVE | TME_HOVER;
				tme.dwHoverTime = 5000;
				m_bTrackMouse = ::_TrackMouseEvent(&tme);
			}
			else
				m_bTrackMouse = FALSE;
			return TRUE;
		}
 		case WM_MOUSEHOVER:
		{
			destroy();
			return TRUE;
		}
		case WM_MOUSELEAVE:
		{
			if (m_bLeftBtnDown == FALSE) {
				destroy();
			} else {
				m_bLeftBtnDown = FALSE;
			}
			return TRUE;
		}
		default:
			bDone = false;
	}
	return FALSE;
}
