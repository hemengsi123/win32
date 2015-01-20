
#ifndef WINDOW_CONTROL_H
#define WINDOW_CONTROL_H

///////////////////////////////////////////////////////////////////
//
// CNppWnd 窗口基类
// 
class CNppWnd
{
protected:
	static LRESULT CALLBACK WndProcWrap(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	// 替换系统默认窗口进程( 通过资源创建control )
	WNDPROC setWndProc(WNDPROC userWndProc = WndProcWrap);
	virtual LPCTSTR getWndClassName()const = 0;
	virtual void destroy() = 0;
public:
	CNppWnd();
	virtual ~CNppWnd();
	HWND create(HWND hwndParent, LPCTSTR lpszCaption, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu = NULL);
	HWND create(HWND hwndParent, LPCTSTR lpszCaption, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);
	bool registerWndClass();
	virtual LRESULT runWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void init(HINSTANCE hInst, HWND parent);
	virtual void reSizeTo(RECT & rc); // should NEVER be const !!!
	virtual void redraw() const;
    virtual void getClientRect(RECT & rc)const;
    virtual void getWndRect(RECT & rc) const;
	virtual int  getWidth() const;
	virtual int  getHeight() const;
    virtual UINT getClassStyle() const;
    virtual BOOL isControl()const;
    BOOL         isVisible() const;
    BOOL         isFocus()const;
    BOOL         isWnd()const;
    BOOL         isEnabled()const;
	HWND         getHSelf() const;
	void         getFocus() const;
    HINSTANCE    getHInst()const;
    BOOL         showWnd(int nCmdShow = SW_SHOW );
	void         display(bool toShow = true) const;
	DWORD        setWndStyle(DWORD nStyle);
	DWORD        getWndStyle() const;
	LRESULT sendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
	LRESULT postMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
	UINT    showModal();
protected:
	HINSTANCE m_hInst;
	HWND m_hParent;
	HWND m_hSelf;
	WNDPROC m_sysWndProc; // default WndProc
};

///////////////////////////////////////////////////////////////////
//
// CNppCtrlWnd 控件基类
// 
class CNppCtrlWnd: public NppWnd
{
public:
	CNppCtrlWnd();
	~CNppCtrlWnd();
	virtual BOOL isControl()const;
private:
	static UINT m_nCtrlCout= 0; // 控件总数量
	UINT m_iCtrlID;
	// 为true时，是通过显示调用CreateWindow 创建;
	// 为false 时表示通过资源 MAKEINTRESOURCE(id) 创建
	bool m_bIsCreated; 
};
#endif //WINDOW_CONTROL_H

