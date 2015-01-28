#ifndef _NPPBASE_H_
#define _NPPBASE_H_

#include <tchar.h>
#include <Shlwapi.h>
#include <windows.h>
#include <string>
#include <vector>
#include <Wingdi.h>
#include <shlobj.h>
#include <atlbase.h>
#include <commctrl.h>
/////////////////////////////
// syserror

#define SYSERRm_hSelf_NULL	56	
#define SYSERR_HWND_NULL  -1

#ifndef ASSERT
#define ASSERT(expr)  _ASSERTE(expr)
#endif
//////////////////////////////
#ifdef _UNICODE 
	typedef std::wstring tstring;
#else
	typedef std::string  tstring;
#endif

#define OUT
#define IN
#define OTIN
// 
#define Int2TStr(str) #str

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
LPCTSTR GetLastErrStr();
/**************************************************************************
 *	Scroll up/down test function
 * @retrn: if up return 1, down return 2, outside return -1
 */
int GetScrollDirection(HWND hWnd, UINT offTop, UINT offBottom);
// 文件大小格式
typedef enum {
	SFMT_BYTES,
	SFMT_KBYTE,
	SFMT_DYNAMIC,
	SFMT_DYNAMIC_EX,
	SFMT_MAX
} eSizeFmt;
tstring GetFileSizeFmtStr(unsigned __int64 size, eSizeFmt sizeFmt = SFMT_KBYTE);
//============================== shell function ===============================
/*
    CSIDL_BITBUCKET, CSIDL_CONTROLS, CSIDL_DESKTOP, CSIDL_DESKTOPDIRECTORY,   
    CSIDL_DRIVES, CSIDL_FONTS, CSIDL_NETHOOD, CSIDL_NETWORK, CSIDL_PERSONAL,   
    CSIDL_PRINTERS, CSIDL_PROGRAMS, CSIDL_RECENT, CSIDL_SENDTO, CSIDL_STARTMENU,   
    CSIDL_STARTUP, CSIDL_TEMPLATES, ENDNUMBER  
*/
BOOL GetSpecialFolderPath(int nFolder, LPTSTR lpszPath, int lpMaxLen = MAX_PATH);
// 
BOOL CreateShortCut(LPCSTR pszPathObj,LPSTR pszParam,LPSTR pszPath,LPSTR pszPathLink,LPSTR pszDesc);

// Resolve files if they are shortcuts
HRESULT ResolveShortCut(LPCTSTR lpszShortcutPath, LPTSTR lpszFilePath, int maxBuf);
#endif

