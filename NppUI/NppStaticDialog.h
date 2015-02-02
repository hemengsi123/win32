
#ifndef STATIC_DIALOG_H
#define STATIC_DIALOG_H

#include "NppWnd.h"

enum PosAlign{ALIGNPOS_LEFT, ALIGNPOS_RIGHT, ALIGNPOS_TOP, ALIGNPOS_BOTTOM};

struct DLGTEMPLATEEX {
      WORD   dlgVer;
      WORD   signature;
      DWORD  helpID;
      DWORD  exStyle;
      DWORD  style; 
      WORD   cDlgItems;
      short  x;
      short  y;    
      short  cx;
      short  cy;
      // The structure has more fields but are variable length
} ;
/**/
class CNppStaticDialog : public CNppBaseWnd
{
public :
	CNppStaticDialog() : CNppBaseWnd() {};
	~CNppStaticDialog(){
		if (isCreated())
			destroy();
	};
	virtual void create(int dialogID, bool isRTL = false);
	virtual LPCTSTR getWndClassName() const {return _T("StaticDailog");};
    virtual bool isCreated() const {
		return (m_hSelf != NULL);
	};

	void goToCenter();
    void destroy() {
		// ::SendMessage(m_hParent, WM_MODELESSDIALOG, MODELESSDIALOGREMOVE, (WPARAM)m_hSelf);
		::DestroyWindow(m_hSelf);
	};

protected :
	RECT _rc;
	static BOOL CALLBACK dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL CALLBACK run_dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;

    void alignWith(HWND handle, HWND handle2Align, PosAlign pos, POINT & point);
	HGLOBAL makeRTLResource(int dialogID, DLGTEMPLATE **ppMyDlgTemplate);
};

#endif //STATIC_DIALOG_H
