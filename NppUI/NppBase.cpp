#include "NppBase.h"

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

bool WildCmp(LPCTSTR string, LPCTSTR wild)
{
	// Written by Jack Handy - jakkhandy@hotmail.com
	// See: http://www.codeproject.com/string/wildcmp.asp
	LPCTSTR		cp = NULL;
	LPCTSTR		mp = NULL;

	while ((*string) && (*wild != '*')) {
		if ((tolower(*wild) != tolower(*string)) && (*wild != '?')) {
			return 0;
		}
		wild++;
		string++;
	}

	while (*string) {
		if (*wild == '*') {
			if (!*++wild) {
				return 1;
			}
			mp = wild;
			cp = string+1;
		} else if ((tolower(*wild) == tolower(*string)) || (*wild == '?')) {
			wild++;
			string++;
		} else {
			wild = mp;
			string = cp++;
		}
	}

	while (*wild == '*') {
		wild++;
	}
	return !*wild;
}

LPCTSTR GetLastErrStr()
{
	static TCHAR errStr[255];
	::ZeroMemory(errStr, sizeof(errStr));
	DWORD errNo = ::GetLastError();
	::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errNo, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) & errStr, sizeof(errStr), NULL);
	return errStr;
}
/**************************************************************************
 *	Scroll up/down test function
 * @retrn: if up return 1, down return 2, outside return -1
 */
int GetScrollDirection(HWND hWnd, UINT offTop, UINT offBottom)
{
	RECT	rcUp	= {0};
	RECT	rcDown	= {0};

//	::GetClientRect(hWnd, &rcUp);
//	::ClientToScreen(hWnd, &rcUp);
	rcDown = rcUp;

	rcUp.top += offTop;
	rcUp.bottom = rcUp.top + 20;
	rcDown.bottom += offBottom;
	rcDown.top = rcDown.bottom - 20;

	POINT	pt		= {0};
	::GetCursorPos(&pt);
	if (::PtInRect(&rcUp, pt) == TRUE)
		return 1; //SCR_UP;
	else if (::PtInRect(&rcDown, pt) == TRUE)
		return 2; //SCR_DOWN;
		
	return -1; //SCR_OUTSIDE;
}
tstring GetFileSizeFmtStr(unsigned __int64 size, eSizeFmt sizeFmt)
{
	TCHAR	TEMP[MAX_PATH];
	tstring str;
	unsigned __int64 subsize = 0;
	
	switch (sizeFmt)
	{
		case SFMT_BYTES:
		{
			// long long x; printf("%I64d", x);
			_stprintf(TEMP, _T("%03I64d"), size % 1000);
			size /= 1000;
			str = TEMP;
			
			while (size)
			{
				_stprintf(TEMP, _T("%03I64d."), size % 1000);
				size /= 1000;
				str = TEMP + str;
			}

			break;
		}
		case SFMT_KBYTE:
		{
			size /= 1024;
			_stprintf(TEMP, _T("%03I64d"), size % 1000);
			size /= 1000;
			str = TEMP;

			while (size)
			{
				_stprintf(TEMP, _T("%03I64d,"), size % 1000);
				size /= 1000;
				str = TEMP + str;
			}

			str = str + _T(" kB");

			break;
		}
		case SFMT_DYNAMIC:
		{
			INT i	= 0;

			str	= _T("000");

			for (i = 0; (i < 4) && (size != 0); i++)
			{
				unsigned __int64 modsize = size & (1024-1);
				_stprintf(TEMP, _T("%03I64d"), modsize);
				size  >>= 10; // 2^10 == 1024
				str     = TEMP;
				if( size != 0)
					subsize = modsize;
			}
			
			while (size)
			{
				_stprintf(TEMP, _T("%03I64d,"), size % 1000);
				size /= 1000;
				str = TEMP + str;
				subsize = 0;
			}
			// 扩大一千倍，确保商是整数
			subsize = (subsize*1000)>>10;
			if( subsize != 0)
			{
				_stprintf(TEMP, _T(".%02d"), subsize);
				TEMP[3] = _T('\0');
				str = str + TEMP;
			}
			switch (i)
			{
				case 0:
				case 1: str = str + _T(" b"); break;
				case 2:	str = str + _T(" k"); break;
				case 3: str = str + _T(" M"); break;
				case 4: str = str + _T(" G"); break;
				default: str = str + _T(" T"); break;
			}
			break;
		}
		case SFMT_DYNAMIC_EX:
		{
			INT		i		= 0;
			__int64 komma	= 0;

			str = _T("000");

			for (i = 0; (i < 3) && (size != 0); i++)
			{
				if (i < 1)
					_stprintf(TEMP, _T("%03I64d"), size);
				else
					_stprintf(TEMP, _T("%03I64d,%I64d"), size % 1024, komma);
				komma = (size % 1024) / 100;
				size /= 1024;
				str = TEMP;
			}

			while (size)
			{
				_stprintf(TEMP, _T("%03I64d,"), size % 1000);
				size /= 1000;
				str = TEMP + str;
			}

			switch (i)
			{
				case 0:
				case 1: str = str + _T(" b"); break;
				case 2: str = str + _T(" k"); break;
				default: str = str + _T(" M"); break;
			}
			break;
		}
		default:
			break;
	}

	for (INT i = 0; i < 2; i++)
	{
		if (str[i] == '0')
			str[i] = ' ';
		else
			break;
	}
	return str;
}
BOOL GetSpecialFolderPath(int nFolder, LPTSTR lpszPath, int lpszMaxLen)
{
	HRESULT       hr;  
    LPITEMIDLIST  ppidl;
	BOOL bret = FALSE;
	::CoInitialize(NULL);
	hr = ::SHGetSpecialFolderLocation(NULL, nFolder, &ppidl);   
    if (hr == S_OK)  
    {  
 		bret = ::SHGetPathFromIDList(ppidl, lpszPath);
		::CoTaskMemFree(ppidl);
    }
	::CoUninitialize();
	return bret;
}
BOOL CreateShortCut(LPCSTR lpszPathObj,LPSTR lpszParam,LPSTR lpszPath,LPSTR lpszPathLink,LPSTR lpszDesc)
{
	HRESULT hRes = S_FALSE;
    CComPtr<IShellLink> ipShellLink;
	// Get a pointer to the IShellLink interface
	hRes = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&ipShellLink);
    if (hRes == S_OK) 
    {
		// Set the path to the shortcut target and add the description. 
        ipShellLink->SetPath(lpszPathObj); 
        ipShellLink->SetDescription(lpszDesc); 
		ipShellLink->SetArguments(lpszParam) ;
	    ipShellLink->SetWorkingDirectory(lpszPath);
		// Get a pointer to the IPersistFile interface
        CComQIPtr<IPersistFile> ipPersistFile;
		// query IShellLink for the IPersistFile interface for saving the shortcut in persistent storage
		hRes = (HRESULT)(ipShellLink->QueryInterface( IID_IPersistFile, (void**)&ipPersistFile)) ;
		if( FAILED( hRes))
	    {
			::CoUninitialize();
	    	return FALSE ;
	    }
	#ifndef UNICODE
		WCHAR wszPath[MAX_PATH] = {0};
		::MultiByteToWideChar( CP_ACP, 0, lpszPathLink, -1, (LPWSTR)wszPath, MAX_PATH) ;
	#else
		WCHAR * wszPath = lpszPathLink;
	#endif
		 // save the link by calling IPersistFile::Save
    	hRes = ipPersistFile->Save((LPWSTR)wszPath, STGM_READWRITE);
    	return TRUE;
    }
	::CoUninitialize();
	return FALSE;
}

HRESULT ResolveShortCut(LPCTSTR lpszShortcutPath, LPTSTR lpszFilePath, int maxBuf)
{
	HRESULT hRes = S_FALSE;
    CComPtr<IShellLink> ipShellLink;
        // buffer that receives the null-terminated string for the drive and path
    TCHAR szPath[MAX_PATH];     
        // structure that receives the information about the shortcut
    WIN32_FIND_DATA wfd;    
    WCHAR wszTemp[MAX_PATH];

    lpszFilePath[0] = '\0';

    // Get a pointer to the IShellLink interface
    hRes = CoCreateInstance(CLSID_ShellLink,
                            NULL, 
                            CLSCTX_INPROC_SERVER,
                            IID_IShellLink,
                            (void**)&ipShellLink); 

    if (hRes == S_OK) 
    { 
        // Get a pointer to the IPersistFile interface
        CComQIPtr<IPersistFile> ipPersistFile(ipShellLink);

        // IPersistFile is using LPCOLESTR, so make sure that the string is Unicode
#if !defined UNICODE
        MultiByteToWideChar(CP_ACP, 0, lpszShortcutPath, -1, wszTemp, MAX_PATH);
#else
        wcsncpy(wszTemp, lpszShortcutPath, MAX_PATH);
#endif

        // Open the shortcut file and initialize it from its contents
        hRes = ipPersistFile->Load(wszTemp, STGM_READ); 
        if (hRes == S_OK) 
        {
            // Try to find the target of a shortcut, even if it has been moved or renamed
            hRes = ipShellLink->Resolve(NULL, SLR_UPDATE); 
            if (hRes == S_OK) 
            {
                // Get the path to the shortcut target
                hRes = ipShellLink->GetPath(szPath, MAX_PATH, &wfd, SLGP_RAWPATH); 
				if (hRes == S_OK) 
				{
	                _tcsncpy(lpszFilePath, szPath, maxBuf);
				}
            } 
        } 
    } 

    return hRes;
}
