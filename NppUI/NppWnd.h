
#ifndef WINDOW_CONTROL_H
#define WINDOW_CONTROL_H

#include <windows.h>

class CNppWnd
{
protected:
	static LRESULT CALLBACK WndProcWrap(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	// 替换系统默认窗口进程( 通过资源创建control )
	WNDPROC setWndProc(WNDPROC userWndProc = WndProcWrap);
	virtual LPCTSTR getWindowClassName()const = 0;
	virtual void destroy() = 0;
public:
	CNppWnd();
	virtual ~CNppWnd();
	virtual LRESULT runWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void init(HINSTANCE hInst, HWND parent);
	HWND create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu = NULL);
	HWND create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);
	bool registerWindowClass();
	BOOL showWnd(int nCmdShow );
	void display(bool toShow = true) const {
		::ShowWindow(m_hSelf, toShow?SW_SHOW:SW_HIDE);
	}
	
	DWORD setWndStyle(DWORD nStyle)
	{
		return (DWORD)::SetWindowLongPtr(m_hSelf, GWL_STYLE, nStyle);
	}
	DWORD getWndStyle() const
	{
		return (DWORD)::GetWindowLongPtr(m_hSelf, GWL_STYLE);
	}
	virtual void reSizeTo(RECT & rc) // should NEVER be const !!!
	{ 
		//::MoveWindow(m_hSelf, rc.left, rc.top, rc.right, rc.bottom, TRUE);
		// it should be :
		::MoveWindow(m_hSelf, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
		redraw();
	};

	virtual void redraw() const {
		::InvalidateRect(m_hSelf, NULL, TRUE);
		::UpdateWindow(m_hSelf);
	};
	
    virtual void getClientRect(RECT & rc) const {
		::GetClientRect(m_hSelf, &rc);
	};

    virtual void getWindowRect(RECT & rc) const {
		::GetWindowRect(m_hSelf, &rc);
	};

	virtual int getWidth() const {
		RECT rc;
		::GetClientRect(m_hSelf, &rc);
		return (rc.right - rc.left);
	};

	virtual int getHeight() const {
		RECT rc;
		::GetClientRect(m_hSelf, &rc);
		return (rc.bottom - rc.top);
	};
	
    virtual bool isVisible() const {
		if (!m_hSelf)
			return false;
		else
			return (::IsWindowVisible(m_hSelf)?true:false);
	};

	HWND getHSelf() const {
		return m_hSelf;
	};

	void getFocus() const {
		::SetFocus(m_hSelf);
	};

    HINSTANCE getHInst() const {
		return m_hInst;
	};
protected:
	HINSTANCE m_hInst;
	HWND m_hParent;
	HWND m_hSelf;
	WNDPROC m_sysWndProc; // default WndProc
};

#endif //WINDOW_CONTROL_H

