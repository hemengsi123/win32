
#include <windows.h>
#include <stdio.h>
// #incldue <wchar.h>
#include <tchar.h>

#include "../common/wcommon.h"
#include "main.h"
#include "resource.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPreInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
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