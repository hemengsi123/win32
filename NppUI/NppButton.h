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
/*复选框最常用的两种样式是BS_CHECKBOX和BS_AUTOCHECKBOX。
  在使用BS_CHECKBOX时，您需要自己向该控件发送BM_SETCHECK消息来设定勾选标记。
  wParam参数设1时设定勾选标记，设0时清除勾选标记。
  通过向该控件发送BM_GETCHECK消息，您可以得到该复选框的目前状态。
  在处理来自控件的WM_COMMAND消息时，您可以用如下的指令来翻转X标记*/
class CNppCheckbox: public CNppButton
{
public:
	CNppCheckbox();
	~CNppCheckbox();
	HWND create(LPCTSTR lpszText = NULL, DWORD dwStyle = 0, DWORD dwExStyle = 0);
	HWND create(LPCTSTR lpszText, DWORD dwStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, DWORD dwExStyle = 0);
	virtual LRESULT handleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	// @param: bSel => BST_CHECKED, BST_UNCHECKED, BST_INDETERMINATE
	void setCheck(int uState);
	int  getCheck();
	BOOL isCheck()const;
private:
	BOOL m_bIsCheck;
};

class CNppRadioButton: public CNppCheckbox
{
public:
	HWND create(LPCTSTR lpszText = NULL, DWORD dwStyle = 0, DWORD dwExStyle = 0);
	HWND create(LPCTSTR lpszText, DWORD dwStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, DWORD dwExStyle = 0);
};
#endif
