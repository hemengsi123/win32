#ifndef NPPBUTTON_H
#define NPPBUTTON_H

class CNppButton: public CNppCtrlWnd
{
public:
	virtual ~CNppButton();
	virtual LPCTSTR getWndClassName()const;
	void init(HINSTANCE hInst, HWND hParent, UINT iCtrlID, LPCTSTR sCtrlName = NULL);
	HWND create(LPCTSTR lpszText = NULL, DWORD dwStyle = 0, DWORD dwExStyle = 0);
	HWND create(LPCTSTR lpszText, DWORD dwStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, DWORD dwExStyle = 0);
private:

};

#endif
