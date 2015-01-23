
#ifndef WINDOW_BASE_H
#define WINDOW_BASE_H

///////////////////////////////////////////////////////////////////
//
// CNppWnd 窗口基类
// 
class CNppWnd
{
protected:
	static LRESULT CALLBACK WndProcWrap(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	// 替换系统默认窗口进程( 通过资源创建control )
	WNDPROC setWndProc(HWND hWnd = NULL, WNDPROC userWndProc = WndProcWrap);
	virtual LPCTSTR getWndClassName()const = 0;
	// 不要在构造和析构函数中调用virtual 函数，可能发生运行时错误:r6025 pure virtual function call
	virtual void destroy() = 0;
public:
	CNppWnd();
	virtual ~CNppWnd();
	virtual HWND create(/*HWND hwndParent, */LPCTSTR lpszCaption, DWORD dwStyle, HMENU hMenu, const RECT rc, DWORD dwExStyle = 0);
	virtual HWND create(/*HWND hwndParent, */LPCTSTR lpszCaption, DWORD dwStyle, HMENU hMenu = NULL, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, DWORD dwExStyle = 0);
	bool registerWndClass();
	virtual LRESULT runWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void init(HINSTANCE hInst, HWND parent);
	virtual void reSizeTo(RECT & rc); // should NEVER be const !!!
	virtual void redraw() const;
    virtual UINT getClassStyle() const;
    virtual BOOL isControl()const;
	void         getClientRect(RECT & rc)const;
    void         getWndRect(RECT & rc) const;
	int          getWidth() const;
	int          getHeight() const;
    BOOL         isVisible() const;
    BOOL         isFocus()const;
    BOOL         isWnd()const;
    BOOL         isEnabled()const;
	HWND         getHSelf() const;
	void         setHSelf(HWND hSelf);
	void         getFocus() const;
    HINSTANCE    getHInst()const;
	HWND         getParent()const;
    BOOL         showWnd(int nCmdShow = SW_SHOW );
	void         display(bool toShow = true) const;
	/*if successful return old style, or return 0*/
	DWORD        setWndStyle(DWORD nStyle);
	DWORD        getWndStyle() const;
	DWORD        setWndExStyle(DWORD dwExStyle);
	DWORD        getWndExStyle() const;
	BOOL         setWndText(LPCTSTR lpszText);
	/*if successful return string len, or return zero*/
	int          getWndText(LPTSTR OUT lpString, int IN nMaxCount);
	LRESULT      sendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
	LRESULT      postMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
protected:
	HINSTANCE m_hInst;
	HWND      m_hParent;
	HWND      m_hSelf;
	WNDPROC   m_sysWndProc; // default WndProc
};

///////////////////////////////////////////////////////////////////
//
// CNppCtrlWnd 控件基类
// 
class CNppCtrlWnd: public CNppWnd
{
public:
	CNppCtrlWnd();
	~CNppCtrlWnd();
	static UINT getCtrlCount();
	virtual void init(HINSTANCE hInst, HWND hParent, UINT iCtrlID);
	virtual HWND create(DWORD dwStyle = 0, DWORD dwExStyle = 0, LPCTSTR lpszCaption = NULL);
	virtual HWND create(LPCTSTR lpszCaption, DWORD dwStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, DWORD dwExStyle = 0);
	virtual LRESULT runCtrlProc(UINT uMsg, WPARAM wParam, LPARAM lParam, bool & bDone);
	virtual BOOL isControl()const;
	virtual void destroy();
	/*@retrn: if id invalid return 0, or nozero*/
	UINT         getCtrlID()const;
	void         setCtrlID(UINT iCtrlID);
	/*@retrn: if return true 是通过显示调用CreateWindow 创建, or 是通过MAKEINTRESOURCE(id) 创建*/
	bool         isCreated()const;
protected:
	virtual LRESULT runWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	static UINT m_nCtrlCount; // 控件总数量
	UINT m_iCtrlID;
	bool m_bIsCreated; 
};

///////////////////////////////////////////////////////////////////
//
// CNppDlg 对话框基类
//

class CNppDlg: public CNppWnd
{
public:
	CNppDlg();
	virtual ~CNppDlg();
	virtual void destroy();
	virtual LPCTSTR getWndClassName()const;
	virtual void init(HINSTANCE hInst, HWND hParent);
	/*@param: bMakeRTL 通过资源模板创建对话框*/
	virtual HWND create(UINT iDlgID, bool bMakeRTL = false);
	virtual HWND create(LPCTSTR lpszCaption, DWORD dwStyle = (WS_VISIBLE|WS_SYSMENU|WS_CAPTION|WS_BORDER), int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, DWORD dwExStyle = 0);
	virtual BOOL runDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	static INT_PTR CALLBACK DlgProcWrap(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HGLOBAL makeRTLResource(int dialogID, DLGTEMPLATE **ppMyDlgTemplate);
private:
	bool m_bIsModel;
	UINT m_iDlgID;
	//DLGPROC m_sysDlgProc;
};



#endif

