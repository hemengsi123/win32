
#include <windows.h>
#include <stdio.h>
// #incldue <wchar.h>
#include <tchar.h>
#include <Shlobj.h>

#include "../common/wcommon.h"
#include "main.h"
#include "resource.h"

// window shell 
void test_SHellFunc()
{
	//
	 IShellFolder   *psfDesktop = NULL;
	 
	 // Get the desktop folder.
     SHGetDesktopFolder(&psfDesktop);  
     LPENUMIDLIST   pEnum;//其实就是EnumList *  
     if(SUCCEEDED(psfDesktop->EnumObjects(NULL,SHCONTF_FOLDERS | SHCONTF_NONFOLDERS,&pEnum)))  
     {  
         LPITEMIDLIST   pidlTemp;  
         DWORD          dwFetched = 1; 
		 int i = 0;
         while(S_OK == (pEnum->Next(1, &pidlTemp, &dwFetched)) && dwFetched)  
         {  
             TCHAR szPath[255];  
             SHGetPathFromIDList(pidlTemp, szPath);                             
            // MessageBox(NULL,(LPWSTR)szPath,TEXT("Message"),MB_OK);  
			dbg_log(_T("%02d  %s"), ++i, szPath);
			//
			// PWSTR lpszName  = NULL;
			// SHGetNameFromIDList(pidlTemp, SIGDN_NORMALDISPLAY, &lpszName);
			// dbg_log(_T("%03d  %x"), i, lpszName);
			// char tmpstr[255] = {0};
			// ::WideCharToMultiByte(CP_UTF8, 0, lpszName, lengthof(lpszName), tmpstr, 255, NULL, NULL);
			
         }  
     }
	 TCHAR szTmpDir[MAX_PATH] = _T("D:\\Program Files (x86)\\Notepad++\\");
	TCHAR szShortName[MAX_PATH] = {0};
	::GetShortPathName(szTmpDir, szShortName, MAX_PATH);
	 dbg_log(_T("shortName = %s"), szShortName);
	 LPITEMIDLIST    pidlTempDir = NULL;
	 DWORD             dwEaten;
	 DWORD             dwAttributes;
	 IShellFolder      *psfTempDir = NULL;
	 IMalloc           *pMalloc = NULL;
	 HRESULT           hr;
	 hr = SHGetMalloc(&pMalloc);
	if(FAILED(hr))
	{
		return;
	}
	  // Get the PIDL for the directory
#ifdef UNICODE
	psfDesktop->ParseDisplayName(NULL, NULL, szTmpDir, &dwEaten, &pidlTempDir, &dwAttributes);
#else
	OLECHAR           szOle[MAX_PATH];
	::MultiByteToWideChar(CP_UTF8, 0, szTmpDir, -1, szOle, MAX_PATH);
	psfDesktop->ParseDisplayName(NULL, NULL, szOle, &dwEaten, &pidlTempDir, &dwAttributes);
#endif
	// Get the IShellFolder for the TEMP directory.
	psfDesktop->BindToObject(pidlTempDir, NULL, IID_IShellFolder, (LPVOID*)&psfTempDir);
	if(SUCCEEDED(psfTempDir->EnumObjects(NULL,SHCONTF_FOLDERS | SHCONTF_NONFOLDERS,&pEnum)))  
     {  
         LPITEMIDLIST   pidlTemp;  
         DWORD          dwFetched = 1; 
		 int i = 0;
         while(S_OK == (pEnum->Next(1, &pidlTemp, &dwFetched)) && dwFetched)  
         {  
             TCHAR szPath[255];  
             SHGetPathFromIDList(pidlTemp, szPath);                               
            // MessageBox(NULL,(LPWSTR)szPath,TEXT("Message"),MB_OK);  
			dbg_log(_T("%04d  %s"), ++i, szPath);
         }  
     }
	 //
	ITEMIDLIST *pidl;
	TCHAR		configPath[MAX_PATH];
	SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &pidl);
	SHGetPathFromIDList(pidl, configPath);
	dbg_log(_T("CSIDL_APPDATA = %s"), configPath);
	 // LPITEMIDLIST lpItemIDList = NULL;
	 // PWSTR lpszName  = NULL;
	 // for(int i = 0; i < 0x0800; i++)
	 // {
		 // SHGetSpecialFolderLocation(NULL, i, &lpItemIDList);
		 // SHGetNameFromIDList(lpItemIDList, SIGDN_NORMALDISPLAY, &lpszName);
		 // dbg_log(_T("\n0x%0x\t%s"), i, lpszName);
		 // if (i > 10)
			// break;
	 // }
	 //
	 if(pidlTempDir)  
		 pMalloc->Free(pidlTempDir);
	if(psfDesktop) 
		psfDesktop->Release();
	pMalloc->Release();
}
// 列出 本机所有逻辑盘符
void test_LogicDrivers()
{
	TCHAR			drivePathName[]	= _T(" :\\\0\0");
	DWORD			serialNr		= 0;
	DWORD			space			= 0;
	DWORD			flags			= 0;
	LPTSTR			volumeName		= (LPTSTR)new TCHAR[MAX_PATH];
	LPTSTR			TEMP			= (LPTSTR)new TCHAR[MAX_PATH];
	BOOL			isValidDrive	= FALSE;
	
	DWORD driveList = ::GetLogicalDrives();
	for (int i = 0; i < 32; i++)
	{
		drivePathName[0] = 'A' + i;
		if (0x01 & (driveList >> i))
		{
			isValidDrive = GetVolumeInformation(drivePathName, TEMP, MAX_PATH, &serialNr, &space, &flags, NULL, 0);
			if (isValidDrive == TRUE)
			{
				wsprintf(volumeName, _T("%c: [%s]"), 'A' + i, TEMP);
			}
			else
			{
				wsprintf(volumeName, _T("%c:"), 'A' + i);
			}
			dbg_log(_T("%s"), volumeName);
		}
	}
}
int test_creatWndExample(HINSTANCE hInstance)
{
	// handle_file(_T("E:\\Simon\\projects\\flair61\\zipsig.exe"));
	TCHAR clsName[] = _T("test1");
	TCHAR wndName[] = _T("Hello, win32");
	HWND hWnd = NULL;
	
	// 只允许运行一个实例
	hWnd = ::FindWindow(NULL, wndName);
	if( hWnd != NULL)
	{
		ShowWindow(hWnd,SW_RESTORE | SW_SHOWNORMAL);  
		SetForegroundWindow(hWnd);  
		return 0;
	}
	WNDCLASS wndClss = {};
	wndClss.style         = CS_HREDRAW | CS_VREDRAW;
	wndClss.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wndClss.lpfnWndProc   = WindowProc;
	wndClss.lpszClassName = clsName;
	wndClss.hInstance     = hInstance;
	wndClss.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
	
	::RegisterClass(&wndClss);
	hWnd = ::CreateWindow(
		clsName,
		wndName,
		WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION ,// | WS_SIZEBOX ,
		258,
		258,
		520,
		100,
		NULL,
		::LoadMenu(hInstance, MAKEINTRESOURCE(IDR_M10_MENU)),  // 加载菜单
		hInstance,
		NULL);
	if( hWnd == NULL)
	{
		dbg_log(_T("CreateWindow failed: %x"), GetLastError());
		return -1;
	}
	::ShowWindow(hWnd, SW_SHOW);
	::UpdateWindow(hWnd);
	HACCEL hAccel = ::LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_M10_ACCELERATORS));
	MSG msg;
	while(::GetMessage(&msg, NULL, 0, 0))
	{
		if( ::TranslateAccelerator(hWnd, hAccel, &msg) == 0)
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	return 0;
}
int test_creatDlgExample(HINSTANCE hInstance)
{
	HWND hWndDlg = ::CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_EXPLORER_DLG), NULL, (DLGPROC)DialogProc, NULL);
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
  
	MSG msg;
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

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPreInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	
	
	test_SHellFunc();
	//test_LogicDrivers();
	//
	// test_creatWndExample(hInstance);
	// test_creatDlgExample(hInstance);
	return 0;
	
}


int handle_file(LPCTSTR lpFilePath)
{
	TCHAR * file = const_cast<TCHAR*>(lpFilePath);
	if( IsExits(file) )
	{
		if( IsDirectory(file) )
		{
			dbg_log(_T("%s is a directory"), file);
		}
		else
		{
			dbg_log(_T("isn't a directory"));
			HANDLE hInFile = ::CreateFile(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if( hInFile != INVALID_HANDLE_VALUE)
			{
				TCHAR tmpPath[MAX_PATH] = {0};
				int npath = 0;
				// 得到 tmp 环境变量 的值
				if( !(npath = ::GetEnvironmentVariable(_T("tmp"), tmpPath, lengthof(tmpPath))) )
				{
					dbg_log(_T("leng: %d"), npath);
					if( !(npath = ::GetEnvironmentVariable(_T("temp"), tmpPath, lengthof(tmpPath))) )
					{
						dbg_log(_T("the %%tmp%% is not exist!"));
						return 0;
					}
					
				}
				
				TCHAR szOutFile[MAX_PATH] = {0};
				TCHAR * pName = _tcsrchr(file, _T('\\'));
				if( pName != NULL)
				{
					// 得到临时文件名字
					wnsprintf(&tmpPath[npath], lengthof(tmpPath)-npath-1, _T("\\%s.tmp"), ++pName);
				}
				else
				{
					wnsprintf(&tmpPath[npath], lengthof(tmpPath)-npath-1, _T("\\%s.tmp"), file);
				}
				dbg_log(_T("%s"), tmpPath);
				// 创建临时文件
				HANDLE hTmpFile = ::CreateFile(tmpPath, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS,0,NULL);
				if( hTmpFile == INVALID_HANDLE_VALUE)
				{
					dbg_log(_T("创建临时文件失败: %x"), ::GetLastError() );
					::CloseHandle(hInFile);
					return -1;
				}
				// 复制文件
				char buff[4096] = {0};
				DWORD rdLen = 0;
				
				while(::ReadFile(hInFile, buff, lengthof(buff), &rdLen, NULL) && rdLen > 0)
				{
					DWORD wtLen = 0;
					::WriteFile(hTmpFile, buff, rdLen, &wtLen, NULL);
					if( rdLen != wtLen)
					{
						dbg_log(_T("fatal write error: %x"), GetLastError());
						return -1;
					}
					dbg_log(_T("write len: %d"), wtLen);
				}
				
				// 得到文件大小
				// DWORD fileSizeHigh = 0;
				// DWORD fileSizeLow  = ::GetFileSize(hInFile, &fileSizeHigh);
				// if(  fileSizeLow ==  INVALID_FILE_SIZE)
				// {
					// DbgPrintf(_T("error: GetFileSize failed\n"));
				// }
				// else
				// {
					// DbgPrintf(L"high:%d\tlow:%d", fileSizeHigh, fileSizeLow);
					// DbgPrintf(L"%s[%d]:", _T(__FUNCTION__), __LINE__);
				// }
				// 删除文件
				// ::DeleteFile(tmpPath);
				::CloseHandle(hInFile);
				::CloseHandle(hTmpFile);
			}
			
		}
	}
	else
	{
		dbg_log(_T("the file isn't exist"));
	}
	return 0;
}
HWND g_hEdt      = NULL;
HWND g_hCombo    = NULL;
HWND g_hCrytoBtn = NULL;
// WindowProc
LRESULT CALLBACK WindowProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	HMENU hmn = ::GetMenu(hWnd);
	HMENU hsmn = ::GetSubMenu(hmn, 0);
	switch(uMsg)
	{
	// 用sendmessage或postmessage发送这个消息给它
	case WM_NOTIFY:
	{
		dbg_log(_T("WM_NOTIFY capture"));
		break;
	}
	case WM_COMMAND:
	{
		DWORD wmID   = LOWORD(wParam);
		DWORD wmEvnt = HIWORD(wParam); 
		switch(wmID)
		{
		case IDM_FILE_NEW:
			dbg_log(_T("evnt: %04X\tlParam: %ld"), wmEvnt, lParam);
			CheckMenuRadioItem(hsmn, IDM_FILE_NEW, IDM_FILE_OPEN, IDM_FILE_NEW, MF_BYCOMMAND);
			MessageBox(hWnd, _T("new file"), _T("提示"), MB_OK);
			break;
		case IDM_FILE_OPEN:
		{
			CheckMenuRadioItem(hsmn, IDM_FILE_NEW, IDM_FILE_OPEN, IDM_FILE_OPEN, MF_BYCOMMAND);
			MessageBox(hWnd, _T("open file"), _T("提示"), MB_OK);
			break;
		}
		case IDC_BTN_CRYTO:
		{
			if(wmEvnt == BN_CLICKED)
			{
				dbg_log(_T("cryto tick"));
				::SetWindowText(g_hEdt, _T("GOOD"));
				// 方法二
				//HWND hEdt = ::GetDlgItem(hWnd, IDC_EDT_SHOW);
				//::SetWindowText(hEdt, _T("GOOD"));
				// test
				//
			}
			break;
		}
		case IDC_CBO_BROWSE:
		{
			dbg_log(_T("evnt: %04X\tlParam: %ld"), wmEvnt, lParam);
			if( wmEvnt == CBN_DROPDOWN)
			{
				dbg_log(_T("evnt: %04X\tlParam: %ld"), wmEvnt, lParam);
			}
			break;
		}
		default:
			break;
		}
		return 0;
	}
	// case WM_LBUTTONDOWN:
	// {
		// dbg_log(_T("左击"));
		// return 0;
	// }
	case WM_CREATE:
	{
		// 得到当前目录路径
		TCHAR szCurDir[MAX_PATH] = {0};
		GetCurrentDirectory(MAX_PATH, szCurDir);
		// dbg_log(_T("curdir: %s"), szCurDir);
		// 得到客户区
		RECT lpClntRect;
		if(!::GetClientRect(hWnd, &lpClntRect) )
		{
			dbg_log(_T("GetClientRect failed") );
			return 0;
		}
		// dbg_log(_T("left: %d\tright: %d"), lpClntRect.left, lpClntRect.right);
		int textSize = 24;
		int textLen  = 380;
		g_hEdt = ::CreateWindow(
			_T("Edit"), // ComboBox
			szCurDir,
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER , // | BS_DEFPUSHBUTTON
			22,
			(lpClntRect.bottom-lpClntRect.top -textSize )/2,
			textLen,
			textSize,
			hWnd,
			(HMENU)IDC_EDT_SHOW,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);
		// ::ShowWindow(hEdt, SW_SHOW);
		::EnableWindow(g_hEdt, true);
		//
		g_hCombo = ::CreateWindow(
			_T("ComboBox"),
			NULL,
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, // | WS_BORDER
			22 + textLen,
			(lpClntRect.bottom-lpClntRect.top -textSize )/2,
			20,
			textSize,
			hWnd,
			(HMENU)IDC_CBO_BROWSE,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);
		//
		g_hCrytoBtn = ::CreateWindow(
			_T("BUTTON"),
			_T("浏览"),
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // |  WS_BORDER
			22 + textLen +40,
			(lpClntRect.bottom-lpClntRect.top -textSize )/2,
			50,
			textSize,
			hWnd,
			(HMENU)IDC_BTN_CRYTO,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);
		// ::ShowWindow(g_hCrytoBtn, SW_SHOW);
		// ::EnableWindow(g_hCrytoBtn, false);
		return 0;
	}			
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
	{
		return true;
	}
	case WM_CLOSE:  
        DestroyWindow(hwndDlg);  
        return TRUE;  
   
    case WM_DESTROY:  
        PostQuitMessage(0);  
        return TRUE;
	default:
		break;
	}
	return FALSE;
}
