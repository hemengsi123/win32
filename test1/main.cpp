
#include <windows.h>
#include <stdio.h>
// #incldue <wchar.h>
#include <tchar.h>

#include "../common/wcommon.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPreInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	wchar_t * file = L"E:\\Simon\\projects\\flair61\\zipsig.exe";
	if( IsExits(file) )
	{
		if( IsDirectory(file) )
		{
			DbgPrintf(L"%s is a directory\n", file);
		}
		else
		{
			DbgPrintf(L"isn't a directory");
			HANDLE hInFile = ::CreateFile(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if( hInFile != INVALID_HANDLE_VALUE)
			{
				// HANDLE hOutFile = ::CreateFile(
				TCHAR szOutFile[MAX_PATH] = {0};
				TCHAR * pName = _tcsrchr(file, L'\\');
				if( pName != NULL)
				{
					DbgPrintf(++pName);
				}
				DWORD fileSizeHigh = 0;
				DWORD fileSizeLow  = ::GetFileSize(hInFile, &fileSizeHigh);
				if(  fileSizeLow ==  INVALID_FILE_SIZE)
				{
					DbgPrintf(L"error: GetFileSize failed\n");
				}
				else
				{
					DbgPrintf(L"high:%d\tlow:%d", fileSizeHigh, fileSizeLow);
				}
				::CloseHandle(hInFile);
			}
			
		}
	}
	else
	{
		DbgPrintf(L"the file isn't exist");
	}
	
	return 0;
}

