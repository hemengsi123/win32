#include "NppLib.h"
#include "NppButton.h"

CNppButton::~CNppButton()
{

}
LPCTSTR CNppButton::getWndClassName()const
{
	return WC_BUTTON;
}
HWND CNppButton::create(LPCTSTR lpszText, DWORD dwStyle, DWORD dwExStyle)
{
	dwStyle |= (WS_CHILDWINDOW | WS_TABSTOP);
	return CNppCtrlWnd::create(dwStyle, dwExStyle, lpszText);
}
HWND CNppButton::create(LPCTSTR lpszText, DWORD dwStyle, int x, int y, int cx, int cy, DWORD dwExStyle)
{
	dwStyle |= (WS_CHILDWINDOW|WS_TABSTOP);
	return CNppCtrlWnd::create(lpszText, dwStyle, x, y, cx, cy, dwExStyle);
}