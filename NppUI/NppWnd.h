#ifndef WINDOW_BASE_H
#define WINDOW_BASE_H

#include "NppString.h"

enum AlignDirect
{
	DEFALIGN    = 0x00,
	LEFTALIGN   = 0x01,
	RIGHTALIGN  = 0x02,
	TOPALIGN    = 0x04,
	BOTTOMALIGN = 0x08,
};
/////////////////////////////////////////////////////////////
//
// WndProc ���ڽ��̲���
// 
class CNppBaseWnd;
typedef struct NppMsgParams
{
	UINT       uMsg;
	CNppBaseWnd *  pSender;
	UINT       iCtrlID;
	CNppString sCtrlName;
	HWND       hwndFrom;
	LRESULT    lResult;
	union
	{
		struct
		{
			WORD low;		//wParam
			WORD high;		//wParam
		}shortVal;			//wParam
		struct
		{
			WORD iID;       // �ؼ�id LOWORD(wParam)
			WORD uCode;     // ֪ͨ�� HIWORD(wParam)
		}cmdCtrl;
		HDC hdc;			//wParam
		HRGN hrgn;			//WM_NCPAINT,etc.
		WPARAM wParam;
	};
	union
	{
		LPCREATESTRUCT lpcreate;		    //WM_CREATE(lParam)
		LPNMHDR lpnmhdr;				    //WM_NOTIFY(lParam)
		//LPROPVNOTIFY lpropvnm;				//for OptionView NOTIFY(lParam)
		LPWINDOWPOS lpwindowpos;			//WM_WINDOWPOSXXX(lParam)
		LPDRAWITEMSTRUCT lpdrawitem;		//WM_DRAWITEM(lParam)
		LPMEASUREITEMSTRUCT lpmeasureitem;	//WM_MEASUREITEM(lParam)
		LPNCCALCSIZE_PARAMS lpncsp;			//WM_NCCALCSIZE(lParam)
		LPDELETEITEMSTRUCT lpdeleteitem;	//WM_DELETEITEM(lParam)
		LPNMTREEVIEW lpnmtreeview;			//WM_NOTIFY(lParam-treeview)
		LPNMLISTVIEW lpnmlistview;			//WM_NOTIFY(lParam-listview)
		POINTS ptMouse;						//�����Ϣ(lParam)		
		HANDLE handle;						//(lParam)
		DWORD  dwVal;						//(lParam)
		int    intVal;                      //(lParam)
		LPARAM lParam;                      //(lParam)
	};
}NppMsgParams;
//////////////////////////////////////////////////////////////////////////////
//
// MESSAGE_MAP
//
enum NppPfnSig
{
	pFnSig_end,
	pFnSig_ctrl,  // NPP_ON_MSGMAP(msgtype, memberpFn)
	pFnSig_ctrl_id, // NPP_ON_MSGMAP_ID(ctrlID, msgtype, memberpFn) 
	pFnSig_ctrl_name, // NPP_ON_MSGMAP_NAME
	pFnSig_msg,
	pFnSig_msg_cmd,
	pFnSig_msg_notify,
};
// NppStdMsgPfn
typedef BOOL (CNppBaseWnd::*NppStdMsgPfn)(NppMsgParams & msg); // 

struct NppMsgMapEntry
{
	UINT        uMsg;
	UINT        iCtrlID; // �ؼ�id
	CNppString  sCtrlName;
	UINT        pfnSig;
	NppStdMsgPfn pfn;
};
union NppMsgMapFunctions
{
	NppStdMsgPfn stdMsgPfn;
	LRESULT (CNppBaseWnd::* pfn_msg_lwl)(WPARAM wParam, LPARAM lParam);
	BOOL (CNppBaseWnd::* pfn_msg_biml)(UINT iCtrlID, UINT uCode, LPARAM lParam);
};
/*************** how to use **************************************************
	NPP_BEGIN_MESSAGE_MAP(CExplorerDlg)
	// �������пؼ������� WM_KEYUP ��Ϣ
	NPP_ON_CTRL_MSGMAP(WM_KEYUP, OnClick)
	// ����ؼ�idΪIDC_CBO_FILTER�ؼ������� WM_LBUTTONDOWN��Ϣ
	NPP_ON_CTRL_MSGMAP_ID(IDC_CBO_FILTER, WM_LBUTTONDOWN, OnBtnAdd)
	// ����ؼ���Ϊ"btnAddAll"�ؼ������� ������Ϣ
	NPP_ON_CTRL_MSGMAP_NAME(_T("btnAddAll"), -1, OnBtnAddAll)
	// ����WM_COMAND��Ϣ�пؼ�idΪ IDC_BTN_DEL�ؼ���BN_CLICKED ֪ͨ��
	NPP_ON_MSGMAP_CMD(IDC_BTN_DEL, BN_CLICKED, OnBtnAddAll) 
	// ����WM_COMAND��Ϣ�пؼ�idΪIDC_BTN_DEL�ؼ�������֪ͨ��
	NPP_ON_MSGMAP_CMD(IDC_BTN_DEL, -1, OnBtnAddAll) 
	// ����WM_COMAND��Ϣ�����пؼ��� BN_CLICKED ��Ϣ
	NPP_ON_MSGMAP_CMD(-1, BN_CLICKED, OnBtnAddAll) 
	
	NPP_END_MESSAGE_MAP()
*******************************************************************************/
#define NPP_BEGIN_MESSAGE_MAP(theClass)                      \
const NppMsgMapEntry theClass::m_messageEntries[] =        \
{                                                            \

#define NPP_END_MESSAGE_MAP()                                \
	{0, -1, _T('\0'), pFnSig_end, (NppStdMsgPfn)0 }            \
};
/************************ �ؼ���Ϣ ****************************/
#define NPP_ON_CTRL_MSGMAP(msgCode, memberpFn)                        \
	{msgCode, -1, _T('\0'), pFnSig_ctrl, (NppStdMsgPfn)&memberpFn},     \

// if msgCode == -1 ����������Ϣ��, or ����ָ����Ϣ��
#define NPP_ON_CTRL_MSGMAP_ID(ctrlID, msgCode, memberpFn)                   \
	{msgCode, ctrlID, _T('\0'), pFnSig_ctrl_id, (NppStdMsgPfn)&memberpFn},   \

// if msgCode == -1 ����������Ϣ��, or ����ָ����Ϣ��
#define NPP_ON_CTRL_MSGMAP_NAME(ctrlName, msgCode, memberpFn)               \
	{msgCode, -1, _T(ctrlName), pFnSig_ctrl_name, (NppStdMsgPfn)&memberpFn},  \

/********************* ������Ϣ ��Ի�����Ϣ **********************/
#define NPP_ON_MSGMAP(msgCode, memberpFn)                          \
	{msgCode, -1, _T('\0'), pFnSig_msg, (NppStdMsgPfn)&memberpFn},   \

// if NotifyCode == -1 ��������֪ͨ��, or ����ָ��֪ͨ��
#define NPP_ON_MSGMAP_CMD(ctrlID, notifyCode, memberpFn)                       \
	{notifyCode, ctrlID, _T('\0'), pFnSig_msg_cmd, (NppStdMsgPfn)&memberpFn},   \

// if NotifyCode == -1 ��������֪ͨ��, or ����ָ��֪ͨ��
#define NPP_ON_MSGMAP_NOTIFY(ctrlID, notifyCode, memberpFn)                       \
	{notifyCode, ctrlID, _T('\0'), pFnSig_msg_notify, (NppStdMsgPfn)&memberpFn},   \

///////////////////////////////////////////////////////////////////
//
// CNppBaseWnd ���ڻ���
// 
class CNppBaseWnd
{
protected:
	virtual LPCTSTR getWndClassName()const = 0;
	// ��Ҫ�ڹ�������������е���virtual ���������ܷ�������ʱ����:r6025 pure virtual function call
	virtual BOOL isControl()const = 0;
	virtual void OnClose() = 0;        // WM_CLOSE
	virtual void OnDestroy() = 0;      // WM_DESTROY
	virtual LRESULT runWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
	virtual LRESULT handleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
	static LRESULT CALLBACK WndProcWrap(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL    registerWndClass();
	/************************ ��Ϣӳ��֧�� **************************/
    const NppMsgMapEntry * findMessageEntry(const NppMsgMapEntry * msg_entries, const NppMsgParams &msgParams);
	// @brief: ����ͨ����Ϣӳ��ĺ���
	BOOL dispathMessage(NppMsgParams & msgParams);
	/************************ end **********************************/
	// �滻ϵͳĬ�ϴ��ڽ���( ͨ����Դ����control )
	WNDPROC setWndProc(HWND hWnd = NULL, WNDPROC userWndProc = WndProcWrap);
public:
	CNppBaseWnd();
	virtual ~CNppBaseWnd();
	virtual HWND create(/*HWND hwndParent, */LPCTSTR lpszCaption, DWORD dwStyle, HMENU hMenu, const RECT rc, DWORD dwExStyle = 0);
	virtual HWND create(/*HWND hwndParent, */LPCTSTR lpszCaption, DWORD dwStyle, HMENU hMenu = NULL, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, DWORD dwExStyle = 0);
	virtual void init(HINSTANCE hInst, HWND parent);
	virtual void reSizeTo(RECT & rc); // should NEVER be const !!!
	virtual void redraw() const;
    virtual UINT getClassStyle() const;
    virtual void loopMessage();
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
	void         gotoCenter(HWND hParent = NULL);
	/* @brief: ��hTag ���ڶ���
	 * @usage: 1)alignTo(hParent, TOPALIGN, LEFTALIGN, 20, -20); cx �� cy ����Ϊ��
	 * 2) alignTo(hControl, TOPALIGN, LEFTALIGN, 20, 20);
	*/
	void         alignTo(HWND hTag, AlignDirect alignFlags1, AlignDirect alignFlags2 = DEFALIGN, int cx = 0, int cy = 0);
protected:
	HINSTANCE m_hInst;
	HWND      m_hParent;
	HWND      m_hSelf;
	WNDPROC   m_sysWndProc; // default WndProc
	NppMsgParams m_msgParams;
	//
	static const NppMsgMapEntry m_messageEntries[];
};
///////////////////////////////////////////////////////////////////
//
// CNppWnd �ؼ�����
// 
class CNppWnd: public CNppBaseWnd
{
public:
	virtual BOOL isControl()const;
	virtual LRESULT runWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT handleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	// If an application processes this message, it should return zero.
	virtual BOOL OnCommand(UINT iCtrlID, UINT uMsg, HWND hwndFrom);
	// The return value is ignored except for notification messages that specify otherwise. 
	virtual BOOL OnNotify(UINT iCtrlID, UINT uMsg, LPNMHDR lpNmhdr);
	virtual void OnCreate();       // WM_CREATE
	virtual void OnClose();        // WM_CLOSE
	virtual void OnDestroy();      // WM_DESTROY
private:
	
};
///////////////////////////////////////////////////////////////////
//
// CNppCtrlWnd �ؼ�����
// 
class CNppCtrlWnd: public CNppBaseWnd
{
public:
	CNppCtrlWnd();
	~CNppCtrlWnd();
	static  UINT getCtrlCount();
	virtual void init(HINSTANCE hInst, HWND hParent, UINT iCtrlID, LPCTSTR sCtrlName = NULL);
	virtual LRESULT handleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL isControl()const;
	/*@retrn: if id invalid return 0, or nozero*/
	UINT    getCtrlID()const;
	void    setCtrlID(UINT iCtrlID);
	LPCTSTR getCtrlName()const;
	void    setCtrlName(LPCTSTR strName);
protected:
	virtual HWND create(DWORD dwStyle = 0, DWORD dwExStyle = 0, LPCTSTR lpszCaption = NULL);
	virtual HWND create(LPCTSTR lpszCaption, DWORD dwStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, DWORD dwExStyle = 0);
	virtual LRESULT runWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnClose();        // WM_CLOSE
	virtual void OnDestroy();      // WM_DESTROY
private:
	static UINT m_nCtrlCount; // �ؼ�������
	UINT m_iCtrlID;
	CNppString m_sCtrlName;  //  �ؼ�����
};

///////////////////////////////////////////////////////////////////
//
// CNppDlg �Ի������
//

class CNppDlg: public CNppBaseWnd
{
public:
	CNppDlg();
	virtual ~CNppDlg();
	virtual BOOL isControl()const;
	virtual LPCTSTR getWndClassName()const;
	virtual void init(HINSTANCE hInst, HWND hParent);
	/*@param: bMakeRTL ͨ����Դģ�崴���Ի���*/
	virtual HWND create(UINT iDlgID, bool bMakeRTL = false);
	virtual HWND create(LPCTSTR lpszCaption, DWORD dwStyle = 0, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, DWORD dwExStyle = 0);
	virtual BOOL runDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT handleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	// If an application processes this message, it should return zero.
	virtual BOOL OnCommand(UINT iCtrlID, UINT uCode, HWND hwndFrom);
	// The return value is ignored except for notification messages that specify otherwise. 
	virtual BOOL OnNotify(UINT iCtrlID, UINT uCode, LPNMHDR lpNmhdr);
	virtual void OnCreate();       // WM_INITDIALOG
	virtual void OnClose();        // WM_CLOSE
	virtual void OnDestroy();      // WM_DESTROY
	UINT    doModal();
	int     doModal(UINT iDlgID);
protected:
	static INT_PTR CALLBACK DlgProcWrap(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HGLOBAL makeRTLResource(int dialogID, DLGTEMPLATE **ppMyDlgTemplate);
private:
	bool m_bIsModel;
	UINT m_iDlgID;
	virtual LRESULT runWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
                                                       \




#endif

