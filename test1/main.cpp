
#include <windows.h>
#include <stdio.h>
// #incldue <wchar.h>
#include <tchar.h>

#include "../common/wcommon.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPreInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	wchar_t * file = _T("E:\\Simon\\projects\\flair61\\zipsig.exe");
	if( IsExits(file) )
	{
		if( IsDirectory(file) )
		{
			DbgPrintf(L"%s is a directory\n", file);
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
				TCHAR * pName = _tcsrchr(file, L'\\');
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
		DbgPrintf(L"the file isn't exist");
	}
	
	return 0;
}

