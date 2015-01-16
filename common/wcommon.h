
#ifndef _WIN32_COMMON_ONE_
#define _WIN32_COMMON_ONE_

#pragma once

// 2015.01.08
#include <io.h>
#include <Shlwapi.h>
#include <tchar.h>
#include <string>
#include <shlobj.h>
// ======== macro ===========
#define lengthof(x) (sizeof((x))/sizeof(*(x)))

#define dbg_log(strFmt, ...) DbgPrintf(_T("%s[%d]: ")##strFmt, _T(__FILE__), __LINE__, __VA_ARGS__);

typedef std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR> > tstring;

// ========= function =========
bool WINAPI IsDirectory(LPCTSTR lpPath)
{
	return GetFileAttributes(lpPath) == FILE_ATTRIBUTE_DIRECTORY;
}
bool IsExits(LPCTSTR lpPath)
{
	return _taccess(lpPath, 0) != -1;
}

bool IsValidParentFolder(WIN32_FIND_DATA Find)
{
	if ((Find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
		(!(Find.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) /*|| exProp.bShowHidden)*/) &&
		 (_tcscmp(Find.cFileName, _T("."))) &&
		 (Find.cFileName[0] != _T('?')))
		return true;

	return false;
}

bool IsValidFile(WIN32_FIND_DATA Find)
{
	if (!(Find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
		(!(Find.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) /*|| exProp.bShowHidden*/))
		return true;

	return false;
}
void DbgPrintf(LPCTSTR lpStrFormt, ...)
{
#ifdef _DEBUG
	TCHAR szBuff[512] = {0};
	int realLen = wnsprintf(szBuff, lengthof(szBuff), _T("[Tip]"));
	va_list args;
	va_start(args, lpStrFormt);
	realLen += wvnsprintf(&szBuff[realLen], lengthof(szBuff) - 2, lpStrFormt, args);
	va_end(args);
	if(realLen < 0)
		::OutputDebugString(_T("lpStrFormt error."));
	else if( realLen >= lengthof(szBuff))
	{
		wnsprintf(szBuff, lengthof(szBuff), _T("[Tip]debug message too long: %d\n"), realLen);
		::OutputDebugString(szBuff);
	}
	else
	{
		::OutputDebugString(szBuff);
	}
	// _tcscat(szBuff, L"\n");
#endif
}

/**************************************************************************
 *	Get system images
 */
HIMAGELIST GetSmallImageList(BOOL bSystem = true)
{
	static
	HIMAGELIST		himlSys	= NULL;
	HIMAGELIST		himl	= NULL;
	SHFILEINFO		sfi		= {0};

	if (bSystem) {
		if (himlSys == NULL) {
			himlSys = (HIMAGELIST)SHGetFileInfo(_T("C:\\"), 0, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
		}
		himl = himlSys;
	} else {
		// himl = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, 6, 30);;
	}

	return himl;
}

#endif