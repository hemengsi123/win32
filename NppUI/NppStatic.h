#ifndef NPPSTATIC_H
#define NPPSTATIC_H

class CNppStatic: public CNppCtrlWnd
{
public:
	CNppStatic();
	virtual ~CNppStatic();
	virtual LPCTSTR getWndClassName()const;
	HWND create(LPCTSTR lpszText = NULL, DWORD dwStyle = 0, DWORD dwExStyle = 0);
	HWND create(LPCTSTR lpszText, DWORD dwStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, DWORD dwExStyle = 0);
};


#endif
