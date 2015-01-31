
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
const NPP_MSGMAP_ENTRY * CNppWnd::FindMessageEntry(const NPP_MSGMAP_ENTRY * lpMsgMapEntries, const NPP_MSGPARAMS &msgParams)
{
//	if(lpMsgMapEntries == NULL)
//		return NULL;
	ASSERT(lpMsgMapEntries != NULL);
	//CNppWnd * caller = msgParams.pSender;
	UINT      ctrlID = msgParams.iCtrlID;
	UINT      uMsg   = msgParams.uMsg;
	const NPP_MSGMAP_ENTRY * lpMsgMapEntry = NULL;
	while( lpMsgMapEntries->pfnSig != PfnSig_end )
	{
		if(lpMsgMapEntries->pfnSig == PfnSig_mm )
		{
			if(uMsg == lpMsgMapEntries->uMsg && lpMsgMapEntries->pfn)
			{
				return lpMsgMapEntries;
			}
		}
		else if(lpMsgMapEntries->pfnSig == PfnSig_imm)
		{
			if(ctrlID ==lpMsgMapEntries->iCtrlID && uMsg == lpMsgMapEntries->uMsg && lpMsgMapEntries->pfn)
				return lpMsgMapEntries;
		}
		else if(lpMsgMapEntries->pfnSig == PfnSig_nmm)
		{
			if(msgParams.sCtrlName == lpMsgMapEntries->sCtrlName && uMsg == lpMsgMapEntries->uMsg && lpMsgMapEntries->pfn)
				return lpMsgMapEntries;
		}
		++lpMsgMapEntries;
	}
	
	return lpMsgMapEntry;
}
BOOL CNppWnd::loopDispath(NPP_MSGPARAMS & msgParams)
{
	const NPP_MSGMAP_ENTRY * lpMsgMapEntry = FindMessageEntry(m_messageEntries, msgParams);
	if( lpMsgMapEntry != NULL)
	{
		// @brief: this 指向控件地址，所以在函数 => lpMsgMapEntry->pfn(this, msgParams)
		// 所以在pfn中使用this指针指向的是对应控件地址
		return (this->*(lpMsgMapEntry->pfn))(msgParams);
	}
	return FALSE;
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
	m_msgParams.uMsg     = uMsg;
	m_msgParams.hwndFrom = hwnd;
	m_msgParams.iCtrlID  = 0;
	//m_msgParams.sCtrlName = _T('\0')
	m_msgParams.pSender  = this;
	m_msgParams.wParam   = wParam;
	m_msgParams.lParam   = lParam;
	m_msgParams.lResult  = 0;
	
	switch(uMsg)
	{
	case WM_COMMAND:
	{
		if( handleCommand(m_msgParams) )
			return m_msgParams.lResult;
		break;
	}
	case WM_NOTIFY:
	{
		if( handleNotify(m_msgParams) )
			return m_msgParams.lResult;
		break;
	}
	default:
		break;
	}
	return handleMessage(m_msgParams);
//	return m_sysWndProc(hwnd, uMsg, wParam, lParam); // 出错
//	return ::CallWindowProc(m_sysWndProc, hwnd, uMsg, wParam, lParam);
}
LRESULT CNppWnd::handleMessage(struct NPP_MSGPARAMS & msgParams)
{
	msgParams.lResult = 0;
	
	return ::CallWindowProc(m_sysWndProc, msgParams.hwndFrom, msgParams.uMsg, msgParams.wParam, msgParams.lParam);
}
BOOL CNppWnd::handleCommand(struct NPP_MSGPARAMS & msgParams)
{
	msgParams.lResult = FALSE;
	
	return msgParams.lResult;
}
BOOL CNppWnd::handleNotify(struct NPP_MSGPARAMS & msgParams)
{
	msgParams.lResult = FALSE;
	
	return msgParams.lResult;
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
void CNppCtrlWnd::init(HINSTANCE hInst, HWND hParent, UINT iCtrlID, LPCTSTR sCtrlName)
{
	CNppWnd::init(hInst, hParent);
	m_iCtrlID   = iCtrlID;
	m_sCtrlName = sCtrlName;
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
	m_msgParams.uMsg      = uMsg;
	m_msgParams.iCtrlID   = m_iCtrlID;
	m_msgParams.pSender   = this;
	m_msgParams.hwndFrom  = hwnd;
	m_msgParams.wParam    = wParam;
	m_msgParams.lParam    = lParam;
	m_msgParams.lResult   = 0;
	m_msgParams.sCtrlName = m_sCtrlName;
	
	return this->handleMessage(m_msgParams);
}
LRESULT CNppCtrlWnd::handleMessage(struct NPP_MSGPARAMS & msgParams)
{
	if( loopDispath(msgParams) )
	{
		return msgParams.lResult;
	}
	
	return ::CallWindowProc(m_sysWndProc, msgParams.hwndFrom, msgParams.uMsg, msgParams.wParam, msgParams.lParam);
}
BOOL CNppCtrlWnd::handleCommand(struct NPP_MSGPARAMS & msgParams)
{
	msgParams.lResult = FALSE;
	
	return msgParams.lResult;
}
BOOL CNppCtrlWnd::handleNotify(struct NPP_MSGPARAMS & msgParams)
{
	msgParams.lResult = FALSE;
	
	return msgParams.lResult;
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
LPCTSTR CNppCtrlWnd::getCtrlName()const
{
	return m_sCtrlName.getData();
}
void CNppCtrlWnd::setCtrlName(LPCTSTR lpszName)
{
	m_sCtrlName = lpszName;
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
	m_msgParams.uMsg     = uMsg;
	m_msgParams.hwndFrom = hDlg;
	m_msgParams.iCtrlID  = m_iDlgID;
	//m_msgParams.sCtrlName = _T('\0')
	m_msgParams.pSender  = this;
	m_msgParams.wParam   = wParam;
	m_msgParams.lParam   = lParam;
	m_msgParams.lResult  = 0;
	
	switch(uMsg)
	{
	case WM_COMMAND:
	{
		//if( handleCommand(m_msgParams) )
		return handleCommand(m_msgParams);
		break;
	}
	case WM_NOTIFY:
	{
		//if( handleNotify(m_msgParams) )
		return handleNotify(m_msgParams);
		break;
	}
	default:
		break;
	}
	
	return handleMessage(m_msgParams);
	// 不可以用默认DefDlgProc 进程，对话框进程会陷入死循环(why?)
	//return ::DefDlgProc(hDlg, uMsg, wParam, lParam);

}
LRESULT CNppDlg::handleMessage(struct NPP_MSGPARAMS & msgParams)
{
	msgParams.lResult = TRUE;
	switch(msgParams.uMsg)
	{
		case WM_CLOSE:
		{
			destroy();
			break;
		}
		case WM_DESTROY:
		{
			//::PostQuitMessage(0);
			break;
		}
		default:
			msgParams.lResult = FALSE;
			break;
	}
	return msgParams.lResult;
}
BOOL CNppDlg::handleCommand(struct NPP_MSGPARAMS & msgParams)
{
	msgParams.lResult = FALSE;
	
	return msgParams.lResult;
}
BOOL CNppDlg::handleNotify(struct NPP_MSGPARAMS & msgParams)
{
	msgParams.lResult = FALSE;
	
	return msgParams.lResult;
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
