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
	bool isExist(LPCTSTR lpszFilePath = NULL)const;
	bool isFile(LPCTSTR lpszFilePath = NULL)const;
	bool isDir(LPCTSTR lpszFilePath = NULL)const;
	bool isEmptyDir(LPCTSTR lpszFilePath = NULL) const;
	LPCTSTR getName(LPCTSTR lpszFilePath = NULL);
	LPCTSTR getPath(LPCTSTR lpszFilePath = NULL);
	LPCTSTR getNameExt(LPCTSTR lpszFilePath = NULL)const;
	LPCTSTR getExtension(LPCTSTR lpszFilePath = NULL) const;
	LPTSTR addBackslash(LPTSTR lpszFilePath = NULL);
	LPCTSTR setName(LPCTSTR lpszName);
	LPCTSTR setPath(LPCTSTR lpszPath);
	void rmExtension(LPTSTR lpszFilePath = NULL);
	LPCTSTR append(LPCTSTR  pszMore=NULL, LPTSTR pszPath =NULL);
	//
	bool isValidFolder(const LPWIN32_FIND_DATA lpfindData)const;
	LPWIN32_FIND_DATA findFirstFile(LPCTSTR lpszDirPath = NULL, LPCTSTR lpszwildcard = _T("*"));
	LPWIN32_FIND_DATA findNextFile(/*LPWIN32_FIND_DATA lpFindFileData*/);
	void findClose();
	bool findIsHidden(const WIN32_FIND_DATA * lpfindData = NULL) const;
	bool findIsDir(const WIN32_FIND_DATA* lpfindData = NULL) const;
	bool findIsFile(const WIN32_FIND_DATA* lpfindData = NULL) const;
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
	//TCHAR m_szPath[MAX_PATH];
	LPTSTR m_lpszPath;
	LPTSTR m_lpszName;
	LPTSTR m_lpszExtension;
	HANDLE m_hFind;
	WIN32_FIND_DATA m_findData;
};
#endif
