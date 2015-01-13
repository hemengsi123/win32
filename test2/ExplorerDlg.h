#ifndef EXPLOERDLG_ONE_H
#define EXPLOERDLG_ONE_H

#include "../nppUI/StaticDialog.h"
#include "../nppUI/ComboOrgi.h"
#include "../nppUI/TreeHelperClass.h"

class CExplorerDlg: public StaticDialog
{
public:
	void create(HINSTANCE hInst, int dialogId);
	BOOL CALLBACK run_dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void initCtrl();
protected:
	HWND m_hTreeCtrl;
	HWND m_listCtrlAll;
	HWND m_listCtrlFiles;
	HWND m_filterCtrl;
	HWND m_splitterCtrl;
	//
	ComboOrgi m_comBoFilter;
	TreeHelper m_treeView;
};

#endif
