#ifndef _NPPBASE_H_
#define _NPPBASE_H_

#include <tchar.h>
#include <Shlwapi.h>
/////////////////////////////
// syserror

#define SYSERR_HSELF_NULL	56	
#define SYSERR_HANDLE_NULL  -1

//////////////////////////////
#define lengthof(x) (sizeof((x))/sizeof(*(x)))
#define DbgLog(strFmt, ...) DbgPrintf(_T("%s[%d]: ")##strFmt, _T(__FILE__), __LINE__, __VA_ARGS__);

//根据指针值删除内存
#ifndef SAFE_DELETE
#define SAFE_DELETE(x)	if( (x)!=NULL ) { delete (x); (x)=NULL; }
#endif
//根据指针值删除数组类型内存
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	if( (x)!=NULL ) { delete[] (x); (x)=NULL; }
#endif

#ifndef dbg_log
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
#endif

#endif
