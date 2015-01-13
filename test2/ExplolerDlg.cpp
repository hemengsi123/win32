
#include "StdAfx.h"

void CExplorerDlg::create(HINSTANCE hInst, int dialogId)
{
	Window::init(hInst, NULL);
	StaticDialog::create(dialogId, false);
}
void CExplorerDlg::initCtrl()
{
	m_hTreeCtrl = ::GetDlgItem(_hSelf, IDC_TREE_FOLDER);
	m_listCtrlAll  = ::GetDlgItem(_hSelf, IDC_LIST_ALL);
	m_listCtrlFiles = ::GetDlgItem(_hSelf, IDC_LIST_FILES);
	m_filterCtrl    = ::GetDlgItem(_hSelf, IDC_CBO_FILTER);
	m_splitterCtrl  = ::GetDlgItem(_hSelf, IDC_BUTTON_SPLITTER);
	
	m_comBoFilter.init(m_filterCtrl);
	m_comBoFilter.addText(_T("*.*"));
	m_comBoFilter.addText(_T("*.txt"));
	m_comBoFilter.setText(_T("*.*"), 1);
	
	m_treeView.init(m_hTreeCtrl);
}
BOOL CALLBACK CExplorerDlg::run_dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_INITDIALOG:
		{
			dbg_log(_T("dialog init"));
			initCtrl();
			return TRUE;
		}
		case WM_COMMAND:
		{
			if(LOWORD(wParam) == IDC_CBO_FILTER)
			{
				dbg_log(_T("H: %04x"), HIWORD(wParam));
			}
			return TRUE;
		}
		case WM_NOTIFY:
		{
			LPNMHDR		nmhdr = (LPNMHDR)lParam;
			if (nmhdr->hwndFrom == m_hTreeCtrl)
			{
				dbg_log(_T("notify"));
			}
			
			return TRUE;
		}
		case EXM_CHANGECOMBO:
		{
			TCHAR	temp[MAX_PATH];
			m_comBoFilter.getSelText(temp);
			dbg_log(_T("combo input: %s"), temp);
			return TRUE;
		}
		// 关闭窗口
		case WM_CLOSE:
		{
			destroy();
			dbg_log(_T("wm_close"));
			return TRUE;
		}
		// 关闭应用程序
		case WM_DESTROY:
		{
			::PostQuitMessage(0);
			dbg_log(_T("wm_destroy"));
			return TRUE;
		}
		default:
			break;
	}
	return false;
}