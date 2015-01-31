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
HWND CNppButton::create(LPCTSTR lpszText, DWORD dwStyle, int x, int y, int cx, int cy, DWORD dwExStyle)
{
	dwStyle |= (WS_CHILDWINDOW|WS_TABSTOP);
	CNppCtrlWnd::create(lpszText, dwStyle, x, y, cx, cy, dwExStyle);
	setWndProc();
	return m_hSelf;
}
LRESULT CNppButton::handleMessage(struct NPP_MSGPARAMS & msgParams)
{
	return CNppCtrlWnd::handleMessage(msgParams);
}