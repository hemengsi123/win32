#ifndef _WIN32_COMMON_ONE_
#define _WIN32_COMMON_ONE_
// 2015.01.08
#include <io.h>
#include <Shlwapi.h>

// ======== macro ===========
#define lengthof(x) (sizeof((x))/sizeof(*(x)))

<<<<<<< HEAD
=======
#define dbg_log(strFmt, ...) DbgPrintf(_T("%s[%d]: ")##strFmt, _T(__FUNCTION__), __LINE__, __VA_ARGS__);

>>>>>>> tmp
// ========= function =========
bool WINAPI IsDirectory(LPCTSTR lpPath)
{
	return GetFileAttributes(lpPath) == FILE_ATTRIBUTE_DIRECTORY;
}
bool IsExits(LPCTSTR lpPath)
{
	return _waccess(lpPath, 0) != -1;
}
void DbgPrintf(LPCTSTR lpStrFormt, ...)
{
#ifdef _DEBUG
	TCHAR szBuff[512] = {0};
	int realLen = wnsprintf(szBuff, lengthof(szBuff), L"[Tip]");
	va_list args;
	va_start(args, lpStrFormt);
	realLen += wvnsprintf(&szBuff[realLen], lengthof(szBuff) - 2, lpStrFormt, args);
	va_end(args);
	if(realLen < 0)
		::OutputDebugString(L"lpStrFormt error.");
	else if( realLen >= lengthof(szBuff))
	{
		wnsprintf(szBuff, lengthof(szBuff), L"[Tip]debug message too long: %d\n", realLen);
		::OutputDebugString(szBuff);
	}
	else
	{
		::OutputDebugString(szBuff);
	}
	// _tcscat(szBuff, L"\n");
#endif
}


#endif