
#include "NppLib.h"
#include "NppWnd.h"

CNppWnd::CNppWnd(): m_hInst(NULL), m_hParent(NULL), m_hSelf(NULL), \
					m_bIsCreated(false), m_sysWndProc(::DefWindowProc)
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
WNDPROC CNppWnd::setWndProc(WNDPROC userWndProc)
{
	::SetWindowLongPtr(m_hSelf, GWLP_USERDATA, LONG_PTR(this));
	m_sysWndProc = reinterpret_cast<WNDPROC>(::SetWindowLongPtr(m_hSelf, GWLP_WNDPROC, (LONG_PTR)userWndProc));
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
HWND CNppWnd::create(HWND hwndParent, LPCTSTR lpszCaption, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu)
{
    return create(hwndParent, lpszCaption, dwStyle, dwExStyle, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hMenu);
}

HWND CNppWnd::create(HWND hwndParent, LPCTSTR lpszCaption, DWORD dwStyle, DWORD dwExStyle, int x, int y, int cx, int cy, HMENU hMenu)
{
	if( !registerWndClass() )
		return NULL;
	m_hSelf = ::CreateWindowEx(dwExStyle, getWndClassName(), lpszCaption, dwStyle, x, y, cx, cy, hwndParent, hMenu, m_hInst, this);
    ASSERT(m_hSelf!=NULL);
    
	m_bIsCreated = true;
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
void CNppWnd::getFocus() const 
{
	::SetFocus(m_hSelf);
}
HINSTANCE CNppWnd::getHInst() const
{
	return m_hInst;
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
UINT CNppWnd::showModal()
{
/*
	ASSERT(::IsWindow(m_hSelf));
    UINT nRet = 0;
    HWND hWndParent = GetWindowOwner(m_hSelf);
    ::ShowWindow(m_hSelf, SW_SHOWNORMAL);
    ::EnableWindow(hWndParent, FALSE);
    MSG msg = { 0 };
    while( ::IsWindow(m_hSelf) && ::GetMessage(&msg, NULL, 0, 0) ) {
        if( msg.message == WM_CLOSE && msg.hwnd == m_hSelf ) {
            nRet = msg.wParam;
            ::EnableWindow(hWndParent, TRUE);
            ::SetFocus(hWndParent);
        }
       // if( !CPaintManagerUI::TranslateMessage(&msg) ) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        //}
        if( msg.message == WM_QUIT ) break;
    }
    ::EnableWindow(hWndParent, TRUE);
    ::SetFocus(hWndParent);
    if( msg.message == WM_QUIT ) ::PostQuitMessage(msg.wParam);
   
    return nRet;
     */
     return 0;
}