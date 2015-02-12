#include "NppLib.h"
#include "NppButton.h"

CNppButton::~CNppButton()
{

}
LPCTSTR CNppButton::getWndClassName()const
{
	return WC_BUTTON;
}
void CNppButton::init(HINSTANCE hInst, HWND hParent, UINT iCtrlID, LPCTSTR sCtrlName)
{
	CNppCtrlWnd::init(hInst, hParent, iCtrlID, sCtrlName);
}
HWND CNppButton::create(LPCTSTR lpszText, DWORD dwStyle, DWORD dwExStyle)
{
	dwStyle |= (WS_CHILDWINDOW | WS_TABSTOP);
	CNppCtrlWnd::create(dwStyle, dwExStyle, lpszText);
	setWndProc();
	return m_hSelf;
}
HWND CNppButton::create(LPCTSTR lpszText, int x, int y, int cx, int cy, DWORD dwStyle, DWORD dwExStyle)
{
	dwStyle |= (WS_CHILDWINDOW|WS_TABSTOP);
	CNppCtrlWnd::create(lpszText, dwStyle, x, y, cx, cy, dwExStyle);
	setWndProc();
	return m_hSelf;
}
//////////////////////////////////////////////////////////////////////////////
//
// CNppCheckbox
// 
CNppCheckbox::CNppCheckbox(): m_bIsCheck(FALSE)
{

}
CNppCheckbox::~CNppCheckbox()
{

}
HWND CNppCheckbox::create(LPCTSTR lpszText, DWORD dwStyle, DWORD dwExStyle)
{
	dwStyle |= (WS_CHILD | WS_TABSTOP | BS_CHECKBOX);
	CNppCtrlWnd::create(dwStyle, dwExStyle, lpszText);
	setWndProc();
	return m_hSelf;
}
HWND CNppCheckbox::create(LPCTSTR lpszText, int x, int y, int cx, int cy, DWORD dwStyle, DWORD dwExStyle)
{
	dwStyle |= (WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_CHECKBOX);
	CNppCtrlWnd::create(lpszText, dwStyle, x, y, cx, cy, dwExStyle);
	setWndProc();
	autoSize(lpszText, NULL, 16, 0);
	return m_hSelf;
}
LRESULT CNppCheckbox::handleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if( uMsg == WM_LBUTTONUP)
	{
		if( m_bIsCheck )
		{
			setCheck(BST_UNCHECKED);
			m_bIsCheck = FALSE;
		}
		else
		{
			setCheck(BST_CHECKED);   // BST_INDETERMINATE  BST_CHECKED
			m_bIsCheck = TRUE;
		}
	}
	return CNppCtrlWnd::handleMessage(hwnd, uMsg, wParam, lParam);
}
void CNppCheckbox::setCheck(int uState)
{
	::SendMessage(m_hSelf, BM_SETCHECK, uState, 0);
}
int CNppCheckbox::getCheck()
{
	return ::SendMessage(m_hSelf, BM_GETCHECK, 0, 0);
}
BOOL CNppCheckbox::isCheck()const
{
	return m_bIsCheck;
}
//////////////////////////////////////////////////////////////////////////////
//
// Radio button
//
HWND CNppRadioButton::create(LPCTSTR lpszText, DWORD dwStyle, DWORD dwExStyle)
{
	dwStyle |= (WS_CHILD | WS_TABSTOP | BS_RADIOBUTTON);
	CNppCtrlWnd::create(dwStyle, dwExStyle, lpszText);
	setWndProc();
	return m_hSelf;
}
HWND CNppRadioButton::create(LPCTSTR lpszText, int x, int y, int cx, int cy, DWORD dwStyle, DWORD dwExStyle)
{
	dwStyle |= (WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON);
	CNppCtrlWnd::create(lpszText, dwStyle, x, y, cx, cy, dwExStyle);
	setWndProc();
//	autoSize(lpszText, NULL, 16, 0);
	return m_hSelf;
}