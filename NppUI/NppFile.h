#ifndef _NPPFILE_H_
#define _NPPFILE_H_

#include <ShellAPI.h>
#include <Shlwapi.h>

class CNppFile
{
public:
	CNppFile(LPCTSTR lpszFilePath = NULL);
	~CNppFile();
	void setFullPath(LPCTSTR lpszFilePath);
	LPCTSTR getFullPath()const;
	BOOL isExist(LPCTSTR lpszFilePath = NULL)const;
	BOOL isFile(LPCTSTR lpszFilePath = NULL)const;
	BOOL isDir(LPCTSTR lpszFilePath = NULL)const;
	BOOL isEmptyDir(LPCTSTR lpszFilePath = NULL) const;
	LPCTSTR getName(LPCTSTR lpszFilePath = NULL);
	LPCTSTR getPath(LPCTSTR lpszFilePath = NULL);
	LPCTSTR getNameExt(LPCTSTR lpszFilePath = NULL)const;
	LPCTSTR getExtension(LPCTSTR lpszFilePath = NULL) const;
	LPTSTR addBackslash(LPTSTR lpszFilePath = NULL);
	LPCTSTR setName(LPCTSTR lpszName);
	LPTSTR  setBuff(LPCTSTR lpszPath);
	LPCTSTR getBuff()const;
	LPCTSTR rmExtension(LPCTSTR lpszFilePath = NULL);
	LPCTSTR append(LPCTSTR  pszMore=NULL, LPTSTR pszPath =NULL);
	//
	BOOL isValidFolder(const LPWIN32_FIND_DATA lpfindData)const;
	LPWIN32_FIND_DATA findFirstFile(LPCTSTR lpszDirPath = NULL, LPCTSTR lpszwildcard = _T("*"));
	LPWIN32_FIND_DATA findNextFile(/*LPWIN32_FIND_DATA lpFindFileData*/);
	void findClose();
	BOOL findIsHidden(const WIN32_FIND_DATA * lpfindData = NULL) const;
	BOOL findIsDir(const WIN32_FIND_DATA* lpfindData = NULL) const;
	BOOL findIsFile(const WIN32_FIND_DATA* lpfindData = NULL) const;
	LPCTSTR findGetName(const WIN32_FIND_DATA* lpfindData = NULL) const;
	DWORD findGetAttri(const WIN32_FIND_DATA* lpfindData = NULL) const;
	unsigned __int64 findGetSize(const WIN32_FIND_DATA* lpfindData = NULL)const;
	LPSYSTEMTIME getSysTime(LPFILETIME lpftTime)const;
	DWORD getLogicalDrives() const;
	int delFile(LPCTSTR lpszFile = NULL, int fFlags = (FOF_ALLOWUNDO|FOF_NOCONFIRMATION), HWND hwnd = NULL)const;
	int copyFile(LPCTSTR lpszTo, LPCTSTR lpszFrom = NULL, int fFlags = (FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR), HWND hwnd = NULL)const;
	int moveFile(LPCTSTR lpszTo, LPCTSTR lpszFrom = NULL, int fFlags = (FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR), HWND hwnd = NULL)const;
	int renameFile(LPCTSTR lpszTo, LPCTSTR lpszFrom = NULL, int fFlags = FOF_NOCONFIRMATION, HWND hwnd = NULL);
private:
	TCHAR m_szFilePath[MAX_PATH];
	TCHAR m_szBuff[MAX_PATH];
	HANDLE m_hFind;
	WIN32_FIND_DATA m_findData;
};
#endif
