#include "NppLib.h"
#include "NppStatic.h"

CNppStatic::CNppStatic()
{

}
CNppStatic::~CNppStatic()
{

}
LPCTSTR CNppStatic::getWndClassName()const
{
	return WC_STATIC;
}
HWND CNppStatic::create(LPCTSTR lpszText, DWORD dwStyle, DWORD dwExStyle)
{
	dwStyle |= (WS_CHILD | WS_TABSTOP);
	CNppCtrlWnd::create(dwStyle, dwExStyle, lpszText);
	setWndProc();
	
	return m_hSelf;
}
HWND CNppStatic::create(LPCTSTR lpszText, DWORD dwStyle, int x, int y, int cx, int cy, DWORD dwExStyle)
{
	dwStyle |= (WS_CHILD | WS_TABSTOP | WS_VISIBLE);
	CNppCtrlWnd::create(lpszText, dwStyle, x, y, cx, cy, dwExStyle);
	setWndProc();
	
	return m_hSelf;
}