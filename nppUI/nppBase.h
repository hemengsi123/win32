#ifndef _NPPBASE_H_
#define _NPPBASE_H_

#include <tchar.h>
#include <Shlwapi.h>
#include <windows.h>
#include <string>
/////////////////////////////
// syserror

#define SYSERR_HSELF_NULL	56	
#define SYSERR_HWND_NULL  -1

//////////////////////////////
#ifdef _UNICODE 
	typedef std::wstring tstring;
#else
	typedef std::string  tstring;
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(x)	if( (x)!=NULL ) { delete (x); (x)=NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	if( (x)!=NULL ) { delete[] (x); (x)=NULL; }
#endif
/////////////////////////////////
#define lengthof(x) (sizeof((x))/sizeof(*(x)))

#define dbg_log(strFmt, ...) DbgPrintf(_T("%s[%d]: ")##strFmt, _T(__FILE__), __LINE__, __VA_ARGS__);
/////////////////////////////////////

void DbgPrintf(LPCTSTR lpStrFormt, ...);
/* 
 *@usage: WildCmp("bl?h.*", "blah.jpg");
*/
bool WildCmp(LPCTSTR string, LPCTSTR wild);
#endif

