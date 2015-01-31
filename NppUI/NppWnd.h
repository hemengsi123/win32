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
class CNppWnd;
typedef struct NPP_MSGPARAMS
{
	UINT       uMsg;
	CNppWnd *  pSender;
	UINT       iCtrlID;
	CNppString sCtrlName;
	HWND       hWnd;
	LRESULT    lResult;
	union
	{
		struct
		{
			WORD low;		//wParam
			WORD high;		//wParam
		}shortVal;			//wParam
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
}NPP_MSGPARAMS, *NPP_LPMSGPARAMS;
//////////////////////////////////////////////////////////////////////////////
//
// MESSAGE_MAP
//
enum NppPfnSig
{
	PfnSig_end,
	PfnSig_mm,  // NPP_ON_MSGMAP(msgtype, memberpFn)
	PfnSig_imm, // NPP_ON_MSGMAP_ID(ctrlID, msgtype, memberpFn) 
	PfnSig_nmm, // NPP_ON_MSGMAP_NAME
};
typedef BOOL (CNppWnd::*NPP_MSGPROC)(NPP_MSGPARAMS & msgParams); // 
struct NPP_MSGMAP_ENTRY
{
	UINT        uMsg;
	UINT        iCtrlID; // �ؼ�id
	CNppString  sCtrlName;
	UINT        pfnSig;
	NPP_MSGPROC pfn;
};
/*************** how to use **************************************************
	NPP_BEGIN_MESSAGE_MAP(CExplorerDlg)
	NPP_ON_MSGMAP(WM_KEYUP, OnClick)
	NPP_ON_MSGMAP_ID(IDC_CBO_FILTER, WM_LBUTTONDOWN, OnBtnAdd)
	NPP_ON_MSGMAP_NAME(_T("btnAddAll"), WM_LBUTTONDOWN, OnBtnAddAll)
	NPP_END_MESSAGE_MAP()
*******************************************************************************/
#define NPP_BEGIN_MESSAGE_MAP(theClass)                      \
const NPP_MSGMAP_ENTRY theClass::m_messageEntries[] =        \
{                                                            \

#define NPP_END_MESSAGE_MAP()                                \
	{0, 0, _T('\0'), PfnSig_end, (NPP_MSGPROC)0 }            \
};

// @brief: ��Ϣӳ����������ؼ��е���Ϣ���ڳ�Ա�����в�Ҫʹ��thisָ��
// (this ָ�������ָ��ؼ����󣬶�����ָ�����Ա����memberpFn �ı�����
// ��Ϊ(this->*(lpMsgMapEntry->pfn))(msgParams)��this��ָ��ؼ������ַ)
#define NPP_ON_MSGMAP(msgtype, memberpFn)                        \
	{msgtype, 0, _T('\0'), PfnSig_mm, (NPP_MSGPROC)&memberpFn},  \


#define NPP_ON_MSGMAP_ID(ctrlID, msgtype, memberpFn)                   \
	{msgtype, ctrlID, _T('\0'), PfnSig_imm, (NPP_MSGPROC)&memberpFn},  \

#define NPP_ON_MSGMAP_NAME(ctrlName, msgtype, memberpFn)          \
	{msgtype, 0, ctrlName, PfnSig_nmm, (NPP_MSGPROC)&memberpFn},  \
///////////////////////////////////////////////////////////////////
//
// CNppWnd ���ڻ���
// 
class CNppWnd
{
protected:
	static LRESULT CALLBACK WndProcWrap(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static const NPP_MSGMAP_ENTRY * FindMessageEntry(const NPP_MSGMAP_ENTRY * msg_entries, const NPP_MSGPARAMS &msgParams);
	// @brief: ����ͨ����Ϣӳ��ĺ���
	BOOL loopDispath(NPP_MSGPARAMS & msgParams);
	// �滻ϵͳĬ�ϴ��ڽ���( ͨ����Դ����control )
	WNDPROC setWndProc(HWND hWnd = NULL, WNDPROC userWndProc = WndProcWrap);
	virtual LPCTSTR getWndClassName()const = 0;
	// ��Ҫ�ڹ�������������е���virtual ���������ܷ�������ʱ����:r6025 pure virtual function call
	virtual void destroy() = 0;
public:
	CNppWnd();
	virtual ~CNppWnd();
	virtual HWND create(/*HWND hwndParent, */LPCTSTR lpszCaption, DWORD dwStyle, HMENU hMenu, const RECT rc, DWORD dwExStyle = 0);
	virtual HWND create(/*HWND hwndParent, */LPCTSTR lpszCaption, DWORD dwStyle, HMENU hMenu = NULL, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, DWORD dwExStyle = 0);
	bool    registerWndClass();
	virtual LRESULT runWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT handleMessage( struct NPP_MSGPARAMS & msgParams);
	virtual BOOL    handleCommand( struct NPP_MSGPARAMS & msgParams);
	virtual BOOL    handleNotify( struct NPP_MSGPARAMS & msgParams);
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
	NPP_MSGPARAMS m_msgParams;
	//
	static const NPP_MSGMAP_ENTRY m_messageEntries[];
};

///////////////////////////////////////////////////////////////////
//
// CNppCtrlWnd �ؼ�����
// 
class CNppCtrlWnd: public CNppWnd
{
public:
	CNppCtrlWnd();
	~CNppCtrlWnd();
	static  UINT getCtrlCount();
	virtual void init(HINSTANCE hInst, HWND hParent, UINT iCtrlID, LPCTSTR sCtrlName = NULL);
	virtual LRESULT runCtrlProc(UINT uMsg, WPARAM wParam, LPARAM lParam, bool & bDone);
	virtual BOOL isControl()const;
	virtual void destroy();
	/*@retrn: if id invalid return 0, or nozero*/
	UINT    getCtrlID()const;
	void    setCtrlID(UINT iCtrlID);
	LPCTSTR getCtrlName()const;
	void    setCtrlName(LPCTSTR strName);
	/*@retrn: if return true ��ͨ����ʾ����CreateWindow ����, or ��ͨ��MAKEINTRESOURCE(id) ����*/
	bool    isCreated()const;
	virtual LRESULT handleMessage(struct NPP_MSGPARAMS & msgParams);
	
protected:
	virtual HWND create(DWORD dwStyle = 0, DWORD dwExStyle = 0, LPCTSTR lpszCaption = NULL);
	virtual HWND create(LPCTSTR lpszCaption, DWORD dwStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, DWORD dwExStyle = 0);
	virtual LRESULT runWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL    handleCommand( struct NPP_MSGPARAMS & msgParams);
	virtual BOOL    handleNotify( struct NPP_MSGPARAMS & msgParams);
	
private:
	static UINT m_nCtrlCount; // �ؼ�������
	UINT m_iCtrlID;
	CNppString m_sCtrlName;  //  �ؼ�����
	bool m_bIsCreated;
};

///////////////////////////////////////////////////////////////////
//
// CNppDlg �Ի������
//

class CNppDlg: public CNppWnd
{
public:
	CNppDlg();
	virtual ~CNppDlg();
	virtual void destroy();
	virtual BOOL isControl()const;
	virtual LPCTSTR getWndClassName()const;
	virtual void init(HINSTANCE hInst, HWND hParent);
	/*@param: bMakeRTL ͨ����Դģ�崴���Ի���*/
	virtual HWND create(UINT iDlgID, bool bMakeRTL = false);
	virtual HWND create(LPCTSTR lpszCaption, DWORD dwStyle = 0, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, DWORD dwExStyle = 0);
	virtual BOOL runDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT handleMessage( struct NPP_MSGPARAMS & msgParams);
	virtual BOOL    handleCommand( struct NPP_MSGPARAMS & msgParams);
	virtual BOOL    handleNotify( struct NPP_MSGPARAMS & msgParams);
	UINT    doModal();
	int     doModal(UINT iDlgID);
protected:
	static INT_PTR CALLBACK DlgProcWrap(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HGLOBAL makeRTLResource(int dialogID, DLGTEMPLATE **ppMyDlgTemplate);
private:
	bool m_bIsModel;
	UINT m_iDlgID;
	//DLGPROC m_sysDlgProc;
};
                                                       \




#endif

