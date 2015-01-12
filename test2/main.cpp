
#include "main.h"
#include "resource.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPreInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	HWND hWndDlg = ::CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_EXPLORER_DLG), NULL, (DLGPROC)DialogProc, NULL)
	if( hWndDlg == NULL)
	{
		dbg_log(_T("creat dialog failed"));
		return -1;
	}
	
	RECT rtDlg;  
    ::GetWindowRect(hWndDlg, &rtDlg);
	
	int nScreenX = ::GetSystemMetrics(SM_CXSCREEN);  
    int nScreenY = ::GetSystemMetrics(SM_CYSCREEN);  
    
    ::SetWindowPos(hWndDlg,   
                HWND_TOP,   
                nScreenX / 2 - rtDlg.right / 2,  
                nScreenY / 2 - rtDlg.bottom / 2,  
                0,  
                0,  
                SWP_NOSIZE | SWP_SHOWWINDOW); //SWP_NOSIZE)  
  
    while(GetMessage(&msg, NULL, NULL, NULL))  
    {  
        if(msg.message == WM_KEYDOWN)  
        {  
            SendMessage(hWndDlg, msg.message, msg.wParam, msg.lParam);  
        }  
        else if(!IsDialogMessage(hWndDlg, &msg))// 如果消息没有被处理, 返回值为0  
        {  
            TranslateMessage(&msg);  
            DispatchMessage(&msg);  
        }  
    }  
    
    return msg.wParam; 
}

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
	{
		return true;
	}
	default:
		break;
	}
	return false;
}