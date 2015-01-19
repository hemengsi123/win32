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

