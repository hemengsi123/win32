
#include "StdAfx.h"

#include <stdexcept>

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPreInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	CExplorerDlg expDlg;
	try
	{
		expDlg.create(hInstance, IDD_EXPLORER_DLG);
		
		expDlg.display();
		expDlg.goToCenter();
		MSG msg;
		while(::GetMessage(&msg, NULL, NULL, NULL))  
		{  
			// if(msg.message == WM_KEYDOWN)  
			// {  
				// dbg_log("key down");
				// ::SendMessage(expDlg.getHSelf(), msg.message, msg.wParam, msg.lParam);  
			// }  
			// else if(!IsDialogMessage(expDlg.getHSelf(), &msg))// 如果消息没有被处理, 返回值为0  
			// {  
				TranslateMessage(&msg);  
				DispatchMessage(&msg);  
			// }
		}
	}
	catch( std::exception excep)
	{
		dbg_log(_T("error: %s"), excep.what());
	}
	return 0;
}
