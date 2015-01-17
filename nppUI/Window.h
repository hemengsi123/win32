
#ifndef WINDOW_CONTROL_H
#define WINDOW_CONTROL_H

//#include <stdlib.h>

// #include <windows.h>

// CNppWindow NppWindow CNppWnd
class Window
{
protected:
		static LRESULT CALLBACK WndProcWrap(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			return (((Window *)(::GetWindowLongPtr(hwnd, GWL_USERDATA)))->runProc(hwnd, uMsg, wParam, lParam));
		}
public:
	Window(): _hInst(NULL), _hParent(NULL), _hSelf(NULL), _sysWndProc(NULL){};
	virtual ~Window() {};
	virtual WNDPROC setWndProc(WNDPROC userWndProc = WndProcWrap)
	{
		::SetWindowLongPtr(_hSelf, GWLP_USERDATA, LONG_PTR(this));
		_sysWndProc = reinterpret_cast<WNDPROC>(::SetWindowLongPtr(_hSelf, GWLP_WNDPROC, (LONG_PTR)userWndProc));
		return _sysWndProc;
	}
	virtual LRESULT runProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return _sysWndProc(hwnd, uMsg, wParam, lParam);
	}
	virtual void init(HINSTANCE hInst, HWND parent)
	{
		_hInst = hInst;
		_hParent = parent;
	}

	virtual void destroy() = 0;

	virtual void display(bool toShow = true) const {
		::ShowWindow(_hSelf, toShow?SW_SHOW:SW_HIDE);
	};
	
	virtual void reSizeTo(RECT & rc) // should NEVER be const !!!
	{ 
		//::MoveWindow(_hSelf, rc.left, rc.top, rc.right, rc.bottom, TRUE);
		// it should be :
		::MoveWindow(_hSelf, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
		redraw();
	};

	virtual void redraw() const {
		::InvalidateRect(_hSelf, NULL, TRUE);
		::UpdateWindow(_hSelf);
	};
	
    virtual void getClientRect(RECT & rc) const {
		::GetClientRect(_hSelf, &rc);
	};

    virtual void getWindowRect(RECT & rc) const {
		::GetWindowRect(_hSelf, &rc);
	};

	virtual int getWidth() const {
		RECT rc;
		::GetClientRect(_hSelf, &rc);
		return (rc.right - rc.left);
	};

	virtual int getHeight() const {
		RECT rc;
		::GetClientRect(_hSelf, &rc);
		return (rc.bottom - rc.top);
	};
	
    virtual bool isVisible() const {
		if (!_hSelf)
			return false;
		else
			return (::IsWindowVisible(_hSelf)?true:false);
	};

	HWND getHSelf() const {
		return _hSelf;
	};

	void getFocus() const {
		::SetFocus(_hSelf);
	};

    HINSTANCE getHinst() const {
		return _hInst;
	};
protected:
	HINSTANCE _hInst;
	HWND _hParent;
	HWND _hSelf;
	WNDPROC _sysWndProc; // default WndProc
};

#endif //WINDOW_CONTROL_H

