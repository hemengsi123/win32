
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
	WNDCLASS wndClss = {};
	wndClss.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wndClss.lpfnWndProc   = WindowProc;
	wndClss.lpszClassName = clsName;
	wndClss.hInstance     = hInstance;
	
	::RegisterClass(&wndClss);
	HWND hWnd = ::CreateWindow(
		clsName,
		_T("Hello, win32"),
		WS_OVERLAPPEDWINDOW,
		258,
		258,
		480,
		250,
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
	
	MSG msg;
	while(::GetMessage(&msg, NULL, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
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
// WindowProc
LRESULT CALLBACK WindowProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}
