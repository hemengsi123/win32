
#include "NppLib.h"
#include "NppWnd.h"

CNppWnd::CNppWnd(): m_hInst(NULL), m_hParent(NULL), m_hSelf(NULL), \
					m_sysWndProc(::DefWindowProc)
{
	
}
CNppWnd::~CNppWnd()
{
	// 不要在构造和析构函数中调用virtual 函数，可能发生运行时错误:r6025 pure virtual function call
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
//	if the wndClassName had exist return true, or to register it
	WNDCLASSEX wndex;
	wndex.cbSize = sizeof(WNDCLASSEX);
	if(::GetClassInfoEx(m_hInst, getWndClassName(), &wndex) )
		return TRUE;
	
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
    return (CS_VREDRAW | CS_HREDRAW);
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
//		return m_sysWndProc(hwnd, uMsg, wParam, lParam); // 出错
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
void CNppWnd::gotoCenter(HWND hParent)
{
    RECT rc;
	if( hParent == NULL )
	{
		if( m_hParent == NULL)
			hParent = ::GetDesktopWindow();
		else
			hParent = m_hParent;
	}
    ::GetClientRect(hParent, &rc);
	
    POINT center;
    center.x = rc.left + (rc.right - rc.left)/2;
    center.y = rc.top + (rc.bottom - rc.top)/2;
    ::ClientToScreen(hParent, &center);
	RECT rcSelf;
	::GetClientRect(m_hSelf, &rcSelf);
	int x = center.x - (rcSelf.right - rcSelf.left)/2;
	int y = center.y - (rcSelf.bottom - rcSelf.top)/2;

	::SetWindowPos(m_hSelf, HWND_TOP, x, y, rcSelf.right - rcSelf.left, rcSelf.bottom - rcSelf.top, SWP_SHOWWINDOW);
}
void CNppWnd::alignTo(HWND hTag, AlignDirect alignFlags1, AlignDirect alignFlags2, int cx, int cy)
{
	RECT rcTag, rcSelf;
	POINT ptTo = {0};

	::GetWindowRect(m_hSelf, &rcSelf);
	if(hTag == m_hParent)
	{
		::GetClientRect(hTag, &rcTag);
		if( alignFlags1 & LEFTALIGN )
		{
			ptTo.x  = rcTag.left + cx;
			if(alignFlags2 & BOTTOMALIGN )
			{
				ptTo.y  = rcTag.bottom;
				ptTo.y -= (rcSelf.bottom -rcSelf.top  + cy);
			}
			else
			{
				ptTo.y  = rcTag.top + cy;
			}
		}
		else if(alignFlags1 & RIGHTALIGN)
		{
			ptTo.x  = rcTag.right;
			ptTo.x -= (rcSelf.right - rcSelf.left + cx);
			if(alignFlags2 & BOTTOMALIGN)
			{
				ptTo.y  = rcTag.bottom;
				ptTo.y -= (rcSelf.bottom -rcSelf.top  + cy);
			}
			else
			{
				ptTo.y  = rcTag.top + cy;
			}
		}
		else if(alignFlags1 & TOPALIGN)
		{
			ptTo.y = rcTag.top + cy;
			if(alignFlags2 & RIGHTALIGN)
			{
				ptTo.x  = rcTag.right;
				ptTo.x -= (rcSelf.right - rcSelf.left + cx);
			}
			else
			{
				ptTo.x = rcTag.left + cx;
			}
		}
		else // bottom
		{
			ptTo.y  = rcTag.bottom;
			ptTo.y -= (rcSelf.bottom -rcSelf.top  + cy);
			if(alignFlags2 & RIGHTALIGN)
			{
				ptTo.x  = rcTag.right;
				ptTo.x -= (rcSelf.right - rcSelf.left + cx);
			}
			else
			{
				ptTo.x = rcTag.left + cx;
			}
		}

	}
	else
	{
		::GetWindowRect(hTag, &rcTag);
		if( alignFlags1 & LEFTALIGN )
		{
			ptTo.x  = rcTag.left;
			ptTo.x -= (rcSelf.right - rcSelf.left + cx);
			if(alignFlags2 & TOPALIGN)
			{
				ptTo.y = rcTag.top + cy;
			}
			else
			{
				ptTo.y  = rcTag.bottom;
				ptTo.y -= (rcSelf.bottom -rcSelf.top  + cy);
			}
		}
		else if(alignFlags1 & RIGHTALIGN)
		{
			//ptTo.x = rcTag.left;
			//ptTo.x += (rcTag.right - rcTag.left + cx);
			ptTo.x = rcTag.right + cx;
			if(alignFlags2 & TOPALIGN)
			{
				ptTo.y = rcTag.top + cy;
			}
			else
			{
				ptTo.y = rcTag.bottom;
				ptTo.y -= (rcSelf.bottom -rcSelf.top + cy);
			}
		}
		else if(alignFlags1 & TOPALIGN)
		{
			ptTo.y  = rcTag.top;
			ptTo.y -= (rcSelf.bottom - rcSelf.top + cy);
			if(alignFlags2 & RIGHTALIGN)
			{
				ptTo.x = rcTag.right;
				ptTo.x -= (rcSelf.right - rcSelf.left + cx);
			}
			else
			{
				ptTo.x = rcTag.left + cx;
			}
		}
		else // BOTTOMALIGN
		{
			ptTo.y  = rcTag.bottom;
			ptTo.y += cy;
			if(alignFlags2 & RIGHTALIGN)
			{
				ptTo.x = rcTag.right;
				ptTo.x -= (rcSelf.right - rcSelf.left + cx);
			}
			else
			{
				ptTo.x = rcTag.left + cx;
			}
		}
		// 将屏幕坐标转成客户端坐标
		::ScreenToClient(m_hSelf, &ptTo);
	}
	::MoveWindow(m_hSelf, ptTo.x, ptTo.y, rcSelf.right - rcSelf.left, rcSelf.bottom - rcSelf.top, TRUE);
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
	dwStyle |= (WS_CHILD | WS_VISIBLE);
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
	if(m_hSelf)
	{
		::DestroyWindow(m_hSelf);
		m_hSelf = NULL;
	}
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
	
}
void CNppDlg::destroy()
{
	if(m_hSelf)
	{
		::DestroyWindow(m_hSelf);
		m_hSelf = NULL;
	}
}
BOOL CNppDlg::isControl()const
{
	return FALSE;
}
LPCTSTR CNppDlg::getWndClassName()const
{
	return _T("MyDialog");
}
INT_PTR CALLBACK CNppDlg::DlgProcWrap(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CNppDlg *pSelfDlg = NULL;
	if( WM_INITDIALOG == uMsg && lParam != NULL)
	{
		pSelfDlg = reinterpret_cast<CNppDlg*>(lParam);
		//pSelfDlg->m_hSelf = hDlg;
		::SetWindowLongPtr(hDlg, GWL_USERDATA, reinterpret_cast<LPARAM>(pSelfDlg));
        pSelfDlg->runDlgProc(hDlg, uMsg, wParam, lParam);
		return TRUE;
	}
	else
	{
		pSelfDlg = reinterpret_cast<CNppDlg *>(::GetWindowLongPtr(hDlg, GWL_USERDATA));
		if( pSelfDlg != NULL )
		{
			if( pSelfDlg->m_hSelf == hDlg )
			{
				return pSelfDlg->runDlgProc(hDlg, uMsg, wParam, lParam);
			}
		}
	}
	return 	FALSE;
}
BOOL CNppDlg::runDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_CLOSE:
		{
			destroy();
			return TRUE;
		}
		case WM_DESTROY:
		{
			//::PostQuitMessage(0);
			return TRUE;
		}
		default:
			break;
	}
	return FALSE;
	// 不可以用默认DefDlgProc 进程，对话框进程会陷入死循环(why?)
	//return ::DefDlgProc(hDlg, uMsg, wParam, lParam);

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
		dbg_log(_T("m_hSelf = %08X"), m_hSelf);
		if( !m_hSelf )
		{
			create(_T("CreateDialogParam failed"));
		}
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
	dwStyle |= (WS_VISIBLE|WS_SYSMENU|WS_CAPTION|WS_BORDER);
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

//	void *ptmp = new (std::nothrow)char[512];
//	memset(ptmp, 0, 512);
//	// create dialog template
//	DlgTemplate& dlgTemp = *(DlgTemplate*)ptmp;//{0};
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
//#ifdef _UNICODE 
//	memcpy(&dlgTemp.wszTitle[0], lpszCaption, (_tcslen(lpszCaption)+1)*2);
//#else
//	::MultiByteToWideChar(CP_ACP, 0, lpszCaption, -1, &dlgTemp.wszTitle[0], 512-sizeof(DlgTemplate));
//#endif
	m_hSelf= ::CreateDialogIndirectParam(m_hInst, (LPCDLGTEMPLATE)&dlgTemp, m_hParent, DlgProcWrap, (LPARAM)this);
	ASSERT(m_hSelf != NULL);
	CNppWnd::setWndText(lpszCaption);
	display();
	//delete []ptmp;
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

UINT CNppDlg::doModal()
{
    if( !::IsWindow(m_hSelf) ) 
    {
		create(_T("haven't create Dailog before doModal"), (WS_VISIBLE|WS_SYSMENU|WS_CAPTION|WS_BORDER), 0, 0, 320, 240);
		gotoCenter();
    }
    UINT nRet = 0;
    ::ShowWindow(m_hSelf, SW_SHOWNORMAL);
    ::EnableWindow(m_hParent, FALSE);
    MSG msg = { 0 };
    while( ::IsWindow(m_hSelf) && ::GetMessage(&msg, NULL, 0, 0) ) {
        if( msg.message == WM_CLOSE && msg.hwnd == m_hSelf ) {
            nRet = msg.wParam;
            ::EnableWindow(m_hParent, TRUE);
            ::SetFocus(m_hParent);
        }
        if( !::TranslateMessage(&msg) ) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        if( msg.message == WM_QUIT ) break;
    }
    ::EnableWindow(m_hParent, TRUE);
    ::SetFocus(m_hParent);
//	::SetActiveWindow(m_hParent);
//	::SetForegroundWindow(m_hParent);
    if( msg.message == WM_QUIT ) ::PostQuitMessage(msg.wParam);

    return nRet;
}

int CNppDlg::doModal(UINT iDlgID)
{
	if(::IsWindow(m_hSelf))
	{
		::SetActiveWindow(m_hSelf);
		doModal();
		return 0;
	}
	return ::DialogBoxParam(m_hInst, MAKEINTRESOURCE(iDlgID), m_hParent, (DLGPROC)DlgProcWrap, (LPARAM)this);
}
