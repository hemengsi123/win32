
#include "NppLib.h"
#include "NppWnd.h"

CNppWnd::CNppWnd(): m_hInst(NULL), m_hParent(NULL), m_hSelf(NULL), \
					m_sysWndProc(::DefWindowProc)
{
	
}
CNppWnd::~CNppWnd()
{
	destroy();
}
void CNppWnd::destroy()
{

}
LRESULT CALLBACK CNppWnd::WndProcWrap(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	 CNppWnd* phSelf = NULL;
    if( uMsg == WM_NCCREATE && lParam != 0)
	{
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        phSelf = static_cast<CNppWnd*>(lpcs->lpCreateParams);
        phSelf->m_hSelf      = hwnd;
		phSelf->m_sysWndProc = ::DefWindowProc;
        ::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(phSelf));
    } 
    else
	{
        phSelf = reinterpret_cast<CNppWnd*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if( uMsg == WM_NCDESTROY && phSelf != NULL ) 
		{
            LRESULT lRes = ::CallWindowProc(phSelf->m_sysWndProc, hwnd, uMsg, wParam, lParam);
            ::SetWindowLongPtr(phSelf->m_hSelf, GWLP_USERDATA, 0L);
            phSelf->m_hSelf = NULL;
            return lRes;
        }
    }
    if( phSelf != NULL ) 
	{
        return phSelf->runWndProc(hwnd, uMsg, wParam, lParam);
    } 
    else
        return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
 
}
WNDPROC CNppWnd::setWndProc(HWND hWnd, WNDPROC userWndProc)
{
	if( !hWnd )
		hWnd = m_hSelf;
	
	::SetWindowLongPtr(hWnd, GWLP_USERDATA, LONG_PTR(this));
	m_sysWndProc = reinterpret_cast<WNDPROC>(::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)userWndProc));
	ASSERT(m_sysWndProc != NULL);
	return m_sysWndProc;
}
bool CNppWnd::registerWndClass()
{
    WNDCLASS wc = { 0 };
    wc.style = getClassStyle();
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hIcon = NULL;
    wc.lpfnWndProc = WndProcWrap;
    wc.hInstance = getHInst();
    wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = getWndClassName();
    ATOM ret = ::RegisterClass(&wc);
    ASSERT(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
    return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}
UINT CNppWnd::getClassStyle() const
{
    return 0;
}
HWND CNppWnd::create(LPCTSTR lpszCaption, DWORD dwStyle, HMENU hMenu, const RECT rc, DWORD dwExStyle)
{
    return create(lpszCaption, dwStyle, hMenu, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, dwExStyle);
}

HWND CNppWnd::create(LPCTSTR lpszCaption, DWORD dwStyle, HMENU hMenu, int x, int y, int cx, int cy, DWORD dwExStyle)
{
	if( !registerWndClass() )
		return NULL;
	m_hSelf = ::CreateWindowEx(dwExStyle, getWndClassName(), lpszCaption, dwStyle, x, y, cx, cy, m_hParent, hMenu, m_hInst, this);
    ASSERT(m_hSelf!=NULL);
	return m_hSelf;
}
LRESULT CNppWnd::runWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//		return m_sysWndProc(hwnd, uMsg, wParam, lParam); // ³ö´í
	return ::CallWindowProc(m_sysWndProc, hwnd, uMsg, wParam, lParam);
}
void CNppWnd::init(HINSTANCE hInst, HWND parent)
{
	m_hInst = hInst;
	m_hParent = parent;
}
BOOL CNppWnd::showWnd(int nCmdShow )
{
	return ::ShowWindow(m_hSelf, nCmdShow);
}
void CNppWnd::display(bool toShow) const 
{
	::ShowWindow(m_hSelf, toShow?SW_SHOW:SW_HIDE);
}
DWORD CNppWnd::setWndStyle(DWORD nStyle)
{
	return (DWORD)::SetWindowLongPtr(m_hSelf, GWL_STYLE, nStyle);
}
DWORD CNppWnd::getWndStyle() const
{
	return (DWORD)::GetWindowLongPtr(m_hSelf, GWL_STYLE);
}
DWORD CNppWnd::setWndExStyle(DWORD dwExStyle)
{
	return (DWORD)::SetWindowLongPtr(m_hSelf, GWL_EXSTYLE, dwExStyle);
}
DWORD CNppWnd::getWndExStyle() const
{
	return (DWORD)::GetWindowLongPtr(m_hSelf, GWL_EXSTYLE);
}
BOOL CNppWnd::setWndText(LPCTSTR lpszText)
{
	if( !lpszText )
		return FALSE;
	return ::SetWindowText(m_hSelf, lpszText);
}
int  CNppWnd::getWndText(LPTSTR OUT lpString, int IN nMaxCount)
{
	return ::GetWindowText(m_hSelf, lpString, nMaxCount);
}
void CNppWnd::reSizeTo(RECT & rc) // should NEVER be const !!!
{ 
	//::MoveWindow(m_hSelf, rc.left, rc.top, rc.right, rc.bottom, TRUE);
	// it should be :
	::MoveWindow(m_hSelf, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
	redraw();
}
void CNppWnd::redraw() const 
{
	::InvalidateRect(m_hSelf, NULL, TRUE);
	::UpdateWindow(m_hSelf);
}
void CNppWnd::getClientRect(RECT & rc) const 
{
	::GetClientRect(m_hSelf, &rc);
}
void CNppWnd::getWndRect(RECT & rc) const 
{
	::GetWindowRect(m_hSelf, &rc);
}
int CNppWnd::getWidth() const 
{
	RECT rc;
	::GetClientRect(m_hSelf, &rc);
	return (rc.right - rc.left);
}
int CNppWnd::getHeight() const 
{
	RECT rc;
	::GetClientRect(m_hSelf, &rc);
	return (rc.bottom - rc.top);
}
BOOL CNppWnd::isVisible() const 
{
	if (!m_hSelf)
		return FALSE;
	else
		return ::IsWindowVisible(m_hSelf);
}
BOOL CNppWnd::isWnd()const
{
	return ::IsWindow(m_hSelf);
}
BOOL CNppWnd::isEnabled()const
{
	return ::IsWindowEnabled(m_hSelf);
}
BOOL CNppWnd::isFocus()const
{
	return (::GetFocus() == m_hSelf);
}
BOOL CNppWnd::isControl()const
{
	return FALSE;
}
HWND CNppWnd::getHSelf() const 
{
	return m_hSelf;
}
void CNppWnd::setHSelf(HWND hSelf)
{
	m_hSelf = hSelf;
}
void CNppWnd::getFocus() const 
{
	::SetFocus(m_hSelf);
}
HINSTANCE CNppWnd::getHInst() const
{
	return m_hInst;
}
HWND CNppWnd::getParent()const
{
	return m_hParent;
}
LRESULT CNppWnd::sendMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ASSERT(::IsWindow(m_hSelf));
    return ::SendMessage(m_hSelf, uMsg, wParam, lParam);
}
LRESULT CNppWnd::postMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ASSERT(::IsWindow(m_hSelf));
    return ::PostMessage(m_hSelf, uMsg, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////////
// 
// CNppCtrlWnd
//
UINT CNppCtrlWnd::m_nCtrlCount = 0;

CNppCtrlWnd::CNppCtrlWnd(): m_bIsCreated(false), m_iCtrlID(0)
{
	++m_nCtrlCount;
}
CNppCtrlWnd::~CNppCtrlWnd()
{
	ASSERT(m_nCtrlCount > 0);
	--m_nCtrlCount;
}
UINT CNppCtrlWnd::getCtrlCount()
{
	return m_nCtrlCount;
}
void CNppCtrlWnd::init(HINSTANCE hInst, HWND hParent, UINT iCtrlIDs)
{
	CNppWnd::init(hInst, hParent);
	m_iCtrlID = iCtrlIDs;
}
HWND CNppCtrlWnd::create(DWORD dwStyle, DWORD dwExStyle, LPCTSTR lpszCaption)
{
	HWND hWnd = ::GetDlgItem(getParent(), (int)m_iCtrlID);
	if(hWnd)
	{
//		setHSelf(hWnd);
//		CNppWnd::setWndProc();
		m_hSelf = hWnd;
		if(lpszCaption )
		{
			setWndText(lpszCaption);
		}
		if( dwStyle > 0)
		{
			setWndStyle(getWndStyle() | dwStyle);
		}
		if(dwExStyle > 0)
		{
			setWndExStyle(getWndExStyle() | dwExStyle);
		}
	}
	else
	{
		create(lpszCaption, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, dwExStyle);
	}
	
    return m_hSelf;
}

HWND CNppCtrlWnd::create(LPCTSTR lpszCaption, DWORD dwStyle, int x, int y, int cx, int cy, DWORD dwExStyle)
{
	m_bIsCreated = true;
	dwStyle |= (WS_CHILD | WS_VISIBLE | WS_BORDER);
	return CNppWnd::create(lpszCaption, dwStyle, (HMENU)m_iCtrlID, x, y, cx, cy, dwExStyle);
}
LRESULT CNppCtrlWnd::runCtrlProc(UINT uMsg, WPARAM wParam, LPARAM lParam, bool & bDone)
{
	bDone = false;
	return 0;
}
LRESULT CNppCtrlWnd::runWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool bDone = false;
	
	LRESULT lres = runCtrlProc(uMsg, wParam, lParam, bDone);
	if( bDone )
		return lres;
	return CNppWnd::runWndProc(hwnd, uMsg, wParam, lParam);
}
BOOL CNppCtrlWnd::isControl()const
{
	return TRUE;
}
void CNppCtrlWnd::destroy()
{

}
UINT CNppCtrlWnd::getCtrlID()const
{
	return m_iCtrlID;
}
void CNppCtrlWnd::setCtrlID(UINT iCtrlID)
{
	m_iCtrlID = iCtrlID;
}
bool CNppCtrlWnd::isCreated()const
{
	return m_bIsCreated;
}
////////////////////////////////////////////////////////////////////////////////
// 
// CNppDlg
//
CNppDlg::CNppDlg(): m_bIsModel(false), m_iDlgID(0)/*, m_sysDlgProc((DLGPROC)::DefDlgProc)*/
{

}
CNppDlg::~CNppDlg()
{
	destroy();
}
void CNppDlg::destroy()
{
	if(m_hSelf)
	{
		::DestroyWindow(m_hSelf);
		m_hSelf = NULL;
	}
}
LPCTSTR CNppDlg::getWndClassName()const
{
	return _T("MyDialog");
}
INT_PTR CALLBACK CNppDlg::DlgProcWrap(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
	{
		return true;
	}
	case WM_CLOSE:  
        DestroyWindow(hDlg);  
        return TRUE;  
   
    case WM_DESTROY:  
        PostQuitMessage(0);  
        return TRUE;
	default:
		break;
	}
	return FALSE;
	/*
	CNppDlg *pSelfDlg = NULL;
	if( WM_INITDIALOG == uMsg && lParam != NULL)
	{
		pSelfDlg = reinterpret_cast<CNppDlg*>(lParam);
		pSelfDlg->m_hSelf = hDlg;
		::SetWindowLong(hDlg, GWL_USERDATA, (long)lParam);
        pSelfDlg->runDlgProc(hDlg, uMsg, wParam, lParam);
		dbg_log(_T("done"));
		return TRUE;
	}
	else
	{
		pSelfDlg = reinterpret_cast<CNppDlg *>(::GetWindowLong(hDlg, GWL_USERDATA));
		if( pSelfDlg )
		{
			return pSelfDlg->runDlgProc(hDlg, uMsg, wParam, lParam);
		}
		else
			return ::DefDlgProc(hDlg, uMsg, wParam, lParam);
	}
	*/
}
BOOL CNppDlg::runDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return FALSE;//::DefDlgProc(hDlg, uMsg, wParam, lParam);//::CallWindowProc(m_sysDlgProc, hDlg, uMsg, wParam, lParam);
}
void CNppDlg::init(HINSTANCE hInst, HWND hParent)
{
	CNppWnd::init(hInst, hParent);
	
}
HWND CNppDlg::create(UINT iDlgID, bool bmakeRTL)
{
	m_iDlgID = iDlgID;
	if( !bmakeRTL )
	{
		m_hSelf = ::CreateDialogParam(m_hInst,  MAKEINTRESOURCE(iDlgID), m_hParent, (DLGPROC)DlgProcWrap, (LPARAM)this);
		if( !m_hSelf )
		{
			create(_T("CreateDialogParam failed"));
		}
		display(true);//::ShowWindow(m_hSelf, SW_SHOW);
	}
	else
	{
		DLGTEMPLATE *pMyDlgTemplate = NULL;
		HGLOBAL hMyDlgTemplate = makeRTLResource(iDlgID, &pMyDlgTemplate);
		m_hSelf = ::CreateDialogIndirectParam(m_hInst, pMyDlgTemplate, m_hParent, (DLGPROC)DlgProcWrap, (LPARAM)this);
		::GlobalFree(hMyDlgTemplate);
	}
	
	return m_hSelf;
}
HWND CNppDlg::create(LPCTSTR lpszCaption, DWORD dwStyle, int x, int y, int cx, int cy, DWORD dwExStyle)
{
	struct DlgTemplate
	{
		// DLGTEMPLATE struct
		DLGTEMPLATE dlgTmp;
		// other
		WORD   menu;      // menu resource
		WORD   wndClass;  // window class
		WCHAR  wszTitle;  // title string of the dialog box
		short  fontSize;  //font size
		WCHAR  wszFont;   // L"MS Sans Serif"
	};
	// create dialog template
	DlgTemplate dlgTemp = {0};
	dlgTemp.dlgTmp.style           = dwStyle;
	dlgTemp.dlgTmp.dwExtendedStyle = dwExStyle;
	dlgTemp.dlgTmp.cdit            = 0;
	dlgTemp.dlgTmp.x               = x;
	dlgTemp.dlgTmp.y               = y;
	dlgTemp.dlgTmp.cx              = cx;
	dlgTemp.dlgTmp.cy              = cy;
	dlgTemp.menu                   = 0x0000; // no menu
	dlgTemp.wndClass               = 0;      // default wndclass
	dlgTemp.wszTitle               = L'\0';
	dbg_log(_T("m_hSelf = 0x%08X"), m_hSelf);
	m_hSelf= ::CreateDialogIndirectParam(m_hInst, (LPCDLGTEMPLATE)&dlgTemp, m_hParent, DlgProcWrap, (LPARAM)this);
	ASSERT(m_hSelf != NULL);
	CNppWnd::setWndText(lpszCaption);
	display();

	return m_hSelf;
}

HGLOBAL CNppDlg::makeRTLResource(int dialogID, DLGTEMPLATE **ppMyDlgTemplate)
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
