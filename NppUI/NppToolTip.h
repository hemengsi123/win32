#ifndef NPPTOOLTIP_H
#define NPPTOOLTIP_H

class CNppToolTip: public CNppCtrlWnd
{
public:
	CNppToolTip();
	~CNppToolTip();
	void destroy();
	LPCTSTR getWndClassName()const;
	LRESULT runCtrlProc(UINT uMsg, WPARAM wParam, LPARAM lParam, bool & bDone);
	void init(HINSTANCE hInst, HWND hParent, UINT iCtrlID = 0, LPCTSTR sCtrlName = NULL);
	HWND create(int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT);
	void show(RECT rectTitle, LPTSTR pszTitle, int iXOff, int iWidthOff);
private:
	BOOL m_bTrackMouse;
	BOOL m_bLeftBtnDown;
	TOOLINFO m_ti;
};

#endif
