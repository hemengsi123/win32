
#include <windows.h>
#include <io.h>
#include <stdio.h>

bool WINAPI IsDirectory(LPCTSTR lpPath)
{
	return GetFileAttributes(lpPath) == FILE_ATTRIBUTE_DIRECTORY;
}
bool IsExits(LPCTSTR lpPath)
{
	return _waccess(lpPath, 0) != -1;
}
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPreInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	wchar_t * file = L"E:\\Simon\\projects\\flair61\\zipsig1.exe";
	if( IsExits(file) )
	{
		if( IsDirectory(file) )
		{
			::MessageBox(NULL, L"is a directory", L"test1", MB_OK);
		}
		else
		{
			::MessageBox(NULL, L"isn't a directory", L"test1", MB_OK);
			HANDLE hFile = ::CreateFile(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if( hFile == INVALID_HANDLE_VALUE)
			{
				OutputDebugString(L"the file isn't exist");
			}
		}
	}
	else
	{
		::MessageBox(NULL, L"the path isn't exist", L"test1", MB_OK);
		OutputDebugString(L"the file isn't exist");
	}
	
	return 0;
}

