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
/*��ѡ����õ�������ʽ��BS_CHECKBOX��BS_AUTOCHECKBOX��
  ��ʹ��BS_CHECKBOXʱ������Ҫ�Լ���ÿؼ�����BM_SETCHECK��Ϣ���趨��ѡ��ǡ�
  wParam������1ʱ�趨��ѡ��ǣ���0ʱ�����ѡ��ǡ�
  ͨ����ÿؼ�����BM_GETCHECK��Ϣ�������Եõ��ø�ѡ���Ŀǰ״̬��
  �ڴ������Կؼ���WM_COMMAND��Ϣʱ�������������µ�ָ������תX���*/
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
