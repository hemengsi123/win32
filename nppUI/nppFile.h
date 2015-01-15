#ifndef _NPPFILE_H_
#define _NPPFILE_H_

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
	LPCTSTR setPath(LPCTSTR lpszPath);
	void rmExtension(LPTSTR lpszFilePath = NULL);
	LPCTSTR append(LPCTSTR  pszMore=NULL, LPTSTR pszPath =NULL);
	//
	bool isValidFolder(const LPWIN32_FIND_DATA lpfindData)const;
	LPWIN32_FIND_DATA findFirstFile(/*LPWIN32_FIND_DATA lpFindFileData*/);
	LPWIN32_FIND_DATA findNextFile(/*LPWIN32_FIND_DATA lpFindFileData*/);
	void findClose();
	bool findIsHidden() const;
	bool findIsDir() const;
	bool findIsFile() const;
	LPCTSTR findGetName() const;
	DWORD findGetAttri() const;
	DWORD getLogicalDrives() const;
private:
	TCHAR m_szFilePath[MAX_PATH];
	//TCHAR m_szPath[MAX_PATH];
	LPSTR m_lpszPath;
	LPSTR m_lpszName;
	LPSTR m_lpszExtension;
	HANDLE m_hFind;
	WIN32_FIND_DATA m_findData;
};
#endif
